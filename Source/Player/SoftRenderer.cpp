
#include "WindowsPrecompiled.h"
#include "SoftRenderer.h"
#include "WindowsRSI.h"
#include "MathHeaders.h"

void SoftRenderer::Initialize()
{
	RSI = new WindowsRSI();
	if (RSI != nullptr)
	{
		RSI->Init(false);
	}
}

void SoftRenderer::Shutdown()
{
	if (RSI != nullptr && RSI->IsIntialized())
	{
		RSI->Shutdown();
		delete RSI;
		RSI = nullptr;
	}
}

void SoftRenderer::Update()
{
	if (RSI != nullptr)
	{
		RSI->Clear(LinearColor(0.125f, 0.5f, 1.f, 1.f)); 

		//RenderCode

		VertexData v[4];
		v[0].pos = Vector2(100.0f, 100.0f);
		v[0].color = LinearColor(1.0f, 0.0f, 0.0f);
		v[1].pos = Vector2(-100.0f, -100.0f);
		v[1].color = LinearColor(0.0f, 1.0f, 0.0f);
		v[2].pos = Vector2(-100.0f, 100.0f);
		v[2].color = LinearColor(0.0f, 0.0f, 1.0f);
		v[3].pos = Vector2(100.0f, -100.0f);
		v[3].color = LinearColor(0.0f, 0.0f, 1.0f);

		int i[6];
		i[0] = 0;
		i[1] = 1;
		i[2] = 2;
		i[3] = 0;
		i[4] = 1;
		i[5] = 3;

		RSI->SetVertexBuffer(v);
		RSI->SetIndexBuffer(i);
		RSI->DrawPrimitive(4, 6);

		RSI->EndFrame();
	}
}


