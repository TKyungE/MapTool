#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)
private:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	LPDIRECT3DDEVICE9 Get_Device(void) { return m_pDevice; }

public:
	HRESULT InitDevice(const GRAPHIC_DESC& GraphicDesc, LPDIRECT3DDEVICE9* ppOut);
	void SetParameters(D3DPRESENT_PARAMETERS& d3dpp);
	void Render_Begin(void);
	void Render_End(HWND hWnd = NULL);

private:
	LPDIRECT3D9 m_p3D;
	LPDIRECT3DDEVICE9 m_pDevice;
	GRAPHIC_DESC m_GraphicDesc;

public:
	virtual void Free(void) override;
};

END