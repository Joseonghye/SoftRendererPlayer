
#include "Precompiled.h"
#include "WindowsRSI.h"
#include "TriangleRasterizer.h"

void WindowsRSI::Init(const bool InbSRGBColorSpace)
{
	InitializeGDI(DisplaySetting::Inst().GetSize(), InbSRGBColorSpace);
}

void WindowsRSI::Shutdown()
{
	ReleaseGDI();
}

void WindowsRSI::Clear(const LinearColor & InClearColor)
{
	SetColor(InClearColor);
	FillBuffer();

	ClearDepthBuffer();
}

void WindowsRSI::BeginFrame()
{

}

void WindowsRSI::EndFrame()
{
	SwapBuffer();
}

void WindowsRSI::DrawScreenPoint(const ScreenPoint & InPoint, const LinearColor & InColor)
{
	PutPixel(InPoint,InColor.ToColor32());
}

void WindowsRSI::DrawVerticalLine(int InX, const LinearColor & InColor)
{
	int height = WindowsGDI::ScreenSize.Y;
	int width = WindowsGDI::ScreenSize.X;

	Color32 color = InColor.ToColor32();
	Color32* dest = ScreenBuffer;

	int xIndex = InX + Math::FloorToInt(((float)width - 1.f)*0.5f);

	for (int y = 0; y < height; y++)
	{
	//	WindowsGDI::ScreenBuffer[InX + width * y] = InColor.ToColor32(true);
		*(dest + xIndex) = color;
		dest += ScreenSize.X;
	}

}

void WindowsRSI::DrawHorizontalLine(int InY, const LinearColor & InColor)
{
	int width = WindowsGDI::ScreenSize.X;


	for (int x = 0; x < width; x++)
	{
		WindowsGDI::ScreenBuffer[InY*width + x] = InColor.ToColor32(true);
	}
}

void WindowsRSI::SetVertexBuffer(VertexData * InVertexData)
{
	verties = InVertexData;
}

void WindowsRSI::SetIndexBuffer(const int * InIndexData)
{
	indices = InIndexData;
}

void WindowsRSI::DrawPrimitive(UINT InVertexSize, UINT InIndexSize)
{

	if (verties == NULL || indices == NULL) return;

	UINT triangleCount = (int)(InIndexSize / 3);
	for (UINT ti = 0; ti < triangleCount; ti++)
	{
		TriangleRasterizer t(
			verties[indices[ti * 3]],
			verties[indices[ti * 3 + 1]],
			verties[indices[ti * 3 + 2]]
		);

		for (int x = t.TopLeft.X; x < t.BottomRight.X; x++)
		{
			for (int y = t.TopLeft.Y; y < t.BottomRight.Y; y++)
			{
				ScreenPoint curPixel(x, y);
				Vector2 curPos = curPixel.ToVector2();

				if (t.IsInside(curPos))
				{
					PutPixel(curPixel, t.GetColor(curPos));
				}
			}
		}
	}

	
}
