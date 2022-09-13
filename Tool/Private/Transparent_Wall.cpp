#include "stdafx.h"
#include "..\Public\Transparent_Wall.h"

#include "GameInstance.h"

Transparent_Wall::Transparent_Wall(LPDIRECT3DDEVICE9 _pGraphic_Device)
	:CGameObject(_pGraphic_Device)
{
}

Transparent_Wall::Transparent_Wall(const Transparent_Wall & rhs)
	:CGameObject(rhs)
{
}

HRESULT Transparent_Wall::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Transparent_Wall::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_IndexPos, pArg, sizeof(INDEXPOS));

	m_pTransformCom->Set_Scaled(m_IndexPos.vScale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_IndexPos.vPos);

	return S_OK;
}

void Transparent_Wall::Tick()
{
	__super::Tick();


	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}



HRESULT Transparent_Wall::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pVIBufferCube->Render();

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT Transparent_Wall::SetUp_Components(void)
{
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_VIBufferCube"), TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCube)))
		return E_FAIL;

	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

Transparent_Wall * Transparent_Wall::Create(LPDIRECT3DDEVICE9 _pGraphic_Device)
{
	Transparent_Wall* pInstance = new Transparent_Wall(_pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : Transparent_Wall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * Transparent_Wall::Clone(void * pArg)
{
	Transparent_Wall* pInstance = new Transparent_Wall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : Transparent_Wall"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void Transparent_Wall::Free(void)
{
	__super::Free();

	Safe_Release(m_pVIBufferCube);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
}
