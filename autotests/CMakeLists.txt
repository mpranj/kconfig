
remove_definitions(-DQT_NO_CAST_FROM_ASCII)

include(ECMAddTests)

find_package(Qt5Test ${REQUIRED_QT_VERSION} CONFIG QUIET)
find_package(Qt5Concurrent ${REQUIRED_QT_VERSION} CONFIG QUIET)
find_package(Elektra)

if(NOT Qt5Test_FOUND)
    message(STATUS "Qt5Test not found, autotests will not be built.")
    return()
endif()

if(NOT Qt5Concurrent_FOUND)
    message(STATUS "Qt5Concurrent not found, autotests will not be built.")
    return()
endif()

# compile KEntryMap into the test since it's not exported
set(kentrymaptest_SRCS kentrymaptest.cpp ../src/core/kconfigdata.cpp)
ecm_add_test(${kentrymaptest_SRCS}
  TEST_NAME kentrymaptest
  LINK_LIBRARIES Qt5::Test
)
target_include_directories(kentrymaptest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/core)

# compile KConfigUtils into the test since it's not exported
set(test_kconfigutils_SRCS test_kconfigutils ../src/kconf_update/kconfigutils.cpp)
ecm_add_test(${test_kconfigutils_SRCS}
  TEST_NAME test_kconfigutils
  LINK_LIBRARIES KF5::ConfigCore Qt5::Test
)
target_include_directories(test_kconfigutils PRIVATE ../src/kconf_update)

qt5_add_resources(sharedconfigresources sharedconfigresources.qrc)

ecm_add_test(ksharedconfigtest ${sharedconfigresources} TEST_NAME kconfigcore-ksharedconfigtest LINK_LIBRARIES KF5::ConfigCore Qt5::Test Qt5::Concurrent)

# test for fallback to :/kconfig/xxxx config resource
qt5_add_resources(fallbackconfigresources fallbackconfigresources.qrc)
ecm_add_test(fallbackconfigresourcestest ${fallbackconfigresources} TEST_NAME kconfigcore-fallbackconfigresourcestest LINK_LIBRARIES KF5::ConfigCore Qt5::Test Qt5::Concurrent)

ecm_add_tests(
   kconfignokdehometest.cpp
   kconfigtest.cpp
   kdesktopfiletest.cpp
   test_kconf_update.cpp
   ksharedconfig_in_global_object
   NAME_PREFIX kconfigcore-
   LINK_LIBRARIES KF5::ConfigCore Qt5::Test Qt5::Concurrent
)

if (ELEKTRA_FOUND AND NOT DEFINED SkipTest)
    include_directories(${ELEKTRA_INCLUDE_DIR})

    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")

    ecm_add_test(
            kconfigelektratest.cpp
            ../src/core/kconfigelektra.cpp
            ../src/core/kconfigdata.cpp
            ../src/core/kconfigbackend.cpp
            ../src/core/kconfigini.cpp
            ${CMAKE_CURRENT_BINARY_DIR}/../src/core/kconfig_core_log_settings.cpp
            TEST_NAME kconfigelektra
            NAME_PREFIX kconfigcore-
            LINK_LIBRARIES KF5::ConfigCore Qt5::Test Qt5::Concurrent ${ELEKTRA_LIBRARIES}
    )
    add_definitions(-DFEAT_ELEKTRA)
endif ()

target_include_directories(test_kconf_update PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../src/kconf_update)

if(TARGET Qt5::Gui)
ecm_add_tests(
   kconfigguitest.cpp
   kconfigloadertest.cpp
   kconfigskeletontest.cpp
   kstandardshortcuttest.cpp
   NAME_PREFIX kconfiggui-
   LINK_LIBRARIES KF5::ConfigGui Qt5::Test
)

# These tests do a global cleanup of ~/.qttest, so they can't run in parallel
set_tests_properties(kconfigcore-kconfigtest PROPERTIES RUN_SERIAL TRUE)
set_tests_properties(kconfigcore-kconfignokdehometest PROPERTIES RUN_SERIAL TRUE)
set_tests_properties(kconfiggui-kconfigguitest PROPERTIES RUN_SERIAL TRUE)

if (NOT CMAKE_CROSSCOMPILING)
    add_subdirectory(kconfig_compiler)
endif()
endif()
