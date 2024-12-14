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
    // ���콺�� �������� ����ĳ����
    static D3DXVECTOR3 ray(LPDIRECT3DDEVICE9 pDevice, int mouseX, int mouseY);
    // ����ĳ���ÿ� �޽��� �ɸ����� Ȯ��
    static BOOL CheckRayMeshIntersection(LPD3DXMESH mesh, D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, D3DXVECTOR3& hitPoint);
    static void OnMouseClick(LPDIRECT3DDEVICE9 d3dDevice, std::vector<LPD3DXMESH> sceneMeshes, int mouseX, int mouseY, D3DXVECTOR3 cameraPosition);
    // ���콺�� ���� ���� �ִ� ���� �Ž��� �ε����� ��ȯ --> ���� ����
    static std::vector<int> GetIntersectedObjects(LPDIRECT3DDEVICE9 d3dDevice, std::vector<LPD3DXMESH> sceneMeshes, int mouseX, int mouseY, D3DXVECTOR3 cameraPosition);
    static bool IntersectRayAABB(D3DXVECTOR3 camPosition, int mouseX, int mouseY, LPDIRECT3DDEVICE9 d3dDevice, D3DXVECTOR3 aabbMin, D3DXVECTOR3 aabbMax);
private:

};



D3DXVECTOR3 Raycast::ray(LPDIRECT3DDEVICE9 pDevice, int mouseX, int mouseY)
{
    // ����Ʈ ���� ��������
    D3DVIEWPORT9 viewport;
    pDevice->GetViewport(&viewport);

    // ���콺 ��ũ�� ��ǥ -> ���� ��ǥ�� ��ȯ (Z ���� ������ ���� �����/�� ���� �ش��ϴ� ���� ����(0.0f~1.0f�� ����ȭ�� ���� ����))
    D3DXVECTOR3 nearPoint(mouseX, mouseY, 0.0f);
    D3DXVECTOR3 farPoint(mouseX, mouseY, 1.0f);

    D3DXMATRIX projectionMatrix, viewMatrix, worldMatrix;
    pDevice->GetTransform(D3DTS_PROJECTION, &projectionMatrix);
    pDevice->GetTransform(D3DTS_VIEW, &viewMatrix);
    D3DXMatrixIdentity(&worldMatrix);  // ���� ��ȯ ��Ʈ������ ���� ��� ���� ���

    // ���콺 ��ǥ�κ��� ���� �� ���Ÿ��� ���� ��ǥ�� ��ȯ
    D3DXVec3Unproject(&nearPoint, &nearPoint, &viewport, &projectionMatrix, &viewMatrix, &worldMatrix);
    D3DXVec3Unproject(&farPoint, &farPoint, &viewport, &projectionMatrix, &viewMatrix, &worldMatrix);

    // nearPoint���� farPoint�� ���ϴ� ������ ���� ���
    D3DXVECTOR3 rayDirection = farPoint - nearPoint;
    D3DXVec3Normalize(&rayDirection, &rayDirection);

    return rayDirection;
}

BOOL Raycast::CheckRayMeshIntersection(LPD3DXMESH mesh, D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, D3DXVECTOR3& hitPoint)
{
    BOOL hit;
    DWORD faceIndex;
    float distance;
    FLOAT u, v; // �̰� ���̰� ��Ȯ�� ��� ��Ҵ��� Ȯ���ϴ°� -> ���⼱ �ȽἭ �Ű������� ���� ���Ǹ� ��

    // ���̿� �޽��� �浹�� �׽�Ʈ
    D3DXIntersect(mesh, &rayOrigin, &rayDirection, &hit, &faceIndex, &u, &v, &distance, NULL, NULL);

    // �浹�� �����Ǹ� hitPoint�� �浹 ���� ����
    if (hit)
    {
        D3DXVECTOR3 hitPos = rayOrigin + (rayDirection * distance);
        hitPoint = hitPos;
    }

    return hit;
}

// ����ĳ��Ʈ �� ���� �ִ� �� ������
void Raycast::OnMouseClick(LPDIRECT3DDEVICE9 d3dDevice, std::vector<LPD3DXMESH> sceneMeshes, int mouseX, int mouseY, D3DXVECTOR3 cameraPosition)
{
    D3DXVECTOR3 rayDirection = ray(d3dDevice, mouseX, mouseY);

    // ��� ��ü�� ���� �浹 �˻�
    for (int i = 0; i < sceneMeshes.size(); i++)
    {
        D3DXVECTOR3 hitPoint;
        if (CheckRayMeshIntersection(sceneMeshes[i], cameraPosition, rayDirection, hitPoint))
        {
            // �浹�� ��ü ó��
            //std::cout << "Mesh " << i << "�� �浹 �߻�!\n";
        }
    }
}

// ����ĳ��Ʈ ���߰˻�
std::vector<int> Raycast::GetIntersectedObjects(LPDIRECT3DDEVICE9 d3dDevice, std::vector<LPD3DXMESH> sceneMeshes, int mouseX, int mouseY, D3DXVECTOR3 cameraPosition)
{
    // �浹�� ��ü���� �ε����� ������ ����
    std::vector<int> intersectedObjects;

    // ���콺 Ŭ�� ��ǥ�κ��� ���� ���� ���ϱ�
    D3DXVECTOR3 rayDirection = ray(d3dDevice, mouseX, mouseY);

    // ��� ��ü�� ���� �浹 �˻�
    for (int i = 0; i < sceneMeshes.size(); i++)
    {
        D3DXVECTOR3 hitPoint;
        if (CheckRayMeshIntersection(sceneMeshes[i], cameraPosition, rayDirection, hitPoint))
        {
            // �浹�� ��ü�� �ε����� ����Ʈ�� �߰�
            intersectedObjects.push_back(i);
        }
    }

    return intersectedObjects; // �浹�� ��ü���� ����Ʈ ��ȯ
}




// �̰� �����ؾߵɵ� -> ���� �˰��� ����ϸ� ��
bool Raycast::IntersectRayAABB(D3DXVECTOR3 camPosition, int mouseX, int mouseY, LPDIRECT3DDEVICE9 d3dDevice, D3DXVECTOR3 aabbMin, D3DXVECTOR3 aabbMax)
{
    D3DXVECTOR3 rayDirection = ray(d3dDevice, mouseX, mouseY);

    // AABB�� �� ������ �������� �ϴ� ��
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
