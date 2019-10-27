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

	FORCEINLINE bool IsInside(const Vector2& In) const
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
		return VertexBuffer[0].Color * oneMinusST + VertexBuffer[1].Color * st.X + VertexBuffer[2].Color * st.Y;
	}

	//���� �߽� ��ǥ��
	FORCEINLINE Vector2 GetBaryCentricCoord(const Vector2& InScreenPosition) const
	{
		Vector2 w = InScreenPosition - Vector2(VertexBuffer[0].Position.X, VertexBuffer[0].Position.Y);
		float dotUW = UVector.Dot(w);
		float dotVW = VVector.Dot(w);
		float s = (DotVV * dotUW - DotUV * dotVW) * InvDenom;
		float t = (DotUU * dotVW - DotUV * dotUW) * InvDenom;
		return Vector2(s, t);
	}

	FORCEINLINE Vector2 GetUV(const Vector2& InScreenPosition) const
	{
		if (!HasUV())
		{
			return Vector2::Zero;
		}
		Vector2 st = GetBaryCentricCoord(InScreenPosition);
		float oneMinusST = 1 - st.X - st.Y;
		return VertexBuffer[0].UV * oneMinusST + VertexBuffer[1].UV * st.X + VertexBuffer[2].UV * st.Y;
	}

public:
	VertexData VertexBuffer[3];
	ScreenPoint TopLeft;
	ScreenPoint BottomRight;

private:
	Vector2 UVector;
	Vector2 VVector;
	float DotUU, DotUV, DotVV, InvDenom;

	bool bHasVertex = false;
	bool bHasColor = false;
	bool bHasUV = false;
};
