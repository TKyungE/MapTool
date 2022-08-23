#include "..\Public\Graphic_Device.h"

IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
	: m_p3D(nullptr), m_pDevice(nullptr)
{
	ZeroMemory(&m_GraphicDesc, sizeof(GRAPHIC_DESC));
}

HRESULT CGraphic_Device::InitDevice(const GRAPHIC_DESC & GraphicDesc, LPDIRECT3DDEVICE9 * ppOut)
{
	m_GraphicDesc = GraphicDesc;

	m_p3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if (FAILED(m_p3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		ERR_MSG(TEXT("GetDeviceCaps Failed"));
		return E_FAIL;
	}

	DWORD vp = 0;

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp);

	if (FAILED(m_p3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_GraphicDesc.hWnd, vp, &d3dpp, &m_pDevice)))
	{
		ERR_MSG(TEXT("CreateDevice Failed"));
		return E_FAIL;
	}

	*ppOut = m_pDevice;

	Safe_AddRef(m_pDevice);

	return S_OK;
}

void CGraphic_Device::SetParameters(D3DPRESENT_PARAMETERS & d3dpp)
{
	d3dpp.BackBufferWidth = m_GraphicDesc.iWinSizeX;
	d3dpp.BackBufferHeight = m_GraphicDesc.iWinSizeY;

	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = m_GraphicDesc.hWnd;

	d3dpp.Windowed = m_GraphicDesc.eWinMode;

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void CGraphic_Device::Render_Begin(void)
{
	m_pDevice->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);

	m_pDevice->BeginScene();
}

void CGraphic_Device::Render_End(HWND hWnd)
{
	m_pDevice->EndScene();

	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
}

void CGraphic_Device::Free(void)
{
	unsigned long RefCnt = 0;
	if (RefCnt = Safe_Release(m_pDevice))
		ERR_MSG(TEXT("Failed to Release : IDirect3DDevice9"));
	
	Safe_Release(m_p3D);
}
