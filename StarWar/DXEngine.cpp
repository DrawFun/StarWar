#include "DXEngine.h"


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

void CDXEngine::Destroy()
{
	if(pInstance != NULL)
	{
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