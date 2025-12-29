// Copyright (c) 2025 Rafael Valoto. All Rights Reserved.
// Project: GamepadCore - Adapter Example
// Description: Example implementation of DeviceRegistry Policy for Tests.
#pragma once
#include <stdint.h>
struct FTest_DeviceRegistryPolicy
{
public:
	using EngineIdType = std::int32_t;

	struct Hasher {
		std::size_t operator()(const EngineIdType& Id) const
		{
			return Id;
		}
	};

	std::int32_t deviceId;

	EngineIdType AllocEngineDevice()
	{
		return deviceId++;
	}


	void DispatchNewGamepad(EngineIdType GamepadId)
	{
		std::cout << "DispatchNewGamepad... Id " << GamepadId << std::endl;
	}

	static void DisconnectDevice(EngineIdType GamepadId)
	{
		std::cout << "DisconnectDevice...Id " << GamepadId << std::endl;
	}

};
