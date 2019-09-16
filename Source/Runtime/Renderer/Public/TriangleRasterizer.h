#pragma once

#include "Vector2.h"
#include "LinearColor.h"
#include "RenderingSoftwareInterface.h"

class TriangleRasterizer
{
public:
	TriangleRasterizer();
	explicit TriangleRasterizer(VertexData InVertex0, VertexData InVertex1, VertexData InVertex2);

public:
	FORCEINLINE bool HasColor() const { return bHasColor; }
	FORCEINLINE bool HasUV() const { return bHasUV; }
	void RecalcBounds();

	const bool IsInside(const Vector2& In) const
	{
		Vector2 st = GetBaryCentricCoord(In);
		if (st.X < 0.f) return false;
		if (st.Y < 0.f) return false;
		if ((st.X + st.Y) > 1.f) return false;

		return true;
	}

	FORCEINLINE LinearColor GetColor(const Vector2& InScreenPosition) const 
	{
		if (!HasColor())
		{
			return LinearColor::Error;
		}

		Vector2 st = GetBaryCentricCoord(InScreenPosition);
		float oneMinusST = 1 - st.X - st.Y;
		return VertexBuffer[0].color * oneMinusST + VertexBuffer[1].color * st.X + VertexBuffer[2].color * st.Y;
	}

	FORCEINLINE Vector2 GetBaryCentricCoord(const Vector2& InScreenPosition) const
	{
		Vector2 w = InScreenPosition - Vector2(VertexBuffer[0].pos.X, VertexBuffer[0].pos.Y);
		float dotUW = U.Dot(w);
		float dotVW = V.Dot(w);
		float s = (VV * dotUW - UV * dotVW) * InvDenom;
		float t = (UU * dotVW - UV * dotUW) * InvDenom;
		return Vector2(s, t);
	}

public:
	VertexData VertexBuffer[3];
	ScreenPoint TopLeft;
	ScreenPoint BottomRight;

private:
	Vector2 U;
	Vector2 V;
	float UU, UV, VV, InvDenom;

	bool bHasVertex = false;
	bool bHasColor = false;
	bool bHasUV = false;
};