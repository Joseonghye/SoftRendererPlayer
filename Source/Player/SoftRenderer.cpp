
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

		//Grid Line
		LinearColor Red(1.0f, 0.0f, 0.0f);
		RSI->DrawVerticalLine(0, Red);
		RSI->DrawHorizontalLine(0, Red);

		ScreenPoint screenSize = DisplaySetting::Inst().GetSize();
		int halfX = Math::FloorToInt(((float)screenSize.X - 1.f)*0.5f);
		int halfY = Math::FloorToInt(((float)screenSize.Y - 1.f)*0.5f);
		
		static int GridSize = 10;
		LinearColor color(0.5f, 0.5f, 0.5f);

		for (int x = 10; x <halfX; x += GridSize)
		{
				RSI->DrawVerticalLine(x, color);
				RSI->DrawVerticalLine(-x, color);
		}
		for (int y = 10; y < halfY; y += GridSize)
		{
			RSI->DrawHorizontalLine(y, color);
			RSI->DrawHorizontalLine(-y, color);
		}

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

	//	RSI->SetVertexBuffer(v);
	//	RSI->SetIndexBuffer(i);
	//	RSI->DrawPrimitive(4, 6);

		/*RSI->DrawLine(Vector2(50.0f, 50.0f), Vector2(100.f, 100.f), LinearColor(0.f, 1.f, 1.f));
		RSI->DrawLine(Vector2(30.f, 30.f), Vector2(100.f, 300.f), LinearColor(0.f, 0.f, 1.f));

		RSI->DrawLine(Vector2(-50.0f, 50.0f), Vector2(-100.f, 100.f), LinearColor(0.f, 1.f, 1.f));
		RSI->DrawLine(Vector2(-30.f, 30.f), Vector2(-100.f, 300.f), LinearColor(0.f, 0.f, 1.f));*/
		
		RSI->DrawLine(Vector2(50.0f, -50.0f), Vector2(100.f, -100.f), LinearColor(0.f, 1.f, 1.f));
		RSI->DrawLine(Vector2(30.f, -30.f), Vector2(100.f, -300.f), LinearColor(0.f, 0.f, 1.f));
		
		RSI->DrawLine(Vector2(-50.0f, -50.0f), Vector2(-100.f, -100.f), LinearColor(0.f, 1.f, 1.f));
		RSI->DrawLine(Vector2(-30.f, -30.f), Vector2(-100.f, -300.f), LinearColor(0.f, 0.f, 1.f));


		RSI->EndFrame();
	}
}


