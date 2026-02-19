#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "winmm.lib")

#include <cmath>
#include <vector>

// 다렉에서 준거 안쓰고 이걸 만드는 이유
// 1. 내가 유니티에 절여저있어서 유니티랑 비슷하게 만들고 싶음
// 2. 이름 좀 길고 치기 귀찮


class Vector3Int;
class Vector3
{
public:
	Vector3(void) : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	~Vector3(void) {}

	float x, y, z;

	// 묵시적 형변환
	operator D3DXVECTOR3(void);
	operator Vector3Int(void);

	// 명시적 형변환
	D3DXVECTOR3 toDx(void) { return D3DXVECTOR3(x, y, z); }
	static D3DXVECTOR3 toDx(const Vector3& object) { return D3DXVECTOR3(object.x, object.y, object.z); }
	static Vector3 toVec3(const D3DXVECTOR3& object) { return Vector3(object.x, object.y, object.z); }

	// 가독성을 위한 정의
	static Vector3 zero(void) { return Vector3{ 0.0f, 0.0f, 0.0f }; }
	static Vector3 one(void) { return Vector3{ 1.0f, 1.0f, 1.0f }; }

	// 연산자 정의
	bool operator==(const Vector3& other) const
	{
		return (x == other.x) && (y == other.y) && (z == other.z);
	}
	bool operator!=(const Vector3& other) const
	{
		return (x != other.x) || (y != other.y) || (z != other.z);
	}
	bool operator>(const Vector3& other) const
	{
		return (x > other.x) && (y > other.y) && (z > other.z);
	}
	bool operator<(const Vector3& other) const
	{
		return (x < other.x) && (y < other.y) && (z < other.z);
	}
	bool operator>=(const Vector3& other) const
	{
		return (x >= other.x) && (y >= other.y) && (z >= other.z);
	}
	bool operator<=(const Vector3& other) const
	{
		return (x <= other.x) && (y <= other.y) && (z <= other.z);
	}

	Vector3 operator+(const Vector3& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	Vector3 operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	Vector3 operator*(const float& i) const
	{
		return Vector3(x * i, y * i, z * i);
	}
	Vector3 operator/(const float& i) const
	{
		return Vector3(x / i, y / i, z / i);
	}

	void operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}
	void operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}
	void operator*=(const float& i)
	{
		x *= i;
		y *= i;
		z *= i;
	}
	void operator/=(const float& i)
	{
		x /= i;
		y /= i;
		z /= i;
	}



	// 벡터 내적
	float dot(const Vector3& other) const { return x * other.x + y * other.y + z + other.z; }
	static float dot(const Vector3& obj, const Vector3& sbj) { return obj.x * sbj.x + obj.y * sbj.y + obj.z + sbj.z; }

	// 선분의 법선 선분에 대한 반사벡터
	Vector3 reflect(const Vector3& other) const
	{
		float _dot = dot(other); // (R = V - 2 X (V dot N) X N)
		return Vector3(x - 2.0f * _dot * other.x, y - 2.0f * _dot * other.y, z - 2.0f * _dot * other.z);
	}
	static Vector3 reflect(const Vector3& object, const Vector3& subject)
	{
		float _dot = dot(object, subject);
		return Vector3(object.x - 2.0f * _dot * subject.x, object.y - 2.0f * _dot * subject.y, object.z - 2.0f * _dot * subject.z);
	}

	// 스칼라 값
	float magnitude(void) const { return std::sqrt(x * x + y * y + z * z); }
	static float magnitude(const Vector3& obj) { return std::sqrt(obj.x * obj.x + obj.y * obj.y + obj.z * obj.z); }

	// 두 점 사잉의 스칼라 값
	float distance(const Vector3& other) { return (*this - other).magnitude(); }
	static float distance(const Vector3& _Vec1, const Vector3& _Vec2) { return (_Vec1 - _Vec2).magnitude(); }

	// 단위벡터
	Vector3 normalize(void) const
	{
		float length = magnitude();
		return { x / length, y / length, z / length };
	}
	static Vector3 normalize(const Vector3& obj)
	{
		float length = obj.magnitude();
		return { obj.x / length, obj.y / length, obj.z / length };
	}

	// 방위각과 상승각에 대응하는 방향벡터
	static Vector3 direction(float azimuth, float elevation)
	{
		// azimuth: 방향각 (0 <= azimuth < 2π) / elevation: 경사각 (-π/2 <= elevation <= π/2)
		while (azimuth >= 0 && azimuth <= 360 && elevation >= 0 && elevation <= 360)
		{
			if (azimuth < 0) { azimuth += 360; }
			if (azimuth > 360) { azimuth -= 360; }
			if (elevation < 0) { elevation += 360; }
			if (elevation > 360) { elevation -= 360; }
		}

		return Vector3(std::cos(elevation) * std::cos(azimuth), std::cos(elevation) * std::sin(azimuth), std::sin(elevation));
	}
	// 방위각에 대응하는 방향벡터
	static Vector3 drection2D(float azimuth) { return Vector3(std::cos(azimuth), 0.0f, std::sin(azimuth)); }

	// 좌표를 기준으로 방위각과 상승각에 대응하는 방향벡터
	Vector3 forward(float azimuth, float elevation) { return *this + direction(azimuth, elevation).normalize(); }
	static Vector3 forward(Vector3 position, float azimuth, float elevation) { return position + direction(azimuth, elevation).normalize(); }

	// 벡터 외적
	Vector3 cross(const Vector3& other) const
	{
		return Vector3(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}
	static Vector3 cross(const Vector3& obj, const Vector3& sbj)
	{
		return Vector3(
			obj.y * sbj.z - obj.z * sbj.y,
			obj.z * sbj.x - obj.x * sbj.z,
			obj.x * sbj.y - obj.y * sbj.x
		);
	}

	// 3차원 수직벡터(y축)
	Vector3 perpendicular(void) const { return cross(Vector3{ -z, y, x }); }
	static Vector3 perpendicular(const Vector3& obj) { return cross(obj, Vector3(-obj.z, obj.y, obj.x)); }
	// 2차원 수직벡터
	Vector3 perpendicular2D(void) const { return Vector3{ -z, y, x }; }
	static Vector3 perpendicular2D(const Vector3& obj) { return Vector3(-obj.z, obj.y, obj.x); }

	inline static std::pair<bool, Vector3> Intersection(const Vector3& point11, const Vector3& point12, const Vector3& point21, const Vector3& point22)
	{
		Vector3 dump;
		return Intersection(point11, point12, point21, point22, dump);
	}

	static std::pair<bool, Vector3> Intersection(const Vector3& point11, const Vector3& point12, const Vector3& point21, const Vector3& point22, Vector3& nearPoint)
	{
		Vector3 vector1 = point12 - point11;
		Vector3 vector2 = point22 - point21;
		Vector3 pointDifference = point11 - point21;

		float vector1Square = Vector3::dot(vector1, vector1);
		float vectorDot = Vector3::dot(vector1, vector2);
		float vector2Square = Vector3::dot(vector2, vector2);
		float dot1 = Vector3::dot(vector1, pointDifference);
		float dot2 = Vector3::dot(vector2, pointDifference);

		float denominator = vector1Square * vector2Square - vectorDot * vectorDot;
		if (denominator == 0.0f)
			return { false, Vector3::zero() };

		float line1Position = (vectorDot * dot2 - vector2Square * dot1) / denominator;
		float line2Position = (vector1Square * dot2 - vectorDot * dot1) / denominator;

		if (line1Position < 0 || line1Position > 1 || line2Position < 0 || line2Position > 1)
		{
			nearPoint = point21 + vector2 * line2Position;
			return { false, Vector3::zero() };
		}

		return { true, point11 + vector1 * line1Position };
	}

};

class Vector3Int
{
public:
	Vector3Int(void) : x(0), y(0), z(0) {}
	Vector3Int(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
	~Vector3Int(void) {}

	int x, y, z;

	// 묵시적 형변환
	operator D3DXVECTOR3(void);
	operator Vector3(void);

	// 명시적 형변환
	D3DXVECTOR3 toDx(void) { return D3DXVECTOR3(x, y, z); }
	static D3DXVECTOR3 toDx(const Vector3Int& object) { return D3DXVECTOR3(static_cast<float>(object.x), static_cast<float>(object.y), static_cast<float>(object.z)); }
	static Vector3Int toVec3(const D3DXVECTOR3& object) { return Vector3Int(static_cast<int>(object.x), static_cast<int>(object.y), static_cast<int>(object.z)); }

	//가독성을 위한 정의
	static Vector3Int zero(void) { return Vector3Int{ 0, 0, 0 }; }
	static Vector3Int one(void) { return Vector3Int{ 1, 1, 1 }; }

	// 연산자 정의
	bool operator==(const Vector3Int& other) const
	{
		return (x == other.x) && (y == other.y) && (z == other.z);
	}
	bool operator!=(const Vector3Int& other) const
	{
		return (x != other.x) || (y != other.y) || (z != other.z);
	}

	Vector3Int operator+(const Vector3Int& other) const
	{
		return Vector3Int(x + other.x, y + other.y, z + other.z);
	}
	Vector3Int operator-(const Vector3Int& other) const
	{
		return Vector3Int(x - other.x, y - other.y, z - other.z);
	}
	Vector3Int operator*(const int& i) const
	{
		return Vector3Int(x * i, y * i, z * i);
	}
	Vector3Int operator/(const int& i) const
	{
		return Vector3Int(x / i, y / i, z / i);
	}

	void operator+=(const Vector3Int& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}
	void operator-=(const Vector3Int& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}
	void operator*=(const int& i)
	{
		x *= i;
		y *= i;
		z *= i;
	}
	void operator/=(const int& i)
	{
		x /= i;
		y /= i;
		z /= i;
	}
	void operator*=(const float& i)
	{
		x = (int)((float)x * i);
		y = (int)((float)y * i);
		z = (int)((float)z * i);
	}
	void operator/=(const float& i)
	{
		x = (int)((float)x / i);
		y = (int)((float)y / i);
		z = (int)((float)z / i);
	}

	// 벡터 내적
	float dot(const Vector3& other) const { return static_cast<float>(x * other.x + y * other.y + z + other.z); }
	static float dot(const Vector3& obj, const Vector3& sbj) { return static_cast<float>(obj.x * sbj.x + obj.y * sbj.y + obj.z + sbj.z); }

	// 스칼라값
	float magnitude(void) const { return std::sqrt(static_cast<float>(x * x + y * y + z * z)); }
	static float magnitude(const Vector3& obj) { return std::sqrt(static_cast<float>(obj.x * obj.x + obj.y * obj.y + obj.z * obj.z)); }

	// 두 점 사이의 스칼라값
	float distance(const Vector3Int& other) { return (*this - other).magnitude(); }
	static float distance(const Vector3Int& obj, const Vector3Int& sbj) { return (obj - sbj).magnitude(); }

};


inline Vector3::operator D3DXVECTOR3() { return D3DXVECTOR3(x, y, z); }
inline Vector3::operator Vector3Int() { return Vector3Int(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z)); }




inline Vector3Int::operator D3DXVECTOR3() { return D3DXVECTOR3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)); }
inline Vector3Int::operator Vector3() { return Vector3Int(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z)); }


