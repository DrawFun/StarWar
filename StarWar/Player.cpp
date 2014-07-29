#include "Player.h"

//Vectors for view and position

extern D3DXVECTOR3	g_vEye;  // Look Vector
extern D3DXVECTOR3	g_vLook;  // Look Vector
extern D3DXVECTOR3	g_vUp;      // Up Vector
extern D3DXVECTOR3	g_vRight;   // Right Vector

CPlayer::CPlayer(HWND hWnd)
{
	m_hWnd = hWnd;
	m_position = D3DXVECTOR3(0.0f, -25.0f, -10.0f);
	moveSpeed = 20;
}

void CPlayer::Draw()
{

}

void CPlayer::Move()
{

}

void CPlayer::Update()
{

}

void CPlayer::Controller(bool isLButtonDown, float elpasedTime)
{
	vRight = g_vRight;
	vEye = m_position;
	vLook = g_vLook;
	vUp = g_vUp;
	//Get current mouse position
	POINT mousePosit;
	GetCursorPos(&mousePosit);
	ScreenToClient(m_hWnd, &mousePosit);

    ptCurrentMousePosit.x = mousePosit.x;
    ptCurrentMousePosit.y = mousePosit.y;

	//Rotation matrix used to tranformer the world
	D3DXMATRIX matRotation;

    if(isLButtonDown)
    {
		int nXDiff = (ptCurrentMousePosit.x - ptLastMousePosit.x);
        int nYDiff = (ptCurrentMousePosit.y - ptLastMousePosit.y);
        
        if(nYDiff != 0)
		{
			//First calculate the radian according the distance
			//Then rotate the radian and get the rotation matrix
			D3DXMatrixRotationAxis(&matRotation, &vRight, D3DXToRadian((float)nYDiff / 6.0f));
			//Rotate look and up directions according the rotation matrix
			D3DXVec3TransformCoord(&vLook, &vLook, &matRotation);
			D3DXVec3TransformCoord(&vUp, &vUp, &matRotation);
		}

		if(nXDiff != 0)
		{
			D3DXMatrixRotationAxis( &matRotation, &D3DXVECTOR3(0,1,0), D3DXToRadian((float)nXDiff / 6.0f) );
			D3DXVec3TransformCoord( &vLook, &vLook, &matRotation );
			D3DXVec3TransformCoord( &vUp, &vUp, &matRotation );
		}
    }

	//Store current position for next process
    ptLastMousePosit.x = ptCurrentMousePosit.x;
    ptLastMousePosit.y = ptCurrentMousePosit.y;

	//Get ket input
	unsigned char keys[256];
	GetKeyboardState(keys);
    
    D3DXVECTOR3 tmpLook  = vLook;
	D3DXVECTOR3 tmpRight = vRight;

	//Move camera position according the direction and distance(time * speed)

	// Up Arrow Key - View moves forward
	if( keys['W'] & 0x80 )
		m_position -= (tmpLook * -moveSpeed) * elpasedTime;

	// Down Arrow Key - View moves backward
	if( keys['S'] & 0x80 )
		m_position += (tmpLook * -moveSpeed) * elpasedTime;

	// Left Arrow Key - View side-steps or strafes to the left
	if( keys['A'] & 0x80 )
		m_position -= (tmpRight * moveSpeed) * elpasedTime;

	// Right Arrow Key - View side-steps or strafes to the right
	if( keys['D'] & 0x80 )
		m_position += (tmpRight * moveSpeed) * elpasedTime;

	//TODO 随地形运动
	//Wandering space constaint
	//Above the terrain 
	if(m_position.y < -28.0f)	m_position.y = -28.0f;

	////Inside the map
	//if(m_position.x <  mapMinX)	m_position.x = mapMinX;
	//if(m_position.x >  mapMaxX)	m_position.x = mapMaxX;
	//if(m_position.z <  mapMinZ)	m_position.z = mapMinZ;
	//if(m_position.z >  mapMaxZ)	m_position.z = mapMaxZ;

	g_vEye = m_position;
	g_vLook = vLook;
	g_vRight = vRight;
	g_vUp = vUp;
}

bool CPlayer::InitVertices()
{
	//Init snowman x model
	LPD3DXBUFFER pD3DXMtrlBuffer;

    D3DXLoadMeshFromX( "Resource//Snowman.x", D3DXMESH_SYSTEMMEM, 
                       g_pd3dDevice, NULL, 
                       &pD3DXMtrlBuffer, NULL, &g_snowManNumMaterials, 
                       &g_pSnowmanMesh );

    D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    g_pSnowmanMeshMaterials = new D3DMATERIAL9[g_snowManNumMaterials];
}




    for( unsigned long i = 0; i < g_snowManNumMaterials; ++i )
    {
        //Set materials
        g_pSnowmanMeshMaterials[i] = d3dxMaterials[i].MatD3D;
        //Set the ambient color for the material
        g_pSnowmanMeshMaterials[i].Ambient = g_pSnowmanMeshMaterials[i].Diffuse;
    }

	//Create textures from files
	D3DXCreateTextureFromFile( g_pd3dDevice, "Resource//Snowman1.jpg", &g_pSnowmanTexture0);
	D3DXCreateTextureFromFile( g_pd3dDevice, "Resource//Snowman2.jpg", &g_pSnowmanTexture1);

    pD3DXMtrlBuffer->Release();