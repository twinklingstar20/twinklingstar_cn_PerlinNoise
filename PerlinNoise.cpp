//--------------------------------------------------------------------------------------
// File: PerlinNoise.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "DXUT.h"
#include "resource.h"

#include "SrClassicalPerlinNoise.h"
#include "SrCamera.h"
#include "SrTerrain.h"

SrCamera			gCamera;
SrTerrain*			gTerrain;


//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
                                      bool bWindowed, void* pUserContext )
{
    // Typically want to skip back buffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// Before a device is created, modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    return true;
}


void setUpLight(IDirect3DDevice9* pd3dDevice)
{
	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	pd3dDevice->SetMaterial( &mtrl );


	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	light.Type       = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r  = 1.0f;
	light.Diffuse.g  = 1.0f;
	light.Diffuse.b  = 1.0f;
	vecDir = D3DXVECTOR3(cosf(timeGetTime()/350.0f),
		1.0f,
		sinf(timeGetTime()/350.0f) );
	vecDir = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );
	light.Range       = 1000.0f;
	pd3dDevice->SetLight( 0, &light );
	pd3dDevice->LightEnable( 0, TRUE );
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );


	pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00505050);

}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                     void* pUserContext )
{
	//Create the terrain.
	SrTerrainParam terrainParam;
	terrainParam.mCenter  = D3DXVECTOR3(0, 0.0f, 0);
	terrainParam.mXSize   = 200;
	terrainParam.mZSize   = 200;
	terrainParam.mYSize   = 15.0f;
	terrainParam.mNumRows = 512;
	terrainParam.mNumCols = 512;
	terrainParam.mF		  = 32.0f;
	terrainParam.mD		  = 32.0f;
	terrainParam.mErode	  = 1.0f;
	gTerrain = new SrTerrain(L"grass.png", terrainParam);
	gTerrain->init(pd3dDevice);

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                    void* pUserContext )
{
	UINT height = pBackBufferSurfaceDesc->Height;
	UINT width  = pBackBufferSurfaceDesc->Width;

	gCamera.setProj(pd3dDevice, D3DX_PI/4, width, height, 1.0f, 1000.0f );

	setUpLight(pd3dDevice);
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    HRESULT hr;

    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 45, 50, 170 ), 1.0f, 0 ) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		gCamera.setView(pd3dDevice);
		gTerrain->render(pd3dDevice);

        V( pd3dDevice->EndScene() );
    }
}

bool	g_blIsLButonDown	= false;
int		g_inLastMouseX		= 0;
int		g_inLastMouseY		= 0;
bool	keys[256];

//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
						 bool* pbNoFurtherProcessing, void* pUserContext )
{
	switch (uMsg)
	{
	case WM_ACTIVATE:
		{

			return 0;
		}

	case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
			case SC_SCREENSAVE:	
			case SC_MONITORPOWER:
				return 0;
			}
			break;
		}

	case WM_CLOSE:
		{
			PostQuitMessage(0);	
			return 0;
		}

	case WM_KEYDOWN:
		{
			keys[wParam] = TRUE;
			if(keys['W'])
			{
				gCamera.moveAxisZ(-WHEEL_DELTA*0.01f);
			}
			if(keys['S'])
			{
				gCamera.moveAxisZ(WHEEL_DELTA*0.01f);
			}
			if(keys['A'])
			{
				gCamera.moveAxisX(WHEEL_DELTA*0.01f);
			}
			if(keys['D'])
			{
				gCamera.moveAxisX(-WHEEL_DELTA*0.01f);
			}
			if(keys['U'])
			{
				gCamera.moveAxisY(-WHEEL_DELTA*0.01f);
			}
			if(keys['J'])
			{
				gCamera.moveAxisY(WHEEL_DELTA*0.01f);
			}
			return 0;
		}

	case WM_KEYUP:
		{
			keys[wParam] = FALSE;
			return 0;
		}
	case WM_LBUTTONDOWN:
		{
			g_inLastMouseX	=	LOWORD(lParam);
			g_inLastMouseY	=	HIWORD(lParam);

			g_blIsLButonDown = true;

			return 0;
		}
	case WM_LBUTTONUP:
		{
			g_blIsLButonDown = false;
			return 0;
		}
	case WM_MOUSELEAVE:
		{
			g_blIsLButonDown = false;
			return 0;
		}
	case WM_MOUSEMOVE:
		{
			int x	=	LOWORD(lParam); 
			int y	=	HIWORD(lParam);

			if( g_blIsLButonDown )
			{
				if( x!=g_inLastMouseX )
				{
					gCamera.rotateAxisY(0.2f*(g_inLastMouseX-x)*D3DX_PI/(180.0f));
					g_inLastMouseX = x;
				}
				if( y!=g_inLastMouseY )
				{
					//gCamera.rotateAxisX(0.2f*(g_inLastMouseY-y)*D3DX_PI/(180.0f));
					g_inLastMouseY = y;
				}
			}
			return 0;
		}
	case WM_MOUSEWHEEL:
		{
			short delta = HIWORD(wParam);
			gCamera.moveAxisZ(-delta/WHEEL_DELTA*0.1f);
			return 0;
		}
	case WM_SIZE:
		{
			return 0;
		}
	}
	return 0;
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
	delete gTerrain;
}


//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // Set the callback functions
    DXUTSetCallbackD3D9DeviceAcceptable( IsD3D9DeviceAcceptable );
    DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
    DXUTSetCallbackD3D9DeviceReset( OnD3D9ResetDevice );
    DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );
    DXUTSetCallbackD3D9DeviceLost( OnD3D9LostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackFrameMove( OnFrameMove );

    // TODO: Perform any application-level initialization here

    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    DXUTInit( true, true ); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( true, true, true );  // handle the default hotkeys
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"PerlinNoise" );
    DXUTCreateDevice( true, 640, 480 );

    // Start the render loop
    DXUTMainLoop();

    // TODO: Perform any application-level cleanup here

    return DXUTGetExitCode();
}


