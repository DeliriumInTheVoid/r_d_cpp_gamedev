project(lesson_17)

file(GLOB_RECURSE SOURCES_CPP "./*.cpp")
file(GLOB_RECURSE SOURCES_HPP "./*.hpp")

add_executable (${PROJECT_NAME} ${SOURCES_CPP} ${SOURCES_HPP})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../hw-utils/headers"
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/VisualLeakDetector/include"
)

add_library(VLD SHARED IMPORTED)
set_target_properties(VLD PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../external/VisualLeakDetector/bin/Win64/vld_x64.dll"
    IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/../external/VisualLeakDetector/lib/Win64/vld.lib"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/../external/VisualLeakDetector/lib/Win64/vld.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/../external/VisualLeakDetector/bin/Win64/vld_x64.dll"
    "${CMAKE_CURRENT_SOURCE_DIR}/../external/VisualLeakDetector/bin/Win64/dbghelp.dll"
    "${CMAKE_CURRENT_SOURCE_DIR}/../external/VisualLeakDetector/bin/Win64/Microsoft.DTfW.DHL.manifest"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        utils
        VLD
)
