#pragma once
#include <string>
#include <cstdint>

std::string redactsEventId;    // event being redacted(const std::string& json);
std::string reason;            // optional reason(const std::string& json);
std::string RedactionInfo parseRedaction(const std::string& json);
std::string bool isRedactionEvent(const std::string& json);
std::string buildRedactionContent(const std(const std::string& json);
std::string string& eventIdToRedact, const std(const std::string& json);
std::string string& reason = "");(const std::string& json);
std::string applyRedaction(const std(const std::string& json);
std::string string& eventContent);(const std::string& json);
std::string bool isRedactedEvent(const std(const std::string& json);
std::string string& eventJson);(const std::string& json);
std::string formatRedactionNotice(const RedactionInfo& info, const std(const std::string& json);
std::string string& originalSender);(const std::string& json);
