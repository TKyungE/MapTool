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

void CMyTerrain::Tick(void)
{
	__super::Tick();

	CGameInstance* pInstance = CGameInstance::Get_Instance();

	if (nullptr == pInstance)
		return;
	
	Safe_AddRef(pInstance);

	if (pInstance->Get_DIMKeyState(DIMK_LBUTTON) < 0)
	{
		_float4x4 matWorld = m_pTransformCom->Get_WorldMatrix();
		D3DXMatrixInverse(&matWorld, nullptr, &matWorld);

		LPDIRECT3DVERTEXBUFFER9 VB = m_pVIBufferCom->Get_VB();
		LPDIRECT3DINDEXBUFFER9 IB = m_pVIBufferCom->Get_IB();

		VTXTEX* pVertices = nullptr;
		FACEINDICES32* pIndices = nullptr;

		VB->Lock(0, 0, (void**)&pVertices, 0);
		IB->Lock(0, 0, (void**)&pIndices, 0);

		for (_uint i = 0; i < m_pVIBufferCom->Get_VIBInfo().m_iNumPrimitive; ++i)
		{
			_float3 LU = pVertices[pIndices[i]._0].vPosition;
			_float3 RU = pVertices[pIndices[i]._1].vPosition;
			_float3 RD = pVertices[pIndices[i]._2].vPosition;

			if (!FAILED(pInstance->Intersect(matWorld, &LU, &RU, &RD)))
			{
				/*_tchar m_szFPS[MAX_PATH] = L"";
				wsprintf(m_szFPS, L"ÁÂÇ¥ : %d, %d, %d", (int)pInstance->Get_TargetPos().x, (int)pInstance->Get_TargetPos().y, (int)pInstance->Get_TargetPos().z);
				ERR_MSG(m_szFPS);*/
				if (!m_bObjectCheck)
				{
					pVertices[pIndices[i]._0].vPosition.y = m_fValue;
					pVertices[pIndices[i]._1].vPosition.y = m_fValue;
					pVertices[pIndices[i]._2].vPosition.y = m_fValue;
				}

				if (0 <= m_iTileCheck)
					Change_Terrain(LU, m_iTileCheck);
				
				break;
			}
		}

		VB->Unlock();
		IB->Unlock();
	}

	Safe_Release(pInstance);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CMyTerrain::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDev(6)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CMyTerrain::SetUp_Components(void)
{
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

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

void CMyTerrain::Change_Terrain(_float3& LU, _int _iTile)
{
	CGameInstance* pInstance = CGameInstance::Get_Instance();
	if (nullptr == pInstance)
		return;

	Safe_AddRef(pInstance);
	
	_float3 vRectPos = LU + _float3(0.5f, 0.f, -0.5f);
	vRectPos.y += 0.001f;
	D3DXVec3TransformCoord(&vRectPos, &vRectPos, &m_pTransformCom->Get_WorldMatrix());

	CTerrainRect::RECTINFO m_tRectInfo;
	m_tRectInfo.vPos = vRectPos;
	m_tRectInfo.vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
	m_tRectInfo.iTex = _iTile;

	_uint i = 0;
	CTerrainRect* pObject;
	while (nullptr != (pObject = (CTerrainRect*)pInstance->Find_Object(TEXT("Layer_TerrainRect"), i)))
	{
		CTransform* pTransform = (CTransform*)pObject->Find_Component(TEXT("Com_Transform"));
		if (m_tRectInfo.vPos == *(_float3*)&pTransform->Get_WorldMatrix().m[3][0])
		{
			pObject->Set_RectInfo(m_tRectInfo);
			Safe_Release(pInstance);
			return;
		}
		
		++i;
	}
	
	if (FAILED(pInstance->Add_GameObject(TEXT("Prototype_GameObject_TerrainRect"), TEXT("Layer_TerrainRect"), &m_tRectInfo)))
	{
		ERR_MSG(TEXT("Failed to Cloned : TerrainRect"));
		return;
	}

	Safe_Release(pInstance);
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

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
