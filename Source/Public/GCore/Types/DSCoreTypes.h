// Copyright (c) 2025 Rafael Valoto. All Rights Reserved.
// Project: GamepadCore
// Description: Cross-platform library for DualSense and generic gamepad input support.
// Targets: Windows, Linux, macOS.

#pragma once

#include <algorithm>
#include <cmath>
#include <cmath> // std::sqrt, std::abs
#include <cstdint>
#include <cstring>
#include <memory> // std::unique_ptr, std::make_unique
#include <mutex>
#include <string>        // std::string
#include <unordered_map> // std::unordered_map
#include <unordered_set> // std::unordered_set
#include <vector>        // std::vector

#define DS_ACC_RES_PER_G 8192
#define DS_ACC_RANGE (4 * DS_ACC_RES_PER_G)
#define DS_GYRO_RES_PER_DEG_S 1024
#define DS_GYRO_RANGE (2048 * DS_GYRO_RES_PER_DEG_S)
#define GRAVITY_MS2 9.80665f
constexpr float DS_PI = 3.1415926535f;
constexpr float DS_RAD_TO_DEG = 180.0f / DS_PI;

namespace DSCoreTypes
{
	struct DSVector2D
	{
		float X = 0.0f;
		float Y = 0.0f;
	};

	struct DSVector3D
	{
		float X = 0.0f;
		float Y = 0.0f;
		float Z = 0.0f;
	};

	struct FDSColor
	{
		uint8_t R = 0;
		uint8_t G = 0;
		uint8_t B = 0;
		uint8_t A = 1;
	};

	namespace LedMasks
	{
		static constexpr std::uint8_t Left = 0x01;
		static constexpr std::uint8_t MiddleLeft = 0x02;
		static constexpr std::uint8_t Middle = 0x04;
		static constexpr std::uint8_t MiddleRight = 0x08;
		static constexpr std::uint8_t Right = 0x10;
	} // namespace LedMasks

	namespace InputMasks
	{
		// (Face Buttons)
		namespace Face
		{
			static constexpr std::uint8_t Square = 0x10;
			static constexpr std::uint8_t Cross = 0x20;
			static constexpr std::uint8_t Circle = 0x40;
			static constexpr std::uint8_t Triangle = 0x80;
		} // namespace Face

		// (D-Pad)
		namespace DPad
		{
			static constexpr std::uint8_t Up = 0x08;
			static constexpr std::uint8_t Right = 0x04;
			static constexpr std::uint8_t Down = 0x02;
			static constexpr std::uint8_t Left = 0x01;
		} // namespace DPad

		// Triggers (Shoulders)
		namespace Shoulder
		{
			static constexpr std::uint8_t L1 = 0x01;
			static constexpr std::uint8_t R1 = 0x02;
			static constexpr std::uint8_t L2 = 0x04;
			static constexpr std::uint8_t R2 = 0x08;
		} // namespace Shoulder

		// Btn Menu
		namespace Menu
		{
			static constexpr std::uint8_t Share = 0x10;    // Select / Create
			static constexpr std::uint8_t Options = 0x20;  // Start
			static constexpr std::uint8_t PS = 0x01;       // Logo
			static constexpr std::uint8_t TouchPad = 0x02; // Click Pad
			static constexpr std::uint8_t Mute = 0x04;     // Mic Button
		} // namespace Menu

		// Sticks (Click L3/R3)
		namespace Stick
		{
			static constexpr std::uint8_t L3 = 0x40;
			static constexpr std::uint8_t R3 = 0x80;
		} // namespace Stick

		// Extras (DualSense Edge / Pro Features)
		namespace Extra
		{
			static constexpr std::uint8_t FnLeft = 0x10;      // FN1
			static constexpr std::uint8_t FnRight = 0x20;     // FN2
			static constexpr std::uint8_t PaddleLeft = 0x40;  // Back Button Left
			static constexpr std::uint8_t PaddleRight = 0x80; // Back Button Right
		} // namespace Extra
	} // namespace InputMasks
} // namespace DSCoreTypes
