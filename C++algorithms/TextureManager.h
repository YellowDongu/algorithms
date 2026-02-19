#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "winmm.lib")

#include <list>
#include <vector>
#include <map>
#include <string>



typedef struct Texture
{
	~Texture() { release(); }

	LPDIRECT3DTEXTURE9 texture = nullptr;
	D3DXIMAGE_INFO textureInfo;

	void release()
	{
		if (texture)
			texture->Release();
	}
}*LPTexture;


class TextureManager
{
public:
	TextureManager() = default;
	~TextureManager() = default;



	static LPTexture loadTexture(LPDIRECT3DDEVICE9 device, const std::wstring& filePath);

private:

};


LPTexture TextureManager::loadTexture(LPDIRECT3DDEVICE9 device, const std::wstring& filePath)
{
	LPTexture newTexture = new Texture();
	ZeroMemory(newTexture, sizeof(newTexture));

	if (FAILED(D3DXGetImageInfoFromFileW(filePath.c_str(), &newTexture->textureInfo)))
	{
		delete newTexture;
		return nullptr;
	}

	if (FAILED(D3DXCreateTextureFromFileExW(
			device,
			filePath.c_str(),
			newTexture->textureInfo.Width,
			newTexture->textureInfo.Height,
			newTexture->textureInfo.MipLevels,
			0,
			newTexture->textureInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			0,
			NULL,
			NULL,
			&newTexture->texture)
	))
	{
		delete newTexture;
		return nullptr;
	}

	return newTexture;
}
