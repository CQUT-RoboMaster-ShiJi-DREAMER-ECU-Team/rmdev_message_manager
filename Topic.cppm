/**
 * @file Topic.cppm
 * @brief
 */

module;

#include "emdevif/core/fatal_handler.h"

#include <cstddef>
#include <cstdint>

#include <string_view>
#include <memory>
#include <vector>
#include <utility>

#define EMDEVIF_MODULE_INTERFACE_UNIT

export module rmdev.message_manager.topic;

import emdevif.core.data_container.message_queue;
import emdevif.core.error_handler;
import rmdev.message_manager.subscriber;

#ifdef __clang__
    #pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "rmdev/message_manager/topic.hpp"
