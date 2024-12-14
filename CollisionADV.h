#pragma once
#include "Vector2.h"
#include <vector>
#include <list>
#include <Windows.h>



enum class CollisionShape
{
	BOX,
	CIRCLE,
	END
};

// AABB �浹 ���� ���
class CollisionObj
{
public:
	CollisionObj() : pos(Vector2::zero()), shapeType(CollisionShape::END){}
	virtual ~CollisionObj() {}



	Vector2			position() const { return pos; }
	CollisionShape	shape() const { return shapeType; }
			
	void			position(Vector2 _position) { pos = _position; }
	void			translate(Vector2 _position) { pos += _position; }

protected:
	Vector2				pos;
	CollisionShape		shapeType;

};



class BoxCollision2D : public CollisionObj
{
public:
	BoxCollision2D(const int& width, const int& height) : w(width), h(height) { shapeType = CollisionShape::BOX; }
	BoxCollision2D() : w(0), h(0) { shapeType = CollisionShape::BOX; }
	virtual ~BoxCollision2D() {}

	int			width()							const		{ return w; }
	int			height()						const		{ return h; }


private:
	int			w, h;
};



class SphereCollision2D : public CollisionObj
{
public:
	SphereCollision2D() : rad(0) { shapeType = CollisionShape::CIRCLE; }
	SphereCollision2D(const int& radius) : rad(radius) { shapeType = CollisionShape::CIRCLE; }
	virtual ~SphereCollision2D() {}

	int			radius()			const			{ return rad; }
	int			diameter()			const			{ return rad * 2; }

	void		radius(int radius)					{ rad = radius; }
private:
	int			rad;
};


enum class CollisionCategory
{
	// TODO : ī�װ����� �߰��ϱ�

	END
};

class CollisionManager
{
public:
	CollisionManager() {}
	~CollisionManager() {}

	//void collision(CollisionCategory objCat, CollisionCategory sbjCat);

	// �Ϲ� �浹 - list/vector��
	static	bool		collision				(const CollisionObj& object, const CollisionObj& subject, Vector2& collisionVec);
	//�簢���� �簢���� �浹
	static	bool		collision				(const BoxCollision2D& object, const BoxCollision2D& subject, Vector2& collisionVec);
	// ���� �簢���� �浹
	static	bool		collision				(const BoxCollision2D& object, const SphereCollision2D& subject, Vector2& collisionVec);
	// ���� ���� �浹
	static	bool		collision				(const SphereCollision2D& object, const SphereCollision2D& subject, Vector2& collisionVec);
	// ���� �簢���� �浹 - ���콺 ������ �ɵ�
	static	bool		collision				(const BoxCollision2D& object, const POINT& point);
	// ���� ���� �浹 - ���콺 ������ �ɵ�
	static	bool		collision				(const SphereCollision2D& object, const POINT& point);

	// ����� ���� - �浹���� ǥ��
	static	void		showCollisionDEBUG		(const BoxCollision2D& object, HDC hdc);
	static	void		showCollisionDEBUG		(const SphereCollision2D& object, HDC hdc);



private:

	std::list<CollisionObj*>  collisionList[(int)CollisionCategory::END];



	static	bool		lineIntersect			(const Vector2& lineStart, const Vector2& lineEnd, const Vector2& center, const Vector2& otherCenter, Vector2& intersection);
	static	Vector2		findIntersection		(const Vector2& center, const Vector2& otherCenter, const std::vector<Vector2>& vertices);
};




// AABB �浹���� ���
bool CollisionManager::collision(const BoxCollision2D& object, const BoxCollision2D& subject, Vector2& collisionVec)
{
	// �ڵ� ����ȭ �����ϳ� ���߿� �ð������� �Ұ���
	Vector2 objArea = Vector2{ (float)object.width() / 2, (float)object.height() / 2 };
	Vector2 sbjArea = Vector2{ (float)subject.width() / 2, (float)subject.height() / 2 };
	Vector2 objMin = object.position() - objArea;
	Vector2 objMax = object.position() + objArea;
	Vector2 sbjMin = subject.position() - sbjArea;
	Vector2 sbjMax = subject.position() + sbjArea;

	RECT objRect = { objMin.x ,objMin.y , objMax.x, objMax.y };
	RECT sbjRect = { sbjMin.x ,sbjMin.y , sbjMax.x, sbjMax.y };
	RECT collisionRect = { 0, 0, 0, 0 };
	bool result = IntersectRect(&collisionRect, &objRect, &sbjRect);

	collisionVec = Vector2{ fabsf(collisionRect.right - collisionRect.left), fabsf(collisionRect.top - collisionRect.bottom) };
	return result;
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

	// �и� 0�̸� �����ϰų� ��ħ => ����� x,y�Ÿ��� �����Ŵϱ�
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


bool CollisionManager::collision(const CollisionObj& object, const CollisionObj& subject, Vector2& collisionVec)
{
	switch (object.shape())
	{
	case CollisionShape::BOX:
	{
		const BoxCollision2D* objBox = static_cast<const BoxCollision2D*>(&object);
		switch (subject.shape())
		{
		case CollisionShape::BOX:
		{
			const BoxCollision2D* sbjBox = static_cast<const BoxCollision2D*>(&subject);
			return collision(*objBox, *sbjBox, collisionVec);
		}
		case CollisionShape::CIRCLE:
		{
			const SphereCollision2D* sbjSphere = static_cast<const SphereCollision2D*>(&subject);
			return collision(*objBox, *sbjSphere, collisionVec);
		}
		default:
			collisionVec = Vector2::zero();
			return false;
		}
		break;
	}
	case CollisionShape::CIRCLE:
	{
		const SphereCollision2D* objSphere = static_cast<const SphereCollision2D*>(&object);
		switch (subject.shape())
		{
		case CollisionShape::BOX:
		{
			const BoxCollision2D* sbjBox = static_cast<const BoxCollision2D*>(&subject);
			return collision(*objSphere, *sbjBox, collisionVec);
		}
		case CollisionShape::CIRCLE:
		{
			const SphereCollision2D* sbjSphere = static_cast<const SphereCollision2D*>(&subject);
			return collision(*objSphere, *sbjSphere, collisionVec);
		}
		default:
			collisionVec = Vector2::zero();
			return false;
		}
	}
	default:
		collisionVec = Vector2::zero();
		return false;
	}
	return false;
}

bool CollisionManager::collision(const BoxCollision2D& object, const POINT& point)
{
	Vector2 objArea = Vector2{ (float)object.width() / 2, (float)object.height() / 2 };
	Vector2 objMin = object.position() - objArea;
	Vector2 objMax = object.position() + objArea;

	RECT objRect = { objMin.x ,objMin.y , objMax.x, objMax.y };

	return PtInRect(&objRect, point);
}

bool CollisionManager::collision(const SphereCollision2D& object, const POINT& point)
{
	return object.radius() < (Vector2{ (float)point.x, (float)point.y } - object.position()).magnitude();
}

void CollisionManager::showCollisionDEBUG(const BoxCollision2D& object, HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); // ��輱
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));  // ���� ����

	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	//�̺κ��� �ٲ�ߵ�! => ȭ����ǥ��� �ϰ� ���尡 �������� ������ �״�� �ص� ������µ� �����ǥ�踦 ����ٰ� �״�� �����ߴٰ��� ū�ϳ�!
	Rectangle(hdc, object.position().x - object.height(), object.position().y - object.height(), object.position().x + object.height(), object.position().y + object.height());

	// ���� ��� �귯�÷� �ǵ�����
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);

	DeleteObject(hPen);
	DeleteObject(hBrush);
}

void CollisionManager::showCollisionDEBUG(const SphereCollision2D& object, HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); // ��輱
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));  // ���� ����

	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	//�̺κ��� �ٲ�ߵ�! => ȭ����ǥ��� �ϰ� ���尡 �������� ������ �״�� �ص� ������µ� �����ǥ�踦 ����ٰ� �״�� �����ߴٰ��� ū�ϳ�!
	Ellipse(hdc, object.position().x - object.radius(), object.position().y - object.radius(), object.position().x + object.radius(), object.position().y + object.radius());

	// ���� ��� �귯�÷� �ǵ�����
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);

	DeleteObject(hPen);
	DeleteObject(hBrush);
}
