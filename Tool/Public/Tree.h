#pragma once


#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

class CTree final : public CGameObject
{
private:
	CTree(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTree(const CTree& rhs);
	virtual ~CTree() = default;
public:
	virtual HRESULT Initialize_Prototype(void) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(void) override;
	virtual HRESULT Render(void) override;

private:
	CRenderer* m_pRendererCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Cube* m_pVIBufferCom = nullptr;
private:
	typedef struct tagIndexPos
	{
		_float3 m_BackGroundPos;
		_uint m_iIndex;
	}INDEXPOS;

	INDEXPOS m_iIndex;
private:
	HRESULT SetUp_Components(void);
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CTree* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;
};

