
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
