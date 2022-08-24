#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype(void);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(void);
	virtual HRESULT Render(void);
protected:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

protected:
	map<const _tchar*, class CComponent*> m_Components;

protected:
	HRESULT Add_Components(const _tchar* pComponentTag, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg = nullptr);

protected:
	class CComponent* Find_Component(const _tchar* pComponentTag);

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free(void) override;
};

END