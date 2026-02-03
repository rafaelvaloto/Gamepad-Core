// Copyright (c) 2025 Rafael Valoto. All Rights Reserved.
// Project: GamepadCore
// Description: Cross-platform library for DualSense and generic gamepad input support.
// Targets: Windows, Linux, macOS.
#pragma once

// --- WINDOWS (MSVC / MinGW) ---
#if defined(_WIN32)
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#pragma warning(push)
#pragma warning(disable : 4456) // declaration hides previous local declaration (shadow)
#pragma warning(disable : 4245) // conversion from 'int' to 'ma_uint32', signed/unsigned mismatch
#pragma warning(disable : 4703) // potentially uninitialized local pointer variable
#endif

// --- LINUX & MAC (GCC / Clang) ---
#if defined(__unix__) || defined(__APPLE__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#if GAMEPAD_CORE_HAS_AUDIO
#include "miniaudio.h"
#endif

#if defined(_WIN32)
#pragma warning(pop)
#elif defined(__linux__) || defined(__APPLE__)
#pragma GCC diagnostic pop
#endif

namespace FGamepadAudio
{
}
