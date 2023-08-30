project(lesson_19)

add_executable (${PROJECT_NAME} "sources/HW_exercises.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()
