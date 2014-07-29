//------------------------------------------------------------------------------
//           Name: Main.cpp
//         Author: Dai Zhuofang
//    Description: This sample demo of first person wandering in a world with 
//				   snowman, terrain and skybox.
//                 
//   Control Keys: W          - View moves forward
//                 S          - View moves backward
//                 A          - View strafes left
//                 D          - View strafes Right
//                 Left Mouse - Perform looking
//                 Mouse      - Look about the scene
//------------------------------------------------------------------------------

#include "Util.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "Camera.h"
#include "DXEngine.h"
#include "Player.h"

//------------------------------------------------------------------------------
// GLOBALS VARIABLES
//------------------------------------------------------------------------------
HWND                    g_hWnd           = NULL;
LPDIRECT3D9             g_pD3D           = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice     = NULL;

//Skybox pointer
CSkyBox *pCSkyBox = NULL;
//Terrain pointer
CTerrain *pCTerrain = NULL;

CCamera *camera = NULL;
CDXEngine  *dxEngine = NULL;
CPlayer *player = NULL;

float mapMinX;
float mapMaxX;
float mapMinZ;
float mapMaxZ;

//Controler related variables 
POINT g_ptLastMousePosit; //Last mouse position
POINT g_ptCurrentMousePosit; //Current mouse position
bool g_bMousing = false; //Whether left key being clicked
float g_fMoveSpeed = 20.0f; //Move speed
float g_fElpasedTime; //Elapsed time between current time and last time
double g_dCurTime; //Current time
double g_dLastTime; //Last time

//Spot light variable
D3DLIGHT9 g_light0;
D3DXVECTOR3	g_spotLightPos(10.0f, -10.0f, 10.0f); //Position

//Snowman related variables
D3DXVECTOR3	g_snowmanPos(10.0f, -30.0f, 10.0f); //Position
LPDIRECT3DTEXTURE9 g_pSnowmanTexture0 = NULL; //First texture
LPDIRECT3DTEXTURE9 g_pSnowmanTexture1 = NULL; //Second texture
LPDIRECT3DVERTEXBUFFER9 g_pSnowmanVertexBuffer = NULL; //D3D vertext buffer 
LPD3DXMESH g_pSnowmanMesh = NULL; //Point to snowman mesh
D3DMATERIAL9 *g_pSnowmanMeshMaterials = NULL; //Point to snowman mesh material
unsigned long g_snowManNumMaterials = 0L; //Number of material

//Vectors for view and position
D3DXVECTOR3	g_vEye(0.0f, -25.0f, -10.0f);    // Eye Position
D3DXVECTOR3	g_vLook(0.0f, 0.0f, 1.0f);  // Look Vector
D3DXVECTOR3	g_vUp(0.0f, 1.0f, 0.0f);      // Up Vector
D3DXVECTOR3	g_vRight(1.0f, 0.0f, 0.0f);   // Right Vector

//------------------------------------------------------------------------------
// PROTOTYPES
//------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void init(void);
void setupLight(void);
void shutDown(void);
void render(void);
void getRealTimeUserInput(void);
void updateViewMatrix(void);

//------------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//------------------------------------------------------------------------------
int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR     lpCmdLine,
					int       nCmdShow )
{
	WNDCLASSEX winClass; 
	MSG        uMsg;

    memset(&uMsg,0,sizeof(uMsg));

	winClass.lpszClassName = "MY_WINDOWS_CLASS";
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc   = WindowProc;
	winClass.hInstance     = hInstance;
	winClass.hIcon	       = LoadIcon(hInstance, NULL);
    winClass.hIconSm	   = LoadIcon(hInstance, NULL);
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName  = NULL;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;

	if( !RegisterClassEx(&winClass) )
		return E_FAIL;

	g_hWnd = CreateWindowEx( NULL, "MY_WINDOWS_CLASS", 
		                     "Snowman Wander",
						     WS_OVERLAPPEDWINDOW | WS_VISIBLE,
					         0, 0, 1280, 900, NULL, NULL, hInstance, NULL );

	if( g_hWnd == NULL )
		return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );
    UpdateWindow( g_hWnd );
	


	//Init D3D device pointer
	dxEngine = CDXEngine::Instance(g_hWnd);
	g_pD3D = dxEngine->GetDx();
	g_pd3dDevice = dxEngine->GetDxDevice();

	player = new CPlayer(g_hWnd);
	player->InitPosition(g_vEye);
	player->InitVertices(g_pd3dDevice);
	
	camera = new CCamera(player, D3DXVECTOR3(0, 30, -30));

	//Init D3D related resource here
	init();

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
		else
		{
			//Calculate the elapsed time
			g_dCurTime     = timeGetTime();
			g_fElpasedTime = (float)((g_dCurTime - g_dLastTime) * 0.001);
			g_dLastTime    = g_dCurTime;

			//Render current frame
		    render();
		}
	}

	//Release resource and close current window
	shutDown();

    UnregisterClass( "MY_WINDOWS_CLASS", winClass.hInstance );

	return uMsg.wParam;
}

//------------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The window's message handler
//------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc( HWND   hWnd, 
							 UINT   msg, 
							 WPARAM wParam, 
							 LPARAM lParam )
{
    switch( msg )
	{
        case WM_KEYDOWN:
		{
			switch( wParam )
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
			}
		}
        break;

		//Monitor left button click
        case WM_LBUTTONDOWN:
		{
			g_bMousing = true;
		}
		break;

		case WM_LBUTTONUP:
		{
			g_bMousing = false;

		}
		break;

		case WM_CLOSE:
		{
			PostQuitMessage(0);	
		}
		
		case WM_MOUSEMOVE:
					
		{
			RECT rec;
			GetWindowRect(g_hWnd, &rec);
			ClipCursor(&rec);
			break;
		}

		
        case WM_DESTROY:
		{
            PostQuitMessage(0);
		}
        break;

		default:
		{
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
		break;
	}

	return 0;
}

//------------------------------------------------------------------------------
// Name: setupLight(void)
// Desc: Setup light setting.
//------------------------------------------------------------------------------
void setupLight(void)
{
	//Enable the lighting render
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//Full of white light
    g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	//A spot light above the snowman
	ZeroMemory( &g_light0, sizeof(D3DLIGHT9) );
	g_light0.Diffuse    = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
    g_light0.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	
    g_light0.Type         = D3DLIGHT_SPOT;
	g_light0.Position     = g_spotLightPos;
	g_light0.Direction    = D3DXVECTOR3( 0.0f, -1.0f, 0.0f );
    g_light0.Attenuation0 = 0.0f;
    g_light0.Attenuation1 = 0.3f;
    g_light0.Attenuation2 = 0.0f;
	g_light0.Phi = 1.0f;
	g_light0.Theta = 0.8f;
    g_light0.Range        = 30.0f;
    g_pd3dDevice->SetLight( 0, &g_light0 );
	g_pd3dDevice->LightEnable(0, TRUE);

	//Enable specular lighting
	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
}


//------------------------------------------------------------------------------
// Name: init()
// Desc: Init the scene
//------------------------------------------------------------------------------
void init( void )
{


	//Init snowman x model
	LPD3DXBUFFER pD3DXMtrlBuffer;

    D3DXLoadMeshFromX( "Resource//Snowman.x", D3DXMESH_SYSTEMMEM, 
                       g_pd3dDevice, NULL, 
                       &pD3DXMtrlBuffer, NULL, &g_snowManNumMaterials, 
                       &g_pSnowmanMesh );

    D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    g_pSnowmanMeshMaterials = new D3DMATERIAL9[g_snowManNumMaterials];

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

	//Construct skybox
	LPCSTR pImageFileNameArray[CSkyBox::TOTAL_SIDES] = {
		"Resource//alpine_front.jpg", "Resource//alpine_back.jpg", "Resource//alpine_left.jpg",
		"Resource//alpine_right.jpg", "Resource//alpine_top.jpg", "Resource//SnowTerrain.jpg"
	};
	pCSkyBox = new CSkyBox(pImageFileNameArray, g_pd3dDevice);

	//Construct terrain
	pCTerrain = new CTerrain(64, 64, -128, -128, 2, -30, 4, "Resource//Heighmap.raw","Resource//SnowTerrain.jpg", g_pd3dDevice);
	pCTerrain->GetSpaceConstraint(mapMinX, mapMaxX, mapMinZ, mapMaxZ);

	//Setup lights
	setupLight();

	//Set samplers
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//Project models
	D3DXMATRIX matProj;

    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
    D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian( 45.0f ), 
                                800.0f / 600.0f, 0.1f, 100.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	
}

//------------------------------------------------------------------------------
// Name : updateViewMatrix()
// Desc : Builds a view matrix suitable for Direct3D according to modified look, up, eye vectors.
// Explaination:
//        Continue to use the matrix transformer from first person fps solution.
//
// Here's what the final matrix should look like:
//
//  |   rx     ux     lx    0 |
//  |   ry     uy     ly    0 |
//  |   rz     uz     lz    0 |
//  | -(r.e) -(u.e) -(l.e)  1 |
//
// Where r = Right vector
//       u = Up vector
//       l = Look vector
//       e = Eye position in world space
//       . = Dot-product operation
//
//------------------------------------------------------------------------------
void updateViewMatrix( void )
{
	D3DXMATRIX view;
	D3DXMatrixIdentity( &view );

	//Re-calculate then normalize the right vector according the look and up directions
	D3DXVec3Normalize( &g_vLook, &g_vLook );
	D3DXVec3Cross( &g_vRight, &g_vUp, &g_vLook );
	D3DXVec3Normalize( &g_vRight, &g_vRight );
	D3DXVec3Cross( &g_vUp, &g_vLook, &g_vRight );
	D3DXVec3Normalize( &g_vUp, &g_vUp );

	view._11 = g_vRight.x;
    view._12 = g_vUp.x;
    view._13 = g_vLook.x;
	view._14 = 0.0f;

	view._21 = g_vRight.y;
    view._22 = g_vUp.y;
    view._23 = g_vLook.y;
	view._24 = 0.0f;

	view._31 = g_vRight.z;
    view._32 = g_vUp.z;
    view._33 = g_vLook.z;
	view._34 = 0.0f;

	view._41 = -D3DXVec3Dot( &g_vEye, &g_vRight );
	view._42 = -D3DXVec3Dot( &g_vEye, &g_vUp );
	view._43 = -D3DXVec3Dot( &g_vEye, &g_vLook );
	view._44 =  1.0f;

	g_pd3dDevice->SetTransform( D3DTS_VIEW, &view ); 


}

//------------------------------------------------------------------------------
// Name: shutDown()
// Desc: release resources and close window
//------------------------------------------------------------------------------
void shutDown( void )
{
	if( g_pSnowmanMeshMaterials != NULL )
        delete[] g_pSnowmanMeshMaterials;

	if( g_pSnowmanMesh != NULL )
        g_pSnowmanMesh->Release(); 

    if( g_pSnowmanVertexBuffer != NULL )
        g_pSnowmanVertexBuffer->Release(); 

	CDXEngine::Destroy();

	delete pCSkyBox;
	delete pCTerrain;
}

//------------------------------------------------------------------------------
// Name: render()
// Desc: Render current frame
//------------------------------------------------------------------------------
void render( void )
{


	//Clear to the background
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_COLORVALUE(0.35f, 0.53f, 0.7, 1.0f), 1.0f, 0 );

	//Get last input and update the view matrix
	//camera->GetRealTimeUserInput(g_bMousing, g_fElpasedTime);
	player->Controller(g_bMousing, g_fElpasedTime);
	camera->LateUpdate();
	updateViewMatrix();

	//Draw skybox, terrain and snowman
    g_pd3dDevice->BeginScene();	
	
	//Init and set world matrix
	D3DXMATRIX matWorld;
	D3DXMatrixScaling(&matWorld, 1.0f, 1.0f, 1.0f);
    g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	//Draw skybox
	pCSkyBox->Draw(g_vEye);
	//Draw terrain
	pCTerrain->Draw();
	
	player->Update(g_pd3dDevice);

	//Draw snowman
	D3DXMATRIX snowmanMove, snowmanScale, snowmanWorldMat;	
	//Zoom in the snowman scale
	D3DXMatrixScaling( &snowmanScale, 0.05f, 0.05f, 0.05f );
	//Tranlate to the default position
	D3DXMatrixTranslation( &snowmanMove, g_snowmanPos.x, g_snowmanPos.y, g_snowmanPos.z);
	
	//Calculate the transformer matrix
	snowmanWorldMat = snowmanScale * snowmanMove;
	
	//Transform the world according the generated matrix
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &snowmanWorldMat);

	//Set texture on stage 0
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//Set RGB mix method
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	
	//Set texture on stage 1
	g_pd3dDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//Set RGB mix method
	g_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
	g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);

    for( unsigned long i = 0; i < g_snowManNumMaterials; ++i )
    {
        g_pd3dDevice->SetMaterial(&g_pSnowmanMeshMaterials[i]);
		
		//Set multiple textures
		g_pd3dDevice->SetTexture(0, g_pSnowmanTexture0);
		g_pd3dDevice->SetTexture(1, g_pSnowmanTexture1);
        g_pSnowmanMesh->DrawSubset(i);
    }

    g_pd3dDevice->EndScene();
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

	player->vUp = g_vUp;
	player->vRight = g_vRight;
	player->vLook = g_vLook;
}

