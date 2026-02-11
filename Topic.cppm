/**
 * @file Topic.cppm
 * @brief
 */

module;

#include "emdevif/fatal_handler.h"

#include <cstddef>
#include <cstdint>

#include <string_view>
#include <vector>
#include <utility>

export module rmdev.messageManager.topic;

import emdevif.container.messageQueue;
import emdevif.container.messageSlot;
import emdevif.errorHandler;
import rmdev.messageManager.subscriber;

export namespace rmdev {

template<class QueueImpl, template<typename AllocatorType> typename Allocator = std::allocator>
    requires(emdevif::ValidMessageQueue<QueueImpl> || emdevif::ValidMessageSlot<QueueImpl>)
class Topic
{
public:
    using ValueType = QueueImpl::ValueType;

    static constexpr std::size_t queue_item_size = QueueImpl::item_size;

    static constexpr bool is_queue_not_slot = emdevif::IsMessageQueue_v<QueueImpl>;

    using QueueListType = std::vector<QueueImpl, Allocator<QueueImpl>>;
    using QueueListIterator = QueueListType::iterator;
    using QueueListConstIterator = QueueListType::const_iterator;

private:
    std::string_view name_;

    QueueListType queue_list_;

public:
    explicit Topic(const std::string_view name, std::size_t initial_queue_list_buffer_count = 3) : name_(name)
    {
        queue_list_.reserve(initial_queue_list_buffer_count);
    }

    QueueListType& getQueueListInstance()
    {
        return queue_list_;
    }
    const QueueListType& getQueueListInstance() const
    {
        return queue_list_;
    }

    std::string_view getName() const
    {
        return name_;
    }

    template<class SubValueType = ValueType>
    Subscriber<SubValueType, QueueImpl> addSubscriber()
    {
        queue_list_.emplace_back(QueueImpl::create({.name = ""}));
        return Subscriber<SubValueType, QueueImpl>{queue_list_.back()};
    }

    auto push(bool in_isr,
              const ValueType& data,
              emdevif::MessageQueueTimeout_t timeout_tick = 0U) -> std::pair<emdevif::ErrorCode, QueueListIterator>
    {
        using emdevif::ErrorCode;

        if constexpr (is_queue_not_slot) {
            for (auto it = queue_list_.begin(); it != queue_list_.end(); ++it) {
                if (ErrorCode ret = it->push(in_isr, data, timeout_tick); ret != ErrorCode::Success) {
                    return {ret, it};
                }
            }

            return {ErrorCode::Success, queue_list_.end()};
        }
        else {
            EMDEVIF_FATAL_HANDLER(
                "It's message slot, method 'push' was not supported. Please use 'forcePush' instead.");
            return {ErrorCode::NotImplemented, queue_list_.end()};
        }
    }

    auto forcePush(bool in_isr, const ValueType& data) -> std::pair<emdevif::ErrorCode, QueueListIterator>
    {
        using emdevif::ErrorCode;

        for (auto it = queue_list_.begin(); it != queue_list_.end(); ++it) {
            if (ErrorCode ret = it->forcePush(in_isr, data); ret != ErrorCode::Success) {
                return {ret, it};
            }
        }

        return {ErrorCode::Success, queue_list_.end()};
    }
};

}  // namespace rmdev
