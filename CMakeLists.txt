cmake_minimum_required(VERSION 3.20.0)
project(RHI VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BACKEND_USE_VULKAN "Use Vulkan backend" ON)
option(BACKEND_USE_DX12 "Use D3D12 backend" OFF)

file(GLOB ASSEMBLY_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cc"
    "${PROJECT_SOURCE_DIR}/src/common/*.cc"
    "${PROJECT_SOURCE_DIR}/src/core/*.cc"
    "${PROJECT_SOURCE_DIR}/src/vk/*.cc"
    "${PROJECT_SOURCE_DIR}/src/vk/core/*.cc"
)

file(GLOB ASSEMBLY_HEADERS
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/common/*.h"
    "${PROJECT_SOURCE_DIR}/src/vk/*.h"
    "${PROJECT_SOURCE_DIR}/src/vk/core/*.h"
)

set(ASSEMBLY_INCLUDE_DIR
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}"
    "${PROJECT_SOURCE_DIR}/src"
)


add_library(${PROJECT_NAME}
    "${ASSEMBLY_SOURCES}"
    "${ASSEMBLY_HEADERS}"
)

if (BACKEND_USE_VULKAN)
    target_compile_definitions("${PROJECT_NAME}" PRIVATE BACKEND_USE_VULKAN)
    find_package(Vulkan REQUIRED)
    target_link_libraries("${PROJECT_NAME}" Vulkan::Vulkan Vulkan::Headers)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(${PROJECT_NAME} user32 gdi32)
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
    "${ASSEMBLY_INCLUDE_DIR}"
    # include any external dependency include directories here as well...
)

# Build examples
# Here you would do something similar to the above for each example executable you want.
# However, you can either create a CMakeLists.txt in the directory of that example and 
# use add_subdirectory(...) or you can just compile them here directly like we just did above.
# Remember to link any libraries you might need
add_subdirectory(examples/instance)

#Generate compiler commands for using clangd LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
