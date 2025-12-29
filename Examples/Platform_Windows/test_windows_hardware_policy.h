// Copyright (c) 2025 Rafael Valoto. All Rights Reserved.
// Project: GamepadCore
// Description: Cross-platform library for DualSense and generic gamepad input support.
// Targets: Windows, Linux, macOS.
#ifdef BUILD_GAMEPAD_CORE_TESTS
#pragma once
#include "test_windows_device_info.h"
#include "GCore/Templates/TGenericHardwareInfo.h"

namespace Ftest_windows_platform
{
	struct Ftest_windows_hardware_policy;
	using Ftest_windows_hardware = GamepadCore::TGenericHardwareInfo<Ftest_windows_hardware_policy>;
	
	struct Ftest_windows_hardware_policy
	{
	public:
		void Read(FDeviceContext* Context)
		{
			Ftest_windows_device_info::Read(Context);
		}

		void Write(FDeviceContext* Context)
		{
			Ftest_windows_device_info::Write(Context);
		}

		void Detect(std::vector<FDeviceContext>& Devices)
		{
			Ftest_windows_device_info::Detect(Devices);
		}

		bool CreateHandle(FDeviceContext* Context)
		{
			return Ftest_windows_device_info::CreateHandle(Context);
		}

		void InvalidateHandle(FDeviceContext* Context)
		{
			Ftest_windows_device_info::InvalidateHandle(Context);
		}

		void ProcessAudioHaptic(FDeviceContext* Context)
		{
			Ftest_windows_device_info::ProcessAudioHapitc(Context);
		}

		void InitializeAudioDevice (FDeviceContext* Context)
		{
		}
	};
}
#endif
