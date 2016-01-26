set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_MICRO 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_MICRO})

add_definitions(-DBUILDING_QT__=1)

WEBKIT_OPTION_BEGIN()
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_DATABASE_PROCESS PUBLIC OFF)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_INDEXED_DATABASE PUBLIC OFF)
WEBKIT_OPTION_END()

set(ENABLE_WEBKIT OFF)
set(ENABLE_WEBKIT2 OFF)
set(ENABLE_WEBCORE OFF)
set(ENABLE_API_TESTS OFF)
set(WTF_USE_UDIS86 1)

if (ENABLE_FTL_JIT)
    if (WTF_CPU_X86_64)
        find_package(LLVM 3.6)
        if (NOT LLVM_FOUND)
            message(FATAL_ERROR "LLVM 3.6 is required for ENABLE_FTL_JIT")
        endif ()
        SET_AND_EXPOSE_TO_BUILD(HAVE_LLVM TRUE)
    else ()
        message(FATAL_ERROR "FTL is only supported for X86_64")
    endif ()
endif ()

find_package(Fontconfig 2.8.0 REQUIRED)
find_package(LibXml2 2.8.0 REQUIRED)
find_package(LibXslt 1.1.7 REQUIRED)
find_package(ICU REQUIRED)
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(Sqlite REQUIRED)
find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_package(WebP)

find_package(Qt5Core 5.2 REQUIRED)
find_package(Qt5Gui 5.2 REQUIRED)
find_package(Qt5Network 5.2 REQUIRED)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# TODO: figure out if we can run automoc only on Qt sources

# From OptionsEfl.cmake
# Optimize binary size for release builds by removing dead sections on unix/gcc.
if (CMAKE_COMPILER_IS_GNUCC AND UNIX AND NOT APPLE)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections -fdata-sections")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections -fdata-sections -fno-rtti")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -Wl,--gc-sections")
endif ()

# From OptionsGTK.cmake
if (CMAKE_MAJOR_VERSION LESS 3)
    # Before CMake 3 it was necessary to use a build script instead of using cmake --build directly
    # to preserve colors and pretty-printing.

    build_command(COMMAND_LINE_TO_BUILD)
    # build_command unconditionally adds -i (ignore errors) for make, and there's
    # no reasonable way to turn that off, so we just replace it with -k, which has
    # the same effect, except that the return code will indicate that an error occurred.
    # See: http://www.cmake.org/cmake/help/v3.0/command/build_command.html
    string(REPLACE " -i" " -k" COMMAND_LINE_TO_BUILD ${COMMAND_LINE_TO_BUILD})
    file(WRITE
        ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/build.sh
        "#!/bin/sh\n"
        "${COMMAND_LINE_TO_BUILD} $@"
    )
    file(COPY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/build.sh
        DESTINATION ${CMAKE_BINARY_DIR}
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE
    )
endif ()

# You can build JavaScriptCore as a static library if you specify it as STATIC
# set(JavaScriptCore_LIBRARY_TYPE STATIC)
