#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CVIBuffer_Cube;
class CTexture;
END



class CPortal final : public CGameObject
{
public:
	typedef struct tagIndexPos
	{
		_float3		vPos;
		_float3  vScale;
		_uint	iIndex;
		_uint iTrun;
	}INDEXPOS;

private:
	CPortal(LPDIRECT3DDEVICE9 _pGraphic_Device);
	CPortal(const CPortal& rhs);
	virtual ~CPortal() = default;

public:
	virtual HRESULT Initialize_Prototype(void) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick() override;
	
	virtual HRESULT Render(void) override;

private:
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Cube* m_pVIBuffer = nullptr;
	CTexture* m_pTextureCom = nullptr;

private:
	INDEXPOS	m_IndexPos;

	DWORD		m_dwTime;
	_uint		m_iTexture = 0;
private:
	HRESULT SetUp_Components(void);
	HRESULT SetUp_RenderState(void);
	HRESULT Release_RenderState(void);
	HRESULT On_SamplerState();
	HRESULT Off_SamplerState();
	
	void	OnBillboard();
public:
	static CPortal* Create(LPDIRECT3DDEVICE9 _pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;
};

