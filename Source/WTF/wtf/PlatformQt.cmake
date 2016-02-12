list(APPEND WTF_SOURCES
    text/qt/StringQt.cpp
)

if (WIN32)
    list(APPEND WTF_SOURCES
        win/MainThreadWin.cpp
        win/WorkItemWin.cpp
        win/WorkQueueWin.cpp
    )
else ()
    list(APPEND WTF_SOURCES
        qt/MainThreadQt.cpp
        qt/RunLoopQt.cpp
        qt/WorkQueueQt.cpp
    )
endif ()

list(APPEND WTF_SYSTEM_INCLUDE_DIRECTORIES
    ${Qt5Core_INCLUDE_DIRS}
)

list(APPEND WTF_LIBRARIES
    ${Qt5Core_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

if (WIN32)
    list(APPEND WTF_LIBRARIES
        winmm
    )

    set(WTF_POST_BUILD_COMMAND "${CMAKE_BINARY_DIR}/DerivedSources/WTF/postBuild.cmd")
    file(WRITE "${WTF_POST_BUILD_COMMAND}" "@xcopy /y /s /d /f \"${WTF_DIR}/wtf/*.h\" \"${DERIVED_SOURCES_DIR}/ForwardingHeaders/WTF\" >nul 2>nul\n@xcopy /y /s /d /f \"${DERIVED_SOURCES_DIR}/WTF/*.h\" \"${DERIVED_SOURCES_DIR}/ForwardingHeaders/WTF\" >nul 2>nul\n")
    file(MAKE_DIRECTORY ${DERIVED_SOURCES_DIR}/ForwardingHeaders/WTF)
endif ()
