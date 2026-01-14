cmake_minimum_required(VERSION 3.20)

project(GamepadAppDLL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32 AND USE_VIGEM)
    set(MOD_SOURCES
            DualSenseMods.cpp
            ../Platform_Windows/test_windows_device_info.cpp
            ../Platform_Windows/ViGEmAdapter/ViGEmAdapter.cpp
    )

    add_subdirectory(../Examples/Platform_Windows/ViGEmAdapter/ViGEmClient-master ViGEmClient)

    add_library(GamepadModCore SHARED ${MOD_SOURCES})

    target_include_directories(GamepadModCore PRIVATE
            ../Examples/Platform_Windows/ViGEmAdapter/ViGEmClient-master/include
    )

    target_compile_definitions(GamepadModCore PRIVATE
        BUILD_GAMEPAD_CORE_TESTS
        USE_VIGEM
    )

    target_link_libraries(GamepadModCore PRIVATE
            GamepadCore
            Setupapi
            Hid
            dxgi
            ole32
            oleaut32
            uuid
            winmm
            ViGEmClient::ViGEmClient
    )

    set_target_properties(GamepadModCore PROPERTIES
            OUTPUT_NAME "DualSenseModCore"
            PREFIX ""
    )
else()
    message(STATUS "GamepadModCore (DualSenseModCore) is disabled (requires Windows and USE_VIGEM=ON)")
endif()