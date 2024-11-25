cmake_minimum_required(VERSION 3.29)

project(call)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)

add_executable(call src/call/cli/main.cpp)

add_library(core
        OBJECT
        src/call/core/lexer.cpp
        include/call/core/lexer.hpp
        include/call/core/settings.hpp
        src/call/core/parser.cpp
        include/call/core/parser.hpp
        include/call/core/action.hpp
        include/call/core/number.hpp
        include/call/core/call.hpp
        src/call/core/interpreter.cpp
        include/call/core/interpreter.hpp
        include/call/core/error.hpp
)

target_compile_options(core
        PRIVATE
        -Wall -fPIC
)

add_library(core-so
        SHARED
        $<TARGET_OBJECTS:core>)
target_link_libraries(call core-so)

project(call-ui)
add_executable(call-ui src/call/ui/main.cpp
        src/call/ui/lexer.cpp
        src/call/ui/lexer.h
        src/call/ui/lexer.ui)

find_package(Qt6 COMPONENTS
        Core
        Widgets
        REQUIRED)
target_link_libraries(call-ui core-so Qt6::Core Qt6::Widgets)
