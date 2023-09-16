project(lesson_27)

file(GLOB_RECURSE SOURCES "./*.cpp")

include_directories(${PROJECT_SOURCE_DIR}/headers)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

add_executable (${PROJECT_NAME} ${SOURCES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        utils
        sfml-system
        sfml-network
        sfml-graphics
        sfml-window
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../hw-utils/headers)
