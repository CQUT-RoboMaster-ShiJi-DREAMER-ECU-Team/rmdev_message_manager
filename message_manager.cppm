/**
 * @file message_manager.cppm
 * @brief 消息管理器，用于线程间/线程与中断之间通信
 */

module;

#include <cstddef>

#include <string_view>
#include <vector>
#include <type_traits>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module rmdev.message_manager;
export import rmdev.message_manager.subscriber;
export import rmdev.message_manager.topic;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "rmdev/message_manager.hpp"
