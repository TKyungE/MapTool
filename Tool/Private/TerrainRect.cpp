#include "stdafx.h"
#include "..\Public\TerrainRect.h"
#include "GameInstance.h"

CTerrainRect::CTerrainRect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	ZeroMemory(&m_tInfo, sizeof(RECTINFO));
}

CTerrainRect::CTerrainRect(const CTerrainRect & rhs)
	: CGameObject(rhs)
{
	memcpy(&m_tInfo, &rhs.m_tInfo, sizeof(RECTINFO));
}

HRESULT CTerrainRect::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrainRect::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_tInfo, pArg, sizeof(RECTINFO));

	if (nullptr != m_pTransformCom)
	{
		m_pTransformCom->Set_State(CTransform::STATE_UP, m_tInfo.vUp);
		m_pTransformCom->Turn(_float3(1.f, 0.f, 0.f), 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tInfo.vPos);
	}

	return S_OK;
}

void CTerrainRect::Tick(void)
{
	__super::Tick();

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CTerrainRect::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDev(m_tInfo.iTex)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CTerrainRect::SetUp_Components(void)
{
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), TEXT("Prototype_Component_Texture_Terrain"), (CComponent**)&m_pTextureCom)))
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

CTerrainRect * CTerrainRect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrainRect* pInstance = new CTerrainRect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMyTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrainRect::Clone(void * pArg)
{
	CTerrainRect* pInstance = new CTerrainRect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMyTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrainRect::Free(void)
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}