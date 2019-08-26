#pragma once

class SoftRenderer
{
public:
	SoftRenderer(SoftRenderer const&) = delete;
	void operator=(SoftRenderer const&) = delete;
	static SoftRenderer& Inst()  //생성하려면Inst 밖에 없음 (생성 경로를 모두 막아둠)
	{
		static SoftRenderer instance;
		return instance;
	}

public:
	void Initialize(HWND InWnd);
	void Shutdown();
	void Update();

public:

private:
	SoftRenderer() { }
	~SoftRenderer() { Shutdown(); }
};
