project(lesson_01)

add_executable (lesson_01 "lesson_01.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET lesson_01 PROPERTY CXX_STANDARD 20)
endif()
