target_sources(tello_test PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/command_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tello_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/swarm_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/video_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/status_response_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/environment.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/environment.cpp)
