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
#include "Mine.h"
#include "Platform.h"
#include "Controller.h"
#include "Transform.h"

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
CPlatform *platform = NULL;
CMine *mine = NULL;
CController *controller = NULL;

//Controler related variables 
POINT g_ptLastMousePosit; //Last mouse position
POINT g_ptCurrentMousePosit; //Current mouse position
bool g_bMousing = false; //Whether left key being clicked
float g_fMoveSpeed = 20.0f; //Move speed
float g_fElpasedTime; //Elapsed time between current time and last time
double g_dCurTime; //Current time
double g_dLastTime; //Last time

//------------------------------------------------------------------------------
// PROTOTYPES
//------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void init(void);
void shutDown(void);
void render(void);

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

	pCSkyBox = new CSkyBox();
	pCSkyBox->InitVertices();
	pCSkyBox->InitColliders();

	//Construct terrain
	pCTerrain = new CTerrain(300, 300, -150, -150, 2, -2, 2);
	pCTerrain->InitVertices();

	platform = new CPlatform(20, 1, 20);
	CTransform platformTramsform(D3DXVECTOR3(-25.0f, 0.0f, -25.0f));
	platform->InitTransform(platformTramsform);
	platform->InitVertices();
	platform->InitColliders();

	player = new CPlayer();
	CTransform playerTramsform(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.05,0.05,0.05));
	player->InitTransform(playerTramsform);	
	player->InitVertices();	
	player->InitColliders();
	
	mine = new CMine(0.01);
	CTransform mineTramsform(D3DXVECTOR3(5,0,5), D3DXVECTOR3(0.05,0.05,0.05));
	mine->InitTransform(mineTramsform);
	mine->InitVertices();	
	mine->InitColliders();
	
	camera = new CCamera(player, D3DXVECTOR3(0, 0, -15));

	controller = new CController(player);

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
// Name: init()
// Desc: Init the scene
//------------------------------------------------------------------------------
void init( void )
{
	//Set samplers
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//Project models
	D3DXMATRIX matProj;

    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
    D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian( 45.0f ), 
                                1280.0f / 900.0f, 0.1f, 200.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	//Full of white light
	    g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 
		0xffffffff);
}

//------------------------------------------------------------------------------
// Name: shutDown()
// Desc: release resources and close window
//------------------------------------------------------------------------------
void shutDown( void )
{

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
                         D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0 );

	//Get last input and update the view matrix
	//camera->GetRealTimeUserInput(g_bMousing, g_fElpasedTime);

		//Get current mouse position
	POINT mousePosit;
	GetCursorPos(&mousePosit);
	ScreenToClient(g_hWnd, &mousePosit);
		//Get ket input
	

	ControllerInput input;
	input.isLButtonDown = g_bMousing;
	input.elpasedTime = g_fElpasedTime;
	input.currentMousePosition = mousePosit;
	GetKeyboardState(input.keys);

	controller->Control(input);
	
	//updateViewMatrix();

	//Draw skybox, terrain and snowman
    g_pd3dDevice->BeginScene();	


	//Init and set world matrix
	D3DXMATRIX matWorld;
	D3DXMatrixScaling(&matWorld, 1.0f, 1.0f, 1.0f);
    g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	//Draw skybox
	pCSkyBox->Update();
	//Draw terrain
	pCTerrain->Update();

	
	platform->Update();	
	

	mine->Update();
	player->Update();



	camera->LateUpdate();


    g_pd3dDevice->EndScene();
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

