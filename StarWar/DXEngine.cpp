#include "DXEngine.h"
#include "StarWarScene.h"

extern HWND g_hWnd;

CDXEngine* CDXEngine::pInstance = NULL;

CDXEngine::CDXEngine(HWND hWnd)
{
	//Init D3D device pointer
    m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

    D3DDISPLAYMODE d3ddm;
    m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );

    d3dpp.Windowed               = TRUE;
    d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat       = d3ddm.Format;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

    m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                          D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                          &d3dpp, &m_pd3dDevice );
	//Set samplers
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//Project models
	D3DXMATRIX matProj;

    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
    D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian( 45.0f ), 
                                1280.0f / 900.0f, 0.1f, 200.0f );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	//Full of white light
	m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof(light) );
	light.Type = D3DLIGHT_DIRECTIONAL;

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;

	vecDir = D3DXVECTOR3(-1.0f, -0.4f, 0.8f );
	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );
	m_pd3dDevice->SetLight( 0, &light );
	m_pd3dDevice->LightEnable( 0, TRUE);
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	D3DXFONT_DESC d3dFont;
	memset(&d3dFont,0,sizeof(d3dFont));
	d3dFont.Height=20;
	d3dFont.Weight=500;
	d3dFont.Italic=FALSE;
	d3dFont.CharSet=DEFAULT_CHARSET;
	strcpy_s(d3dFont.FaceName,"Times New Roman");
	d3dFont.Height = 28;
	D3DXCreateFontIndirect(m_pd3dDevice, &d3dFont, &m_pFont);


}

CDXEngine* CDXEngine::Instance(HWND hWnd)
{
    if(NULL == pInstance)
    {
		if(hWnd != NULL)
		{
			pInstance = new CDXEngine(hWnd);
		}
    }
    return pInstance;
}

LPDIRECT3DDEVICE9 CDXEngine::GetDxDevice()
{
	return m_pd3dDevice;
}
	
LPDIRECT3D9 CDXEngine::GetDx()
{
	return m_pD3D;
}

ID3DXFont *CDXEngine::GetFont()
{
	return m_pFont;
}

//×ÊÔ´ÊÍ·Å
void CDXEngine::Destroy()
{
	if(pInstance != NULL)
	{
		if( pInstance->m_pFont != NULL)
		{
			pInstance->m_pFont->Release();
		}

		if( pInstance->m_pd3dDevice != NULL )
		{
			pInstance->m_pd3dDevice->Release();
		}

		if(pInstance->m_pD3D != NULL )
		{
			pInstance->m_pD3D->Release();
		}

		delete pInstance;
		pInstance = NULL;
	}

}

void CDXEngine::InitScene()
{
	m_pScene = new CStarWarScene();
}

void CDXEngine::Update(ControllerInput &input)
{
	//Clear to the background
    m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0 );

	m_pd3dDevice->BeginScene();	
	m_pScene->Update(input);

    m_pd3dDevice->EndScene();
    m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}