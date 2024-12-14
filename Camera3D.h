#pragma once
#include "Vector3.h"


#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "winmm.lib")



class Camera
{
public:
    enum CameraType { AIRCRAFT, LANDOBJECT };
    Camera() : cameraType(AIRCRAFT) {}
    Camera(CameraType cameraType) : cameraType(cameraType) {}
    ~Camera() {}

    void init(D3DXVECTOR3 position, D3DXVECTOR3 rotation);
    void Update(LPDIRECT3DDEVICE9 Device);

    void strafe(float units);
    void fly(float units);
    void walk(float units);
    void pitch(float angle);
    void yaw(float angle);
    void roll(float angle);
    void getViewMatrix(D3DXMATRIX* V);
    void setCameraType(CameraType _cameraType) { cameraType = _cameraType; }
    void setPosition(D3DXVECTOR3 pos);
    void lookPosition(D3DXVECTOR3 pos) { _look = pos; }

    D3DXVECTOR3 getPosition(D3DXVECTOR3* pos) const { return _pos; }
    D3DXVECTOR3 getRight() const { return _right; }
    D3DXVECTOR3 getUp() const { return _up; }
    D3DXVECTOR3 get1ook() const { return _look; }
private:
    CameraType cameraType;
    D3DXVECTOR3 _right;
    D3DXVECTOR3 _up;
    D3DXVECTOR3 _look;
    D3DXVECTOR3 _pos;

    bool temp = false;
};



void Camera::getViewMatrix(D3DXMATRIX* V)
{
    // 카메라의 축이 서로 직각이 되도록 한다.
    D3DXVec3Normalize(&_look, &_look);
    D3DXVec3Cross(&_up, &_look, &_right);
    D3DXVec3Normalize(&_up, &_up);
    D3DXVec3Cross(&_right, &_up, &_look);
    D3DXVec3Normalize(&_right, &_right);
    // 뷰 행렬율 구성한다.
    float x = -D3DXVec3Dot(&_right, &_pos);
    float y = -D3DXVec3Dot(&_up, &_pos);
    float z = -D3DXVec3Dot(&_look, &_pos);
    (*V) (0, 0) = _right.x;
    (*V) (0, 1) = _up.x;
    (*V) (0, 2) = _look.x;
    (*V) (0, 3) = 0.0f;

    (*V) (1, 0) = _right.y;
    (*V) (1, 1) = _up.y;
    (*V) (1, 2) = _look.y;
    (*V) (1, 3) = 0.0f;

    (*V) (2, 0) = _right.z;
    (*V) (2, 1) = _up.z;
    (*V) (2, 2) = _look.z;
    (*V) (2, 3) = 0.0f;

    (*V) (3, 0) = x;
    (*V) (3, 1) = y;
    (*V) (3, 2) = z;
    (*V) (3, 3) = 1.0f;

}

void Camera::setPosition(D3DXVECTOR3 pos)
{
    D3DXVECTOR3 diff = pos - _pos;
    _pos = pos;
    _right = _right + diff;
    _up = _up + diff;
    _look = _look + diff;
}

void Camera::pitch(float angle)
{
    D3DXMATRIX T;
    D3DXMatrixRotationAxis(&T, &_right, angle);
    D3DXVec3TransformCoord(&_up, &_up, &T);
    D3DXVec3TransformCoord(&_look, &_look, &T);
}
void Camera::yaw(float angle)
{
    D3DXMATRIX T;
    // 상의 물체에 대해서는 월드 y (0 , 1, 이를 준으로 회전한 .
    if (cameraType == LANDOBJECT)
        D3DXMatrixRotationY(&T, angle);
    // 행 체에 대해서는 의 상향 벡터로 회전한다 .
    if (cameraType == AIRCRAFT)
        D3DXMatrixRotationAxis(&T, &_up, angle);

    // rotate rlght and look around _up or y-axis
    D3DXVec3TransformCoord(&_right, &_right, &T);
    D3DXVec3TransformCoord(&_look, &_look, &T);
}

void Camera::roll(float angle)
{
    // 행 타입일 경우만 전한다.
    if (cameraType == AIRCRAFT)
    {
        D3DXMATRIX T;
        D3DXMatrixRotationAxis(&T, &_look, angle);
        //look 벡터 준으로 위와 쪽으로 회전한다 .
        D3DXVec3TransformCoord(&_right, &_right, &T);
        D3DXVec3TransformCoord(&_up, &_up, &T);
    }
}
void Camera::walk(float units)
{
    if (units == 0) return;

    // 지상의 체는 xz 병연 로 움직임을 제한한다.
    if (cameraType == LANDOBJECT)
    {
        // 바라보고 있는 방향을 계산한다.
        D3DXVECTOR3 dir;
        D3DXVECTOR3 worldUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        D3DXVec3Cross(&dir, &_right, &worldUp);
        _pos += D3DXVECTOR3(dir.x, 0.0f, dir.z) * units;
    }
    if (cameraType == AIRCRAFT)
        _pos += _look * units;
}

void Camera::init(D3DXVECTOR3 position, D3DXVECTOR3 rotation)
{
    _pos = position;
    _right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
    _up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    _look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
    pitch(rotation.x);
    yaw(rotation.y);
    roll(rotation.z);

}

void Camera::Update(LPDIRECT3DDEVICE9 Device)
{
    D3DXMATRIX V;
    getViewMatrix(&V);
    Device->SetTransform(D3DTS_VIEW, &V);


    if (!temp) // 시야 세팅(각과 거리) -- init에 넣어야되는데 그건 좀있다가
    {
        // 예를 들어, Perspective Matrix 설정
        D3DXMATRIX projectionMatrix;
        D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 4, 800 / 600, 0.1f, 1000.0f);
        Device->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
        temp = true;
    }
}
void Camera::strafe(float units)
{
    // 지상의 는 X Z 영면으로 움직임을 제한한다 .
    if (cameraType == LANDOBJECT)
        _pos += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units;

    if (cameraType == AIRCRAFT)
        _pos += _right * units;
}

void Camera::fly(float units)
{
    if (cameraType == AIRCRAFT)
        _pos += _up * units;
}




class MainCamera
{
public:
    MainCamera();
    ~MainCamera();

    void Init(LPDIRECT3DDEVICE9 _d3dDevice, Vector3 _camPosition, Vector3 _camRotation);
    //void update();

    void position(Vector3 _position);
    void rotation(Vector3 _rotation);
    void translate(Vector3 _towards);
    void rotate(Vector3 _towards);

    void overrollingCheck(Vector3& _rotation);

    Vector3 position() const { return camPosition; }
    Vector3 rotation() const { return camPosition; }
private:
    LPDIRECT3DDEVICE9 d3dDevice;
    Camera camera;
    Vector3 camPosition;
    Vector3 camRotation;
};

MainCamera::MainCamera() : camPosition(Vector3::zero()), camRotation(Vector3::zero()), d3dDevice(nullptr)
{
}

MainCamera::~MainCamera()
{
}


void MainCamera::Init(LPDIRECT3DDEVICE9 _d3dDevice, Vector3 _camPosition, Vector3 _camRotation)
{
    d3dDevice = _d3dDevice;

    camera.init(camPosition, camRotation);
}

/*
void MainCamera::update()
{
    Vector3 currentInput = Vector3(vhInput->horizontal(), 0, vhInput->vertical()) * 0.1f;
    translate(currentInput);
    Vector3 rotationInput(Vector3::zero());
    rotationInput.x = (int)(Input->getButton(KeyType::R)) - (int)(Input->getButton(KeyType::F));
    rotationInput.y = (int)(Input->getButton(KeyType::X)) - (int)(Input->getButton(KeyType::Z));
    rotationInput.z = (int)(Input->getButton(KeyType::Q)) - (int)(Input->getButton(KeyType::E));
    rotationInput *= 0.05f;
    if (rotationInput != Vector3::zero())
    {
        rotate(rotationInput);
    }

    camera.Update(d3dDevice);
}
*/

void MainCamera::position(Vector3 _position)
{
    camera.setPosition(_position);
    camPosition = _position;
}

void MainCamera::rotation(Vector3 _rotation)
{
    if (abs(_rotation.x / 360.0f) > 0 || abs(_rotation.z / 360.0f) > 0 || abs(_rotation.y / 360.0f) > 0)
        overrollingCheck(_rotation);
    Vector3 diff = _rotation - camRotation;
    diff.x -= diff.x > 180.0f ? 360.0f : 0.0f;
    diff.x += diff.x < -180.0f ? 360.0f : 0.0f;
    diff.y -= diff.y > 180.0f ? 360.0f : 0.0f;
    diff.y += diff.y < -180.0f ? 360.0f : 0.0f;
    diff.z -= diff.z > 180.0f ? 360.0f : 0.0f;
    diff.z += diff.z < -180.0f ? 360.0f : 0.0f;
    camera.pitch(diff.x);
    camera.yaw(diff.y);
    camera.roll(diff.z);

    camRotation = _rotation;
    overrollingCheck(camRotation);
}

void MainCamera::translate(Vector3 _towards)
{
    camera.strafe(_towards.x);
    camera.fly(_towards.y);
    camera.walk(_towards.z);
    camPosition += _towards;
}

void MainCamera::rotate(Vector3 _towards)
{
    camera.pitch(_towards.x);
    camera.yaw(_towards.y);
    camera.roll(_towards.z);

    camRotation += _towards;
    overrollingCheck(camRotation);
}

void MainCamera::overrollingCheck(Vector3& _rotation)
{
    while (_rotation.x > 360.0f || _rotation.y > 360.0f || _rotation.z > 360.0f)
    {
        _rotation.x -= _rotation.x > 360.0f ? 360.0f : 0.0f;
        _rotation.y -= _rotation.y > 360.0f ? 360.0f : 0.0f;
        _rotation.z -= _rotation.z > 360.0f ? 360.0f : 0.0f;
    }
    while (_rotation.x < 0.0f || _rotation.y < 0.0f || _rotation.z < 0.0f)
    {
        _rotation.x += (_rotation.x < 0.0f ? 360.0f : 0.0f);
        _rotation.y += (_rotation.y < 0.0f ? 360.0f : 0.0f);
        _rotation.z += (_rotation.z < 0.0f ? 360.0f : 0.0f);
    }
}

/*
void MainCamera::update(LPDIRECT3DDEVICE9& d3dDevice)
{

    D3DXMATRIX matView;
    D3DXMATRIX matProj;
    D3DXVECTOR3 eye = camPosition;  // 카메라 위치
    D3DXVECTOR3 at = lookingPosition;    // 카메라가 바라보는 위치
    D3DXVECTOR3 up = upDirection;    // 카메라의 위쪽 방향

    D3DXMatrixLookAtLH(&matView, &eye, &at, &up);
    d3dDevice->SetTransform(D3DTS_VIEW, &matView);
    // 투영 행렬 설정
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 800.0f / 600.0f, 1.0f, 100.0f);
    d3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

    Vector3 currentInput = Vector3(vhInput->horizontal(), 0, vhInput->vertical()) * 2.5f;
    camPosition += currentInput;
    //rotation set
    setCam();
    D3DXVECTOR3 eye = camPosition;
    D3DXVECTOR3 at = lookingPosition;
    D3DXVECTOR3 up = upDirection;
    D3DXMatrixLookAtLH(&matView, &eye, &at, &up);
    d3dDevice->SetTransform(D3DTS_VIEW, &matView);
}
*/
