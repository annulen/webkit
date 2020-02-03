add_custom_target(WebKitTestRunner-forwarding-headers
    COMMAND ${PERL_EXECUTABLE} ${WEBKIT_DIR}/Scripts/generate-forwarding-headers.pl --include-path ${WEBKIT_TESTRUNNER_DIR} --include-path ${WEBKIT_TESTRUNNER_SHARED_DIR} --output ${FORWARDING_HEADERS_DIR} --platform qt
)

set(ForwardingHeadersForWebKitTestRunner_NAME WebKitTestRunner-forwarding-headers)

list(APPEND WebKitTestRunner_SOURCES
    qt/main.cpp
    qt/PlatformWebViewQt.cpp
    qt/TestControllerQt.cpp
    qt/EventSenderProxyQt.cpp
    qt/TestInvocationQt.cpp
)
message(${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR})
list(APPEND WebKitTestRunner_INCLUDE_DIRECTORIES
    ${FORWARDING_HEADERS_DIR}
    "${WEBKITLEGACY_DIR}/qt/WebCoreSupport"
)

list(APPEND WebKitTestRunner_SYSTEM_INCLUDE_DIRECTORIES
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${Qt5Quick_PRIVATE_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Widgets_PRIVATE_INCLUDE_DIRS}
)

#Target should be WTR or WebKitTestRunner?
QTWEBKIT_GENERATE_MOC_FILES_CPP(WebKit
qt/main.cpp
qt/PlatformWebViewQt.cpp
qt/TestControllerQt.cpp
)

list(APPEND WebKitTestRunner_LIBRARIES
    WebCore
    ${Qt5Core_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Quick_LIBRARIES}
    ${Qt5Test_LIBRARIES}
)

set(WebKitTestRunnerInjectedBundle_LIBRARIES
    WebCoreTestSupport
    WebKit
    ${Qt5Quick_LIBRARIES}
)

list(APPEND WebKitTestRunnerInjectedBundle_SOURCES
    ${WEBKIT_TESTRUNNER_INJECTEDBUNDLE_DIR}/qt/TestRunnerQt.cpp
)

add_definitions(
    -DTOP_LEVEL_DIR="${CMAKE_SOURCE_DIR}"
)
