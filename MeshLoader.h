#pragma once
#include <string>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "winmm.lib")



class DXMesh
{
public:
    DXMesh() : pMesh(NULL), pTexture(NULL), pMaterials(NULL), depthStencil(NULL), numMaterials(0) {}
    ~DXMesh() { release(); }

    void release();

    LPD3DXMESH pMesh;
    LPDIRECT3DTEXTURE9 pTexture;
    LPD3DXBUFFER pMaterials;
    DWORD numMaterials;
    LPDIRECT3DSURFACE9 depthStencil;
private:

};

void DXMesh::release()
{
    if (pMesh)
    {
        pMesh->Release();
        pMesh = NULL;
    }

    if (pTexture)
    {
        pTexture->Release();
        pTexture = NULL;
    }

    if (pMaterials)
    {
        pMaterials->Release();
        pMaterials = NULL;
    }

    if (depthStencil)
    {
        depthStencil->Release();
        depthStencil = NULL;
    }
}


class DXMeshLoader
{
public:
    static bool LoadXFile(LPDIRECT3DDEVICE9 pDevice, std::string filename, DXMesh& output);
    static bool LoadXFile(LPDIRECT3DDEVICE9 pDevice, std::wstring filename, DXMesh& output);
    static void RenderMesh(LPDIRECT3DDEVICE9 pDevice, DXMesh& mesh);

    static void screenSize(LPDIRECT3DDEVICE9 d3dDevice, unsigned int& width, unsigned int& height);
private:

    static void initDepth(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DSURFACE9& depthStencil);
};


bool DXMeshLoader::LoadXFile(LPDIRECT3DDEVICE9 pDevice, std::string filename, DXMesh& output)
{
    // Load the mesh from the .x file
    if (FAILED(D3DXLoadMeshFromXA(filename.c_str(), D3DXMESH_MANAGED, pDevice, NULL, &output.pMaterials, NULL, &output.numMaterials, &output.pMesh)))
    {
        return false;
    }

    // Load materials and textures (if exisist)
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)output.pMaterials->GetBufferPointer();
    for (DWORD i = 0; i < output.numMaterials; i++)
    {
        if (d3dxMaterials[i].pTextureFilename)
        {
            if (FAILED(D3DXCreateTextureFromFileA(pDevice, d3dxMaterials[i].pTextureFilename, &output.pTexture)))
            {
                output.pTexture = NULL;
            }
        }
    }

    return true;
}
inline bool DXMeshLoader::LoadXFile(LPDIRECT3DDEVICE9 pDevice, std::wstring filename, DXMesh& output)
{
    // Load the mesh from the .x file
    if (FAILED(D3DXLoadMeshFromX(filename.c_str(), D3DXMESH_MANAGED, pDevice, NULL, &output.pMaterials, NULL, &output.numMaterials, &output.pMesh)))
    {
        return false;
    }

    // Load materials and textures (if exisist)
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)output.pMaterials->GetBufferPointer();
    for (DWORD i = 0; i < output.numMaterials; i++)
    {
        if (d3dxMaterials[i].pTextureFilename)
        {
            if (FAILED(D3DXCreateTextureFromFileA(pDevice, d3dxMaterials[i].pTextureFilename, &output.pTexture)))
            {
                output.pTexture = NULL;
            }
        }
    }

    return true;
}
void DXMeshLoader::RenderMesh(LPDIRECT3DDEVICE9 pDevice, DXMesh& mesh)
{
    if (mesh.pMesh)
    {
        pDevice->SetTexture(0, mesh.pTexture);
        mesh.pMesh->DrawSubset(0);
        pDevice->SetTexture(0, NULL);
    }
}

void DXMeshLoader::screenSize(LPDIRECT3DDEVICE9 d3dDevice, unsigned int& width, unsigned int& height)
{
    // ���� ���� Ÿ���� �����ɴϴ�.
    LPDIRECT3DSURFACE9 pRenderTarget = nullptr;
    d3dDevice->GetRenderTarget(0, &pRenderTarget);

    if (pRenderTarget)
    {
        // ���� Ÿ���� �����̽� ������ �����ɴϴ�.
        D3DSURFACE_DESC desc;
        pRenderTarget->GetDesc(&desc);

        // �����̽��� �ʺ�� ���̸� �����ɴϴ�.
        width = desc.Width;
        height = desc.Height;

        // ���� Ÿ���� �����մϴ�.
        pRenderTarget->Release();
    }
    else
    {
        // ���� Ÿ���� �������� ���� ���, �⺻ ���� �����մϴ�.
        width = 0;
        height = 0;
    }
}


void DXMeshLoader::initDepth(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DSURFACE9& depthStencil)
{
    unsigned int width, height;
    screenSize(pDevice, width, height);

    // ���� ���ٽ� ���� ����
    HRESULT hr = pDevice->CreateDepthStencilSurface(
        width,  // ȭ�� �ʺ�
        height,  // ȭ�� ����
        D3DFMT_D24S8, // ���� ���� ����
        D3DMULTISAMPLE_NONE, // ��Ƽ ���ø�
        0, // ��Ƽ ���ø� ǰ��
        TRUE, // ���۸� ��Ƽ ���ø��� �������� ����
        &depthStencil, // ������ ���� ���ٽ� ���۸� ���� ����
        nullptr // ���� ���� ���ٽ� ����
    );
}









class DXMeshContainer
{
public:
    DXMeshContainer() : pMesh(NULL), pTexture(NULL), pMaterials(NULL), depthStencil(NULL), numMaterials(0) {}
    ~DXMeshContainer() { release(); }


    void release();

    HRESULT LoadXFile(LPDIRECT3DDEVICE9 pDevice, std::string filename);
    void RenderMesh(LPDIRECT3DDEVICE9 pDevice);
    void screenSize(LPDIRECT3DDEVICE9 d3dDevice, unsigned int& width, unsigned int& height);
private:
    LPD3DXMESH pMesh;
    LPDIRECT3DTEXTURE9 pTexture;
    LPD3DXBUFFER pMaterials;
    DWORD numMaterials;
    LPDIRECT3DSURFACE9 depthStencil;

    void initDepth(LPDIRECT3DDEVICE9 pDevice);

};




void DXMeshContainer::release()
{
    if (pMesh)
    {
        pMesh->Release();
        pMesh = NULL;
    }

    if (pTexture)
    {
        pTexture->Release();
        pTexture = NULL;
    }

    if (pMaterials)
    {
        pMaterials->Release();
        pMaterials = NULL;
    }

    if (depthStencil)
    {
        depthStencil->Release();
        depthStencil = NULL;
    }
}

HRESULT DXMeshContainer::LoadXFile(LPDIRECT3DDEVICE9 pDevice, std::string filename)
{
    // Load the mesh from the .x file
    if (FAILED(D3DXLoadMeshFromXA(filename.c_str(), D3DXMESH_MANAGED, pDevice, NULL, &pMaterials, NULL, &numMaterials, &pMesh)))
        return E_FAIL;

    // Load materials and textures (if exisist)
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pMaterials->GetBufferPointer();
    for (DWORD i = 0; i < numMaterials; i++)
    {
        if (d3dxMaterials[i].pTextureFilename)
        {
            if (FAILED(D3DXCreateTextureFromFileA(pDevice, d3dxMaterials[i].pTextureFilename, &pTexture)))
            {
                pTexture = NULL;
            }
        }
    }

    return S_OK;
}

void DXMeshContainer::RenderMesh(LPDIRECT3DDEVICE9 pDevice)
{
    if (pMesh)
    {
        pDevice->SetTexture(0, pTexture);
        pMesh->DrawSubset(0);
        pDevice->SetTexture(0, NULL);
    }
}

void DXMeshContainer::initDepth(LPDIRECT3DDEVICE9 pDevice)
{
    unsigned int width, height;
    screenSize(pDevice, width, height);

    // ���� ���ٽ� ���� ����
    HRESULT hr = pDevice->CreateDepthStencilSurface(
        width,  // ȭ�� �ʺ�
        height,  // ȭ�� ����
        D3DFMT_D24S8, // ���� ���� ����
        D3DMULTISAMPLE_NONE, // ��Ƽ ���ø�
        0, // ��Ƽ ���ø� ǰ��
        TRUE, // ���۸� ��Ƽ ���ø��� �������� ����
        &depthStencil, // ������ ���� ���ٽ� ���۸� ���� ����
        nullptr // ���� ���� ���ٽ� ����
    );
}

void DXMeshContainer::screenSize(LPDIRECT3DDEVICE9 d3dDevice, unsigned int& width, unsigned int& height)
{
    // ���� ���� Ÿ���� �����ɴϴ�.
    LPDIRECT3DSURFACE9 pRenderTarget = nullptr;
    d3dDevice->GetRenderTarget(0, &pRenderTarget);

    if (pRenderTarget)
    {
        // ���� Ÿ���� �����̽� ������ �����ɴϴ�.
        D3DSURFACE_DESC desc;
        pRenderTarget->GetDesc(&desc);

        // �����̽��� �ʺ�� ���̸� �����ɴϴ�.
        width = desc.Width;
        height = desc.Height;

        // ���� Ÿ���� �����մϴ�.
        pRenderTarget->Release();
    }
    else
    {
        // ���� Ÿ���� �������� ���� ���, �⺻ ���� �����մϴ�.
        width = 0;
        height = 0;
    }
}