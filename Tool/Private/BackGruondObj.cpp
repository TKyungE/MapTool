#include "stdafx.h"
#include "..\Public\BackGruondObj.h"
#include "GameInstance.h"
#include "../Default/MainFrm.h"
#include "../Default/MyForm.h"
#include "../Default/ToolView.h"
#include <string>
CBackGroundObj::CBackGroundObj(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CBackGroundObj::CBackGroundObj(const CBackGroundObj & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBackGroundObj::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBackGroundObj::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_iIndex, pArg, sizeof(INDEXPOS));
	m_pTransformCom->Set_Scaled(m_iIndex.m_Scale);

	m_iIndex.m_BackGroundPos.y += 0.5f * m_iIndex.m_Scale.y;

	for (_uint i = 0; i < m_iIndex.iTrun; ++i)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), 1);
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_iIndex.m_BackGroundPos);

	return S_OK;
}

void CBackGroundObj::Tick(void)
{
	__super::Tick();


	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

}


HRESULT CBackGroundObj::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (m_iIndex.m_iIndex != 100)
	{

		if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
			return E_FAIL;

		if (FAILED(m_pTextureCom->Bind_OnGraphicDev(m_iIndex.m_iIndex)))
			return E_FAIL;

		if (FAILED(SetUp_RenderState()))
			return E_FAIL;

		m_pVIBufferCom->Render();

		if (FAILED(Release_RenderState()))
			return E_FAIL;
	}
	else
	{
		CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CMyForm*		pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_MainSplitter.GetPane(0, 0));
		
		CString strstr;
		int index = pMyForm->m_ObejctListBox.GetCurSel();
		if (index != LB_ERR)
		{
			pMyForm->m_ObejctListBox.GetText(index, strstr);

			CString str;
			if (strstr == TEXT("BackGround"))
			{
				pMyForm->m_EditIndex.GetWindowText(str);

				int iIndex = _ttoi(str);

				CGameInstance* pInstance = CGameInstance::Get_Instance();

				Safe_AddRef(pInstance);
				_float3 vPos = pInstance->Get_TargetPos();

				CString strX, strY, strZ;

				pMyForm->m_EditSizeX.GetWindowText(strX);
				pMyForm->m_EditSizeY.GetWindowText(strY);
				pMyForm->m_EditSizeZ.GetWindowText(strZ);

				float fScaleX = _float(_wtof(strX));
				float fScaleY = _float(_wtof(strY));
				float fScaleZ = _float(_wtof(strZ));

				m_pTransformCom->Set_Scaled(_float3(fScaleX, fScaleY, fScaleZ));
				vPos.y = 0.5f * fScaleY;

				CString strTrun;
				pMyForm->m_EditTrun.GetWindowText(strTrun);
				_uint iTurn = _float(_wtof(strTrun));

				if (m_iTurn != iTurn)
				{
					if (m_iTurn > iTurn)  // 2    1 
					{
						_uint iTurn2 = m_iTurn - iTurn;

						for (_uint i = 0; i < iTurn2; ++i)
							m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), -1);
						
						m_iTurn = iTurn;
					}
					else
					{
						_uint iTrun3 = iTurn - m_iTurn;

						for (_uint i = 0; i < iTrun3; ++i)
							m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), 1);
						
						m_iTurn = iTurn;
					}
				}
				
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

				if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
					return E_FAIL;

				if (FAILED(m_pTextureCom->Bind_OnGraphicDev(iIndex)))
					return E_FAIL;

				if (FAILED(SetUp_RenderState()))
					return E_FAIL;

				m_pVIBufferCom->Render();

				if (FAILED(Release_RenderState()))
					return E_FAIL;

				Safe_Release(pInstance);
			}
		}
	}
	return S_OK;
}

HRESULT CBackGroundObj::SetUp_Components(void)
{
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), TEXT("Prototype_Component_Texture_BackGround"), (CComponent**)&m_pTextureCom)))
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

HRESULT CBackGroundObj::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return S_OK;
}

HRESULT CBackGroundObj::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetTexture(0, nullptr);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

CBackGroundObj * CBackGroundObj::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackGroundObj* pInstance = new CBackGroundObj(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CBackGroundObj"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBackGroundObj::Clone(void * pArg)
{
	CBackGroundObj* pInstance = new CBackGroundObj(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CBackGroundObj"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGroundObj::Free(void)
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
}
