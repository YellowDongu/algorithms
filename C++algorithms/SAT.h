#pragma once
#include <cmath>
#include <vector>
#include "Vector2.h"



// sat(Separating Axis Theorem)
// 한 도형의 한 선분을 가져옴
// 가져온 선분을 연장시킴
// 연장된 선분에 다른 도형의 꼭지점과 직교되는 선분을 그음
// 직교되는 선분들의 교차점(투영된 도형의 꼭지점들이라 함)을 모두 가져옴 => 2차원의 도형을 한차원 아래로 내려서 표현
// 가져온 점들을 하나의 선으로 이음 => 하나라도 안겹친게 있다면 그건 충돌 안된거임(어디선가 떨어져있는 부분이 있다는 거니깐)
// 이 선과 가져온 선분이 겹치는지 확인
// 안겹치면 충돌 안한거임
// 겹치면 다른 선분을 가져와서 위를 반복
// 다른 도형도 위의 과정을 반복
// 모두 겹치는거면 충돌된거임
// https://www.sevenson.com.au/programming/sat/

// sat를 완벽하게 다루는 건 아니라서 다각형을 사용해보는건 좀 더 뜯어봐야될듯

struct TriangleMesh // 메쉬의 기본인 삼각형을 사용
{
	Vector2 centerPos;
	std::vector<Vector2> vertices;
};


struct Circle // 원은 혹시 몰라서 일단 만들어놓음
{
	Vector2 centerPos;
	float radius;
};


// 삼각형&Vector2 전용으로 만든 충돌 감지 알고리즘
// 물론 vector2를 x,y좌표로 풀고 삼각형대신 사각형을 넣게 하는 등 약간의 수정만 있으면 다른 환경에서도 잘 돌아간다.
class SATCollision2D
{
public:
	static bool checkCollision(const TriangleMesh& rectA, const TriangleMesh& rectB, Vector2& collisionDir);
	static bool checkCollision(const TriangleMesh& tri, const Circle& circle, Vector2& collisionVec);
	// 구와 구의 충돌 처리는 너무 쉬워서 스킵
private:
	static std::vector<Vector2> getAxes(const std::vector<Vector2>& vertices);
	static void project(const std::vector<Vector2>& vertices, const Vector2& axis, float& min, float& max);
	static bool isOverlapping(const float& minA, const float& maxA, const float& minB, const float& maxB);
	static bool lineIntersect(const Vector2& lineStart, const Vector2& lineEnd, const Vector2& center, const Vector2& otherCenter, Vector2& intersection);
	static Vector2 findIntersection(const Vector2& center, const Vector2& otherCenter, const std::vector<Vector2>& vertices);
};

// 도형의 선분들의 수직선분을 벡터화시키고 묶어두기
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

// 한 선분을 기준으로 다른 도형을 투영해 한 차원 아래로 내리고 계산
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

// 삼각형간의 충돌계산
bool SATCollision2D::checkCollision(const TriangleMesh& rectA, const TriangleMesh& rectB, Vector2& collisionDir)
{
	float smallestOverlap = FLT_MAX; // 이건 얼마나 충돌했는지 기록
	// 위 두개를 없애도 상관은 없지만 충돌 후 틩겨나가게 처리하려면 필요할 수 있어서 만들어둠

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
			collisionDir = axis; // 현재 축을 충돌한 면의 법선으로 저장
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
			collisionDir = axis; // 현재 축을 충돌한 면의 법선으로 저장
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

	// 분모가 0이면 평행하거나 겹침
	if (denominator == 0.0f)
		return false;

	float insersectAtLine = ((lineStart.x - center.x) * (center.y - otherCenter.y) - (lineStart.y - center.y) * (center.x - otherCenter.x)) / denominator;
	float intersectAtCenterLine = ((lineStart.x - center.x) * (lineStart.y - lineEnd.y) - (lineStart.y - center.y) * (lineStart.x - lineEnd.x)) / denominator;

	// t와 u가 [0, 1] 범위 내에 있을 때 선분이 교차함
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
		Vector2 point2 = vertices[(i + 1) % vertices.size()]; // 다각형은 닫힌 도형이므로 마지막 점과 첫 번째 점을 연결

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


// 어느 방향으로 얼마나 겹쳤으면 겹친 방향벡터의 반대방향들을 합해서 나온 방향벡터를 겹친 도형에게 더해주면 or 두 도형에게 고루 분배하면 원상복귀가 되지 않을까
// 다만 단위벡터면 안됨
// 자신이 가지고 있는 속력 벡터(속도 아님!)를 저장해서
// 충돌이 났고 충돌 방항벡터가 나왔으면
// 반사벡터를 구해서 자신의 속력 벡터에 덮어쓰면
// 충돌해서 틩겨나오는걸 구현할 수 있을까?

// 속력이 떨어지는건 마찰로 구해야되는데
// 자신이 가지고 있는 질랑에 마찰력을 구해서
// 마찰력 = 정마찰계수/동마찰계수 * 수직항력
// 프레임마다 빼버리면 된다.