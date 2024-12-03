cmake_minimum_required(VERSION 3.29)

project(call)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 20)

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


find_package(wxWidgets COMPONENTS net core base richtext stc)
IF (wxWidgets_FOUND)
    MESSAGE(STATUS "Found wxWidgets.")
ELSE (wxWidgets_FOUND)
    return()
ENDIF (wxWidgets_FOUND)

if (wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif ()

project(call-ui)
add_executable(call-ui src/call/ui/main.cpp
        src/call/ui/call_frame.cpp)
target_link_libraries(call-ui core-so)
target_link_libraries(call-ui ${wxWidgets_LIBRARIES})

