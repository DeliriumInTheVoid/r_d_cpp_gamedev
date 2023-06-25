project(lesson_03)

add_executable (lesson_03 "lesson_03.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET lesson_01 PROPERTY CXX_STANDARD 20)
endif()
