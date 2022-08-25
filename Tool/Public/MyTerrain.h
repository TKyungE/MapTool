#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTexture;
class CTransform;
class CVIBuffer_Terrain;
class CRenderer;
END

class CToolView;
class CMyTerrain final : public CGameObject
{
private:
	CMyTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMyTerrain(const CMyTerrain& rhs);
	virtual ~CMyTerrain() = default;

public:
	CVIBuffer_Terrain* Get_VIBufferCom(void) { return m_pVIBufferCom; }

public:
	virtual HRESULT Initialize_Prototype(void) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(void) override;
	virtual HRESULT Render(void) override;
private:
	CRenderer* m_pRendererCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Terrain* m_pVIBufferCom = nullptr;
private:
	HRESULT SetUp_Components(void);

public:
	static CMyTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;
};

