#pragma once

#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Component_Manager.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:	//Engine
	HRESULT Initialize_Engine(const GRAPHIC_DESC& GraphicDesc, LPDIRECT3DDEVICE9* ppOut);
	void Tick_Engine();
	void Clear(void);

public: //Graphic_Device
	void Render_Begin(void);
	void Render_End(HWND hWnd = 0);

public: //Object_Manager
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg = nullptr);

public: //Component_Manager
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(const _tchar* pPrototypeTag, void* pArg = nullptr);

public:
	static void Release_Engine(void);

private:
	CGraphic_Device* m_pGraphic_Device = nullptr;
	CObject_Manager* m_pObject_Manager = nullptr;
	CComponent_Manager* m_pComponent_Manager = nullptr;
	
public:
	virtual void Free(void) override;
};

END