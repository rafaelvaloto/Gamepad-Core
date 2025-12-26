#pragma once

#if _WIN32
#include <audioclient.h>
#include <mmdeviceapi.h>
#endif

#if _WIN32
#include <audioclient.h>
#include <mmdeviceapi.h>

#ifdef PlaySound
#undef PlaySound
#endif
#endif

struct FAudioDeviceContext
{
	FAudioDeviceContext() = default;

	~FAudioDeviceContext()
	{
		Close();
	}

	void Close()
	{
#if _WIN32
		if (RenderClient)
		{
			RenderClient->Release();
			RenderClient = nullptr;
		}
		if (AudioClient)
		{
			AudioClient->Stop();
			AudioClient->Release();
			AudioClient = nullptr;
		}
		if (Device)
		{
			Device->Release();
			Device = nullptr;
		}
#endif
	}

	bool IsValid() const
	{
#if _WIN32
		return Device != nullptr && AudioClient != nullptr && RenderClient != nullptr;
#endif
	}

public:
#if _WIN32
	IMMDevice* Device = nullptr;
	IAudioClient* AudioClient = nullptr;
	IAudioRenderClient* RenderClient = nullptr;
	WAVEFORMATEX* MixFormat = nullptr;
#endif

	int SampleRate = 48000;
	int NumChannels = 4;
};
