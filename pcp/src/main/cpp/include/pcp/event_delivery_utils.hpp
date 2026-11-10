#pragma once
#include <string>
#include <cstdint>
namespace pcp {
enum class DeliveryStatus { SENDING, SENT, FAILED, QUEUED };
struct EventDelivery { std::string eventId; DeliveryStatus status=DeliveryStatus::QUEUED; int retryCount=0; int64_t queuedAtMs=0; std::string error; };
std::string formatDeliveryStatus(DeliveryStatus s);
bool canRetryDelivery(const EventDelivery& d, int maxRetries=5);
int64_t getNextRetryMs(const EventDelivery& d);
}
