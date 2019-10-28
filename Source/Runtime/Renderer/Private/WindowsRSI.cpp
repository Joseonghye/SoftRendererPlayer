
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
	PutPixel(InPoint, InColor.ToColor32());
}

int WindowsRSI::SetTexture(RSITexture & InRSITexture)
{

	MainTexture.TextureBuffer = InRSITexture.TextureBuffer;
	MainTexture.Height = InRSITexture.Height;
	MainTexture.Width = InRSITexture.Width;

	HasTexture = true;

	return 0;
}

LinearColor WindowsRSI::DrawPoint(ScreenPoint & InScreenPoint, LinearColor & InColor)
{
	return LinearColor();
}

LinearColor WindowsRSI::GetTextureSample(const Vector2 & InUV)
{
	UINT width = MainTexture.Width;
	UINT height = MainTexture.Height;

	UINT pixelX = Math::FloorToInt(InUV.X * width);
	UINT pixelY = Math::FloorToInt(InUV.Y * height);

	pixelX %= width;
	pixelY %= height;

	UINT textureIndex = width * pixelY + pixelX;
	return MainTexture.TextureBuffer[textureIndex];
}


void WindowsRSI::SetVertexBuffer(VertexData * InVertexData)
{
	VertexBuffer = InVertexData;
}

void WindowsRSI::SetIndexBuffer(const int * InIndexData)
{
	IndexBuffer = InIndexData;
}

void WindowsRSI::DrawPrimitive(UINT InVertexSize, UINT InIndexSize)
{
	if (VertexBuffer == nullptr || IndexBuffer == nullptr)
	{
		return;
	}

	UINT triangleCount = (int)(InIndexSize / 3);
	for (UINT ti = 0; ti < triangleCount; ti++)
	{
		VertexData tv[3] = {
			VertexBuffer[IndexBuffer[ti * 3]],
			VertexBuffer[IndexBuffer[ti * 3 + 1]],
			VertexBuffer[IndexBuffer[ti * 3 + 2]]
		};
		VertexData temp;

		//	// TO-DO. 퇴화 삼각형인지 식별.

			// 1. 삼각형 정점 소팅
		bool normal = false;
		if (tv[0].Position.Y < tv[1].Position.Y)
		{
			if (tv[0].Position.Y < tv[2].Position.Y)
			{
				if (tv[1].Position.Y < tv[2].Position.Y)
				{
					temp = tv[0];
					tv[0] = tv[2];
					tv[2] = tv[0];
				}
				else if (tv[1].Position.Y > tv[2].Position.Y)
				{
					temp = tv[0];
					tv[0] = tv[1];
					tv[1] = tv[2];
					tv[2] = temp;
				}
				else
				{
					if (tv[1].Position.X < tv[2].Position.X)
					{
						temp = tv[0];
						tv[0] = tv[1];
						tv[1] = tv[2];
						tv[2] = temp;
					}
					else
					{
						temp = tv[0];
						tv[0] = tv[2];
						tv[2] = temp;
					}
				}
			}
			else if (tv[0].Position.Y > tv[2].Position.Y)
			{
				temp = tv[0];
				tv[0] = tv[1];
				tv[1] = temp;
			}
			else
			{
				normal = true;
				if (tv[0].Position.X < tv[2].Position.X)
				{
					temp = tv[0];
					tv[0] = tv[1];
					tv[1] = tv[2];
					tv[2] = temp;
				}
				else
				{
					temp = tv[0];
					tv[0] = tv[1];
					tv[1] = temp;
				}
			}
		}
		else if (tv[0].Position.Y > tv[1].Position.Y)
		{
			if (tv[1].Position.Y < tv[2].Position.Y)
			{
				temp = tv[1];
				tv[1] = tv[2];
				tv[2] = temp;
			}
			else if (tv[1].Position.Y == tv[2].Position.Y)
			{
				normal = true;
				if (tv[1].Position.X > tv[2].Position.X)
				{
					temp = tv[1];
					tv[1] = tv[2];
					tv[2] = temp;
				}
			}
		}
		else
		{
			if (tv[0].Position.X > tv[1].Position.X)
			{
				temp = tv[0];
				tv[0] = tv[1];
				tv[1] = temp;
			}

			if (tv[0].Position.Y < tv[2].Position.Y)
			{
				normal = true;
				temp = tv[0];
				tv[0] = tv[2];
				tv[2] = tv[1];
				tv[1] = temp;
			}
		}

		// 2. 삼각형 패턴 파악 ( Top-Flat , Bottom-Flat, Normal )
		if (normal)
		{
			DrawBottomFlatTriangle(tv);
		}
		else
		{
			if (tv[0].Position.Y == tv[1].Position.Y)
			{
				DrawTopFlatTriangle(tv);
			}
			else
			{
				VertexData newV = tv[1];
				float height = tv[0].Position.Y - tv[2].Position.Y;
				float width = tv[2].Position.X - tv[0].Position.X;

				if (height == 0.0f)
				{
					return;
				}

				TriangleRasterizer t(tv[0], tv[1], tv[2]);
				t.RecalcBounds();

				float gradient = width / height;
				newV.Position.X = gradient * (tv[0].Position.Y - tv[1].Position.Y) + tv[0].Position.X;
				newV.Color = t.GetColor(newV.Position);
				newV.UV = t.GetUV(newV.Position);

				if (newV.Position.X > tv[1].Position.X)
				{
					VertexData upperTriangle[3] = { tv[0], tv[1], newV };
					VertexData bottomTriangle[3] = { tv[1], newV, tv[2] };
					DrawTopFlatTriangle(bottomTriangle);
					DrawBottomFlatTriangle(upperTriangle);
				}
				else
				{
					VertexData upperTriangle[3] = { tv[0], newV, tv[1] };
					VertexData bottomTriangle[3] = { newV, tv[1], tv[2] };
					DrawTopFlatTriangle(bottomTriangle);
					DrawBottomFlatTriangle(upperTriangle);
				}

			}

			/*
		TriangleRasterizer t(
			VertexBuffer[IndexBuffer[ti * 3]],
			VertexBuffer[IndexBuffer[ti * 3 + 1]],
			VertexBuffer[IndexBuffer[ti * 3 + 2]]);

		t.RecalcBounds();

		for (int x = t.TopLeft.X; x < t.BottomRight.X; ++x)
		{
			for (int y = t.TopLeft.Y; y < t.BottomRight.Y; ++y)
			{
				ScreenPoint currentPixel(x, y);
				Vector2 currentPos = currentPixel.ToVector2();
				if (t.IsInside(currentPos))
				{
					if (HasTexture)
					{
						PutPixel(currentPixel, GetTextureSample(t.GetUV(currentPos)));
					}
					else
					{
						PutPixel(currentPixel, t.GetColor(currentPos));
					}
				}
			}
		*/

		}
	}
}


void WindowsRSI::DrawLine(const Vector2 & InStartPos, const Vector2 & InEndPos, const LinearColor & InColor)
{
	int W = InEndPos.X - InStartPos.X;
	int H = InEndPos.Y - InStartPos.Y;

	int x = InStartPos.X;
	int y = InStartPos.Y;

	int F, dF1, dF2;

	if (W < 0)
	{
		if ((W * W) <= (H * H))
		{
			F = 2 * W + H;

			dF1 = 2 * W;
			dF2 = 2 * (W + H);

			for (y = InStartPos.Y; y <= InEndPos.Y; y++)
			{
				DrawScreenPoint(ScreenPoint(x, y), InColor.ToColor32());

				if (F < 0)
				{
					F -= dF1;
				}
				else
				{
					F -= dF2;
					--x;
				}
			}


		}
		else
		{
			F = 2 * H + W;

			dF1 = 2 * H;
			dF2 = 2 * (H + W);


			for (x = InStartPos.X; x > InEndPos.X; x--)
			{
				DrawScreenPoint(ScreenPoint(x, y), InColor.ToColor32());

				if (F < 0)
				{
					F += dF1;
				}
				else
				{
					F += dF2;
					y++;
				}
			}


		}
	}
	else
	{
		if (W <= H)
		{
			F = 2 * W - H;

			dF1 = 2 * W;
			dF2 = 2 * (W - H);


			for (y = InStartPos.Y; y <= InEndPos.Y; y++)
			{
				DrawScreenPoint(ScreenPoint(x, y), InColor.ToColor32());

				if (F < 0)
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
		else
		{
			F = 2 * H - W;

			dF1 = 2 * H;
			dF2 = 2 * (H - W);

			for (x = InStartPos.X; x <= InEndPos.X; ++x)
			{
				DrawScreenPoint(ScreenPoint(x, y), InColor.ToColor32());
				if (F < 0)
				{
					F += dF1;
				}
				else
				{
					++y;
					F += dF2;
				}
			}
		}

	}

}

void WindowsRSI::DrawVerticalLine(int InX, const LinearColor & InColor)
{
	Color32 color = InColor.ToColor32();
	Color32* dest = ScreenBuffer;

	int xIndex = InX + Math::FloorToInt(((float)ScreenSize.X - 1.f) * 0.5f);
	for (int y = 0; y < ScreenSize.Y; ++y)
	{
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



void WindowsRSI::DrawTopFlatTriangle(VertexData * tvs, bool DrawLastLine)
{
	float W1 = tvs[0].Position.X - tvs[2].Position.X;
	float H1 = tvs[0].Position.Y - tvs[2].Position.Y;
	float Inc1 = W1 / H1;

	float W2 = tvs[1].Position.X - tvs[2].Position.X;
	float H2 = tvs[1].Position.Y - tvs[2].Position.Y;
	float Inc2 = W2 / H2;

	// 기울기에 따른 증가량 계산
	float X1 = tvs[2].Position.X;
	float X2 = tvs[2].Position.X;

	TriangleRasterizer t(
		tvs[0],
		tvs[1],
		tvs[2]);
	t.RecalcBounds();

	// 4. Y값을 증가시키며 루프
	for (int i = tvs[2].Position.Y; i <= tvs[0].Position.Y; i++)
	{
		float tempX1 = X1;
		float tempX2 = X2;
		float temp;

		if (tempX1 > tempX2)
		{
			temp = tempX1;
			tempX1 = tempX2;
			tempX2 = temp;
		}

		TriangleRasterizer tr(tvs[1], tvs[0], tvs[2]);
		for (int j = tempX1; j <= tempX2; j++)
		{
			ScreenPoint currentPixel(j, i);
			Vector2 currentPos = currentPixel.ToVector2();

				if (HasTexture)
				{
					PutPixel(currentPixel, GetTextureSample(tr.GetUV(currentPos)));
				}
				else
				{
					PutPixel(currentPixel, tr.GetColor(currentPos));
				}
		}

		X1 += Inc1;
		X2 += Inc2;
	}

}

void WindowsRSI::DrawBottomFlatTriangle(VertexData * tvs)
{
	float W1 = tvs[1].Position.X - tvs[0].Position.X;
	float H1 = tvs[1].Position.Y - tvs[0].Position.Y;
	float Inc1 = W1 / H1;

	float W2 = tvs[2].Position.X - tvs[0].Position.X;
	float H2 = tvs[2].Position.Y - tvs[0].Position.Y;
	float Inc2 = W2 / H2;

	// 기울기에 따른 증가량 계산
	float X1 = tvs[0].Position.X;
	float X2 = tvs[0].Position.X;


	// 4. Y값을 줄여나가면서 루프
	for (int i = tvs[0].Position.Y; i >= tvs[2].Position.Y; i--)
	{
		float tempX1 = X1;
		float tempX2 = X2;
		float temp;

		if (tempX1 > tempX2)
		{
			temp = tempX1;
			tempX1 = tempX2;
			tempX2 = temp;
		}
		TriangleRasterizer tr(tvs[0], tvs[1], tvs[2]);
		for (int j = tempX1; j <= tempX2; j++)
		{
			ScreenPoint currentPixel(j, i);
			Vector2 currentPos = currentPixel.ToVector2();

			if (HasTexture)
			{
				PutPixel(currentPixel, GetTextureSample(tr.GetUV(currentPos)));
			}
			else
			{
				PutPixel(currentPixel, tr.GetColor(currentPos));
			}

		}

		X1 -= Inc1;
		X2 -= Inc2;

	}

}
