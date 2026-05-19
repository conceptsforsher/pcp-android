#include "progressive/eventcache.hpp"
#include <sstream>
#include <android/log.h>

#define LOG_TAG "EventCache"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

namespace progressive {

void EventCache::put(const CachedEvent& event) {
    // Update reverse index: remove old relation if any
    auto it = events_.find(event.eventId);
    if (it != events_.end() && !it->second.sourceEventId.empty()) {
        auto& vec = relationIndex_[it->second.sourceEventId];
        vec.erase(std::remove(vec.begin(), vec.end(), event.eventId), vec.end());
    }

    // Store the event
    events_[event.eventId] = event;

    // Update reverse index
    if (!event.sourceEventId.empty()) {
        relationIndex_[event.sourceEventId].push_back(event.eventId);
    }
}

const CachedEvent* EventCache::get(const std::string& eventId) const {
    auto it = events_.find(eventId);
    if (it != events_.end()) return &it->second;
    return nullptr;
}

std::vector<const CachedEvent*> EventCache::getRelations(const std::string& sourceEventId) const {
    std::vector<const CachedEvent*> results;
    auto idxIt = relationIndex_.find(sourceEventId);
    if (idxIt != relationIndex_.end()) {
        for (const auto& relatedId : idxIt->second) {
            auto eventIt = events_.find(relatedId);
            if (eventIt != events_.end()) {
                results.push_back(&eventIt->second);
            }
        }
    }
    return results;
}

void EventCache::clear() {
    events_.clear();
    relationIndex_.clear();
}

std::string EventCache::getContextData(const std::string& eventId) const {
    auto it = events_.find(eventId);
    if (it == events_.end()) {
        return R"({"cached": false})";
    }

    const auto& e = it->second;
    std::ostringstream json;

    json << "{";
    json << R"("cached": true,)";
    json << R"("eventId": ")" << e.eventId << R"(",)";
    json << R"("senderId": ")" << e.senderId << R"(",)";
    json << R"("senderName": ")" << e.senderName << R"(",)";
    json << R"("timestamp": ")" << e.timestamp << R"(",)";
    json << R"("body": ")" << e.body << R"(",)";
    json << R"("msgType": ")" << e.msgType << R"(",)";
    json << R"("eventType": ")" << e.eventType << R"(",)";
    json << R"("sentByMe": )" << (e.sentByMe ? "true" : "false") << ",";
    json << R"("hasFailed": )" << (e.hasFailed ? "true" : "false") << ",";
    json << R"("reactionCount": )" << e.reactionCount << ",";
    json << R"("hasBeenEdited": )" << (e.hasBeenEdited ? "true" : "false");

    // Add relation info if present
    if (!e.relationType.empty()) {
        json << R"(,"relationType": ")" << e.relationType << R"(")";
        json << R"(,"sourceEventId": ")" << e.sourceEventId << R"(")";
    }

    // Add reactions summary
    auto relations = getRelations(eventId);
    if (!relations.empty()) {
        json << R"(,"reactions": [)";
        for (size_t i = 0; i < relations.size(); ++i) {
            if (i > 0) json << ",";
            json << R"({"key": ")" << relations[i]->body
                 << R"(", "count": )" << relations[i]->reactionCount
                 << R"(", "addedByMe": )" << (relations[i]->sentByMe ? "true" : "false")
                 << "}";
        }
        json << "]";
    }

    json << "}";
    return json.str();
}

} // namespace progressive
