cmake_minimum_required(VERSION 3.22.1)

project(using-vulkan)

set(CMAKE_BUILD_TYPE Debug)

find_package(Vulkan REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_NAME "executable")

add_subdirectory(external/glfw)
add_subdirectory(external/glm)

add_executable(${EXECUTABLE_NAME}

    main.cpp

    src/header.cpp
    src/instance.cpp
    src/validation.cpp
    src/TriangleApplication.cpp
)

# target_compile_definitions(${EXECUTABLE_NAME} PUBLIC NDEBUG)

target_include_directories(${EXECUTABLE_NAME}
    
    # PRIVATE external/glfw/include # No need cause it is getting included automatically when we are linking to it
    PRIVATE external/glm

    PRIVATE src/include
)

target_link_libraries(${EXECUTABLE_NAME}
    PRIVATE glfw
    PRIVATE Vulkan::Vulkan
)