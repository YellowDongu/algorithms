#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "winmm.lib")

#include <cmath>

// �ٷ����� �ذ� �Ⱦ��� �̰� ����� ����
// 1. ���� ����Ƽ�� �������־ ����Ƽ�� ����ϰ� ����� ����
// 2. �̸� �� ��� ġ�� ����


class Vector3Int;
class Vector3
{
public:
	Vector3(void) : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	~Vector3(void) {}

	float x, y, z;

	// ������ ����ȯ
	operator D3DXVECTOR3(void);
	operator Vector3Int(void);

	// ����� ����ȯ
	D3DXVECTOR3 toDx(void) { return D3DXVECTOR3(x, y, z); }
	static D3DXVECTOR3 toDx(const Vector3& object) { return D3DXVECTOR3(object.x, object.y, object.z); }
	static Vector3 toVec3(const D3DXVECTOR3& object) { return Vector3(object.x, object.y, object.z); }

	// �������� ���� ����
	static Vector3 zero(void) { return Vector3{ 0.0f, 0.0f, 0.0f }; }
	static Vector3 one(void) { return Vector3{ 1.0f, 1.0f, 1.0f }; }

	// ������ ����
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



	// ���� ����
	float dot(const Vector3& other) const { return x * other.x + y * other.y + z + other.z; }
	static float dot(const Vector3& obj, const Vector3& sbj) { return obj.x * sbj.x + obj.y * sbj.y + obj.z + sbj.z; }

	// ������ ���� ���п� ���� �ݻ纤��
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

	// ��Į�� ��
	float magnitude(void) const { return std::sqrt(x * x + y * y + z * z); }
	static float magnitude(const Vector3& obj) { return std::sqrt(obj.x * obj.x + obj.y * obj.y + obj.z * obj.z); }

	// �� �� ������ ��Į�� ��
	float distance(const Vector3& other) { return (*this - other).magnitude(); }
	static float distance(const Vector3& _Vec1, const Vector3& _Vec2) { return (_Vec1 - _Vec2).magnitude(); }

	// ��������
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

	// �������� ��°��� �����ϴ� ���⺤��
	static Vector3 direction(float azimuth, float elevation)
	{
		// azimuth: ���Ⱒ (0 <= azimuth < 2��) / elevation: ��簢 (-��/2 <= elevation <= ��/2)
		while (azimuth >= 0 && azimuth <= 360 && elevation >= 0 && elevation <= 360)
		{
			if (azimuth < 0) { azimuth += 360; }
			if (azimuth > 360) { azimuth -= 360; }
			if (elevation < 0) { elevation += 360; }
			if (elevation > 360) { elevation -= 360; }
		}

		return Vector3(std::cos(elevation) * std::cos(azimuth), std::cos(elevation) * std::sin(azimuth), std::sin(elevation));
	}
	// �������� �����ϴ� ���⺤��
	static Vector3 drection2D(float azimuth) { return Vector3(std::cos(azimuth), 0.0f, std::sin(azimuth)); }

	// ��ǥ�� �������� �������� ��°��� �����ϴ� ���⺤��
	Vector3 forward(float azimuth, float elevation) { return *this + direction(azimuth, elevation).normalize(); }
	static Vector3 forward(Vector3 position, float azimuth, float elevation) { return position + direction(azimuth, elevation).normalize(); }

	// ���� ����
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

	// 3���� ��������(y��)
	Vector3 perpendicular(void) const { return cross(Vector3{ -z, y, x }); }
	static Vector3 perpendicular(const Vector3& obj) { return cross(obj, Vector3(-obj.z, obj.y, obj.x)); }
	// 2���� ��������
	Vector3 perpendicular2D(void) const { return Vector3{ -z, y, x }; }
	static Vector3 perpendicular2D(const Vector3& obj) { return Vector3(-obj.z, obj.y, obj.x); }

};

class Vector3Int
{
public:
	Vector3Int(void) : x(0), y(0), z(0) {}
	Vector3Int(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
	~Vector3Int(void) {}

	int x, y, z;

	// ������ ����ȯ
	operator D3DXVECTOR3(void);
	operator Vector3(void);

	// ����� ����ȯ
	D3DXVECTOR3 toDx(void) { return D3DXVECTOR3(x, y, z); }
	static D3DXVECTOR3 toDx(const Vector3Int& object) { return D3DXVECTOR3(static_cast<float>(object.x), static_cast<float>(object.y), static_cast<float>(object.z)); }
	static Vector3Int toVec3(const D3DXVECTOR3& object) { return Vector3Int(static_cast<int>(object.x), static_cast<int>(object.y), static_cast<int>(object.z)); }

	//�������� ���� ����
	static Vector3Int zero(void) { return Vector3Int{ 0, 0, 0 }; }
	static Vector3Int one(void) { return Vector3Int{ 1, 1, 1 }; }

	// ������ ����
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

	// ���� ����
	float dot(const Vector3& other) const { return static_cast<float>(x * other.x + y * other.y + z + other.z); }
	static float dot(const Vector3& obj, const Vector3& sbj) { return static_cast<float>(obj.x * sbj.x + obj.y * sbj.y + obj.z + sbj.z); }

	// ��Į��
	float magnitude(void) const { return std::sqrt(static_cast<float>(x * x + y * y + z * z)); }
	static float magnitude(const Vector3& obj) { return std::sqrt(static_cast<float>(obj.x * obj.x + obj.y * obj.y + obj.z * obj.z)); }

	// �� �� ������ ��Į��
	float distance(const Vector3Int& other) { return (*this - other).magnitude(); }
	static float distance(const Vector3Int& obj, const Vector3Int& sbj) { return (obj - sbj).magnitude(); }

};


inline Vector3::operator D3DXVECTOR3() { return D3DXVECTOR3(x, y, z); }
inline Vector3::operator Vector3Int() { return Vector3Int(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z)); }




inline Vector3Int::operator D3DXVECTOR3() { return D3DXVECTOR3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)); }
inline Vector3Int::operator Vector3() { return Vector3Int(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z)); }

