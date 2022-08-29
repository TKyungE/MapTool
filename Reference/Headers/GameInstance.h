#pragma once

#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Input_Device.h"
#include "Picking.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:	//Engine
	HRESULT Initialize_Engine(HINSTANCE hInstance,const GRAPHIC_DESC& GraphicDesc, LPDIRECT3DDEVICE9* ppOut);
	void Tick_Engine(void);
	void Clear(void);

public: //Graphic_Device
	void Render_Begin(void);
	void Render_End(HWND hWnd = 0);

public: /* For.Input_Device */
	_char Get_DIKState(_uchar eKeyID);
	_char Get_DIMKeyState(DIMK eMouseKeyID);
	_long Get_DIMMoveState(DIMM eMouseMoveID);

public: //Object_Manager
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg = nullptr);
	CGameObject* Find_Object(const _tchar* pLayerTag, _uint iIndex);

public: //Component_Manager
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(const _tchar* pPrototypeTag, void* pArg = nullptr);
	
public: //Picking
	HRESULT Intersect(_float4x4 InvWorld, _float3* LU, _float3* RU, _float3* RD);
	void Tick();
	_float3 Get_TargetPos(void);

public:
	static void Release_Engine(void);

private:
	CGraphic_Device* m_pGraphic_Device = nullptr;
	CObject_Manager* m_pObject_Manager = nullptr;
	CComponent_Manager* m_pComponent_Manager = nullptr;
	CInput_Device*					m_pInput_Device = nullptr;
	CPicking*		m_pPicking = nullptr;
public:
	virtual void Free(void) override;
};

END