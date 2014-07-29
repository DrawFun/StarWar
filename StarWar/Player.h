#ifndef _STARWAR_PLAYER_H_
#define _STARWAR_PLAYER_H_

#include "Node.h"

class CPlayer : public CNode
{
protected:
	HWND m_hWnd;
	POINT ptLastMousePosit; //Last mouse position
	POINT ptCurrentMousePosit; //Current mouse position

	float elpasedTime; //Elapsed time between current time and last time	
	float moveSpeed; //Move speed

public:
	//Vectors for view and position
	D3DXVECTOR3	vEye;    // Eye Position
	D3DXVECTOR3	vLook;  // Look Vector
	D3DXVECTOR3	vUp;      // Up Vector
	D3DXVECTOR3	vRight;   // Right Vector

public:
	CPlayer(HWND hWnd);
	void Update(LPDIRECT3DDEVICE9 pd3dDevice);
	bool InitPosition
	(const D3DXVECTOR3 &position = D3DXVECTOR3(0, 0, 0), 
	 const D3DXVECTOR3 &scale = D3DXVECTOR3(1, 1, 1),
	 const D3DXVECTOR3 &rotation = D3DXVECTOR3(0, 0, 0));
	bool InitVertices(LPDIRECT3DDEVICE9 pd3dDevice);
	bool InitTextures();
	bool InitNormals();
	void Draw();
	void Move();
	void Controller(bool isLButtonDown, float elpasedTime);
};

#endif

