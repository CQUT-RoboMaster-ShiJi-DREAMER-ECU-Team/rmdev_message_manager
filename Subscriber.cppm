/**
 * @file Subscriber.cppm
 * @brief 消息管理 - 订阅者
 */

module;

#include "emdevif/core/fatal_handler.h"

#include <cstddef>

#include <type_traits>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module rmdev.message_manager.subscriber;

import emdevif.core.data_container.message_queue;
import emdevif.core.error_handler;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "rmdev/message_manager/subscriber.hpp"
