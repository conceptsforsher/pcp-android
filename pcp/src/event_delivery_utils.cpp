#include "pcp/event_delivery_utils.hpp"
#include <chrono>
namespace pcp {
std::string formatDeliveryStatus(DeliveryStatus s) { switch(s){case DeliveryStatus::SENDING:return"Sending...";case DeliveryStatus::SENT:return"Sent";case DeliveryStatus::FAILED:return"Failed";default:return"Queued";} }
bool canRetryDelivery(const EventDelivery& d, int max) { return d.retryCount < max && d.status != DeliveryStatus::SENT; }
int64_t getNextRetryMs(const EventDelivery& d) { return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() + (1LL<<d.retryCount)*1000; }
}
