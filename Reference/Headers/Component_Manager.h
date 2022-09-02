#pragma once

#include "Renderer.h"
#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"
#include "Renderer.h"


BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(const _tchar* pPrototypeTag, void* pArg = nullptr);
	void Clear(void);

private:
	map<const _tchar*, class CComponent*> m_Prototypes;
	typedef map<const _tchar*, class CComponent*> PROTOTYPES;

private:
	class CComponent* Find_Component(const _tchar* pPrototypeTag);

public:
	virtual void Free(void) override;
};

END