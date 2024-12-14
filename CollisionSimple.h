#pragma once
#include "Vector2.h"
#include <vector>


// AABB �浹 ���� ���

class BoxCollision2D
{
public:
	BoxCollision2D(const int& width, const int& height) : w(width), h(height), pos(Vector2::zero()) {}
	BoxCollision2D() : BoxCollision2D(0, 0) {}
	~BoxCollision2D() {}

	int width() const { return w; }
	int height() const { return h; }
	Vector2 position() const { return pos; }

	void position(Vector2 _position) { pos = _position; }
	void translate(Vector2 _position) { pos += _position; }

private:
	Vector2 pos;
	int w, h;
};



class SphereCollision2D
{
public:
	SphereCollision2D() : SphereCollision2D(Vector2::zero(), 0) {}
	SphereCollision2D(Vector2 initPosition, const int& radius) : pos(initPosition), rad(radius) {}
	~SphereCollision2D() {}

	int radius() const { return rad; }
	int diameter() const { return rad * 2; }
	Vector2 position() const { return pos; }

	void radius(int radius) { rad = radius; }
private:
	Vector2 pos;
	int rad;
};


class CollisionManager
{
public:
	//�簢���� �簢���� �浹
	static bool collision(const BoxCollision2D& object, const BoxCollision2D& subject, Vector2& collisionVec);
	// ���� �簢���� �浹
	static bool collision(const BoxCollision2D& object, const SphereCollision2D& subject, Vector2& collisionVec);
	// ���� ���� �浹
	static bool collision(const SphereCollision2D& object, const SphereCollision2D& subject, Vector2& collisionVec);

private:
	static bool lineIntersect(const Vector2& lineStart, const Vector2& lineEnd, const Vector2& center, const Vector2& otherCenter, Vector2& intersection);
	static Vector2 findIntersection(const Vector2& center, const Vector2& otherCenter, const std::vector<Vector2>& vertices);
};




// AABB �浹���� ���
bool CollisionManager::collision(const BoxCollision2D& object, const BoxCollision2D& subject, Vector2& collisionVec)
{
	Vector2 objArea = Vector2{ (float)object.width() / 2, (float)object.height() / 2 };
	Vector2 sbjArea = Vector2{ (float)subject.width() / 2, (float)subject.height() / 2 };
	Vector2 objMin = object.position() - objArea;
	Vector2 objMax = object.position() + objArea;
	Vector2 sbjMin = subject.position() - sbjArea;
	Vector2 sbjMax = subject.position() + sbjArea;
	float collisionX = 0;
	float collisionY = 0;

	if (objMax.x >= sbjMin.x && sbjMax.x >= objMin.x)
	{
		float overlapStart = (sbjMin.x < objMin.x) ? objMin.x : sbjMin.x;
		float overlapEnd = (sbjMax.x > objMax.x) ? objMax.x : sbjMax.x;

		collisionX = overlapEnd - overlapStart;
	}

	if (objMax.y >= sbjMin.y && sbjMax.y >= objMin.y)
	{
		float overlapStart = (sbjMin.y < objMin.y) ? objMin.y : sbjMin.y;
		float overlapEnd = (sbjMax.y > objMax.y) ? objMax.y : sbjMax.y;

		collisionY = overlapEnd - overlapStart;
	}

	collisionVec = (collisionX < collisionY) ? Vector2{ collisionX, 0.0f } : Vector2{ 0.0f, collisionY };
	return (collisionX != 0.0f) && (collisionY != 0.0f);
}

// ������ �����ϴ� ������ ���� �߽ɱ����� �Ÿ��� ���������� ª���� �浹�ϴ� ����
bool CollisionManager::collision(const BoxCollision2D& object, const SphereCollision2D& subject, Vector2& collisionVec)
{
	Vector2 area = Vector2{ (float)object.width() / 2, (float)object.height() / 2 };
	Vector2 min = object.position() - area;
	std::vector<Vector2> vertices{
		min,
		min + Vector2{(float)object.width(), 0},
		min + Vector2{0, (float)object.height()},
		object.position() + area
	};
	Vector2 nearPos = findIntersection(object.position(), subject.position(), vertices);
	float collisionAmount = subject.radius() - Vector2::distance(nearPos, subject.position());
	collisionVec = (subject.position() - nearPos).normalize() * collisionAmount;
	return collisionAmount >= 0;
}

// �� ������ �������� ã�� �� intersection���� ��ȯ, �����Ǿ����� ���ε� ��ȯ
// lineStart�� lineEnd�� �ٰ����� ���� �� �ϳ�, center�� �ٰ����� �߽�, otherCenter�� ���� �߽�
bool CollisionManager::lineIntersect(const Vector2& lineStart, const Vector2& lineEnd, const Vector2& center, const Vector2& otherCenter, Vector2& intersection)
{
	float denominator = (lineStart.x - lineEnd.x) * (center.y - otherCenter.y) - (lineStart.y - lineEnd.y) * (center.x - otherCenter.x);

	// �и� 0�̸� �����ϰų� ��ħ
	if (denominator == 0.0f)
		return false;

	float tPosNormal = ((lineStart.x - center.x) * (center.y - otherCenter.y) - (lineStart.y - center.y) * (center.x - otherCenter.x)) / denominator;
	float uPosNormal = ((lineStart.x - center.x) * (lineStart.y - lineEnd.y) - (lineStart.y - center.y) * (lineStart.x - lineEnd.x)) / denominator;

	// t�� u�� [0, 1] ���� ���� ���� �� ������ ������ (t�� u�� �ؽ�ó���� ���� ���� ��ǥ��)
	if (tPosNormal >= 0 && tPosNormal <= 1 && uPosNormal >= 0 && uPosNormal <= 1)
	{
		intersection.x = lineStart.x + tPosNormal * (lineEnd.x - lineStart.x);
		intersection.y = lineStart.y + tPosNormal * (lineEnd.y - lineStart.y);
		return true;
	}

	return false;
}

// �ٰ����� �߽ɿ��� �ٸ� ������ ���� ���� �ٰ��� ���е��� ������ �� ���� ������ ������ ��ȯ
Vector2 CollisionManager::findIntersection(const Vector2& center, const Vector2& otherCenter, const std::vector<Vector2>& vertices)
{
	Vector2 result = { 0, 0 };
	float maxDistance = FLT_MIN;

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		Vector2 point1 = vertices[i];
		Vector2 point2 = vertices[(i + 1) % vertices.size()]; // �ٰ����� ���� �����̹Ƿ� ������ ���� ù ��° ���� ����

		Vector2 intersection = { 0,0 };
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

bool CollisionManager::collision(const SphereCollision2D& object, const SphereCollision2D& subject, Vector2& collisionVec)
{
	Vector2 distance = subject.position() - object.position();
	float collisionPwr = (object.radius() + subject.radius()) - distance.magnitude();

	collisionVec = distance.normalize() * collisionPwr;
	return collisionPwr > 0;
}