#pragma once

#include "WindowsRSI.h"

class SoftRenderer
{
public:
	enum class RenderMode
	{
		TWO,
		THREE_PERSP
	};

	SoftRenderer(SoftRenderer const&) = delete;
	void operator=(SoftRenderer const&) = delete;
	static SoftRenderer& Inst()
	{
		static SoftRenderer instance;
		return instance;
	}

public:
	void SetRenderMode(RenderMode InRenderMode) { CurrentRenderMode = InRenderMode; }
	RenderMode GetRenderMode() const { return CurrentRenderMode; }
	void Initialize();
	void Shutdown();
	void Update();

	float GetFrameFPS() { return FrameFPS;}
	float GetAverageFPS() { return FrameCount == 0 ? 0.0f : FrameCount / ElapsedTime; }
	float GetElapsedTime() const { return ElapsedTime; }
	int GetFrameCount() const { return FrameCount; }	

	void PreUpdate();
	void PostUpdate();

private:
	SoftRenderer() { }
	~SoftRenderer() { Shutdown(); }

	RenderMode CurrentRenderMode = RenderMode::TWO;

	WindowsRSI* RSI = nullptr;

//	double MilliSecFrequency = 0;
//	double FrameMilliSec = 0;
//	float FrameSec = 0;
	float FrameFPS = 0;
	float AverageFPS = 0;
	float ElapsedTime = 0;
	LONGLONG FrameTimeStamp = 0;
	LONGLONG StartTimeStamp = 0;
	LONGLONG CyclesPerMilliSeconds = 0;
	LONGLONG FrameTime = 0;
	LONGLONG ElapsedTimeInMs = 0;
	int FrameCount = 0;
};
