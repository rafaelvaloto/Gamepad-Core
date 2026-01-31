cmake_minimum_required(VERSION 3.20)

project(GamepadCoreIntegrationTest)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IMPORTANT: Add Unicode definitions to avoid conversion errors from CHAR* to LPCWSTR
# and to ensure linking with correct Windows API versions (CreateFileW, etc)
if(WIN32)
    add_compile_definitions(UNICODE _UNICODE)
endif()

# 1. Configure Platform Sources and Libs
set(PLATFORM_SOURCES "")
set(PLATFORM_LIBS "")

if(AUTOMATED_TESTS)
    add_compile_definitions(AUTOMATED_TESTS)
endif()
if(WIN32)
    # On Windows, we need to compile the WindowsDeviceInfo implementation
    # CMAKE_SOURCE_DIR points to project root (GamepadCore)
    list(APPEND PLATFORM_SOURCES
            "${CMAKE_SOURCE_DIR}/Examples/Platform_Windows/test_windows_device_info.cpp"
    )

    # Mandatory system libraries for HID and SetupAPI
    # + libraries required for miniaudio (WASAPI backend uses COM)
    list(APPEND PLATFORM_LIBS
        Setupapi
        Hid
        ole32      # COM - used by miniaudio/WASAPI
        oleaut32   # OLE Automation
        uuid       # GUIDs for COM interfaces
        winmm      # Multimedia APIs - required for miniaudio
    )

elseif(UNIX AND NOT APPLE)
    # Linux configuration (for future use)
    list(APPEND PLATFORM_SOURCES
            "${CMAKE_SOURCE_DIR}/Examples/Platform_Linux/test_linux_device_info.cpp"
    )
    find_package(SDL2 REQUIRED)
    list(APPEND PLATFORM_LIBS ${SDL2_LIBRARIES})
    include_directories(${SDL2_INCLUDE_DIRS})
endif()


# 2. Create Executables
# Include test-gamepad-outputs.cpp AND the platform implementation file (WindowsDeviceInfo.cpp)
add_executable(test-gamepad-outputs
        Features/test-gamepad-outputs.cpp
        ${PLATFORM_SOURCES}
)

# Audio Haptics Integration Test - Reads .wav file and sends to controller
add_executable(test-audio-haptics
        Features/test-audio-haptics.cpp
        ${PLATFORM_SOURCES}
)
target_compile_definitions(test-audio-haptics PRIVATE BUILD_GAMEPAD_CORE_TESTS)

# Channels Haptics Integration Test - Different WAV for different controllers
add_executable(test-channels-haptics
        Features/test-channels-haptics.cpp
        ${PLATFORM_SOURCES}
)
target_compile_definitions(test-channels-haptics PRIVATE BUILD_GAMEPAD_CORE_TESTS)

# Gamepad Input Integration Test
add_executable(test-gamepad-inputs
        Features/test-gamepad-inputs.cpp
        ${PLATFORM_SOURCES}
)

# 3. Configure Includes
# The test needs to find Core and Examples headers (where Policies are)
set(COMMON_INCLUDES
        "${CMAKE_SOURCE_DIR}/Source/Public"
        "${CMAKE_SOURCE_DIR}/Source/Private"
        "${CMAKE_SOURCE_DIR}/Examples/Platform_Linux"
        "${CMAKE_SOURCE_DIR}/Examples/Platform_Windows"
        "${CMAKE_SOURCE_DIR}"
)

target_include_directories(test-gamepad-outputs PRIVATE ${COMMON_INCLUDES})
target_include_directories(test-audio-haptics PRIVATE ${COMMON_INCLUDES})
target_include_directories(test-channels-haptics PRIVATE ${COMMON_INCLUDES})
target_include_directories(test-gamepad-inputs PRIVATE ${COMMON_INCLUDES})

# Register tests with CTest
if(BUILD_TESTS)
    add_test(NAME GamepadOutputs COMMAND test-gamepad-outputs)
    add_test(NAME AudioHaptics COMMAND test-audio-haptics)
    add_test(NAME GamepadInputs COMMAND test-gamepad-inputs)
endif()

# Add project root path as a compile definition
target_compile_definitions(test-audio-haptics PRIVATE GAMEPAD_CORE_PROJECT_ROOT="${CMAKE_SOURCE_DIR}")
target_compile_definitions(test-channels-haptics PRIVATE GAMEPAD_CORE_PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

# 4. Linking
# Link with static lib (GamepadCore) and Windows libs (Setupapi, Hid)
target_link_libraries(test-gamepad-outputs
        PRIVATE
        GamepadCore
        ${PLATFORM_LIBS}
)

target_link_libraries(test-audio-haptics
        PRIVATE
        GamepadCore
        ${PLATFORM_LIBS}
)

target_link_libraries(test-channels-haptics
        PRIVATE
        GamepadCore
        ${PLATFORM_LIBS}
)

target_link_libraries(test-gamepad-inputs
        PRIVATE
        GamepadCore
        ${PLATFORM_LIBS}
)
