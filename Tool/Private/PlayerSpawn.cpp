#include "stdafx.h"
#include "..\Public\PlayerSpawn.h"
#include "GameInstance.h"

CPlayerSpawn::CPlayerSpawn(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CPlayerSpawn::CPlayerSpawn(const CPlayerSpawn & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayerSpawn::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerSpawn::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	_float3 vPos;
	memcpy(&vPos, &pArg, sizeof(_float3));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

	return S_OK;
}

void CPlayerSpawn::Tick(void)
{
	__super::Tick();
}

HRESULT CPlayerSpawn::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	
	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;
	
	if (FAILED(m_pTextureCom->Bind_OnGraphicDev(0)))
		return E_FAIL;
	
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPlayerSpawn::SetUp_Components(void)
{
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), TEXT("Prototype_Component_Texture_PlayerSpawn"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CPlayerSpawn * CPlayerSpawn::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayerSpawn* pInstance = new CPlayerSpawn(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CPlayerSpawn"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayerSpawn::Clone(void * pArg)
{
	CPlayerSpawn* pInstance = new CPlayerSpawn(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CPlayerSpawn"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerSpawn::Free(void)
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
}
