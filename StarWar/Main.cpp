//------------------------------------------------------------------------------
//           Name: Main.cpp
//         Author: Dai Zhuofang
//                 
//   Control Keys: W          - View moves forward
//                 S          - View moves backward
//                 A          - View strafes left
//                 D          - View strafes Right
//                 Left Mouse - Perform looking
//                 Mouse      - Look about the scene
//------------------------------------------------------------------------------

#include "Util.h"
#include "DXEngine.h"
#include "StarWarScene.h"
#include "FPS.h"

//------------------------------------------------------------------------------
// 全局变量
//------------------------------------------------------------------------------
HWND g_hWnd = NULL;

// 控制器相关变量
POINT g_ptLastMousePosit; //Last mouse position
POINT g_ptCurrentMousePosit; //Current mouse position
bool g_bMousing = false; //Whether left key being clicked
float g_fElpasedTime; //Elapsed time between current time and last time
double g_dCurTime; //Current time
double g_dLastTime; //Last time

//------------------------------------------------------------------------------
// 函数原型
//------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow);

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void init(void);
void shutDown(void);
void render(void);
	CFPS GameTime;
	char s[50];

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
		                     "Star War",
						     WS_OVERLAPPEDWINDOW | WS_VISIBLE,
					         0, 0, 1280, 900, NULL, NULL, hInstance, NULL );

	if( g_hWnd == NULL )
		return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );
    UpdateWindow( g_hWnd );
	
	//Init Engine
	CDXEngine::Instance(g_hWnd);
	//Init Scene
	CDXEngine::Instance()->InitScene();
	//Init Time Ticker for FPS
	GameTime.Start();

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


			ControllerInput input;
			POINT mousePosit;
			GetCursorPos(&mousePosit);
			ScreenToClient(g_hWnd, &mousePosit);
			input.isLButtonDown = g_bMousing;
			input.elpasedTime = g_fElpasedTime;
			input.currentMousePosition = mousePosit;
			GetKeyboardState(input.keys);

			CDXEngine::Instance()->Update(input);

			GameTime.Tick();
			GameTime.CalcFPS();
			sprintf(s, "FPS: %f", GameTime.GetFPS());
			SetWindowText(g_hWnd, s);
		}
	}

	GameTime.Stop();
	//Release resource and close current window
	CDXEngine::Destroy();

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
			//限制鼠标移动
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