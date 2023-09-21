project(battle_tanks_client)

file(GLOB_RECURSE SOURCES "./*.cpp")

include_directories(${PROJECT_SOURCE_DIR}/headers)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)

add_executable (${PROJECT_NAME} ${SOURCES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/game_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        sfml-system
        sfml-network
        sfml-graphics
        sfml-window
        box2d::box2d
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../external/headers)