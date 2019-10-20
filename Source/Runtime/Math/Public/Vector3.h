#pragma once

#include "CoreDefinition.h"

struct Vector3
{
  Vector3() { }
  constexpr FORCEINLINE Vector3(float InX,float InY, float InZ):X(InX), Y(InY), Z(InZ) { }
  FORCEINLINE float SizeSquared() const;
	FORCEINLINE float Dot(const Vector3& InV) const;
	FORCEINLINE bool IsZero() const
	{
		return X == 0.f && Y == 0.f;
	}

	FORCEINLINE float operator[](int InIndex) const;
	FORCEINLINE float& operator[](int InIndex);

	FORCEINLINE Vector3 operator*(float InScalar) const;
	FORCEINLINE Vector3 operator/(float InScalar) const;
	FORCEINLINE Vector3 operator+(const Vector3& InV) const;
	FORCEINLINE Vector3 operator-(const Vector3& InV) const;
	FORCEINLINE Vector3& operator+=(const Vector3& InV);

	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 One;
	static const Vector3 Zero;

public:
	float X = 0.f;
	float Y = 0.f;
  float Z = 0.f;
};

FORCEINLINE Vector3 Vector3::operator*(float InScalar) const
{
	return Vector3(X * InScalar, Y * InScalar, Z * InScalar);
}

FORCEINLINE Vector3 Vector3::operator/(float InScalar) const
{
	return Vector2(X / InScalar, Y / InScalar, Z/InScalar);
}

FORCEINLINE Vector3 Vector3::operator+(const Vector3& InV) const
{
	return Vector2(X + InV.X, Y + InV.Y, Z + InV.Z);
}

FORCEINLINE Vector3 Vector3::operator-(const Vector3& InV) const
{
	return Vector2(X - InV.X, Y - InV.Y, Z -InV.Z);
}

FORCEINLINE Vector3& Vector3::operator+=(const Vector3& InV)
{
	X += InV.X;
	Y += InV.Y;
  Z += InV.Z;
	return *this;
}


FORCEINLINE float Vector3::SizeSquared() const
{
	return X * X + Y * Y+ Z*Z;
}

FORCEINLINE float Vector3::Dot(const Vector3& InV) const
{
	return X * InV.X + Y * InV.Y + Z * InV.Z;
}

FORCEINLINE float Vector3::operator[](int InIndex) const 
{
	return ((float *)this)[InIndex];
}

FORCEINLINE float &Vector3::operator[](int InIndex)
{
	return ((float *)this)[InIndex];
}
