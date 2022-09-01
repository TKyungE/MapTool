#include "stdafx.h"
#include "..\Public\Tree.h"
#include "GameInstance.h"

CTree::CTree(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTree::CTree(const CTree & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTree::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTree::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	memcpy(&m_iIndex, pArg, sizeof(INDEXPOS));

	m_pTransformCom->Set_Scaled(m_iIndex.m_Scale);

	m_iIndex.m_BackGroundPos.y += 0.5f * m_iIndex.m_Scale.y;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_iIndex.m_BackGroundPos);

	_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	m_pRectTransform->Set_Scaled(_float3(m_iIndex.m_Scale.x + vPos.x, m_iIndex.m_Scale.y, m_iIndex.m_Scale.z));

	vPos.y += m_pTransformCom->Get_State(CTransform::STATE_POSITION).y * 0.5f;
	m_pRectTransform->Set_State(CTransform::STATE_POSITION, vPos);

	m_pRectTransform2->Set_Scaled(_float3(m_iIndex.m_Scale.x + vPos.x, m_iIndex.m_Scale.y, m_iIndex.m_Scale.z));
	m_pRectTransform2->Set_State(CTransform::STATE_POSITION, vPos);
	m_pRectTransform2->Turn(_float3(0.f, 1.f, 0.f), 1.f);

	return S_OK;
}

void CTree::Tick(void)
{
	__super::Tick();

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CTree::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;


	if (FAILED(m_pTextureCom->Bind_OnGraphicDev(0)))
		return E_FAIL;



	if (FAILED(SetUp_RenderState()))
		return E_FAIL;


	m_pVIBufferCom->Render();


	if (FAILED(Release_RenderState()))
		return E_FAIL;


	_uint iIndex = 0;
	if (FAILED(m_pRectTransform->Bind_OnGraphicDev()))
		return E_FAIL;

	if (FAILED(m_pRectTexture->Bind_OnGraphicDev(iIndex)))
		return E_FAIL;

	if (FAILED(SetUp_Rect_RenderState()))
		return E_FAIL;

	m_VIBufferRect->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;





	if (FAILED(m_pRectTransform2->Bind_OnGraphicDev()))
		return E_FAIL;

	if (FAILED(m_pRectTexture2->Bind_OnGraphicDev(iIndex)))
		return E_FAIL;

	if (FAILED(SetUp_Rect_RenderState()))
		return E_FAIL;

	m_VIBufferRect2->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTree::SetUp_Components(void)
{
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), TEXT("Prototype_Component_Texture_Tree"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), TEXT("Prototype_Component_Texture_TreeRect"), (CComponent**)&m_pRectTexture)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), TEXT("Prototype_Component_Texture_TreeRect"), (CComponent**)&m_pRectTexture2)))
		return E_FAIL;


	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer1"), TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_VIBufferRect)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer2"), TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_VIBufferRect2)))
		return E_FAIL;

	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Transform1"), TEXT("Prototype_Component_Transform"), (CComponent**)&	m_pRectTransform, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Transform2"), TEXT("Prototype_Component_Transform"), (CComponent**)&	m_pRectTransform2, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTree::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CTree::SetUp_Rect_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	return S_OK;
}

HRESULT CTree::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetTexture(0, nullptr);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

CTree * CTree::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTree* pInstance = new CTree(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTree"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTree::Clone(void * pArg)
{
	CTree* pInstance = new CTree(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTree"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTree::Free(void)
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRectTexture);
	Safe_Release(m_pRectTransform);
	Safe_Release(m_VIBufferRect);
	Safe_Release(m_pRectTexture2);
	Safe_Release(m_pRectTransform2);
	Safe_Release(m_VIBufferRect2);
}
