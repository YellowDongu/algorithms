#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "winmm.lib")

//
//Frustum Culling
/*
view Frustum
ī�޶󿡼� �� �� �ִ� ���� ����, �� �ȿ� ��ü�� ������ ȭ�鿡 ������
������ 6��ü(����ü) - �� ���(View Matrix), ���� ���(Projection Matrix)�� ����� ����
�� ����� ���ϸ� ��ü ������ ��ü�� �������� ���� �ִ��� �˻� ����

Frustum Culling(�������� �ø�)
������ �浹ó���� �ƶ��� ����
�þ߰� ���ߴ� ������ ����ü�� �����ؼ� �� ��ü�� �����̳� �޽��� ��ġ�� ������ �ִ°��� �Ǵ��ϴ°���
��ü�� ��� �ڽ�(Bounding Box)�� ��(Bounding Sphere)�� �� �������Ұ� ���Ͽ� �� ��ü�� ���̴� ������ �ִ��� Ȯ��

�ƴϸ� �ش� ����ü �ȿ� ��ü�� �߽ɰ��� ��ü�� ��ǥ�� ������ �ִٰ� ǥ���ص� ��������
�̰� ����ī�޶� Ŭ������ �����ؼ� ���°� ������


���� �м��� ������ ���� �ڵ� :: ��뿡 ���� ���
*/


class Frustum
{
public:
    Frustum() : planes{} {}

    void update(D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);


    bool CheckSphere(const D3DXVECTOR3& center, float radius) const;
    bool CheckAABB(const D3DXVECTOR3& min, const D3DXVECTOR3& max) const;
private:
    D3DXPLANE planes[6];

    void UpdateFrustum(D3DXMATRIX _viewProjMatrix);
};

void Frustum::update(D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
    UpdateFrustum(viewMatrix * projectionMatrix);
}

// ��-���� ��Ŀ��� ���������� ������ ����
void Frustum::UpdateFrustum(D3DXMATRIX _viewProjMatrix)
{
    planes[0].a = _viewProjMatrix._14 + _viewProjMatrix._11; // Left plane
    planes[0].b = _viewProjMatrix._24 + _viewProjMatrix._21;
    planes[0].c = _viewProjMatrix._34 + _viewProjMatrix._31;
    planes[0].d = _viewProjMatrix._44 + _viewProjMatrix._41;

    planes[1].a = _viewProjMatrix._14 - _viewProjMatrix._11; // Right plane
    planes[1].b = _viewProjMatrix._24 - _viewProjMatrix._21;
    planes[1].c = _viewProjMatrix._34 - _viewProjMatrix._31;
    planes[1].d = _viewProjMatrix._44 - _viewProjMatrix._41;

    planes[2].a = _viewProjMatrix._14 - _viewProjMatrix._12; // Top plane
    planes[2].b = _viewProjMatrix._24 - _viewProjMatrix._22;
    planes[2].c = _viewProjMatrix._34 - _viewProjMatrix._32;
    planes[2].d = _viewProjMatrix._44 - _viewProjMatrix._42;

    planes[3].a = _viewProjMatrix._14 + _viewProjMatrix._12; // Bottom plane
    planes[3].b = _viewProjMatrix._24 + _viewProjMatrix._22;
    planes[3].c = _viewProjMatrix._34 + _viewProjMatrix._32;
    planes[3].d = _viewProjMatrix._44 + _viewProjMatrix._42;

    planes[4].a = _viewProjMatrix._13; // Near plane
    planes[4].b = _viewProjMatrix._23;
    planes[4].c = _viewProjMatrix._33;
    planes[4].d = _viewProjMatrix._43;

    planes[5].a = _viewProjMatrix._14 - _viewProjMatrix._13; // Far plane
    planes[5].b = _viewProjMatrix._24 - _viewProjMatrix._23;
    planes[5].c = _viewProjMatrix._34 - _viewProjMatrix._33;
    planes[5].d = _viewProjMatrix._44 - _viewProjMatrix._43;

    // �� ����� ����ȭ
    for (int i = 0; i < 6; i++)
        D3DXPlaneNormalize(&planes[i], &planes[i]);
}

// �������� ��ü�� �������� ��� �� �ϳ��� ������ �Ѿ�ԵǸ� ȭ�鿡 ������ ����
bool Frustum::CheckSphere(const D3DXVECTOR3& center, float radius) const
{
    for (int i = 0; i < 6; i++)
    {
        if (D3DXPlaneDotCoord(&planes[i], &center) + radius < 0.0f)
            return false;  // �������� �ۿ� ����
    }
    return true;  // �������� �ȿ� ����
}

// AABB �浹�˻������� ���������� �ȿ� �ִ��� Ȯ��
bool Frustum::CheckAABB(const D3DXVECTOR3& min, const D3DXVECTOR3& max) const
{
    // ���������� 6�� ���� ��� ���� ������ ���� ���� Ȯ��
    for (int i = 0; i < 6; ++i)
    {
        D3DXVECTOR3 vmin, vmax;

        // �������� ����� ������ ���� ��� ������ ���� �� ���� ����� ���� ����
        if (planes[i].a > 0)
        {
            vmin.x = min.x;
            vmax.x = max.x;
        }
        else
        {
            vmin.x = max.x;
            vmax.x = min.x;
        }

        if (planes[i].b > 0)
        {
            vmin.y = min.y;
            vmax.y = max.y;
        }
        else
        {
            vmin.y = max.y;
            vmax.y = min.y;
        }

        if (planes[i].c > 0)
        {
            vmin.z = min.z;
            vmax.z = max.z;
        }
        else
        {
            vmin.z = max.z;
            vmax.z = min.z;
        }

        // ��鿡 ���� ��� ���ڰ� ������ �ٱ��� ������ false ��ȯ
        if (D3DXPlaneDotCoord(&planes[i], &vmax) < 0.0f)
            return false;
    }

    return true; // ��� ���ڰ� �������� �ȿ� ����
}

// �������� ����
void temp(D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
    Frustum frustum;
    frustum.update(viewMatrix, projectionMatrix);

    // ��ü�� ��� ���� �������� �ȿ� �ִ��� Ȯ��
    D3DXVECTOR3 objectCenter = D3DXVECTOR3(0.0f, 0.0f, 5.0f); // ��ü�� �߽�
    float objectRadius = 1.0f; // ��ü�� ������
    if (frustum.CheckSphere(objectCenter, objectRadius))
    {
        // ��ü�� ȭ�鿡 ���� (�������� �ȿ� ����)
    }
    else
    {
        // ��ü�� ȭ�鿡 ������ ���� (�������� �ۿ� ����)
    }
}