cmake_minimum_required(VERSION 3.28...4.0.2)

project(rmdev_message_manager CXX)

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME}
    PUBLIC emdevif_core
    PRIVATE emdevif_compile_options
)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET ${PROJECT_NAME}_modules TYPE CXX_MODULES FILES
        message_manager.cppm Topic.cppm Subscriber.cppm
)
