/**
 * @file Subscriber.cppm
 * @brief 消息管理 - 订阅者
 */

module;

#include "emdevif/fatal_handler.h"

#include <cstddef>

export module rmdev.messageManager.subscriber;

import emdevif.container.messageQueue;
import emdevif.container.messageSlot;
import emdevif.errorHandler;

using emdevif::ErrorCode;
using emdevif::MessageQueueTimeout_t;

export namespace rmdev {

template<typename T, class QueueImpl>
    requires(emdevif::ValidMessageQueue<QueueImpl> || emdevif::ValidMessageSlot<QueueImpl>)
class Subscriber
{
public:
    using ValueType = T;

    static constexpr bool is_queue_not_slot = emdevif::IsMessageQueue_v<QueueImpl>;

private:
    QueueImpl& queue_;

    using QueueType_ = QueueImpl::ValueType;

public:
    explicit Subscriber(QueueImpl& queue) : queue_(queue) {}

    ErrorCode pop(bool in_isr, ValueType& data, MessageQueueTimeout_t timeout_tick = 0U)
    {
        if constexpr (is_queue_not_slot) {
            QueueType_ origin_data;

            auto ret = queue_.pop(in_isr, origin_data, timeout_tick);
            data = static_cast<ValueType>(origin_data);

            return ret;
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

    ErrorCode peek(bool in_isr, ValueType& data, MessageQueueTimeout_t timeout_tick = 0U)
    {
        QueueType_ origin_data;

        auto ret = queue_.peek(in_isr, origin_data, timeout_tick);
        data = static_cast<ValueType>(origin_data);

        return ret;
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
