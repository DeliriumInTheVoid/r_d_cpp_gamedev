project(common-logic)
cmake_minimum_required (VERSION 3.20)

set(LIB_NAME bt-common)

include_directories(${PROJECT_SOURCE_DIR}/headers)
file(GLOB_RECURSE SOURCES_CPP "./*.cpp")

find_package(SFML COMPONENTS system CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(${LIB_NAME} STATIC ${SOURCES_CPP})

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD 20)
endif()

set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${LIB_NAME} PUBLIC include)

target_link_libraries(${LIB_NAME}
    PUBLIC
        sfml-system
        sfml-network
        box2d::box2d
        nlohmann_json::nlohmann_json
)