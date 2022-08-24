#include "stdafx.h"
#include "..\Public\Camera_Dynamic.h"
#include "GameInstance.h"

CCamera_Dynamic::CCamera_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Dynamic::CCamera_Dynamic(const CCamera_Dynamic & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Dynamic::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Dynamic::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&((CAMERADESC_DERIVED*)pArg)->CameraDesc)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Dynamic::Tick()
{
	__super::Tick();

	if (GetKeyState('W') < 0)
	{
		m_pTransform->Go_Straight();
	}

	if (GetKeyState('S') < 0)
	{
		m_pTransform->Go_Backward();
	}

	if (GetKeyState('A') < 0)
	{

		m_pTransform->Go_Left();
	}

	if (GetKeyState('D') < 0)
	{
		m_pTransform->Go_Right();
	}

	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	_long			MouseMove = 0;

	if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_X))
	{
		m_pTransform->Turn(_float3(0.f, 1.f, 0.f), MouseMove * 0.001f);
	}

	if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y))
	{
		m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), MouseMove * 0.001f);
	}

	Safe_Release(pGameInstance);


	if (FAILED(Bind_OnGraphicDev()))
		return;
}

HRESULT CCamera_Dynamic::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}


CCamera_Dynamic * CCamera_Dynamic::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Dynamic*	pInstance = new CCamera_Dynamic(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCamera_Dynamic"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Dynamic::Clone(void* pArg)
{
	CCamera_Dynamic*	pInstance = new CCamera_Dynamic(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCamera_Dynamic"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CCamera_Dynamic::Free()
{
	__super::Free();
}
