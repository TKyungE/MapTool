#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

class CMonsterSpawn final : public CGameObject
{
private:
	CMonsterSpawn(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMonsterSpawn(const CMonsterSpawn&  rhs);
	virtual ~CMonsterSpawn() = default;

public:
	CTransform*		Get_Transform(void) { return m_pTransformCom; }

public:
	virtual HRESULT Initialize_Prototype(void) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(void) override;
	virtual HRESULT Render(void) override;

private:
	CRenderer* m_pRendererCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;

private:
	HRESULT SetUp_Components(void);
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CMonsterSpawn* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;
};

