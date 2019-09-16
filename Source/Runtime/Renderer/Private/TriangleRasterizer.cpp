#include "Precompiled.h"
#include "TriangleRasterizer.h"

TriangleRasterizer::TriangleRasterizer()
{
	RecalcBounds();
}

TriangleRasterizer::TriangleRasterizer(VertexData InVertex0, VertexData InVertex1, VertexData InVertex2)
{
	VertexBuffer[0] = InVertex0;
	VertexBuffer[1] = InVertex1;
	VertexBuffer[2] = InVertex2;

	bHasColor = true;
	bHasUV = true;

	RecalcBounds();
}

void TriangleRasterizer::RecalcBounds()
{

	Vector2 minPos = Vector2(INFINITY, INFINITY);
	Vector2 maxPos = Vector2(-INFINITY, -INFINITY);

	for (int i = 0; i < 3; i++)
	{
		if (VertexBuffer[i].pos.X < minPos.X) minPos.X = VertexBuffer[i].pos.X;
		if (VertexBuffer[i].pos.Y < minPos.Y) minPos.Y = VertexBuffer[i].pos.Y;
		if (VertexBuffer[i].pos.X > maxPos.X) maxPos.X = VertexBuffer[i].pos.X;
		if (VertexBuffer[i].pos.Y > maxPos.Y) maxPos.Y = VertexBuffer[i].pos.Y;
	}

	U = VertexBuffer[1].pos - VertexBuffer[0].pos;
	V = VertexBuffer[2].pos - VertexBuffer[0].pos;

	UU = U.Dot(U);
	UV = U.Dot(V);
	VV = V.Dot(V);
	InvDenom = 1.f / (UU * VV - UV * UV);

	TopLeft = ScreenPoint(minPos);
	BottomRight = ScreenPoint(maxPos);
}
