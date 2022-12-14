#include "stdafx.h"
#include "..\Public\MyTerrain.h"
#include "GameInstance.h"

CMyTerrain::CMyTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CMyTerrain::CMyTerrain(const CMyTerrain & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMyTerrain::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CMyTerrain::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CMyTerrain::Render(void)
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

HRESULT CMyTerrain::SetUp_Components(void)
{
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), TEXT("Prototype_Component_Texture_Terrain"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), TEXT("Prototype_Component_VIBuffer_Terrain"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CMyTerrain * CMyTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMyTerrain* pInstance = new CMyTerrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMyTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMyTerrain::Clone(void * pArg)
{
	CMyTerrain* pInstance = new CMyTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMyTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMyTerrain::Free(void)
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
