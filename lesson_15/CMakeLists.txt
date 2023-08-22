project(lesson_15)

file(GLOB_RECURSE SOURCES_CPP "./*.cpp")
file(GLOB_RECURSE SOURCES_HPP "./*.hpp")

add_executable (${PROJECT_NAME} ${SOURCES_CPP} ${SOURCES_HPP})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE utils)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../hw-utils/headers)
