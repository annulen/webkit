set_property(DIRECTORY . PROPERTY FOLDER "WebKitLegacy")

set(WebKitLegacy_SOURCES
    Storage/InProcessIDBServer.cpp
    Storage/StorageAreaImpl.cpp
    Storage/StorageAreaSync.cpp
    Storage/StorageNamespaceImpl.cpp
    Storage/StorageSyncManager.cpp
    Storage/StorageThread.cpp
    Storage/StorageTracker.cpp
    Storage/WebDatabaseProvider.cpp
    Storage/WebStorageNamespaceProvider.cpp

    WebCoreSupport/NetworkStorageSessionMap.cpp
    WebCoreSupport/WebBroadcastChannelRegistry.cpp
    WebCoreSupport/WebResourceLoadScheduler.cpp
    WebCoreSupport/WebViewGroup.cpp
)

set(WebKitLegacy_PRIVATE_INCLUDE_DIRECTORIES
    "${CMAKE_BINARY_DIR}"
    "${WEBKITLEGACY_DIR}/Storage"
    "${WEBKITLEGACY_DIR}/WebCoreSupport"
    "${WebKitLegacy_DERIVED_SOURCES_DIR}"
)

set(WebKitLegacy_FRAMEWORKS
    JavaScriptCore
    PAL
    WTF
    WebCore
)
if (NOT USE_SYSTEM_MALLOC)
    list(APPEND WebKit_FRAMEWORKS bmalloc)
endif ()

set(WebKitLegacy_INTERFACE_LIBRARIES WebKitLegacy)
set(WebKitLegacy_INTERFACE_INCLUDE_DIRECTORIES ${WebKitLegacy_FRAMEWORK_HEADERS_DIR})
set(WebKitLegacy_INTERFACE_DEPENDENCIES WebKitLegacy_CopyHeaders)

WEBKIT_FRAMEWORK_DECLARE(WebKitLegacy)
WEBKIT_INCLUDE_CONFIG_FILES_IF_EXISTS()

set(WebKitLegacy_PRIVATE_HEADERS_LOCATION Headers/${PROJECT_VERSION}/QtWebKit/private)

WEBKIT_FRAMEWORK(WebKitLegacy)

if (${PORT} STREQUAL "Qt")
    set_target_properties(WebKitLegacy PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
    install(TARGETS WebKitLegacy EXPORT WebKitTargets
            DESTINATION "${LIB_INSTALL_DIR}"
            RUNTIME DESTINATION "${BIN_INSTALL_DIR}"
    )
    if (MSVC AND NOT QT_STATIC_BUILD)
        install(FILES $<TARGET_PDB_FILE:WebKitLegacy> DESTINATION "${BIN_INSTALL_DIR}" OPTIONAL)
    endif ()

    if (SEPARATE_DEBUG_INFO)
        QTWEBKIT_SEPARATE_DEBUG_INFO(WebKitLegacy WebKitLegacy_DEBUG_INFO)
        install(FILES ${WebKitLegacy_DEBUG_INFO} DESTINATION "${LIB_INSTALL_DIR}" OPTIONAL)
    endif ()

    if (NOT MSVC)
        ADD_PREFIX_HEADER(WebKitLegacy "${WEBKITLEGACY_DIR}/qt/WebKitPrefix.h")
    endif ()

    if (MACOS_BUILD_FRAMEWORKS)
        set_target_properties(WebKitLegacy PROPERTIES
            FRAMEWORK_VERSION ${PROJECT_VERSION_MAJOR}
            SOVERSION ${MACOS_COMPATIBILITY_VERSION}
            MACOSX_FRAMEWORK_IDENTIFIER org.qt-project.QtWebKit
        )
    endif ()

    if (USE_LINKER_VERSION_SCRIPT)
        set(VERSION_SCRIPT "${CMAKE_BINARY_DIR}/QtWebKit.version")
        add_custom_command(TARGET WebKitLegacy PRE_LINK
            COMMAND ${PERL_EXECUTABLE} ${TOOLS_DIR}/qt/generate-version-script.pl ${Qt5_VERSION} > ${VERSION_SCRIPT}
            VERBATIM
        )
    set_target_properties(WebKitLegacy PROPERTIES LINK_FLAGS -Wl,--version-script,${VERSION_SCRIPT})
    endif ()

    if (ENABLE_WEBKIT)
        add_dependencies(WebKitLegacy WebKit)
    endif ()
endif ()

if (TARGET WebKitLegacyGUID)
    add_dependencies(WebKitLegacy WebKitLegacyGUID)
endif ()
WEBKIT_COPY_FILES(WebKitLegacy_CopyHeaders
    DESTINATION ${WebKitLegacy_FRAMEWORK_HEADERS_DIR}/WebKitLegacy
    FILES ${WebKitLegacy_PUBLIC_FRAMEWORK_HEADERS}
    FLATTENED
)
