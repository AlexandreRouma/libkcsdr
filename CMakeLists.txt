cmake_minimum_required(VERSION 3.13)
project(libkcsdr VERSION 0.2.0)

# Options
option(BUILD_KCSDR_UTILS "Build KCSDR utilities such as kcsdr_info" ON)
option(INSTALL_UDEV_RULES "Install UDEV rules (Linux only)" ON)

# List all source files
file(GLOB_RECURSE SRC "src/*.c")

# On windows, all symbols must be exported
if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif ()

# Create dynamic libs
add_library(kcsdr SHARED ${SRC})

# Set optimisation flags
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    # Debug Flags
    if (MSVC)
        target_compile_options(kcsdr PRIVATE /EHsc)
    else ()
        target_compile_options(kcsdr PRIVATE -g -Og)
    endif ()
else()
    # Normal Flags
    if (MSVC)
        target_compile_options(kcsdr PRIVATE /O2 /Ob2 /EHsc)
    else ()
        target_compile_options(kcsdr PRIVATE -O3)
    endif ()
endif()

# Include the include folder
target_include_directories(kcsdr PUBLIC "include/")

# Link to FTD3XX
# TODO: GENERALIZE FOR LINUX
target_link_directories(kcsdr PUBLIC "vendor/FTD3XXLibrary_1.3.0.10/x64/DLL")
target_include_directories(kcsdr PUBLIC "vendor/FTD3XXLibrary_1.3.0.10")
target_link_libraries(kcsdr PUBLIC FTD3XX)

# TODO: Have it default instead of override
if (MSVC)
    set(CMAKE_INSTALL_PREFIX "C:/Program Files/Deepace/KCSDR/")
    set(CMAKE_INSTALL_BINDIR "bin")
    set(CMAKE_INSTALL_LIBDIR "lib")
    set(CMAKE_INSTALL_INCLUDEDIR "include")
else ()
    include(GNUInstallDirs)
endif ()

if (NOT MSVC)
    # Configure pkgconfig file
    configure_file(${CMAKE_SOURCE_DIR}/libkcsdr.pc.in ${CMAKE_CURRENT_BINARY_DIR}/libkcsdr.pc @ONLY)

    # Install pkgconfig file
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libkcsdr.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif ()

# Install the library
if (MSVC)
    install(TARGETS kcsdr)
else ()
    install(TARGETS kcsdr DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif ()

# Install the headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# # On Windows, install dependencies
# if (MSVC)
#     install(FILES $<TARGET_FILE_DIR:kcsdr>/FTD3XX.dll DESTINATION ${CMAKE_INSTALL_BINDIR})
# endif ()

# Build utils if enabled
if (BUILD_KCSDR_UTILS)
    add_subdirectory("utils/kcsdr_info")
endif ()

# Create uninstall target
configure_file(${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake @ONLY)
add_custom_target(uninstall ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)