#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONALPHABLEND, RENDER_ALPHABLEND, RENDER_UI, RENDER_END };

private:
	CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;

public:
	virtual HRESULT Initialize_Prototype(void) override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	HRESULT Render_GameObjects(void);

private:
	list<class CGameObject*> m_GameObjects[RENDER_END];
	typedef list<class CGameObject*> GAMEOBJECTS;

private:
	HRESULT Render_Priority(void);
	HRESULT Render_NonAlphaBlend(void);
	HRESULT Render_AlphaBlend(void);
	HRESULT Render_UI(void);

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free(void);
};

END