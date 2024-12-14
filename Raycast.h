#pragma once
#include <vector>


#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "winmm.lib")


class Raycast
{
public:
    // 마우스가 찍은곳에 레이캐스팅
    static D3DXVECTOR3 ray(LPDIRECT3DDEVICE9 pDevice, int mouseX, int mouseY);
    // 레이캐스팅에 메쉬가 걸리는지 확인
    static BOOL CheckRayMeshIntersection(LPD3DXMESH mesh, D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, D3DXVECTOR3& hitPoint);
    static void OnMouseClick(LPDIRECT3DDEVICE9 d3dDevice, std::vector<LPD3DXMESH> sceneMeshes, int mouseX, int mouseY, D3DXVECTOR3 cameraPosition);
    // 마우스가 찍은 곳에 있는 여러 매쉬의 인덱스를 반환 --> 변형 가능
    static std::vector<int> GetIntersectedObjects(LPDIRECT3DDEVICE9 d3dDevice, std::vector<LPD3DXMESH> sceneMeshes, int mouseX, int mouseY, D3DXVECTOR3 cameraPosition);
    static bool IntersectRayAABB(D3DXVECTOR3 camPosition, int mouseX, int mouseY, LPDIRECT3DDEVICE9 d3dDevice, D3DXVECTOR3 aabbMin, D3DXVECTOR3 aabbMax);
private:

};



D3DXVECTOR3 Raycast::ray(LPDIRECT3DDEVICE9 pDevice, int mouseX, int mouseY)
{
    // 뷰포트 정보 가져오기
    D3DVIEWPORT9 viewport;
    pDevice->GetViewport(&viewport);

    // 마우스 스크린 좌표 -> 월드 좌표로 변환 (Z 값은 설정할 제일 가까운/먼 점에 해당하는 값을 설정(0.0f~1.0f의 정규화된 값을 넣음))
    D3DXVECTOR3 nearPoint(mouseX, mouseY, 0.0f);
    D3DXVECTOR3 farPoint(mouseX, mouseY, 1.0f);

    D3DXMATRIX projectionMatrix, viewMatrix, worldMatrix;
    pDevice->GetTransform(D3DTS_PROJECTION, &projectionMatrix);
    pDevice->GetTransform(D3DTS_VIEW, &viewMatrix);
    D3DXMatrixIdentity(&worldMatrix);  // 월드 변환 매트릭스가 없을 경우 단위 행렬

    // 마우스 좌표로부터 근접 및 원거리의 월드 좌표로 변환
    D3DXVec3Unproject(&nearPoint, &nearPoint, &viewport, &projectionMatrix, &viewMatrix, &worldMatrix);
    D3DXVec3Unproject(&farPoint, &farPoint, &viewport, &projectionMatrix, &viewMatrix, &worldMatrix);

    // nearPoint에서 farPoint로 향하는 레이의 방향 계산
    D3DXVECTOR3 rayDirection = farPoint - nearPoint;
    D3DXVec3Normalize(&rayDirection, &rayDirection);

    return rayDirection;
}

BOOL Raycast::CheckRayMeshIntersection(LPD3DXMESH mesh, D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, D3DXVECTOR3& hitPoint)
{
    BOOL hit;
    DWORD faceIndex;
    float distance;
    FLOAT u, v; // 이건 레이가 정확히 어디에 닿았는지 확인하는거 -> 여기선 안써서 매개변수를 위해 정의만 함

    // 레이와 메쉬의 충돌을 테스트
    D3DXIntersect(mesh, &rayOrigin, &rayDirection, &hit, &faceIndex, &u, &v, &distance, NULL, NULL);

    // 충돌이 감지되면 hitPoint에 충돌 지점 저장
    if (hit)
    {
        D3DXVECTOR3 hitPos = rayOrigin + (rayDirection * distance);
        hitPoint = hitPos;
    }

    return hit;
}

// 레이캐스트 젤 위에 있는 것 가져옴
void Raycast::OnMouseClick(LPDIRECT3DDEVICE9 d3dDevice, std::vector<LPD3DXMESH> sceneMeshes, int mouseX, int mouseY, D3DXVECTOR3 cameraPosition)
{
    D3DXVECTOR3 rayDirection = ray(d3dDevice, mouseX, mouseY);

    // 모든 객체에 대해 충돌 검사
    for (int i = 0; i < sceneMeshes.size(); i++)
    {
        D3DXVECTOR3 hitPoint;
        if (CheckRayMeshIntersection(sceneMeshes[i], cameraPosition, rayDirection, hitPoint))
        {
            // 충돌한 객체 처리
            //std::cout << "Mesh " << i << "에 충돌 발생!\n";
        }
    }
}

// 레이캐스트 다중검색
std::vector<int> Raycast::GetIntersectedObjects(LPDIRECT3DDEVICE9 d3dDevice, std::vector<LPD3DXMESH> sceneMeshes, int mouseX, int mouseY, D3DXVECTOR3 cameraPosition)
{
    // 충돌한 객체들의 인덱스를 저장할 벡터
    std::vector<int> intersectedObjects;

    // 마우스 클릭 좌표로부터 레이 방향 구하기
    D3DXVECTOR3 rayDirection = ray(d3dDevice, mouseX, mouseY);

    // 모든 객체에 대해 충돌 검사
    for (int i = 0; i < sceneMeshes.size(); i++)
    {
        D3DXVECTOR3 hitPoint;
        if (CheckRayMeshIntersection(sceneMeshes[i], cameraPosition, rayDirection, hitPoint))
        {
            // 충돌한 객체의 인덱스를 리스트에 추가
            intersectedObjects.push_back(i);
        }
    }

    return intersectedObjects; // 충돌한 객체들의 리스트 반환
}




// 이건 보류해야될듯 -> 원리 알고나서 사용하면 됨
bool Raycast::IntersectRayAABB(D3DXVECTOR3 camPosition, int mouseX, int mouseY, LPDIRECT3DDEVICE9 d3dDevice, D3DXVECTOR3 aabbMin, D3DXVECTOR3 aabbMax)
{
    D3DXVECTOR3 rayDirection = ray(d3dDevice, mouseX, mouseY);

    // AABB의 축 정렬을 기준으로 하는 면
    float tMin = 0.0f;
    float tMax = FLT_MAX;

    for (int i = 0; i < 3; ++i)
    {
        float invDir = 1.0f / rayDirection[i];
        float t0 = (aabbMin[i] - camPosition[i]) * invDir;
        float t1 = (aabbMax[i] - camPosition[i]) * invDir;

        if (invDir < 0.0f)
        {
            std::swap(t0, t1);
        }

        tMin = max(tMin, t0);
        tMax = min(tMax, t1);

        if (tMin > tMax)
        {
            return false;
        }
    }

    return true;
}
