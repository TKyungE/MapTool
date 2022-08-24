#include "..\Public\GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::Get_Instance())
	, m_pObject_Manager(CObject_Manager::Get_Instance())
	, m_pComponent_Manager(CComponent_Manager::Get_Instance())
	, m_pInput_Device(CInput_Device::Get_Instance())
	, m_pPicking(CPicking::Get_Instance())
{
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pPicking);
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInstance, const GRAPHIC_DESC & GraphicDesc, LPDIRECT3DDEVICE9 * ppOut)
{
	if (nullptr == m_pGraphic_Device || nullptr == m_pObject_Manager)
		return E_FAIL;
	
	// 그래픽 디바이스 초기화
	if (FAILED(m_pGraphic_Device->InitDevice(GraphicDesc, ppOut)))
		return E_FAIL;

	if (FAILED(m_pInput_Device->Initialize(hInstance, GraphicDesc.hWnd)))
		return E_FAIL;

	if (FAILED(m_pPicking->Initialize(GraphicDesc, ppOut)))
		return E_FAIL;


	return S_OK;
}

void CGameInstance::Tick_Engine(void)
{
	if (nullptr == m_pObject_Manager)
		return;
	
	m_pInput_Device->Update();

	m_pObject_Manager->Tick();

	m_pPicking->Tick();
}

void CGameInstance::Clear(void)
{
	if (nullptr == m_pObject_Manager || nullptr == m_pComponent_Manager)
		return;
	
	m_pComponent_Manager->Clear();
	m_pObject_Manager->Clear();
}

void CGameInstance::Render_Begin(void)
{
	if (nullptr == m_pGraphic_Device)
		return;
	
	m_pGraphic_Device->Render_Begin();
}

void CGameInstance::Render_End(HWND hWnd)
{
	if (nullptr == m_pGraphic_Device)
		return;
	
	m_pGraphic_Device->Render_End(hWnd);
}

_char CGameInstance::Get_DIKState(_uchar eKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKState(eKeyID);
}

_char CGameInstance::Get_DIMKeyState(DIMK eMouseKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMKeyState(eMouseKeyID);
}


_long CGameInstance::Get_DIMMoveState(DIMM eMouseMoveID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMMoveState(eMouseMoveID);
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;
	
	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Add_GameObject(const _tchar * pPrototypeTag, const _tchar * pLayerTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;
	
	return m_pObject_Manager->Add_GameObject(pPrototypeTag, pLayerTag, pArg);
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;
	
	return m_pComponent_Manager->Add_Prototype(pPrototypeTag, pPrototype);
}

CComponent * CGameInstance::Clone_Component(const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;
	
	return m_pComponent_Manager->Clone_Component(pPrototypeTag, pArg);
}

HRESULT CGameInstance::Intersect(_float4x4 InvWorld, _float3 * LU, _float3 * RU, _float3 * RD)
{
	if (nullptr == m_pPicking)
		return E_FAIL;

	return m_pPicking->Intersect(InvWorld, LU, RU, RD);
}

void CGameInstance::Tick()
{
	if (nullptr == m_pPicking)
		return ;

	 m_pPicking->Tick();
}

_float3 CGameInstance::Get_TargetPos(void)
{
	if (nullptr == m_pPicking)
		return _float3();

	return m_pPicking->Get_TargetPos();
}

void CGameInstance::Release_Engine(void)
{
	CGameInstance::Get_Instance()->Destroy_Instance();

	CObject_Manager::Get_Instance()->Destroy_Instance();

	CComponent_Manager::Get_Instance()->Destroy_Instance();

	CInput_Device::Get_Instance()->Destroy_Instance();

	CPicking::Get_Instance()->Destroy_Instance();

	CGraphic_Device::Get_Instance()->Destroy_Instance();
}

void CGameInstance::Free(void)
{
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pPicking);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);
}
