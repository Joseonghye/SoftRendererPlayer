
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
	
	Vector2 startPos = InStartPos.X < InEndPos.X ? InStartPos : InEndPos;
	Vector2 finishPos = InStartPos.X < InEndPos.X ? InEndPos : InStartPos;
	int x = startPos.X;
	int y = startPos.Y;

	int w = finishPos.X - startPos.X;
	int h = finishPos.Y - startPos.Y;

	
	if (h > 0) //1,3
	{
		if (w > h)
		{
			int F = h - 2 * w;

			int dF1 = 2 * w;
			int dF2 = 2 * (w - h);
			for (; y <= finishPos.Y; y++)
			{

				DrawScreenPoint(ScreenPoint(x, y), InColor);

				if (F < 0)
					F += dF1;
				else
				{
					x++;
					F += dF2;
				}
			}
		}
		else
		{
			if (w < 0) w *= -1;
			if (h < 0) h *= -1;
			int F = h - 2 * w;

			int dF1 = -2 * w;
			int dF2 = 2 * (h - w);

			for (; y <= finishPos.Y; y++)
			{
				DrawScreenPoint(ScreenPoint(x, y), InColor);

				if (F >= 0)
				{
					F += dF1;
				}
				else
				{
					++x;
					F += dF2;
				}
			}
		}
	}
	else // 2,4
	{
		if (h < 0)  h *= -1;
		int F = 2 * h - w;

		int dF1 = 2 * h;
		int dF2 = 2 * (h - w);

		for (; x <= finishPos.X; x++)
		{
			PutPixel(ScreenPoint(x, y), InColor);

			if (F < 0)
			{
				F += dF1;
			}
			else
			{
				--y;
				F += dF2;
			}
		}
	}

/*
	if (w < 0)
	{
		if( h < 0) // 3
		{
			w *= -1;
			h *= -1;
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
					x--;
					F -= dF2;
				}
			}
		}
		else // 4
		{
			w *= -1;
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
	else
	{
		if (h < 0) //2
		{
			h *= -1;
			//2
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
		else //1
		{
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
	}
	*/
}

void WindowsRSI::SetVertexBuffer(VertexData * InVertexData)
{
	verties = InVertexData;
}

void WindowsRSI::SetIndexBuffer(const int * InIndexData)
{
	indices = InIndexData;
}

void WindowsRSI::DrawBottomFlatTriangle(VertexData * tvs)
{
	float dx1 = tvs[1].pos.X - tvs[0].pos.X;
	float dx2 = tvs[2].pos.X - tvs[0].pos.X;
	float dy = tvs[0].pos.Y - tvs[1].pos.Y;

	// y 값이 같음  = 삼각형X
	if (dy <= 0) return;

	//기울기
	float gradient1 = dx1 / dy;
	float gradient2 = dx2 / dy;

	PutPixel(ScreenPoint(tvs[0].pos), LinearColor(0.f, 1.f, 0.f));
	float startY = tvs[0].pos.Y;
	float startX = tvs[0].pos.X;
	float curY = floorf(tvs[0].pos.Y) - 0.5f;
	float destY = tvs[1].pos.Y;

	while (curY >= destY)
	{
		float deltaY = startY - curY;
		float leftX = gradient1 * deltaY + startX;
		float rightX = gradient2 * deltaY + startX;
		int startX = Math::FloorToInt(leftX);
		int endX = Math::FloorToInt(rightX);
		int pixelY = Math::FloorToInt(curY);
		for (int p = startX; p <= endX; ++p)
		{
			PutPixel(ScreenPoint(p, pixelY), LinearColor(0.f, 1.f, 0.f));
		}
		curY -= 1.0f;
	}

	// 마지막 라인을 그린다.
	int pixelX1 = Math::FloorToInt(tvs[0].pos.X);
	int pixelX2 = Math::FloorToInt(tvs[1].pos.X);
	int pixelY = Math::FloorToInt(destY);
	for (int p = pixelX1; p <= pixelX2; ++p)
	{
		PutPixel(ScreenPoint(p, pixelY), LinearColor(0.f, 1.f, 0.f));
	}
}

void WindowsRSI::DrawTopFlatTriangle(VertexData * tvs, bool DrawLastLine)
{
	float dx1 = tvs[0].pos.X - tvs[2].pos.X;
	float dx2 = tvs[1].pos.X - tvs[2].pos.X;
	float dy = tvs[2].pos.Y - tvs[1].pos.Y;

	if (dy >= 0)
	{
		return;
	}

	float gradient1 = dx1 / dy;
	float gradient2 = dx2 / dy;

	PutPixel(ScreenPoint(tvs[2].pos), LinearColor(1.f, 0.f, 0.f));
	float startY = tvs[2].pos.Y;
	float startX = tvs[2].pos.X;
	float currentY = floorf(tvs[2].pos.Y) - 0.5f;
	float destY = tvs[1].pos.Y;
	while (currentY <= destY)
	{
		float deltaY = startY - currentY;
		float leftX = gradient1 * deltaY + startX;
		float rightX = gradient2 * deltaY + startX;
		int pixelX1 = Math::FloorToInt(leftX);
		int pixelX2 = Math::FloorToInt(rightX);
		int pixelY = Math::FloorToInt(currentY);
		for (int p = pixelX1; p <= pixelX2; ++p)
		{
			PutPixel(ScreenPoint(p, pixelY), LinearColor(1.f, 0.f, 0.f));
		}
		currentY += 1.0f;
	}

	if (DrawLastLine)
	{
		// 마지막 라인을 그린다.
		int pixelX1 = Math::FloorToInt(tvs[1].pos.X);
		int pixelX2 = Math::FloorToInt(tvs[2].pos.X);
		int pixelY = Math::FloorToInt(destY);
		for (int p = pixelX1; p <= pixelX2; ++p)
		{
			PutPixel(ScreenPoint(p, pixelY), LinearColor(1.f, 0.f, 0.f));
		}
	}
}

void WindowsRSI::DrawPrimitive(UINT InVertexSize, UINT InIndexSize)
{

	if (verties == NULL || indices == NULL) return;

	UINT triangleCount = (int)(InIndexSize / 3);
	for (UINT ti = 0; ti < triangleCount; ti++)
	{
		VertexData tv[3] = {
			verties[indices[ti * 3]],
			verties[indices[ti * 3 + 1]],
			verties[indices[ti * 3 + 2]]
		};

		VertexData tmp;

		// 정점 소팅
		// 1-1. 0번과 1번의 Y값 비교
		if (tv[0].pos.Y == tv[1].pos.Y)
		{
			// X 값을 비교.
			if (tv[0].pos.X > tv[1].pos.X)
			{
				// 0번과 1번을 Swap
				tmp = tv[0];
				tv[0] = tv[1];
				tv[1] = tmp;
			}
		}
		else
		{
			// Y값 비교
			if (tv[0].pos.Y < tv[1].pos.Y)
			{
				// 0번과 1번을 Swap
				tmp = tv[0];
				tv[0] = tv[1];
				tv[1] = tmp;
			}
		}
		// 1-2. 1,2번 Y비교
		if (tv[1].pos.Y == tv[2].pos.Y)
		{
			// X 값을 비교.
			if (tv[1].pos.X > tv[2].pos.X)
			{
				// 1번과 2번을 Swap
				tmp = tv[1];
				tv[1] = tv[2];
				tv[2] = tmp;
			}
		}
		else
		{
			if (tv[1].pos.Y < tv[2].pos.Y)
			{
				// 1번과 2번을 Swap
				tmp = tv[1];
				tv[1] = tv[2];
				tv[2] = tmp;
			}
		}
		if (tv[0].pos.Y == tv[1].pos.Y)
		{
			// X 값을 비교.
			if (tv[0].pos.X > tv[1].pos.X)
			{
				// 0번과 1번을 Swap
				tmp = tv[0];
				tv[0] = tv[1];
				tv[1] = tmp;
			}
		}
		else
		{
			if (tv[0].pos.Y < tv[1].pos.Y)
			{
				// 0번과 1번을 Swap
				tmp = tv[0];
				tv[0] = tv[1];
				tv[1] = tmp;
			}
		}

		//2 패턴파악
		// 2-1. Top-Flat
		if (tv[0].pos.Y == tv[1].pos.Y)
		{
			DrawTopFlatTriangle(tv,true);
		}
		// 2-2. Bottom-Flat
		else if (tv[1].pos.Y == tv[2].pos.Y)
		{
			DrawBottomFlatTriangle(tv);
		}
		else
		{
			// 삼각형을 두 개로 쪼갠다.
			VertexData newV = tv[1];
			float height = tv[0].pos.Y - tv[2].pos.Y;
			float width = tv[2].pos.X - tv[0].pos.X;

			if (height == 0.0f)
			{
				return;
			}

			float gradient = width / height;
			newV.pos.X = gradient * (tv[0].pos.Y - tv[1].pos.Y) + tv[0].pos.X;

			if (newV.pos.X > tv[1].pos.X)
			{
				VertexData upperTriangle[3] = { tv[0], tv[1], newV };
				VertexData bottomTriangle[3] = { tv[1], newV, tv[2] };
				DrawTopFlatTriangle(bottomTriangle,false);
				DrawBottomFlatTriangle(upperTriangle);
			}
			else
			{
				VertexData upperTriangle[3] = { tv[0], newV, tv[1] };
				VertexData bottomTriangle[3] = { newV, tv[1], tv[2] };
				DrawTopFlatTriangle(bottomTriangle,false);
				DrawBottomFlatTriangle(upperTriangle);
			}
		}
		


	/*	for (int x = t.TopLeft.X; x < t.BottomRight.X; x++)
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
		}*/
	}

	
}
