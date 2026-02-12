/**
 * @file Subscriber.cppm
 * @brief 消息管理 - 订阅者
 */

module;

#include "emdevif/fatal_handler.h"

#include <cstddef>

#include <type_traits>

export module rmdev.messageManager.subscriber;

import emdevif.container.messageQueue;
import emdevif.container.messageSlot;
import emdevif.errorHandler;

using emdevif::ErrorCode;
using emdevif::MessageQueueTimeout_t;

export namespace rmdev {

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

    ErrorCode pop(bool in_isr, ViewType& data, MessageQueueTimeout_t timeout_tick = 0U)
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
    ErrorCode pop(bool in_isr)
    {
        if constexpr (is_queue_not_slot) {
            return queue_.pop(in_isr);
        }
        else {
            EMDEVIF_FATAL_HANDLER("It's message slot, method 'pop' was not supported. Please use 'peek' instead.");
        }
    }

    ErrorCode peek(bool in_isr, ViewType& data, MessageQueueTimeout_t timeout_tick = 0U)
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
