remove_definitions(-DQT_ASCII_CAST_WARNINGS)

include_directories(
    "${CMAKE_SOURCE_DIR}/Source"
    "${FORWARDING_HEADERS_DIR}"
    "${WEBKIT_DIR}/qt/Api"
    "${WEBKIT_DIR}/qt/WidgetApi"
)

add_definitions(-DTESTS_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

set(3rdPartyLibTests_LIBRARIES
    ${Qt5Test_LIBRARIES}
    WebKitWidgets
)

set(3rdPartyLibTests_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/tests/bin/3rdpartylibs
)

set(3rdPartyLibTests
    icu
    jpeg
    png
    sqlite
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${3rdPartyLibTests_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${3rdPartyLibTests_RUNTIME_OUTPUT_DIRECTORY})

foreach (testName ${3rdPartyLibTests})
    list(APPEND tst_${testName}_SOURCES ${testName}/tst_${testName}.cpp)
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${testName}/tst_${testName}.qrc")
        qt5_add_resources(tst_${testName}_SOURCES ${testName}/tst_${testName}.qrc)
    endif ()

    add_executable(tst_${testName} ${tst_${testName}_SOURCES})
    target_include_directories(tst_${testName} PRIVATE ${testName})
    target_link_libraries(tst_${testName} ${3rdPartyLibTests_LIBRARIES})
    set_target_properties(tst_${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${3rdPartyLibTests_RUNTIME_OUTPUT_DIRECTORY} AUTOMOC ON)

    add_test(${testName} "${3rdPartyLibTests_RUNTIME_OUTPUT_DIRECTORY}/tst_${testName}")
    set_tests_properties(${testName} PROPERTIES TIMEOUT 60)
endforeach ()
