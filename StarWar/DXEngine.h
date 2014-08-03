#ifndef _STARWAR_DXENGINE_H_
#define _STARWAR_DXENGINE_H_

#include "Scene.h"

//�����ܹ�������ģʽ������DX�����Դָ��
class CDXEngine
{
protected:
	static CDXEngine* pInstance; //Static member, the only instance
	LPDIRECT3D9	m_pD3D;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	ID3DXFont *m_pFont;
	CDXEngine(HWND hWnd);	
	HWND m_hWnd;

private:
	//����ָ��
	CScene *m_pScene;

public:
	static CDXEngine* Instance(HWND hWnd = NULL);
    static void Destroy();

	void Update(ControllerInput &input);
	void InitScene();

	LPDIRECT3DDEVICE9 GetDxDevice();
	LPDIRECT3D9 GetDx();
	ID3DXFont *GetFont();
};

#endif