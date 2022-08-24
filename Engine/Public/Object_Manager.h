#pragma once

#include "Base.h"

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg = nullptr);
	void Tick(void);
	void Clear(void);
	class CLayer* Find_Layer(const _tchar* pLayerTag);

private:
	map<const _tchar*, class CGameObject*> m_Prototypes;
	typedef map<const _tchar*, class CGameObject*> PROTOTYPES;

private:
	map<const _tchar*, class CLayer*> m_Layers;
	typedef map<const _tchar*, class CLayer*> LAYERS;

private:
	class CGameObject* Find_Prototype(const _tchar* pPrototypeTag);

public:
	virtual void Free(void) override;
};

END