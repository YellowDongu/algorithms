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
카메라에서 볼 수 있는 공간 정의, 이 안에 객체가 있으면 화면에 렌더링
가상의 6면체(절두체) - 뷰 행렬(View Matrix), 투영 행렬(Projection Matrix)을 사용해 정의
두 행렬을 곱하면 전체 씬에서 객체가 프러스텀 내에 있는지 검사 가능

Frustum Culling(프러스텀 컬링)
일종의 충돌처리랑 맥락이 같다
시야가 비추는 구간을 육면체로 구현해서 각 객체의 영역이나 메쉬에 겹치는 구간이 있는건지 판단하는거임
객체의 경계 박스(Bounding Box)나 구(Bounding Sphere)를 뷰 프러스텀과 비교하여 그 객체가 보이는 영역에 있는지 확인

아니면 해당 육면체 안에 객체의 중심같은 객체의 좌표가 있으면 있다고 표현해도 괜찮을듯
이걸 메인카메라 클래스에 통합해서 쓰는게 나을듯


아직 분석이 끝나지 않은 코드 :: 사용에 주의 요망
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

// 뷰-투영 행렬에서 프러스텀의 평면들을 추출
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

    // 각 평면을 정규화
    for (int i = 0; i < 6; i++)
        D3DXPlaneNormalize(&planes[i], &planes[i]);
}

// 구형태의 객체가 프러스텀 평면 중 하나를 완전히 넘어가게되면 화면에 보이지 않음
bool Frustum::CheckSphere(const D3DXVECTOR3& center, float radius) const
{
    for (int i = 0; i < 6; i++)
    {
        if (D3DXPlaneDotCoord(&planes[i], &center) + radius < 0.0f)
            return false;  // 프러스텀 밖에 있음
    }
    return true;  // 프러스텀 안에 있음
}

// AABB 충돌검사기법으로 프러스텀의 안에 있는지 확인
bool Frustum::CheckAABB(const D3DXVECTOR3& min, const D3DXVECTOR3& max) const
{
    // 프러스텀의 6개 평면과 경계 상자 사이의 교차 여부 확인
    for (int i = 0; i < 6; ++i)
    {
        D3DXVECTOR3 vmin, vmax;

        // 프러스텀 평면의 법선에 따라 경계 상자의 가장 먼 점과 가까운 점을 구함
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

        // 평면에 대해 경계 상자가 완전히 바깥에 있으면 false 반환
        if (D3DXPlaneDotCoord(&planes[i], &vmax) < 0.0f)
            return false;
    }

    return true; // 경계 상자가 프러스텀 안에 있음
}

// 프러스텀 사용법
void temp(D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
    Frustum frustum;
    frustum.update(viewMatrix, projectionMatrix);

    // 객체의 경계 구가 프러스텀 안에 있는지 확인
    D3DXVECTOR3 objectCenter = D3DXVECTOR3(0.0f, 0.0f, 5.0f); // 객체의 중심
    float objectRadius = 1.0f; // 객체의 반지름
    if (frustum.CheckSphere(objectCenter, objectRadius))
    {
        // 객체가 화면에 보임 (프러스텀 안에 있음)
    }
    else
    {
        // 객체가 화면에 보이지 않음 (프러스텀 밖에 있음)
    }
}