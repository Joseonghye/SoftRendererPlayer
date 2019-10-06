
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
	Color32 color = InColor.ToColor32();
	Color32* dest = ScreenBuffer;
	int startIndex = Math::FloorToInt(((float)ScreenSize.Y - 1.f) * 0.5f) - InY;
	dest = dest + startIndex * ScreenSize.X;
	for (int x = 0; x < ScreenSize.X; x++)
	{
		*dest = color;
		dest++;
	}
}

void WindowsRSI::DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const LinearColor& InColor)
{
	int x = InStartPos.X;
	int y = InStartPos.Y;

	bool chk = false;
	int w, h;
	if (InStartPos.X < InEndPos.X) {
		 w = InEndPos.X - InStartPos.X;
		if (InStartPos.Y < InEndPos.Y) {
			h = InEndPos.Y - InStartPos.Y;
		}
		else
		{
			 h = InStartPos.Y - InEndPos.Y;
		}
	}
	else {
		chk = true;
		w = InStartPos.X - InEndPos.X;
		if (InStartPos.Y < InEndPos.Y) {
			
			h = InEndPos.Y - InStartPos.Y;
		}
		else
		{
			h = InStartPos.Y - InEndPos.Y;
		}
	}
		

	
	if (w < 0) return;
	if (h < 0)return;

	int a = h / w;

	/*if (a <= 1) {
		int XF = 2 * h - w;

		int XdF1 = 2 * h;
		int XdF2 = 2 * (h - w);

		for (; x <= InEndPos.X; x++)
		{
			ScreenPoint p(x, y);
			DrawScreenPoint(p, InColor);

			if (XF < 0)
				XF += XdF1;
			else
			{
				y++;
				XF += XdF2;
			}
		}
	}
	else
	{*/

	if (a < 0)
	{// 오른쪽 위 / 왼쪽 아래
		if(!chk){
			int F = h - (2 * w);
			int dF1 = -2 * w;
			int dF2 = 2 * (h - w);
			for (; y >= InEndPos.Y; y--)
			{

				ScreenPoint p(x, y);
				DrawScreenPoint(p, InColor);

				if (F < 0)
					F -= dF1;
				else
				{
					x++;
					F -= dF2;
				}
			}
		}
		else
		{
			int F = -h - (2 * w);
			int dF1 = -2 * w;
			int dF2 = -2 * (h + w);
			for (; y >= InEndPos.Y; y--)
			{

				ScreenPoint p(x, y);
				DrawScreenPoint(p, InColor);

				if (F < 0)
					F -= dF1;
				else
				{
					x++;
					F -= dF2;
				}
			}
		}
	}

	else {
		//오른쪽 아래 / 왼쪽 위
		if (!chk) {
			int F = h - 2 * w;

			int dF1 = 2 * w;
			int dF2 = 2 * (w - h);
			for (; y <= InEndPos.Y; y++)
			{

				ScreenPoint p(x, y);
				DrawScreenPoint(p, InColor);

				if (F < 0)
					F += dF1;
				else
				{
					x++;
					F += dF2;
				}
			}
		}
		else {
			int F = -h + 2 * w;
			int dF1 = 2 * w;
			int dF2 = 2 * (w - h);
			for (; y <= InEndPos.Y; y++)
			{

				ScreenPoint p(x, y);
				DrawScreenPoint(p, InColor);

				if (F < 0)
					F += dF1;
				else
				{
					x--;
					F += dF2;
				}
			}
		}
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
