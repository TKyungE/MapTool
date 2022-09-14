#pragma once


#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer_Cube;
class CRenderer;
END

class Transparent_Wall final : public CGameObject
{
public:
	typedef struct tagIndexPos
	{
		_float3		vPos;
		_float3		vScale;
	}INDEXPOS;
private:
	Transparent_Wall(LPDIRECT3DDEVICE9 _pGraphic_Device);
	Transparent_Wall(const Transparent_Wall& rhs);
	virtual ~Transparent_Wall() = default;
public:
	virtual HRESULT Initialize_Prototype(void) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick();

	virtual HRESULT Render(void) override;
private:
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Cube*	m_pVIBufferCube = nullptr;
	CRenderer*	m_pRendererCom = nullptr;

	INDEXPOS	m_IndexPos;
	_bool		m_bCheck = false;
private:
	HRESULT SetUp_Components(void);
public:
	static Transparent_Wall* Create(LPDIRECT3DDEVICE9 _pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;
};

