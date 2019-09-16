
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
		/*RSI->DrawScreenPoint(ScreenPoint(0, 0), LinearColor(1.f, 0.f, 0.f, 1.f));

		Vector2 center(0.f, 0.f);
		float radius = 10.f;

		float minfX = center.X - radius;
		float maxfX = center.X + radius;
		float minfY = center.Y - radius;
		float maxfY = center.Y + radius;


		Vector2 minPos(center.X - radius, center.Y - radius);
		Vector2 maxPos(center.X + radius, center.Y + radius);

		ScreenPoint minPosScreen(minPos);
		ScreenPoint maxPosScreen(maxPos);


		LinearColor circleColor(1.f, 0.f, 0.f, 1.f);
		for (int x = minPosScreen.X; x <= maxPosScreen.X; x++)
		{
			for (int y = minPosScreen.Y; y <= maxPosScreen.Y; y++)
			{
				ScreenPoint targetPixel(x, y);
				Vector2 targetPos = targetPixel.ToVector2();

				float disSquare = (targetPos - center).SizeSquared();

				if (disSquare < (radius * radius)) 
				{
					RSI->DrawScreenPoint(ScreenPoint(x, y), circleColor);
				}
			}
		}*/

		Vector2 A(0.0f, 100.0f);
		Vector2 B(-100.0f, -100.0f);
		Vector2 C(100.0f, -100.0f);

		Vector2 minPos = A;
		Vector2 maxPos = A;

		if (A.X > B.X)
		{
			minPos.X = B.X;
		}
		else
		{
			maxPos.X = B.X;
		}
		if (A.Y > B.Y)
		{
			minPos.Y = B.Y;
		}
		else
		{
			maxPos.Y = B.Y;	
		}

		if (minPos.X > C.X) minPos.X = C.X;
		else if (maxPos.X < C.X) maxPos.X = C.X;

		if (minPos.Y > C.Y) minPos.Y = C.Y;
		else if (maxPos.Y < C.Y) maxPos.Y = C.Y;

		Vector2 u = B - A;
		Vector2 v = C - A;

		float UU = u.Dot(u);
		float UV = u.Dot(v);
		float VV = v.Dot(v);
		float denominator = 1.0f / (UU*VV - UV * UV);

		LinearColor color(1.f, 0.f, 0.f, 1.f);
		for (int x = minPos.X; x <= maxPos.X; x++)
		{
			for (int y = minPos.Y; y <= maxPos.Y; y++)
			{
				Vector2 P(x, y);
				Vector2 w = P - A;

				float UW = u.Dot(w);
				float VW = v.Dot(v);

			//	float s = (VV* UW - UV * VW) * denominator;
			//	float t = (UU*VW - UV * UW) * denominator;

				float s = ((u.Dot(w) * v.Dot(v)) - (w.Dot(v) * u.Dot(v))) / ((u.Dot(u) * v.Dot(v)) - (u.Dot(v) * u.Dot(v)));
				float t = ((w.Dot(v) * u.Dot(u)) - (u.Dot(w) * u.Dot(v))) / ((u.Dot(u) * v.Dot(v)) - (u.Dot(v) * u.Dot(v)));

				if ((0 <= s && s <= 1) && (0 <= t && t <= 1))
				{
					RSI->DrawScreenPoint(ScreenPoint(x,y), color);
				}
			}
		}
		RSI->EndFrame();
	}
}


