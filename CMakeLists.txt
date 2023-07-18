cmake_minimum_required (VERSION 3.20)

project(r_d_cpp_gamedev)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_subdirectory ("lesson_01")
add_subdirectory ("lesson_03")
add_subdirectory ("lesson_04")
add_subdirectory ("lesson_05")
add_subdirectory ("lesson_06")
add_subdirectory ("lesson_07")
add_subdirectory ("lesson_08")
add_subdirectory ("lesson_09")
