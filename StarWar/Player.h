#ifndef _STARWAR_PLAYER_H_
#define _STARWAR_PLAYER_H_

#include "DrawableNode.h"

class CPlayer : public CDrawableNode
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
	void Update();
	bool InitVertices();
	bool InitTextures();
	bool InitNormals();
	void Draw();
	void Move();
	void Controller(bool isLButtonDown, float elpasedTime);
};

#endif

