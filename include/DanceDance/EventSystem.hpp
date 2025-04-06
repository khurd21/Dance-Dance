#ifndef DANCE_DANCE_EVENT_SYSTEM_HPP
#define DANCE_DANCE_EVENT_SYSTEM_HPP

#include <algorithm>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace dd {

class EventSystem {
  public:
    template <typename EventType>
    using Callback = std::function<void(const EventType&)>;

    template <typename EventType>
    void subscribe(Callback<EventType> callback);

    template <typename EventType>
    void publish(const EventType& eventType);

  private:
    std::unordered_map<std::type_index, std::vector<std::function<void(const void*)>>> m_callbacks;
};

template <typename EventType>
void EventSystem::subscribe(Callback<EventType> callback) {
    m_callbacks[typeid(EventType)].emplace_back([callback](const void* event) { callback(*static_cast<const EventType*>(event)); });
}

template <typename EventType>
void EventSystem::publish(const EventType& eventType) {
    if (!m_callbacks.contains(typeid(eventType))) {
        return;
    }
    const auto& callbacks = m_callbacks.at(typeid(eventType));
    std::ranges::for_each(callbacks, [&](const auto& callback) { callback(&eventType); });
}

} // namespace dd

#endif // DANCE_DAMCE_EVENT_SYSTEM_HPP