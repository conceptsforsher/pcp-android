#include "matrix_push_rules.hpp"
#include <QJsonArray>
#include <QJsonValue>
#include <QRegularExpression>
#include <QDebug>

namespace pcp_chat {

MatrixPushRules::MatrixPushRules(QObject *parent) : QObject(parent) {}
MatrixPushRules::~MatrixPushRules() = default;

void MatrixPushRules::loadGlobalRules(const QJsonObject &data)
{
    m_globalRules.clear();

    if (data.contains("global")) {
        QJsonObject global = data["global"].toObject();
        QStringList categories = {"override", "content", "room", "sender", "underride"};

        for (const QString &category : categories) {
            if (global.contains(category)) {
                QJsonArray rules = global[category].toArray();
                for (const QJsonValue &val : rules) {
                    PushRule rule = parseRule(val.toObject());
                    rule.kind = category;
                    m_globalRules.append(rule);
                }
            }
        }
    }

    // Sort by priority (highest first)
    std::sort(m_globalRules.begin(), m_globalRules.end(),
              [](const PushRule &a, const PushRule &b) {
                  return a.priority > b.priority;
              });
}

void MatrixPushRules::mergeFromSync(const QJsonObject &data)
{
    Q_UNUSED(data);
}

QList<PushRule> MatrixPushRules::matchingRules(const QString &roomId,
                                                 const QString &sender,
                                                 bool isDirect, bool isHighlighted,
                                                 const QString &body,
                                                 int memberCount) const
{
    QList<PushRule> matched;

    for (const PushRule &rule : m_globalRules) {
        if (!rule.enabled) continue;

        bool matches = true;

        for (const PushCondition &cond : rule.conditions) {
            if (cond.kind == "event_match") {
                if (cond.key == "room_id") {
                    if (!QRegularExpression(QRegularExpression::wildcardToRegularExpression(cond.pattern))
                             .match(roomId).hasMatch()) {
                        matches = false;
                        break;
                    }
                }
            } else if (cond.kind == "contains_display_name") {
                matches = isHighlighted;
            } else if (cond.kind == "room_member_count") {
                bool condMet = false;
                if (cond.is == "2" && memberCount <= 2) condMet = true;
                if (cond.is == ">2" && memberCount > 2) condMet = true;
                if (!condMet) { matches = false; break; }
            }
        }

        // Rule-specific matching
        if (rule.kind == "room" && rule.rule != roomId) continue;
        if (rule.kind == "sender" && rule.rule != sender) continue;

        if (matches)
            matched.append(rule);
    }

    return matched;
}

bool MatrixPushRules::isNoisyRule(const PushRule &rule) const
{
    return rule.actions.contains(PushAction::Notify) &&
           !rule.tweaks.contains("sound") &&
           rule.tweaks.value("sound") != "default";
}

bool MatrixPushRules::shouldNotify(const PushRule &rule) const
{
    return rule.actions.contains(PushAction::Notify) ||
           (!rule.actions.contains(PushAction::DontNotify) &&
            rule.tweaks.contains("highlight"));
}

QString MatrixPushRules::soundForRule(const PushRule &rule) const
{
    return rule.tweaks.value("sound", "default");
}

QString MatrixPushRules::highlightColor(const PushRule &rule) const
{
    return rule.tweaks.value("color", "#FF0000");
}

PushRule MatrixPushRules::parseRule(const QJsonObject &obj)
{
    PushRule rule;
    rule.ruleId = obj["rule_id"].toString();
    rule.isDefault = obj["default"].toBool(true);
    rule.enabled = obj["enabled"].toBool(true);
    rule.priority = obj["priority_class"].toInt(0);

    if (obj.contains("conditions")) {
        QJsonArray conds = obj["conditions"].toArray();
        for (const QJsonValue &val : conds) {
            QJsonObject c = val.toObject();
            PushCondition cond;
            cond.kind = c["kind"].toString();
            cond.key = c["key"].toString();
            cond.pattern = c["pattern"].toString();
            cond.is = c["is"].toString();
            cond.raw = c;
            rule.conditions.append(cond);
        }
    }

    if (obj.contains("actions")) {
        QJsonArray actions = obj["actions"].toArray();
        for (const QJsonValue &val : actions) {
            if (val.isString()) {
                QString action = val.toString();
                if (action == "notify") rule.actions.append(PushAction::Notify);
                else if (action == "dont_notify") rule.actions.append(PushAction::DontNotify);
                else if (action == "coalesce") rule.actions.append(PushAction::Coalesce);
            } else if (val.isObject()) {
                QJsonObject actionObj = val.toObject();
                if (actionObj.contains("set_tweak")) {
                    rule.actions.append(PushAction::SetTweak);
                    QString tweak = actionObj["set_tweak"].toString();
                    if (tweak == "sound")
                        rule.tweaks["sound"] = actionObj["value"].toString("default");
                    else if (tweak == "highlight")
                        rule.tweaks["highlight"] = actionObj["value"].toString();
                    else
                        rule.tweaks[tweak] = actionObj["value"].toString();
                }
            }
        }
    }

    rule.raw = obj;
    return rule;
}

QJsonObject MatrixPushRules::serializeRule(const PushRule &rule)
{
    QJsonObject obj;
    obj["rule_id"] = rule.ruleId;
    obj["default"] = rule.isDefault;
    obj["enabled"] = rule.enabled;

    QJsonArray actions;
    for (PushAction action : rule.actions) {
        if (action == PushAction::Notify) actions.append("notify");
        else if (action == PushAction::DontNotify) actions.append("dont_notify");
        else if (action == PushAction::Coalesce) actions.append("coalesce");
        else if (action == PushAction::SetTweak) {
            QJsonObject tweakObj;
            for (auto it = rule.tweaks.begin(); it != rule.tweaks.end(); ++it) {
                tweakObj["set_tweak"] = it.key();
                tweakObj["value"] = it.value();
            }
            actions.append(tweakObj);
        }
    }
    obj["actions"] = actions;

    return obj;
}

} // namespace pcp_chat
