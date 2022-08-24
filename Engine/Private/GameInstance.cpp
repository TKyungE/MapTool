#include "..\Public\GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::Get_Instance())
	, m_pObject_Manager(CObject_Manager::Get_Instance())
	, m_pComponent_Manager(CComponent_Manager::Get_Instance())
{
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CGameInstance::Initialize_Engine(const GRAPHIC_DESC & GraphicDesc, LPDIRECT3DDEVICE9 * ppOut)
{
	if (nullptr == m_pGraphic_Device || nullptr == m_pObject_Manager)
		return E_FAIL;
	
	// 그래픽 디바이스 초기화
	if (FAILED(m_pGraphic_Device->InitDevice(GraphicDesc, ppOut)))
		return E_FAIL;


	return S_OK;
}

void CGameInstance::Tick_Engine()
{
	if (nullptr == m_pObject_Manager)
		return;
	m_pObject_Manager->Tick();
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

void CGameInstance::Release_Engine(void)
{
	CGameInstance::Get_Instance()->Destroy_Instance();

	CObject_Manager::Get_Instance()->Destroy_Instance();

	CComponent_Manager::Get_Instance()->Destroy_Instance();

	CGraphic_Device::Get_Instance()->Destroy_Instance();
}

void CGameInstance::Free(void)
{
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pGraphic_Device);
}
