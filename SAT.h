#pragma once
#include <cmath>
#include <vector>
#include "Vector2.h"



// sat(Separating Axis Theorem)
// �� ������ �� ������ ������
// ������ ������ �����Ŵ
// ����� ���п� �ٸ� ������ �������� �����Ǵ� ������ ����
// �����Ǵ� ���е��� ������(������ ������ ���������̶� ��)�� ��� ������ => 2������ ������ ������ �Ʒ��� ������ ǥ��
// ������ ������ �ϳ��� ������ ���� => �ϳ��� �Ȱ�ģ�� �ִٸ� �װ� �浹 �ȵȰ���(��𼱰� �������ִ� �κ��� �ִٴ� �Ŵϱ�)
// �� ���� ������ ������ ��ġ���� Ȯ��
// �Ȱ�ġ�� �浹 ���Ѱ���
// ��ġ�� �ٸ� ������ �����ͼ� ���� �ݺ�
// �ٸ� ������ ���� ������ �ݺ�
// ��� ��ġ�°Ÿ� �浹�Ȱ���
// https://www.sevenson.com.au/programming/sat/

// sat�� �Ϻ��ϰ� �ٷ�� �� �ƴ϶� �ٰ����� ����غ��°� �� �� �����ߵɵ�

struct TriangleMesh // �޽��� �⺻�� �ﰢ���� ���
{
	Vector2 centerPos;
	std::vector<Vector2> vertices;
};


struct Circle // ���� Ȥ�� ���� �ϴ� ��������
{
	Vector2 centerPos;
	float radius;
};


// �ﰢ��&Vector2 �������� ���� �浹 ���� �˰���
// ���� vector2�� x,y��ǥ�� Ǯ�� �ﰢ����� �簢���� �ְ� �ϴ� �� �ణ�� ������ ������ �ٸ� ȯ�濡���� �� ���ư���.
class SATCollision2D
{
public:
	static bool checkCollision(const TriangleMesh& rectA, const TriangleMesh& rectB, Vector2& collisionDir);
	static bool checkCollision(const TriangleMesh& tri, const Circle& circle, Vector2& collisionVec);
	// ���� ���� �浹 ó���� �ʹ� ������ ��ŵ
private:
	static std::vector<Vector2> getAxes(const std::vector<Vector2>& vertices);
	static void project(const std::vector<Vector2>& vertices, const Vector2& axis, float& min, float& max);
	static bool isOverlapping(const float& minA, const float& maxA, const float& minB, const float& maxB);
	static bool lineIntersect(const Vector2& lineStart, const Vector2& lineEnd, const Vector2& center, const Vector2& otherCenter, Vector2& intersection);
	static Vector2 findIntersection(const Vector2& center, const Vector2& otherCenter, const std::vector<Vector2>& vertices);
};

// ������ ���е��� ���������� ����ȭ��Ű�� ����α�
std::vector<Vector2> SATCollision2D::getAxes(const std::vector<Vector2>& vertices)
{
	std::vector<Vector2> axes;
	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		Vector2 edge = vertices[(i + 1) % vertices.size()] - vertices[i];
		axes.push_back(edge.perpendicular().normalize());
	}
	return axes;
}

// �� ������ �������� �ٸ� ������ ������ �� ���� �Ʒ��� ������ ���
void SATCollision2D::project(const std::vector<Vector2>& vertices, const Vector2& axis, float& min, float& max)
{
	min = max = axis.dot(vertices[0]);
	for (const Vector2& vertex : vertices)
	{
		float projection = axis.dot(vertex);
		if (projection < min) min = projection;
		if (projection > max) max = projection;
	}
}

bool SATCollision2D::isOverlapping(const float& minA, const float& maxA, const float& minB, const float& maxB)
{
	return !(maxA < minB || maxB < minA);
}

// �ﰢ������ �浹���
bool SATCollision2D::checkCollision(const TriangleMesh& rectA, const TriangleMesh& rectB, Vector2& collisionDir)
{
	float smallestOverlap = FLT_MAX; // �̰� �󸶳� �浹�ߴ��� ���
	// �� �ΰ��� ���ֵ� ����� ������ �浹 �� ���ܳ����� ó���Ϸ��� �ʿ��� �� �־ ������

	std::vector<Vector2> axesA = getAxes(rectA.vertices);
	std::vector<Vector2> axesB = getAxes(rectB.vertices);

	for (const Vector2& axis : axesA)
	{
		float minA, maxA, minB, maxB;
		project(rectA.vertices, axis, minA, maxA);
		project(rectB.vertices, axis, minB, maxB);
		if (!isOverlapping(minA, maxA, minB, maxB))
		{
			return false;
		}
		float overlap = std::min(maxA - minB, maxB - minA);
		if (overlap < smallestOverlap)
		{
			smallestOverlap = overlap;
			collisionDir = axis; // ���� ���� �浹�� ���� �������� ����
		}
	}

	for (const Vector2& axis : axesB)
	{
		float minA, maxA, minB, maxB;
		project(rectA.vertices, axis, minA, maxA);
		project(rectB.vertices, axis, minB, maxB);
		if (!isOverlapping(minA, maxA, minB, maxB))
		{
			return false;
		}
		float overlap = std::min(maxA - minB, maxB - minA);
		if (overlap < smallestOverlap)
		{
			smallestOverlap = overlap;
			collisionDir = axis; // ���� ���� �浹�� ���� �������� ����
		}
	}

	return true;
}

bool SATCollision2D::checkCollision(const TriangleMesh& tri, const Circle& circle, Vector2& collisionVec)
{
	std::vector<Vector2> vertices = tri.vertices;
	Vector2 nearPos = findIntersection(tri.centerPos, circle.centerPos, vertices);
	float collisionAmount = circle.radius - Vector2::distance(nearPos, circle.centerPos);
	collisionVec = (circle.centerPos - nearPos).normalize() * collisionAmount;
	return collisionAmount >= 0;
}



bool SATCollision2D::lineIntersect(const Vector2& lineStart, const Vector2& lineEnd, const Vector2& center, const Vector2& otherCenter, Vector2& intersection)
{
	float denominator = (lineStart.x - lineEnd.x) * (center.y - otherCenter.y) - (lineStart.y - lineEnd.y) * (center.x - otherCenter.x);

	// �и� 0�̸� �����ϰų� ��ħ
	if (denominator == 0.0f)
		return false;

	float insersectAtLine = ((lineStart.x - center.x) * (center.y - otherCenter.y) - (lineStart.y - center.y) * (center.x - otherCenter.x)) / denominator;
	float intersectAtCenterLine = ((lineStart.x - center.x) * (lineStart.y - lineEnd.y) - (lineStart.y - center.y) * (lineStart.x - lineEnd.x)) / denominator;

	// t�� u�� [0, 1] ���� ���� ���� �� ������ ������
	if (insersectAtLine >= 0 && insersectAtLine <= 1 && intersectAtCenterLine >= 0 && intersectAtCenterLine <= 1)
	{
		intersection.x = lineStart.x + insersectAtLine * (lineEnd.x - lineStart.x);
		intersection.y = lineStart.y + insersectAtLine * (lineEnd.y - lineStart.y);
		return true;
	}

	return false;
}

Vector2 SATCollision2D::findIntersection(const Vector2& center, const Vector2& otherCenter, const std::vector<Vector2>& vertices)
{
	Vector2 result = { 0, 0 };
	float maxDistance = FLT_MIN;

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		Vector2 point1 = vertices[i];
		Vector2 point2 = vertices[(i + 1) % vertices.size()]; // �ٰ����� ���� �����̹Ƿ� ������ ���� ù ��° ���� ����

		Vector2 intersection = {0,0};
		if (lineIntersect(center, otherCenter, point1, point2, intersection))
		{
			float distance = (intersection - center).magnitude();

			if (distance > maxDistance)
			{
				maxDistance = distance;
				result = intersection;
			}
		}
	}

	return result;
}


// ��� �������� �󸶳� �������� ��ģ ���⺤���� �ݴ������� ���ؼ� ���� ���⺤�͸� ��ģ �������� �����ָ� or �� �������� ��� �й��ϸ� ���󺹱Ͱ� ���� ������
// �ٸ� �������͸� �ȵ�
// �ڽ��� ������ �ִ� �ӷ� ����(�ӵ� �ƴ�!)�� �����ؼ�
// �浹�� ���� �浹 ���׺��Ͱ� ��������
// �ݻ纤�͸� ���ؼ� �ڽ��� �ӷ� ���Ϳ� �����
// �浹�ؼ� ���ܳ����°� ������ �� ������?

// �ӷ��� �������°� ������ ���ؾߵǴµ�
// �ڽ��� ������ �ִ� ������ �������� ���ؼ�
// ������ = ���������/��������� * �����׷�
// �����Ӹ��� �������� �ȴ�.