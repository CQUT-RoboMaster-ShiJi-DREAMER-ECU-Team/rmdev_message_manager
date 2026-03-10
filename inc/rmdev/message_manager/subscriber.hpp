/**
 * @file subscriber.hpp
 * @brief
 */

#pragma once
#ifndef RMDEV_MESSAGE_MANAGER_SUBSCRIBER_HPP
    #define RMDEV_MESSAGE_MANAGER_SUBSCRIBER_HPP

    #include "emdevif/core/detail/config.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include "emdevif/core/fatal_handler.h"

        #include "emdevif/core/error_handler.hpp"
        #include "emdevif/core/data_container/message_queue.hpp"
        #include "emdevif/core/data_container/message_slot.hpp"

        #include <cstddef>

        #include <type_traits>
    #endif

EMDEVIF_MODULE_EXPORT
namespace rmdev {

template<typename ViewType_, class QueueImpl>
    requires(emdevif::ValidMessageQueue<QueueImpl> || emdevif::ValidMessageSlot<QueueImpl>)
class Subscriber
{
public:
    using ViewType = ViewType_;

    static constexpr bool is_queue_not_slot = emdevif::IsMessageQueue_v<QueueImpl>;

    using ValueType = QueueImpl::ValueType;

    static_assert(std::is_convertible_v<ValueType, ViewType>, "`ValueType\' should be convertable to `ViewType\'");

    static constexpr bool view_type_equals_to_value_type = std::is_same_v<ValueType, ViewType>;

private:
    QueueImpl& queue_;

public:
    explicit Subscriber(QueueImpl& queue) : queue_(queue) {}

    emdevif::ErrorCode pop(bool in_isr, ViewType& data, emdevif::MessageQueueTimeout_t timeout_tick = 0U)
    {
        if constexpr (is_queue_not_slot) {
            if constexpr (!view_type_equals_to_value_type) {
                ValueType origin_data;

                auto ret = queue_.pop(in_isr, origin_data, timeout_tick);
                data = static_cast<ViewType>(origin_data);

                return ret;
            }
            else {
                return queue_.pop(in_isr, data, timeout_tick);
            }
        }
        else {
            EMDEVIF_FATAL_HANDLER("It's message slot, method 'pop' was not supported. Please use 'peek' instead.");
        }
    }
    emdevif::ErrorCode pop(bool in_isr)
    {
        if constexpr (is_queue_not_slot) {
            return queue_.pop(in_isr);
        }
        else {
            EMDEVIF_FATAL_HANDLER("It's message slot, method 'pop' was not supported. Please use 'peek' instead.");
        }
    }

    emdevif::ErrorCode peek(bool in_isr, ViewType& data, emdevif::MessageQueueTimeout_t timeout_tick = 0U)
    {
        if constexpr (!view_type_equals_to_value_type) {
            ValueType origin_data;

            auto ret = queue_.peek(in_isr, origin_data, timeout_tick);
            data = static_cast<ViewType>(origin_data);

            return ret;
        }
        else {
            return queue_.peek(in_isr, data, timeout_tick);
        }
    }

    void clear()
    {
        queue_.clear();
    }

    [[nodiscard]] std::size_t storeCount() const
    {
        return queue_.storeCount();
    }

    [[nodiscard]] std::size_t remainCount() const
    {
        return queue_.remainCount();
    }

    [[nodiscard]] static constexpr std::size_t maxItemCount()
    {
        return QueueImpl::item_size;
    }

    [[nodiscard]] void* getHandle() const
    {
        return queue_.getHandle();
    }

    [[nodiscard]] QueueImpl& getQueueInstance()
    {
        return queue_;
    }
    [[nodiscard]] const QueueImpl& getQueueInstance() const
    {
        return queue_;
    }
};

}  // namespace rmdev

#endif  // !RMDEV_MESSAGE_MANAGER_SUBSCRIBER_HPP
