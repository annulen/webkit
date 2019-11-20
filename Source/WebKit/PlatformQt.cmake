set(WebKit_WebProcess_OUTPUT_NAME QtWebProcess)
set(WebKit_NetworkProcess_OUTPUT_NAME QtWebNetworkProcess)
set(WebKit_PluginProcess_OUTPUT_NAME QtWebPluginProcess)
set(WebKit_DatabaseProcess_OUTPUT_NAME QtWebStorageProcess)

file(MAKE_DIRECTORY ${DERIVED_SOURCES_WEBKIT_DIR})

if (SHARED_CORE)
    set(WebKit_LIBRARY_TYPE SHARED)
else ()
    set(WebKit_LIBRARY_TYPE STATIC)
endif ()

add_definitions(-DBUILDING_WEBKIT)

if (${JavaScriptCore_LIBRARY_TYPE} MATCHES STATIC)
    add_definitions(-DSTATICALLY_LINKED_WITH_WTF -DSTATICALLY_LINKED_WITH_JavaScriptCore)
endif ()

QTWEBKIT_SKIP_AUTOMOC(WebKit)

#set(WebKit_USE_PREFIX_HEADER ON)

list(APPEND WebKit_INCLUDE_DIRECTORIES
    "${FORWARDING_HEADERS_DIR}"
    "${FORWARDING_HEADERS_DIR}/QtWebKit"

    "${WEBCORE_DIR}/platform/graphics/opentype"
    "${WEBCORE_DIR}/platform/graphics/qt"
    "${WEBCORE_DIR}/platform/network/qt"
    "${WEBCORE_DIR}/platform/qt"

    # The WebKit Qt APIs depend on qwebkitglobal.h, which lives in WebKitLegacy
    "${WEBKITLEGACY_DIR}/qt/Api"
    "${WEBKITLEGACY_DIR}/qt/Plugins"

    "${WEBKIT_DIR}/NetworkProcess/CustomProtocols/qt"
    "${WEBKIT_DIR}/NetworkProcess/qt"

    "${WEBKIT_DIR}/Shared/CoordinatedGraphics"
    "${WEBKIT_DIR}/Shared/CoordinatedGraphics/threadedcompositor"
    "${WEBKIT_DIR}/Shared/Plugins/unix"
    "${WEBKIT_DIR}/Shared/qt"
    "${WEBKIT_DIR}/Shared/unix"

    "${WEBKIT_DIR}/UIProcess/API/C/qt"
    "${WEBKIT_DIR}/UIProcess/API/qt"
    "${WEBKIT_DIR}/UIProcess/API/cpp/qt"
    "${WEBKIT_DIR}/UIProcess/CoordinatedGraphics"
    "${WEBKIT_DIR}/UIProcess/InspectorServer/qt"
    "${WEBKIT_DIR}/UIProcess/gstreamer"
    "${WEBKIT_DIR}/UIProcess/qt"

    "${WEBKIT_DIR}/WebProcess/Plugins/Netscape/unix"
    "${WEBKIT_DIR}/WebProcess/Plugins/Netscape/x11"
    "${WEBKIT_DIR}/WebProcess/WebCoreSupport/qt"
    "${WEBKIT_DIR}/WebProcess/WebPage/CoordinatedGraphics"
    "${WEBKIT_DIR}/WebProcess/qt"
)

list(APPEND WebKit_SOURCES
    #NetworkProcess/CustomProtocols/qt/CustomProtocolManagerQt.cpp

    NetworkProcess/cache/NetworkCacheDataQt.cpp
    NetworkProcess/cache/NetworkCacheIOChannelQt.cpp

    NetworkProcess/Downloads/qt/DownloadQt.cpp
    #NetworkProcess/Downloads/qt/QtFileDownloader.cpp

    NetworkProcess/qt/NetworkProcessMainQt.cpp
    NetworkProcess/qt/NetworkProcessQt.cpp
    NetworkProcess/qt/QtNetworkAccessManager.cpp
    NetworkProcess/qt/RemoteNetworkingContextQt.cpp

    Platform/qt/LoggingQt.cpp
    Platform/qt/ModuleQt.cpp

    PluginProcess/qt/PluginControllerProxyQt.cpp
    PluginProcess/qt/PluginProcessMainQt.cpp
    PluginProcess/qt/PluginProcessQt.cpp

    Shared/API/c/qt/WKImageQt.cpp

    #Shared/CoordinatedGraphics/CoordinatedBackingStore.cpp
    #Shared/CoordinatedGraphics/CoordinatedGraphicsArgumentCoders.cpp
    Shared/CoordinatedGraphics/CoordinatedGraphicsScene.cpp
    #Shared/CoordinatedGraphics/WebCoordinatedSurface.cpp

    Shared/Plugins/Netscape/unix/NetscapePluginModuleUnix.cpp

    Shared/Plugins/unix/PluginSearchPath.cpp

    #Shared/qt/ArgumentCodersQt.cpp
    Shared/qt/AuxiliaryProcessMainQt.cpp
    Shared/qt/NativeWebKeyboardEventQt.cpp
    Shared/qt/NativeWebMouseEventQt.cpp
    Shared/qt/NativeWebTouchEventQt.cpp
    Shared/qt/NativeWebWheelEventQt.cpp
    Shared/qt/ProcessExecutablePathQt.cpp
    Shared/qt/QtNetworkReplyData.cpp
    #Shared/qt/QtNetworkRequestData.cpp
    Shared/qt/ShareableBitmapQt.cpp
    Shared/qt/WebCoreArgumentCodersQt.cpp
    Shared/qt/WebEventFactoryQt.cpp
    Shared/qt/WebGestureEvent.cpp

    UIProcess/BackingStore.cpp
    UIProcess/DefaultUndoController.cpp
    UIProcess/LegacySessionStateCodingNone.cpp

    UIProcess/API/C/qt/WKIconDatabaseQt.cpp

    UIProcess/API/cpp/qt/WKStringQt.cpp
    UIProcess/API/cpp/qt/WKURLQt.cpp

    #UIProcess/API/qt/qquicknetworkreply.cpp
    UIProcess/API/qt/qquicknetworkrequest.cpp
    #UIProcess/API/qt/qquickurlschemedelegate.cpp
    UIProcess/API/qt/qquickwebpage.cpp
    UIProcess/API/qt/qquickwebview.cpp
    UIProcess/API/qt/qtwebsecurityorigin.cpp
    UIProcess/API/qt/qwebchannelwebkittransport.cpp
    UIProcess/API/qt/qwebdownloaditem.cpp
    UIProcess/API/qt/qwebdownloaditem_p.h
    UIProcess/API/qt/qwebdownloaditem_p_p.h
    #UIProcess/API/qt/qwebiconimageprovider.cpp
    #UIProcess/API/qt/qwebkittest.cpp
    #UIProcess/API/qt/qwebloadrequest.cpp
    UIProcess/API/qt/qwebnavigationhistory.cpp
    UIProcess/API/qt/qwebnavigationrequest.cpp
    UIProcess/API/qt/qwebpermissionrequest.cpp
    #UIProcess/API/qt/qwebpreferences.cpp

    UIProcess/CoordinatedGraphics/DrawingAreaProxyCoordinatedGraphics.cpp

    #UIProcess/InspectorServer/qt/WebInspectorServerQt.cpp
    #UIProcess/InspectorServer/qt/WebSocketServerQt.cpp

    UIProcess/Launcher/qt/ProcessLauncherQt.cpp

    UIProcess/Plugins/qt/PluginProcessProxyQt.cpp

    UIProcess/Plugins/unix/PluginInfoStoreUnix.cpp

    #UIProcess/Storage/StorageManager.cpp

    UIProcess/WebsiteData/unix/WebsiteDataStoreUnix.cpp
    UIProcess/WebSiteData/qt/WebSiteDataStoreQt.cpp

    UIProcess/gstreamer/InstallMissingMediaPluginsPermissionRequest.cpp
    UIProcess/gstreamer/WebPageProxyGStreamer.cpp

    #UIProcess/qt/BackingStoreQt.cpp
    #UIProcess/qt/ColorChooserContextObject.h
    #UIProcess/qt/DialogContextObjects.h
    #UIProcess/qt/ItemSelectorContextObject.cpp
    #UIProcess/qt/QrcSchemeHandler.cpp
    #UIProcess/qt/QtDialogRunner.cpp
    #UIProcess/qt/QtDownloadManager.cpp
    #UIProcess/qt/QtGestureRecognizer.cpp
    #UIProcess/qt/QtPageClient.cpp
    #UIProcess/qt/QtPanGestureRecognizer.cpp
    #UIProcess/qt/QtPinchGestureRecognizer.cpp
    #UIProcess/qt/QtTapGestureRecognizer.cpp
    ##UIProcess/qt/QtWebContext.cpp
    #UIProcess/qt/QtWebError.cpp
    #UIProcess/qt/QtWebIconDatabaseClient.cpp
    UIProcess/qt/QtWebPageEventHandler.cpp
    #UIProcess/qt/QtWebPagePolicyClient.cpp
    #UIProcess/qt/QtWebPageSGNode.cpp
    #UIProcess/qt/QtWebPageUIClient.cpp
    UIProcess/qt/TextCheckerQt.cpp
    #UIProcess/qt/WebColorPickerQt.cpp
    #UIProcess/qt/WebContextMenuProxyQt.cpp
    #UIProcess/qt/WebGeolocationProviderQt.cpp
    UIProcess/qt/WebInspectorProxyQt.cpp
    UIProcess/qt/WebPageProxyQt.cpp
    #UIProcess/qt/WebPopupMenuProxyQt.cpp
    UIProcess/qt/WebPreferencesQt.cpp
    UIProcess/qt/WebProcessPoolQt.cpp

    WebProcess/Cookies/qt/WebCookieManagerQt.cpp

    WebProcess/InjectedBundle/qt/InjectedBundleQt.cpp

    WebProcess/Plugins/Netscape/qt/PluginProxyQt.cpp

    WebProcess/Plugins/Netscape/unix/NetscapePluginUnix.cpp

    WebProcess/Plugins/Netscape/x11/NetscapePluginX11.cpp

    WebProcess/WebCoreSupport/qt/WebContextMenuClientQt.cpp
    WebProcess/WebCoreSupport/qt/WebDragClientQt.cpp
    WebProcess/WebCoreSupport/qt/WebEditorClientQt.cpp
    WebProcess/WebCoreSupport/qt/WebFrameNetworkingContext.cpp
    WebProcess/WebCoreSupport/qt/WebPopupMenuQt.cpp

    WebProcess/WebPage/CoordinatedGraphics/DrawingAreaCoordinatedGraphics.cpp
    WebProcess/WebPage/CoordinatedGraphics/LayerTreeHost.cpp
    WebProcess/WebPage/CoordinatedGraphics/CompositingCoordinator.cpp

    WebProcess/WebPage/gstreamer/WebPageGStreamer.cpp

    WebProcess/WebPage/qt/WebInspectorUIQt.cpp
    WebProcess/WebPage/qt/WebPageQt.cpp

    #WebProcess/qt/QtBuiltinBundle.cpp
    #WebProcess/qt/QtBuiltinBundlePage.cpp
    WebProcess/qt/SeccompFiltersWebProcessQt.cpp
    WebProcess/qt/WebProcessMainQt.cpp
    WebProcess/qt/WebProcessQt.cpp
)

if (USE_MACH_PORTS)
    list(APPEND WebKit_INCLUDE_DIRECTORIES
        "${WEBKIT_DIR}/Platform/IPC/cocoa"
        "${WEBKIT_DIR}/Platform/mac"
    )
    list(APPEND WebKit_SOURCES
        Platform/IPC/cocoa/ConnectionCocoa.mm

        Platform/cocoa/SharedMemoryCocoa.cpp

        Platform/mac/MachUtilities.cpp
    )
    list(APPEND WebKit_LIBRARIES
        objc
    )
elseif (WIN32)
    list(APPEND WebKit_SOURCES
        Platform/IPC/win/AttachmentWin.cpp
        Platform/IPC/win/ConnectionWin.cpp

        Platform/win/SharedMemoryWin.cpp
    )
else ()
    list(APPEND WebKit_INCLUDE_DIRECTORIES
        "${WEBKIT_DIR}/Platform/IPC/unix"
        "${WEBKIT_DIR}/UIProcess/linux"
    )
    list(APPEND WebKit_SOURCES
        Platform/IPC/unix/AttachmentUnix.cpp
        Platform/IPC/unix/ConnectionUnix.cpp

        Platform/unix/SharedMemoryUnix.cpp

        UIProcess/linux/MemoryPressureMonitor.cpp
    )
endif ()

if (ENABLE_NETSCAPE_PLUGIN_API)
    # We don't build PluginProcess on Win and Mac because we don't
    # support WK2 NPAPI on these platforms, however NPAPI works in WK1.
    # Some WK2 code is guarded with ENABLE(NETSCAPE_PLUGIN_API) now
    # so it should be compiled even when we don't want PluginProcess
    # Enabling PLUGIN_PROCESS without building PluginProcess executable
    # fixes things
    add_definitions(-DENABLE_PLUGIN_PROCESS=1)
endif ()

list(APPEND WebKit_SYSTEM_INCLUDE_DIRECTORIES
    ${GLIB_INCLUDE_DIRS}
    ${GSTREAMER_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${Qt5Quick_PRIVATE_INCLUDE_DIRS}
    ${SQLITE_INCLUDE_DIR}
)

list(APPEND WebKit_LIBRARIES
    ${Qt5Positioning_LIBRARIES}
    ${Qt5Quick_LIBRARIES}
    ${Qt5WebChannel_LIBRARIES}
    ${X11_X11_LIB}
)

# list(APPEND WebKit_MESSAGES_IN_FILES
#     UIProcess/CoordinatedGraphics/CoordinatedLayerTreeHostProxy.messages.in

#     WebProcess/WebPage/CoordinatedGraphics/CoordinatedLayerTreeHost.messages.in
# )

list(APPEND WebProcess_SOURCES
    qt/MainQt.cpp
)

if (NOT SHARED_CORE)
    set(WebProcess_LIBRARIES
        WebKitLegacy
    )
    set(NetworkProcess_LIBRARIES
        WebKitLegacy
    )
    set(DatabaseProcess_LIBRARIES
        WebKitLegacy
    )
    set(PluginProcess_LIBRARIES
        WebKitLegacy
    )
endif ()

# FIXME: Allow building without widgets
list(APPEND WebProcess_LIBRARIES
    Qt5::Widgets
    WebKitWidgets
)

list(APPEND NetworkProcess_SOURCES
    NetworkProcess/EntryPoint/qt/NetworkProcessMain.cpp
)

list(APPEND PluginProcess_SOURCES
    qt/PluginMainQt.cpp
)

add_custom_target(WebKit-forwarding-headers
    COMMAND ${PERL_EXECUTABLE} ${WEBKIT_DIR}/Scripts/generate-forwarding-headers.pl --include-path ${WEBKIT_DIR} --output ${FORWARDING_HEADERS_DIR} --platform qt
)

set(WEBKIT_EXTRA_DEPENDENCIES
     WebKit-forwarding-headers
)

set(WebKit_PUBLIC_FRAMEWORK_HEADERS
    Shared/API/c/WKArray.h
    Shared/API/c/WKBase.h
    Shared/API/c/WKCertificateInfo.h
    Shared/API/c/WKConnectionRef.h
    Shared/API/c/WKContextMenuItem.h
    Shared/API/c/WKContextMenuItemTypes.h
    Shared/API/c/WKData.h
    Shared/API/c/WKDeclarationSpecifiers.h
    Shared/API/c/WKDeprecated.h
    Shared/API/c/WKDiagnosticLoggingResultType.h
    Shared/API/c/WKDictionary.h
    Shared/API/c/WKErrorRef.h
    Shared/API/c/WKEvent.h
    Shared/API/c/WKFindOptions.h
    Shared/API/c/WKGeometry.h
    Shared/API/c/WKImage.h
    Shared/API/c/WKMutableArray.h
    Shared/API/c/WKMutableDictionary.h
    Shared/API/c/WKNumber.h
    Shared/API/c/WKPageLoadTypes.h
    Shared/API/c/WKPageLoadTypesPrivate.h
    Shared/API/c/WKPageVisibilityTypes.h
    Shared/API/c/WKPluginInformation.h
    Shared/API/c/WKSecurityOriginRef.h
    Shared/API/c/WKSerializedScriptValue.h
    Shared/API/c/WKString.h
    Shared/API/c/WKStringPrivate.h
    Shared/API/c/WKType.h
    Shared/API/c/WKURL.h
    Shared/API/c/WKURLRequest.h
    Shared/API/c/WKURLResponse.h
    Shared/API/c/WKUserContentInjectedFrames.h
    Shared/API/c/WKUserScriptInjectionTime.h

    UIProcess/API/C/WKAuthenticationChallenge.h
    UIProcess/API/C/WKAuthenticationDecisionListener.h
    UIProcess/API/C/WKBackForwardListItemRef.h
    UIProcess/API/C/WKBackForwardListRef.h
    UIProcess/API/C/WKContext.h
    UIProcess/API/C/WKContextConfigurationRef.h
    UIProcess/API/C/WKContextConnectionClient.h
    UIProcess/API/C/WKContextDownloadClient.h
    UIProcess/API/C/WKContextHistoryClient.h
    UIProcess/API/C/WKContextInjectedBundleClient.h
    UIProcess/API/C/WKContextPrivate.h
    UIProcess/API/C/WKCookieManager.h
    UIProcess/API/C/WKCredential.h
    UIProcess/API/C/WKCredentialTypes.h
    UIProcess/API/C/WKDownload.h
    UIProcess/API/C/WKFormSubmissionListener.h
    UIProcess/API/C/WKFrame.h
    UIProcess/API/C/WKFrameHandleRef.h
    UIProcess/API/C/WKFrameInfoRef.h
    UIProcess/API/C/WKFramePolicyListener.h
    UIProcess/API/C/WKGeolocationManager.h
    UIProcess/API/C/WKGeolocationPermissionRequest.h
    UIProcess/API/C/WKGeolocationPosition.h
    UIProcess/API/C/WKHTTPCookieStoreRef.h
    UIProcess/API/C/WKHitTestResult.h
    UIProcess/API/C/WKIconDatabase.h
    UIProcess/API/C/WKInspector.h
    UIProcess/API/C/WKLayoutMode.h
    UIProcess/API/C/WKMessageListener.h
    UIProcess/API/C/WKMockDisplay.h
    UIProcess/API/C/WKMockMediaDevice.h
    UIProcess/API/C/WKNativeEvent.h
    UIProcess/API/C/WKNavigationActionRef.h
    UIProcess/API/C/WKNavigationDataRef.h
    UIProcess/API/C/WKNavigationRef.h
    UIProcess/API/C/WKNavigationResponseRef.h
    UIProcess/API/C/WKNotification.h
    UIProcess/API/C/WKNotificationManager.h
    UIProcess/API/C/WKNotificationPermissionRequest.h
    UIProcess/API/C/WKNotificationProvider.h
    UIProcess/API/C/WKOpenPanelParametersRef.h
    UIProcess/API/C/WKOpenPanelResultListener.h
    UIProcess/API/C/WKPage.h
    UIProcess/API/C/WKPageConfigurationRef.h
    UIProcess/API/C/WKPageContextMenuClient.h
    UIProcess/API/C/WKPageDiagnosticLoggingClient.h
    UIProcess/API/C/WKPageFindClient.h
    UIProcess/API/C/WKPageFindMatchesClient.h
    UIProcess/API/C/WKPageFormClient.h
    UIProcess/API/C/WKPageGroup.h
    UIProcess/API/C/WKPageInjectedBundleClient.h
    UIProcess/API/C/WKPageLoaderClient.h
    UIProcess/API/C/WKPageNavigationClient.h
    UIProcess/API/C/WKPagePolicyClient.h
    UIProcess/API/C/WKPagePrivate.h
    UIProcess/API/C/WKPageRenderingProgressEvents.h
    UIProcess/API/C/WKPageStateClient.h
    UIProcess/API/C/WKPageUIClient.h
    UIProcess/API/C/WKPluginLoadPolicy.h
    UIProcess/API/C/WKPreferencesRef.h
    UIProcess/API/C/WKPreferencesRefPrivate.h
    UIProcess/API/C/WKProcessTerminationReason.h
    UIProcess/API/C/WKProtectionSpace.h
    UIProcess/API/C/WKProtectionSpaceTypes.h
    UIProcess/API/C/WKResourceCacheManager.h
    UIProcess/API/C/WKSessionStateRef.h
    UIProcess/API/C/WKTestingSupport.h
    UIProcess/API/C/WKTextChecker.h
    UIProcess/API/C/WKUserContentControllerRef.h
    UIProcess/API/C/WKUserContentExtensionStoreRef.h
    UIProcess/API/C/WKUserMediaPermissionCheck.h
    UIProcess/API/C/WKUserMediaPermissionRequest.h
    UIProcess/API/C/WKUserScriptRef.h
    UIProcess/API/C/WKViewportAttributes.h
    UIProcess/API/C/WKWebsiteDataStoreConfigurationRef.h
    UIProcess/API/C/WKWebsiteDataStoreRef.h
    UIProcess/API/C/WKWebsitePolicies.h
    UIProcess/API/C/WKWindowFeaturesRef.h
    UIProcess/API/C/WebKit2_C.h

    UIProcess/API/cpp/WKRetainPtr.h

    WebProcess/InjectedBundle/API/c/WKBundle.h
    WebProcess/InjectedBundle/API/c/WKBundleAPICast.h
    WebProcess/InjectedBundle/API/c/WKBundleBackForwardList.h
    WebProcess/InjectedBundle/API/c/WKBundleBackForwardListItem.h
    WebProcess/InjectedBundle/API/c/WKBundleDOMWindowExtension.h
    WebProcess/InjectedBundle/API/c/WKBundleFileHandleRef.h
    WebProcess/InjectedBundle/API/c/WKBundleFrame.h
    WebProcess/InjectedBundle/API/c/WKBundleFramePrivate.h
    WebProcess/InjectedBundle/API/c/WKBundleHitTestResult.h
    WebProcess/InjectedBundle/API/c/WKBundleInitialize.h
    WebProcess/InjectedBundle/API/c/WKBundleInspector.h
    WebProcess/InjectedBundle/API/c/WKBundleNavigationAction.h
    WebProcess/InjectedBundle/API/c/WKBundleNavigationActionPrivate.h
    WebProcess/InjectedBundle/API/c/WKBundleNodeHandle.h
    WebProcess/InjectedBundle/API/c/WKBundleNodeHandlePrivate.h
    WebProcess/InjectedBundle/API/c/WKBundlePage.h
    WebProcess/InjectedBundle/API/c/WKBundlePageBanner.h
    WebProcess/InjectedBundle/API/c/WKBundlePageContextMenuClient.h
    WebProcess/InjectedBundle/API/c/WKBundlePageEditorClient.h
    WebProcess/InjectedBundle/API/c/WKBundlePageFormClient.h
    WebProcess/InjectedBundle/API/c/WKBundlePageFullScreenClient.h
    WebProcess/InjectedBundle/API/c/WKBundlePageGroup.h
    WebProcess/InjectedBundle/API/c/WKBundlePageLoaderClient.h
    WebProcess/InjectedBundle/API/c/WKBundlePageOverlay.h
    WebProcess/InjectedBundle/API/c/WKBundlePagePolicyClient.h
    WebProcess/InjectedBundle/API/c/WKBundlePagePrivate.h
    WebProcess/InjectedBundle/API/c/WKBundlePageResourceLoadClient.h
    WebProcess/InjectedBundle/API/c/WKBundlePageUIClient.h
    WebProcess/InjectedBundle/API/c/WKBundlePrivate.h
    WebProcess/InjectedBundle/API/c/WKBundleRangeHandle.h
    WebProcess/InjectedBundle/API/c/WKBundleScriptWorld.h
)

set(WebKit_FORWARDING_HEADERS_DIRECTORIES
    Shared/API/c

    UIProcess/API/C
    UIProcess/API/cpp

    WebProcess/InjectedBundle/API/c
)

WEBKIT_MAKE_FORWARDING_HEADERS(WebKit
    TARGET_NAME WebKitFrameworkHeaders
    DESTINATION ${WebKit_FRAMEWORK_HEADERS_DIR}/WebKit
    FILES ${WebKit_PUBLIC_FRAMEWORK_HEADERS}
    FLATTENED
)

WEBKIT_CREATE_FORWARDING_HEADERS(QtWebKit/private DIRECTORIES UIProcess/API/qt)

if (ENABLE_API_TESTS)
    add_subdirectory(UIProcess/API/qt/tests)
endif ()

file(GLOB WebKit_PRIVATE_HEADERS UIProcess/API/qt/*_p.h)
install(
    FILES
       ${WebKit_PRIVATE_HEADERS}
    DESTINATION
        ${KDE_INSTALL_INCLUDEDIR}/QtWebKit/${PROJECT_VERSION}/QtWebKit/private
    COMPONENT Data
)
