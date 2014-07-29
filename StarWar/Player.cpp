#include "Player.h"

//Vectors for view and position

extern D3DXVECTOR3	g_vEye;  // Look Vector
extern D3DXVECTOR3	g_vLook;  // Look Vector
extern D3DXVECTOR3	g_vUp;      // Up Vector
extern D3DXVECTOR3	g_vRight;   // Right Vector

//Snowman related variables
D3DXVECTOR3	snowmanPos(10.0f, -30.0f, 10.0f); //Position
LPDIRECT3DTEXTURE9 pSnowmanTexture0 = NULL; //First texture
LPDIRECT3DTEXTURE9 pSnowmanTexture1 = NULL; //Second texture
LPDIRECT3DVERTEXBUFFER9 pSnowmanVertexBuffer = NULL; //D3D vertext buffer 
LPD3DXMESH pSnowmanMesh = NULL; //Point to snowman mesh
D3DMATERIAL9 *pSnowmanMeshMaterials = NULL; //Point to snowman mesh material
unsigned long snowManNumMaterials = 0L; //Number of material
D3DXMATRIX snowmanRX, snowmanRY, snowmanRZ;
float ry, rz;

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

void CPlayer::Update(LPDIRECT3DDEVICE9 pd3dDevice)
{
	//Draw snowman
	D3DXMATRIX snowmanMove, snowmanScale, snowmanRX, snowmanRY, snowmanRZ, snowmanWorldMat;	
	//Zoom in the snowman scale
	D3DXMatrixScaling( &snowmanScale, 0.05f, 0.05f, 0.05f );
	//Tranlate to the default position
	D3DXMatrixTranslation( &snowmanMove, m_position.x, m_position.y, m_position.z);

	//D3DXMatrixRotationY(&snowmanRY, ry);  
	

	//Calculate the transformer matrix
	//snowmanWorldMat = snowmanScale * snowmanMove;
	snowmanWorldMat = snowmanScale *snowmanRY;
	
	//Transform the world according the generated matrix
	pd3dDevice->SetTransform(D3DTS_WORLD, &snowmanWorldMat);

	//Set texture on stage 0
	pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//Set RGB mix method
	pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	
	//Set texture on stage 1
	pd3dDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//Set RGB mix method
	pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
	pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);

    for( unsigned long i = 0; i < snowManNumMaterials; ++i )
    {
        pd3dDevice->SetMaterial(&pSnowmanMeshMaterials[i]);
		
		//Set multiple textures
		pd3dDevice->SetTexture(0, pSnowmanTexture0);
		pd3dDevice->SetTexture(1, pSnowmanTexture1);
        pSnowmanMesh->DrawSubset(i);
    }
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
			
			ry = D3DXToRadian((float)nYDiff / 6.0f);
			//Rotate look and up directions according the rotation matrix
			D3DXVec3TransformCoord(&vLook, &vLook, &matRotation);
			D3DXVec3TransformCoord(&vUp, &vUp, &matRotation);			
		}

		if(nXDiff != 0)
		{
			D3DXMatrixRotationAxis( &matRotation, &D3DXVECTOR3(0,1,0), D3DXToRadian((float)nXDiff / 6.0f) );
			snowmanRZ = matRotation;
		
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

bool CPlayer::InitPosition(const D3DXVECTOR3 &position, const D3DXVECTOR3 &scale, const D3DXVECTOR3 &rotation)
{
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;
	return true;
}

bool CPlayer::InitVertices(LPDIRECT3DDEVICE9 pd3dDevice)
{
	//Init snowman x model
	LPD3DXBUFFER pD3DXMtrlBuffer;

    D3DXLoadMeshFromX( "Resource//Snowman.x", D3DXMESH_SYSTEMMEM, 
                       pd3dDevice, NULL, 
                       &pD3DXMtrlBuffer, NULL, &snowManNumMaterials, 
                       &pSnowmanMesh );

    D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    pSnowmanMeshMaterials = new D3DMATERIAL9[snowManNumMaterials];
    for( unsigned long i = 0; i < snowManNumMaterials; ++i )
    {
        //Set materials
        pSnowmanMeshMaterials[i] = d3dxMaterials[i].MatD3D;
        //Set the ambient color for the material
        pSnowmanMeshMaterials[i].Ambient = pSnowmanMeshMaterials[i].Diffuse;
    }
	//Create textures from files
	D3DXCreateTextureFromFile( pd3dDevice, "Resource//Snowman1.jpg", &pSnowmanTexture0);
	D3DXCreateTextureFromFile( pd3dDevice, "Resource//Snowman2.jpg", &pSnowmanTexture1);

    pD3DXMtrlBuffer->Release();
	return true;
}

bool CPlayer::InitNormals()
{
	return true;
}

bool CPlayer::InitTextures()
{
	return true;
}





