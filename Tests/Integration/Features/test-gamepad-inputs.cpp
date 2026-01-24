// Copyright (c) 2025 Rafael Valoto. All Rights Reserved.
#ifdef BUILD_GAMEPAD_CORE_TESTS
#include "../../../Examples/Adapters/Tests/test_device_registry_policy.h"
#include "GCore/Interfaces/IPlatformHardwareInfo.h"
#include "GCore/Templates/TBasicDeviceRegistry.h"
#include "GCore/Types/Structs/Context/DeviceContext.h"
#include "GCore/Types/Structs/Context/InputContext.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

using TestDeviceRegistry = GamepadCore::TBasicDeviceRegistry<Ftest_device_registry_policy>;

#if _WIN32
#include "../../../Examples/Platform_Windows/test_windows_hardware_policy.h"
using TestHardwarePolicy = Ftest_windows_platform::Ftest_windows_hardware_policy;
using TestHardwareInfo = Ftest_windows_platform::Ftest_windows_hardware;
#elif __unix__
#include "../../../Examples/Platform_Linux/test_linux_hardware_policy.h"
using TestHardwarePolicy = Ftest_linux_platform::Ftest_linux_hardware_policy;
using TestHardwareInfo = Ftest_linux_platform::Ftest_linux_hardware;
#endif

int main(int argc, char* argv[])
{
	bool bLogButtons = false;
	bool bLogAnalogs = false;
	bool bLogTouch = false;
	bool bLogSensors = false;

	for (int i = 1; i < argc; ++i)
	{
		std::string_view arg(argv[i]);
		if (arg == "--buttons")
		{
			bLogButtons = true;
		}
		else if (arg == "--analogs")
		{
			bLogAnalogs = true;
		}
		else if (arg == "--touch")
		{
			bLogTouch = true;
		}
		else if (arg == "--sensors")
		{
			bLogSensors = true;
		}
	}

	// Default behavior if no flags are provided (keep backward compatibility or minimal log)
	if (!bLogButtons && !bLogAnalogs && !bLogTouch && !bLogSensors)
	{
		bLogAnalogs = true;
	}

	std::cout << "--- Gamepad Input Test ---" << std::endl;

	auto HardwareImpl = std::make_unique<TestHardwareInfo>();
	IPlatformHardwareInfo::SetInstance(std::move(HardwareImpl));

	auto Registry = std::make_unique<TestDeviceRegistry>();

	const int32_t TargetDeviceId = 0;
	bool bWasConnected = false;

#ifdef AUTOMATED_TESTS
	std::cout << "[Test] Automated mode active. The test will end in 5s." << std::endl;
	auto startTime = std::chrono::steady_clock::now();
#endif

	std::cout << "Reading inputs. Press Ctrl+C to stop." << std::endl;
	std::cout << std::fixed << std::setprecision(3);

	while (true)
	{
#ifdef AUTOMATED_TESTS
		auto now = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count() >= 5)
		{
			std::cout << "\n[Test] Timeout reached (5s). Finishing..." << std::endl;
			break;
		}
#endif
		float DeltaTime = 0.016f;
		Registry->PlugAndPlay(DeltaTime);

		ISonyGamepad* Gamepad = Registry->GetLibrary(TargetDeviceId);

		if (Gamepad && Gamepad->IsConnected())
		{
			if (!bWasConnected)
			{
				bWasConnected = true;
				std::cout << "\n>>> CONTROLLER CONNECTED! <<<" << std::endl;

				if (bLogTouch)
				{
					Gamepad->EnableTouch(true);
				}

				if (bLogSensors)
				{
					Gamepad->EnableMotionSensor(true);
				}

				Gamepad->SetLightbar({0, 255, 0}); // Green on connect
				Gamepad->UpdateOutput();
			}

			Gamepad->UpdateInput(DeltaTime);
			FDeviceContext* Context = Gamepad->GetMutableDeviceContext();
			FInputContext* Input = Context->GetInputState();

			if (Input)
			{
				std::cout << "\r";
				if (bLogAnalogs)
				{
					std::cout << "LStick: [" << std::setw(6) << Input->LeftAnalog.X << ", " << std::setw(6) << Input->LeftAnalog.Y << "] | "
					          << "RStick: [" << std::setw(6) << Input->RightAnalog.X << ", " << std::setw(6) << Input->RightAnalog.Y << "] | "
					          << "LTrig: " << std::setw(5) << Input->LeftTriggerAnalog << " | "
					          << "RTrig: " << std::setw(5) << Input->RightTriggerAnalog << " | ";
				}

				if (bLogButtons)
				{
					std::cout << "Btns: "
					          << (Input->bCross ? "X " : "_ ")
					          << (Input->bCircle ? "O " : "_ ")
					          << (Input->bTriangle ? "T " : "_ ")
					          << (Input->bSquare ? "S " : "_ ")
					          << (Input->bDpadUp ? "U " : "_ ")
					          << (Input->bDpadDown ? "D " : "_ ")
					          << (Input->bDpadLeft ? "L " : "_ ")
					          << (Input->bDpadRight ? "R " : "_ ")
					          << (Input->bLeftShoulder ? "L1 " : "__ ")
					          << (Input->bRightShoulder ? "R1 " : "__ ")
					          << (Input->bLeftStick ? "L3 " : "__ ")
					          << (Input->bRightStick ? "R3 " : "__ ")
					          << (Input->bShare ? "Sh " : "__ ")
					          << (Input->bStart ? "St " : "__ ")
					          << (Input->bPSButton ? "PS " : "__ ")
					          << (Input->bMute ? "M " : "_ ")
					          << "| ";
				}

				if (bLogTouch)
				{
					std::cout << "Touch: [" << (Input->bIsTouching ? "YES" : "NO ") << "] "
					          << "ID: " << std::setw(2) << Input->TouchId << " "
					          << "Fng: " << Input->TouchFingerCount << " "
					          << "Dir: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)Input->DirectionRaw << std::dec << std::setfill(' ') << " "
					          << "Pos: [" << std::setw(6) << Input->TouchPosition.X << ", " << std::setw(6) << Input->TouchPosition.Y << "] "
					          << "Rel: [" << std::setw(6) << Input->TouchRelative.X << ", " << std::setw(6) << Input->TouchRelative.Y << "] "
					          << "Rad: [" << std::setw(6) << Input->TouchRadius.X << ", " << std::setw(6) << Input->TouchRadius.Y << "] | ";
				}

				if (bLogSensors)
				{
					std::cout << "Gyro: [" << std::setw(6) << Input->Gyroscope.X << ", " << std::setw(6) << Input->Gyroscope.Y << ", " << std::setw(6) << Input->Gyroscope.Z << "] | "
					          << "Accel: [" << std::setw(6) << Input->Accelerometer.X << ", " << std::setw(6) << Input->Accelerometer.Y << ", " << std::setw(6) << Input->Accelerometer.Z << "] | ";
				}

				std::cout << std::flush;

				// Keep some original logic for visual feedback on controller
				if (Input->bCross)
				{
					Gamepad->SetLightbar({255, 0, 0});
					Gamepad->UpdateOutput();
				}
				else if (Input->bCircle)
				{
					Gamepad->SetLightbar({0, 0, 255});
					Gamepad->UpdateOutput();
				}
				else if (Input->bTriangle)
				{
					Gamepad->SetVibration(0, 0);
					Gamepad->SetLightbar({0, 0, 0});
					Gamepad->UpdateOutput();
				}
			}
		}
		else
		{
			if (bWasConnected)
			{
				bWasConnected = false;
				std::cout << "\n>>> CONTROLLER DISCONNECTED! <<<" << std::endl;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	return 0;
}
#endif
