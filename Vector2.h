#pragma once
#include <cmath>

#define pi			3.14159265358979f

class Vector2Int;
class Vector2
{
public:
	float x, y;

	Vector2(float _x, float _y) : x(_x), y(_y) {}

	operator Vector2Int() const;
	Vector2Int toVec2Int() const;
	static Vector2Int toVec2Int(const Vector2& obj);


	bool operator==(const Vector2& other) const
	{
		return (x == other.x) && (y == other.y);
	}
	bool operator!=(const Vector2& other) const
	{
		return (x != other.x) || (y != other.y);
	}

	Vector2 operator+(const Vector2& other) const
	{
		return { x + other.x, y + other.y };
	}
	Vector2 operator-(const Vector2& other) const
	{
		return { x - other.x, y - other.y };
	}
	Vector2 operator*(const float& i) const
	{
		return { x * i, y * i };
	}
	Vector2 operator/(const int& i) const
	{
		return { x / i, y / i };
	}

	void operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
	}
	void operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
	}
	void operator*=(const float& i)
	{
		x *= i;
		y *= i;
	}
	void operator/=(const float& i)
	{
		x /= i;
		y /= i;
	}
	void operator=(const Vector2& other)
	{
		x = other.x;
		y = other.y;
	}

	//��������
	Vector2& operator++()
	{
		x++;
		y++;
		return *this;
	}
	// ��������
	Vector2 operator++(int)
	{
		Vector2 temp{ x, y };
		x++;
		y++;
		return temp;
	}

	Vector2& operator--()
	{
		x--;
		y--;
		return *this;
	}
	Vector2 operator--(int)
	{
		Vector2 temp{ x, y };
		x--;
		y--;
		return temp;
	}

	// ��ǥ ���밪
	Vector2 absolute() const 
	{
		return { abs(x) ,abs(y) };
	}
	static Vector2 absolute(const Vector2& obj)
	{
		return { abs(obj.x) ,abs(obj.y) };
	}

	// ���� ����
	float dot(const Vector2& other) const
	{
		return x * other.x + y * other.y;
	}
	static float dot(const Vector2& obj, const Vector2& sbj)
	{
		return obj.x * sbj.x + obj.y * sbj.y;
	}

	// ������ ũ��(��Į��)
	float magnitude() const
	{
		return static_cast<float>(std::sqrt(x * x + y * y));
	}
	static float magnitude(const Vector2& obj)
	{
		return static_cast<float>(std::sqrt(obj.x * obj.x + obj.y * obj.y));
	}

	// �� ��ǥ �� ��� �Ÿ�
	float distance(const Vector2& sbj)
	{
		return (*this - sbj).magnitude();
	}
	static float distance(const Vector2& _Vec1, const Vector2& _Vec2)
	{
		return (_Vec1 - _Vec2).magnitude();
	}

	// ���� ������ ��Ÿ��
	Vector2 perpendicular() const
	{
		return { -y, x };
	}
	static Vector2 perpendicular(const Vector2& obj)
	{
		return { -obj.y, obj.x };
	}

	// ���� ���� ǥ�� (���� 0~1�� ���� ����)
	Vector2 normalize() const
	{
		float length = (*this).magnitude();
		return { x / length, y / length };
	}
	static Vector2 normalize(const Vector2& obj)
	{
		float length = obj.magnitude();
		return { obj.x / length, obj.y / length };
	}

	// ���� ������ ���� ���ͷ� ��Ÿ����
	static Vector2 getDirectionVector(float _angle)
	{
		return Vector2(std::cos(_angle), std::sin(_angle));
	}

	// ���� ���� ������ ������ �˷���
	Vector2 forward(float _rotation) const
	{
		Vector2 DirectionVec = getDirectionVector(_rotation);
		return Vector2(x * DirectionVec.x - y * DirectionVec.y, x * DirectionVec.y + y * DirectionVec.x);
	}

	// ���͸� �� ���� ������ �������
	static float angle(const Vector2& directionVec)
	{
		Vector2 normalVec = directionVec.normalize();
		float acosAng = acos(normalVec.x) * (180.0f / pi);
		if (normalVec.y < 0) { acosAng = 360.0f - acosAng; }
		return acosAng;
	}
	

	// �Ի纤�͸� ���к��Ϳ� ���� �ݻ纤�ͷ� �������
	static Vector2 reflect(const Vector2& vector, const Vector2& segment)
	{
		Vector2 normalVec = segment.perpendicular().normalize();
		return vector - normalVec * (2 * dot(vector, normalVec));
	}


	static Vector2 zero() { return { 0.0f, 0.0f }; }
	static Vector2 one() { return { 1.0f, 1.0f }; }

};

class Vector2Int
{
public:
	int x, y;

	Vector2Int(int _x, int _y) : x(_x), y(_y) {}

	Vector2 toVec2() const
	{
		return { static_cast<float>(x), static_cast<float>(y) };
	}
	static Vector2 toVec2(const Vector2Int& obj)
	{
		return { static_cast<float>(obj.x), static_cast<float>(obj.y) };
	}

	Vector2Int absolute() const // ��ǥ ���밪
	{
		return { (int)abs(x), (int)abs(y) };
	}
	static Vector2Int absolute(const Vector2& obj) // ��ǥ ���밪
	{
		return { (int)abs(obj.x), (int)abs(obj.y) };
	}

	operator Vector2() const;

	bool operator==(const Vector2Int& other) const
	{
		return (x == other.x) && (y == other.y);
	}
	bool operator!=(const Vector2Int& other) const
	{
		return (x != other.x) || (y != other.y);
	}

	Vector2Int operator+(const Vector2Int& other) const
	{
		return Vector2Int(x + other.x, y + other.y);
	}
	Vector2Int operator-(const Vector2Int& other) const
	{
		return Vector2Int(x - other.x, y - other.y);
	}
	Vector2Int operator*(const int& i) const
	{
		return Vector2Int(x * i, y * i);
	}
	Vector2Int operator/(const int& i) const
	{
		return Vector2Int(x / i, y / i);
	}

	void operator+=(const Vector2Int& other)
	{
		x += other.x;
		y += other.y;
	}
	void operator-=(const Vector2Int& other)
	{
		x -= other.x;
		y -= other.y;
	}
	void operator*=(const int& i)
	{
		x *= i;
		y *= i;
	}
	void operator/=(const int& i)
	{
		x /= i;
		y /= i;
	}
	void operator=(const Vector2Int& other)
	{
		x = other.x;
		y = other.y;
	}
	//��������
	Vector2Int& operator++()
	{
		x++;
		y++;
		return *this;
	}
	// ��������
	Vector2Int operator++(int)
	{
		Vector2Int temp{ x, y };
		x++;
		y++;
		return temp;
	}

	Vector2Int& operator--()
	{
		x--;
		y--;
		return *this;
	}
	Vector2Int operator--(int)
	{
		Vector2Int temp{ x, y };
		x--;
		y--;
		return temp;
	}

	// �� ��ǥ �� ��� �Ÿ�
	float distance(const Vector2Int& sbj)
	{
		return (*this - sbj).magnitude();
	}
	static float distance(const Vector2Int& _Vec1, const Vector2Int& _Vec2)
	{
		return (_Vec1 - _Vec2).magnitude();
	}

	// ���� ����
	int dot(const Vector2Int& other) const
	{
		return x * other.x + y * other.y;
	}
	static int dot(const Vector2Int& obj, const Vector2Int& sbj)
	{
		return obj.x * sbj.x + obj.y * sbj.y;
	}

	// ��Į�� ��
	float magnitude() const
	{
		return static_cast<float>(std::sqrt(x * x + y * y));
	}
	static float magnitude(const Vector2Int& obj)
	{
		return static_cast<float>(std::sqrt(obj.x * obj.x + obj.y * obj.y));
	}

	// ���� ����
	Vector2Int perpendicular() const
	{
		return { -y, x };
	}
	static Vector2Int perpendicular(const Vector2Int& obj)
	{
		return { -obj.y, obj.x };
	}

	static Vector2Int zero() { return { 0,0 }; }
	static Vector2Int one() { return { 1,1 }; }
};



 Vector2Int Vector2::toVec2Int() const { return { static_cast<int>(x), static_cast<int>(y) }; }
 Vector2Int Vector2::toVec2Int(const Vector2& obj) { return { static_cast<int>(obj.x), static_cast<int>(obj.y) }; }

 Vector2::operator Vector2Int() const { return Vector2Int(static_cast<int>(x), static_cast<int>(y)); }
 Vector2Int::operator Vector2() const { return Vector2(static_cast<float>(x), static_cast<float>(y)); }

