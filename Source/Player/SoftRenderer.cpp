
#include "WindowsPrecompiled.h"
#include "SoftRenderer.h"
#include "WindowsRSI.h"

void SoftRenderer::Initialize()
{
	LARGE_INTEGER frequency;
	if (!QueryPerformanceFrequency(&frequency))
	{
		return;
	}
	CyclesPerMilliSeconds = (float)(frequency.QuadPart / 1000.f);

	RSI = new WindowsRSI();
	if (RSI == nullptr)
	{
		return;
	}
	RSI->Init(false);
	MainTexture.LoadPNGFile("steve.png");
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

void SoftRenderer::PreUpdate()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	FrameTimeStamp = currentTime.QuadPart;
	if (FrameCount == 0)
	{
		StartTimeStamp = currentTime.QuadPart;
	}
}

void SoftRenderer::Update()
{
	if (RSI != nullptr)
	{
		// Render Start
		RSI->Clear(LinearColor(0.25f, 0.25f, 0.25f, 1.f));

		// Word Axis
		RSI->DrawVerticalLine(0, LinearColor(0.f, 1.f, 0.f, 1.f));
		RSI->DrawHorizontalLine(0, LinearColor(1.f, 0.f, 0.f, 1.f));

		// Grid Line
		ScreenPoint screenSize = DisplaySetting::Inst().GetSize();
		static int gridSize = 100;
		static LinearColor gridColor = LinearColor(0.5f, 0.5f, 0.5f, 1.f);
		int halfX = Math::FloorToInt(((float)screenSize.X - 1.f) * 0.5f);
		int halfY = Math::FloorToInt(((float)screenSize.Y - 1.f) * 0.5f);
		for (int ix = gridSize; ix < halfX; ix += gridSize)
		{
			RSI->DrawVerticalLine(ix, gridColor);
			RSI->DrawVerticalLine(-ix, gridColor);
		}

		for (int iy = gridSize; iy < halfY; iy += gridSize)
		{
			RSI->DrawHorizontalLine(iy, gridColor);
			RSI->DrawHorizontalLine(-iy, gridColor);
		}

		float deltaTime = FrameTime / 1000.f;

		float cos, sin = 0.f;
		Math::GetSinCos(cos, sin, 30.f);

	/*	float sineWave0To1 = (sinf(ElapsedTime / 1000 * Math::TwoPI) + 1) * 0.5f;
		float minScale = 0.8f;
		float maxScale = 1.6f;
		float currentScale = 0.5f + sineWave0To1 * (maxScale - minScale);*/

		static float moveSpeed = 100.f;
		static float rotateSpeed = 180.f;
		float currentScale = 100.f;
		 
		Vector2 deltaMove = Vector2(
			InputManager.GetXAxis() * moveSpeed, 
			InputManager.GetYAxis() * moveSpeed) * deltaTime;

		static GameObject2D camera;
		camera.GetTransform().AddPosition(deltaMove);

		static GameObject2D quad;
		quad.GetTransform().SetScale(Vector2::One * currentScale);
		quad.GetTransform().AddRotation(rotateSpeed * deltaTime);
		Matrix3x3 fMat = camera.GetTransform().GetViewMatrix() * quad.GetTransform().GetTRS();

		// Define Quad Data
		const int vertexCount = 4;
		VertexData v[vertexCount];
		v[0].Position = Vector2(-0.5f, -0.5f);
		v[0].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[0].UV = Vector2(0.125f, 0.25f);

		v[1].Position = Vector2(-0.5f, 0.5f);
		v[1].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[1].UV = Vector2(0.125f, 0.125f);

		v[2].Position = Vector2(0.5f, 0.5f);
		v[2].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[2].UV = Vector2(0.25f, 0.125f);

		v[3].Position = Vector2(0.5f, -0.5f);
		v[3].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[3].UV = Vector2(0.25f, 0.25f);
		const int indexCount = 6;
		int i[indexCount] = { 0, 1, 2, 0, 2, 3 };

		// Draw Call
		RSI->SetTexture(RSITexture(MainTexture.GetBuffer(), MainTexture.GetWidth(), MainTexture.GetHeight()));
		RSI->SetVertexBuffer(v);
		RSI->SetIndexBuffer(i);

		// Transform
		for (int i = 0; i < vertexCount; ++i)
		{
			v[i].Position = fMat * v[i].Position;
		}
		RSI->DrawPrimitive(vertexCount, indexCount);

	//	quad.GetTransform().AddPosition(deltaMove);
	//	quad.GetTransform().SetScale(Vector2::One * currentScale);
	//	quad.GetTransform().AddRotation(rotateSpeed * deltaTime);
	//	Matrix3x3 fMat = quad.GetTransform().GetTRS();
	//	Matrix3x3 fMat = Camera.GetTransform().GetViewMatrix() * quad.GetTransform().GetTRS();

		// Define Mesh Data
	/*
		VertexData v[9];
		v[0].Position = Vector2(-150.0f, -100.0f);
		v[0].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[1].Position = Vector2(-200.0f, 100.0f);
		v[1].Color = LinearColor(0.0f, 1.0f, 0.0f);
		v[2].Position = Vector2(-100.0f, 100.0f);
		v[2].Color = LinearColor(0.0f, 0.0f, 1.0f);

		v[3].Position = Vector2(150.0f, 100.0f);
		v[3].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[4].Position = Vector2(100.0f, -100.0f);
		v[4].Color = LinearColor(0.0f, 1.0f, 0.0f);
		v[5].Position = Vector2(200.0f, -100.0f);
		v[5].Color = LinearColor(0.0f, 0.0f, 1.0f);

		v[6].Position = Vector2(50.0f, 100.0f);
		v[6].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[7].Position = Vector2(-50.0f, 0.0f);
		v[7].Color = LinearColor(0.0f, 1.0f, 0.0f);
		v[8].Position = Vector2(0.0f, -100.0f);
		v[8].Color = LinearColor(0.0f, 0.0f, 1.0f);

		
		for (int i = 0; i < 9; ++i)
		{
			v[i].Position = fMat * v[i].Position;
		}
		

		int i[9];
		i[0] = 0;
		i[1] = 1;
		i[2] = 2;
		i[3] = 3;
		i[4] = 4;
		i[5] = 5;
		i[6] = 6;
		i[7] = 7;
		i[8] = 8;

		// Draw Call
		RSI->SetVertexBuffer(v);
		RSI->SetIndexBuffer(i);
		RSI->DrawPrimitive(9, 9);
		*/

		// Draw Line
		//RSI->DrawLine(Vector2(30.f, 30.f), Vector2(200.f, 50.f), LinearColor(0.f, 0.f, 1.f));
		//RSI->DrawLine(Vector2(30.f, 30.f), Vector2(50.f, 200.f), LinearColor(0.f, 0.f, 1.f));
		//RSI->DrawLine(Vector2(-30.f, 30.f), Vector2(-200.f, 50.f), LinearColor(0.f, 0.f, 1.f));
		//RSI->DrawLine(Vector2(-30.f, 30.f), Vector2(-50.f, 200.f), LinearColor(0.f, 0.f, 1.f));
		//RSI->DrawLine(Vector2(-30.f, -30.f), Vector2(-200.f, -50.f), LinearColor(0.f, 0.f, 1.f));
		//RSI->DrawLine(Vector2(-30.f, -30.f), Vector2(-50.f, -200.f), LinearColor(0.f, 0.f, 1.f));
		//RSI->DrawLine(Vector2(30.f, -30.f), Vector2(200.f, -50.f), LinearColor(0.f, 0.f, 1.f));
		//RSI->DrawLine(Vector2(30.f, -30.f), Vector2(50.f, -200.f), LinearColor(0.f, 0.f, 1.f));

		// Render Finish
		RSI->EndFrame();
	}
}

void SoftRenderer::PostUpdate()
{
	FrameCount++;

	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	LONGLONG frameCycles = currentTime.QuadPart - FrameTimeStamp;
	LONGLONG elapsedCycles = currentTime.QuadPart - StartTimeStamp;

	FrameTime = frameCycles / CyclesPerMilliSeconds;
	ElapsedTime = elapsedCycles / CyclesPerMilliSeconds;
	FrameFPS = FrameTime == 0.f ? 0.f : 1000.f / FrameTime;
	AverageFPS = ElapsedTime == 0.f ? 0.f : 1000.f / ElapsedTime * FrameCount;
}


