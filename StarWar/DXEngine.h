#ifndef _STARWAR_DXENGINE_H_
#define _STARWAR_DXENGINE_H_

#include "Util.h"

class CDXEngine
{
protected:
	static CDXEngine* pInstance; //Static member, the only instance
	LPDIRECT3D9	m_pD3D;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	HWND hWnd;
	CDXEngine(HWND hWnd);	

public:
	static CDXEngine* Instance(HWND hWnd);
    static void Destroy();
	LPDIRECT3DDEVICE9 GetDxDevice();
	LPDIRECT3D9 GetDx();
};

#endif