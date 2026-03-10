/**
 * @file message_manager.hpp
 * @brief
 */

#pragma once
#ifndef RMDEV_MESSAGE_MANAGER_HPP
    #define RMDEV_MESSAGE_MANAGER_HPP

    #include "emdevif/core/detail/config.hpp"

    #ifndef EMDEVIF_MODULE_INTERFACE_UNIT
        #include "rmdev/message_manager/subscriber.hpp"
        #include "rmdev/message_manager/topic.hpp"

        #include <cstddef>

        #include <string_view>
        #include <vector>
        #include <type_traits>
    #endif

EMDEVIF_MODULE_EXPORT
namespace rmdev {

}  // namespace rmdev

#endif  // !RMDEV_MESSAGE_MANAGER_HPP
