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

// AABB 충돌 감지 기법
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
	// TODO : 카테고리끼리 추가하기

	END
};

class CollisionManager
{
public:
	CollisionManager() {}
	~CollisionManager() {}

	//void collision(CollisionCategory objCat, CollisionCategory sbjCat);

	// 일반 충돌 - list/vector용
	static	bool		collision				(const CollisionObj& object, const CollisionObj& subject, Vector2& collisionVec);
	//사각형과 사각형의 충돌
	static	bool		collision				(const BoxCollision2D& object, const BoxCollision2D& subject, Vector2& collisionVec);
	// 원과 사각형의 충돌
	static	bool		collision				(const BoxCollision2D& object, const SphereCollision2D& subject, Vector2& collisionVec);
	// 원과 원의 충돌
	static	bool		collision				(const SphereCollision2D& object, const SphereCollision2D& subject, Vector2& collisionVec);
	// 점과 사각형의 충돌 - 마우스 전용이 될듯
	static	bool		collision				(const BoxCollision2D& object, const POINT& point);
	// 점과 원의 충돌 - 마우스 전용이 될듯
	static	bool		collision				(const SphereCollision2D& object, const POINT& point);

	// 디버그 전용 - 충돌영역 표시
	static	void		showCollisionDEBUG		(const BoxCollision2D& object, HDC hdc);
	static	void		showCollisionDEBUG		(const SphereCollision2D& object, HDC hdc);



private:

	std::list<CollisionObj*>  collisionList[(int)CollisionCategory::END];



	static	bool		lineIntersect			(const Vector2& lineStart, const Vector2& lineEnd, const Vector2& center, const Vector2& otherCenter, Vector2& intersection);
	static	Vector2		findIntersection		(const Vector2& center, const Vector2& otherCenter, const std::vector<Vector2>& vertices);
};




// AABB 충돌감지 기법
bool CollisionManager::collision(const BoxCollision2D& object, const BoxCollision2D& subject, Vector2& collisionVec)
{
	// 코드 최적화 가능하나 나중에 시간남을때 할거임
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

// 선분이 교차하는 지점과 원의 중심까지의 거리가 반지름보다 짧으면 충돌하는 판정
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

// 두 선분의 교차점을 찾은 후 intersection으로 반환, 교차되었는지 여부도 반환
// lineStart와 lineEnd는 다각형의 선분 중 하나, center는 다각형의 중심, otherCenter는 원의 중심
bool CollisionManager::lineIntersect(const Vector2& lineStart, const Vector2& lineEnd, const Vector2& center, const Vector2& otherCenter, Vector2& intersection)
{
	float denominator = (lineStart.x - lineEnd.x) * (center.y - otherCenter.y) - (lineStart.y - lineEnd.y) * (center.x - otherCenter.x);

	// 분모가 0이면 평행하거나 겹침 => 상대적 x,y거리가 같을거니깐
	if (denominator == 0.0f)
		return false;

	float tPosNormal = ((lineStart.x - center.x) * (center.y - otherCenter.y) - (lineStart.y - center.y) * (center.x - otherCenter.x)) / denominator;
	float uPosNormal = ((lineStart.x - center.x) * (lineStart.y - lineEnd.y) - (lineStart.y - center.y) * (lineStart.x - lineEnd.x)) / denominator;

	// t와 u가 [0, 1] 범위 내에 있을 때 선분이 교차함 (t와 u는 텍스처같은 곳에 쓰는 좌표계)
	if (tPosNormal >= 0 && tPosNormal <= 1 && uPosNormal >= 0 && uPosNormal <= 1)
	{
		intersection.x = lineStart.x + tPosNormal * (lineEnd.x - lineStart.x);
		intersection.y = lineStart.y + tPosNormal * (lineEnd.y - lineStart.y);
		return true;
	}

	return false;
}

// 다각형의 중심에서 다른 점으로 뻗은 선과 다각형 선분들의 교차점 중 제일 마지막 교차점 반환
Vector2 CollisionManager::findIntersection(const Vector2& center, const Vector2& otherCenter, const std::vector<Vector2>& vertices)
{
	Vector2 result = { 0, 0 };
	float maxDistance = FLT_MIN;

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		Vector2 point1 = vertices[i];
		Vector2 point2 = vertices[(i + 1) % vertices.size()]; // 다각형은 닫힌 도형이므로 마지막 점과 첫 번째 점을 연결

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
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); // 경계선
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));  // 내부 색상

	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	//이부분은 바꿔야됨! => 화면좌표계로 하고 월드가 움직이지 않으면 그대로 해도 상관없는데 상대좌표계를 여기다가 그대로 대입했다가는 큰일남!
	Rectangle(hdc, object.position().x - object.height(), object.position().y - object.height(), object.position().x + object.height(), object.position().y + object.height());

	// 이전 펜과 브러시로 되돌리기
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);

	DeleteObject(hPen);
	DeleteObject(hBrush);
}

void CollisionManager::showCollisionDEBUG(const SphereCollision2D& object, HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); // 경계선
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));  // 내부 색상

	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	//이부분은 바꿔야됨! => 화면좌표계로 하고 월드가 움직이지 않으면 그대로 해도 상관없는데 상대좌표계를 여기다가 그대로 대입했다가는 큰일남!
	Ellipse(hdc, object.position().x - object.radius(), object.position().y - object.radius(), object.position().x + object.radius(), object.position().y + object.radius());

	// 이전 펜과 브러시로 되돌리기
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);

	DeleteObject(hPen);
	DeleteObject(hBrush);
}
