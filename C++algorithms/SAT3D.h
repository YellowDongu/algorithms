#pragma once
#include <vector>
#include "Vector3.h"


#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "winmm.lib")

/*
1차 필터링: SAT를 사용한 단순 도형 충돌 검사
2차 필터링: D3DXIntersect를 사용한 세밀한 충돌 검사

BOOL D3DXIntersect(
  LPDIRECT3DDEVICE9 pDevice, Direct3D   장치 포인터
  LPD3DXMESH pMesh,                     충돌을 검사할 메쉬
  const D3DXVECTOR3* pRayOrigin,        레이의 시작점
  const D3DXVECTOR3* pRayDirection,     레이의 방향 벡터
  BOOL* pHit,                           충돌 여부를 반환받을 변수
  D3DXVECTOR3* pHitPosition,            충돌 지점을 반환받을 변수
  D3DXVECTOR3* pHitNormal,              충돌 표면의 법선 벡터를 반환받을 변수
  float* pHitDistance,                  충돌까지의 거리를 반환받을 변수
  DWORD* pFaceIndex                     충돌이 발생한 삼각형의 인덱스를 반환받을 변수
);


bool CheckMeshCollision(LPDIRECT3DDEVICE9 pDevice, LPD3DXMESH meshA, LPD3DXMESH meshB) {
    BOOL hit;
    D3DXVECTOR3 rayOrigin(0.0f, 0.0f, 0.0f); // 초기 레이 원점
    D3DXVECTOR3 rayDirection(0.0f, 0.0f, 1.0f); // 초기 레이 방향
    float hitDistance;
    D3DXVECTOR3 hitPosition, hitNormal;
    DWORD faceIndex;

    // 메쉬A의 각 삼각형에 대해 레이와의 교차 검사
    for (DWORD i = 0; i < meshA->GetNumFaces(); ++i) {
        if (D3DXIntersect(pDevice, meshA, &rayOrigin, &rayDirection, &hit, &hitPosition, &hitNormal, &hitDistance, &faceIndex)) {
            // 메쉬A의 삼각형과 레이의 교차 여부를 검사하여 메쉬B와의 충돌을 추정
            if (hit) {
                return true; // 충돌 발생
            }
        }
    }

    return false; // 충돌 없음
}   
*/



struct PolygonMesh
{
    std::vector<D3DXVECTOR3> vertices; // 정점
    std::vector<D3DXVECTOR3> normals; // 법선벡터
    std::vector<int> indices;
    Vector3 position;
};
struct SphereMesh
{
    float radius;
    Vector3 position;
};

class SATCollision3D
{
public:
    static bool checkCollision(const PolygonMesh& meshA, const PolygonMesh& meshB, Vector3& collisionDir);
    static bool checkCollision(const PolygonMesh& polygon, const SphereMesh& sphere, Vector3& collisionVec);
    static bool checkCollision(const SphereMesh& obj, const SphereMesh& sbj, Vector3& collisionVec);
private:
    static std::vector<Vector3> getAxes(const PolygonMesh& meshA, const PolygonMesh& meshB);
    static void project(const std::vector<Vector3>& vertices, const Vector3& axis, float& min, float& max);
    static bool isOverlapping(float minA, float maxA, float minB, float maxB);
    static bool IntersectRayTriangle(const Vector3& rayOrigin, const Vector3& rayDir, std::vector<Vector3>& vertices, float& intersectDir);
    static Vector3 FindLastIntersection(const Vector3& rayOrigin, const Vector3& rayDir, const std::vector<Vector3>& vertices, const std::vector<int>& indices);
};

// 충돌 축 생성: 두 다면체의 법선 벡터와 모서리 교차곱을 사용
std::vector<Vector3> SATCollision3D::getAxes(const PolygonMesh& meshA, const PolygonMesh& meshB)
{
    std::vector<Vector3> axes;

    // A와 B의 각 면의 법선 벡터를 축으로 추가
    for (const D3DXVECTOR3& normalA : meshA.normals)
    {
        axes.push_back(Vector3::toVec3(normalA));
    }
    for (const D3DXVECTOR3& normalB : meshB.normals)
    {
        axes.push_back(Vector3::toVec3(normalB));
    }

    for (size_t i = 0; i < meshA.vertices.size(); ++i)
    {
        for (size_t j = 0; j < meshB.vertices.size(); ++j)
        {
            Vector3 edgeA = Vector3::toVec3(meshA.vertices[(i + 1) % meshA.vertices.size()] - meshA.vertices[i]);
            Vector3 edgeB = Vector3::toVec3(meshB.vertices[(j + 1) % meshB.vertices.size()] - meshB.vertices[j]);
            Vector3 axis = edgeA.cross(edgeB).normalize();
            if (axis.magnitude() > 0.0f)
            {
                axes.push_back(axis);
            }
        }
    }

    return axes;
}

// 투영 계산
void SATCollision3D::project(const std::vector<Vector3>& vertices, const Vector3& axis, float& min, float& max)
{
    float projection = axis.dot(vertices[0]);
    min = max = projection;
    for (const Vector3& vertex : vertices)
    {
        projection = axis.dot(vertex);
        if (projection < min) min = projection;
        if (projection > max) max = projection;
    }
}

// 투영이 겹치는지 확인
bool SATCollision3D::isOverlapping(float minA, float maxA, float minB, float maxB)
{
    return !(maxA < minB || maxB < minA);
}

// 3D 충돌 감지
bool SATCollision3D::checkCollision(const PolygonMesh& meshA, const PolygonMesh& meshB, Vector3& collisionDir)
{
    float smallestOverlap = FLT_MAX;

    std::vector<Vector3> axes = getAxes(meshA, meshB);
    std::vector<Vector3> meshAVertices;
    for (const D3DXVECTOR3& const vertice : meshA.vertices)
    {
        meshAVertices.push_back(Vector3::toVec3(vertice));
    }
    std::vector<Vector3> meshBVertices;
    for (const D3DXVECTOR3& const vertice : meshB.vertices)
    {
        meshAVertices.push_back(Vector3::toVec3(vertice));
    }

    for (const Vector3& axis : axes)
    {
        float minA, maxA, minB, maxB;
        project(meshAVertices, axis, minA, maxA);
        project(meshBVertices, axis, minB, maxB);

        if (!isOverlapping(minA, maxA, minB, maxB))
        {
            return false; // 충돌이 없는 축이 하나라도 있으면 충돌 없음
        }
        float overlap = (maxA - minB < maxB - minA) ? (maxA - minB) : (maxB - minA);
        if (overlap < smallestOverlap)
        {
            smallestOverlap = overlap;
            collisionDir = axis; // 현재 축을 충돌한 면의 법선으로 저장
        }
    }

    return true; // 모든 축에서 겹침이 있으면 충돌 발생
}



bool SATCollision3D::IntersectRayTriangle(const Vector3& rayOrigin, const Vector3& rayDir, std::vector<Vector3>& vertices, float& intersectDir)
{
    float uPos = 0, vPos = 0;
    const float EPSILON = 1e-6f;
    Vector3 line1 = vertices[1] - vertices[0];
    Vector3 line2 = vertices[2] - vertices[0];
    Vector3 rayCross = line1.cross(rayDir); 
    Vector3 relativeRayOrigin = rayOrigin - vertices[0];
    float denominator = line2.dot(rayCross);

    if (denominator > -EPSILON && denominator < EPSILON)
        return false; // Ray is parallel to the triangle

    float reciprocalDen = 1.0f / denominator;
    uPos = reciprocalDen * relativeRayOrigin.dot(rayCross);

    if (uPos < 0.0f || uPos > 1.0f)
        return false;

    Vector3 tempCrossResult = relativeRayOrigin.cross(line1);
    vPos = reciprocalDen * rayDir.dot(tempCrossResult);

    if (vPos < 0.0f || uPos + vPos > 1.0f)
        return false;

    intersectDir = reciprocalDen * line2.dot(tempCrossResult);

    if (intersectDir > EPSILON) // Intersection occurs
        return true;

    return false;
}

Vector3 SATCollision3D::FindLastIntersection(const Vector3& rayOrigin, const Vector3& rayDir, const std::vector<Vector3>& vertices, const std::vector<int>& indices)
{
    float maxDist = -FLT_MAX;
    Vector3 intersectPoint = {0,0,0};

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        std::vector<Vector3> points{ vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]] };

        float distance;
        if (IntersectRayTriangle(rayOrigin, rayDir, points, distance))
        {
            if (distance > maxDist)
            {
                maxDist = distance;
                intersectPoint = rayOrigin + rayDir * distance;
            }
        }
    }

    return intersectPoint;
}

bool SATCollision3D::checkCollision(const PolygonMesh& polygon, const SphereMesh& sphere, Vector3& collisionVec)
{
    std::vector<Vector3> vertices;
    for (D3DXVECTOR3 dxVertice : polygon.vertices)
    {
        vertices.push_back(Vector3::toVec3(dxVertice));
    }

    Vector3 intersectPoint = FindLastIntersection(polygon.position, sphere.position, vertices, polygon.indices);
    collisionVec = intersectPoint - sphere.position;
    return collisionVec.magnitude() > sphere.radius;
}

bool SATCollision3D::checkCollision(const SphereMesh& obj, const SphereMesh& sbj, Vector3& collisionVec)
{
    Vector3 direction = sbj.position - obj.position;

    float collisionAmount = (obj.radius + sbj.radius) - direction.magnitude();

    collisionVec = direction.normalize() * collisionAmount;

    return collisionAmount < 0;
}