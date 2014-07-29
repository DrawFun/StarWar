#ifndef _STARWAR_SKYBOX_H_
#define _STARWAR_SKYBOX_H_

#include "Util.h"

//Define SkyBoxVertex
typedef PositionNormalDiffUseColorTexVertex SkyBoxVertex;

class CSkyBox
{
public:
	//Total sides of skybox
	static const UINT16 TOTAL_SIDES = 6;
	//Number of vertices per side
	static const UINT16 VERTICES_PER_SIDE = 4;
	//Total vertices of skybox, 6 * 4
	static const UINT16 TOTAL_VERTICES = 24;
	//Enum variable of each side. Introduced for readability
	enum TSides {FRONT, BACK, LEFT, RIGHT, CEILING, FLOOR};
public:
	//Constructor.
	CSkyBox(const LPCSTR (&pImageFileNameArray)[TOTAL_SIDES], LPDIRECT3DDEVICE9 g_pd3dDevice);
	//Deconstructor.
	~CSkyBox();
public:
	//Draw the skybox.
	void Draw(D3DXVECTOR3 &vEye);
private:
	//Release resource.
	void Release();
	//Create vertex buffer.
	bool InitVertices();
	//Load skybox texture from file.
	bool LoadTexture(const LPCSTR *pImageFileNameArray);
	//Texture point array of each side
	LPDIRECT3DTEXTURE9 m_pTextureArray[TOTAL_SIDES];
	//Vertices array of skybox
	SkyBoxVertex *m_pVertexArray;
	//Vertices buffer
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	//Point to the global D3D device.
	LPDIRECT3DDEVICE9 m_pd3dDevice;
};

#endif