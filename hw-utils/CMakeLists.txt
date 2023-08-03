project(hw-utils)

file(GLOB_RECURSE SOURCES_CPP "./*.cpp")
file(GLOB_RECURSE SOURCES_HPP "./*.hpp")

add_library(utils STATIC ${SOURCES_CPP} ${SOURCES_HPP})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET utils PROPERTY CXX_STANDARD 20)
endif()
