#include "stdafx.h"
#include "..\Public\TerrainRect.h"
#include "GameInstance.h"
#include "MyTerrain.h"

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
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tInfo.vPos);

	return S_OK;
}

void CTerrainRect::Tick(void)
{
	__super::Tick();

	CGameInstance* pInstance = CGameInstance::Get_Instance();
	if (nullptr == pInstance)
	{
		ERR_MSG(TEXT("Failed to Get : CGameInstance"));
		return;
	}

	Safe_AddRef(pInstance);

	CMyTerrain* pTerrain = (CMyTerrain*)pInstance->Find_Object(TEXT("Layer_BackGround"), 0);
	if (nullptr == pTerrain)
	{
		ERR_MSG(TEXT("Failed to Get : CMyTerrain"));
		return;
	}

	Safe_AddRef(pTerrain);

	CVIBuffer_Terrain* pVIBufferTerrain = (CVIBuffer_Terrain*)pTerrain->Find_Component(TEXT("Com_VIBuffer"));
	if (nullptr == pVIBufferTerrain)
	{
		ERR_MSG(TEXT("Failed to Get : CVIBuffer_Terrain"));
		return;
	}

	Safe_AddRef(pVIBufferTerrain);

	LPDIRECT3DVERTEXBUFFER9 VB = pVIBufferTerrain->Get_VB();
	_float3 VerArray[4] = {
		m_pVIBufferCom->Get_VertexArray(0),
		m_pVIBufferCom->Get_VertexArray(1),
		m_pVIBufferCom->Get_VertexArray(2),
		m_pVIBufferCom->Get_VertexArray(3)
	};

	VTXTEX* pVertices = nullptr;

	VB->Lock(0, 0, (void**)&pVertices, 0);

	m_iIndex = pVIBufferTerrain->Get_VIBInfoDerived().m_iNumVerticesX * (_uint)m_tInfo.vPos.z + (_uint)m_tInfo.vPos.x;
	m_iIndices[0] = m_iIndex + pVIBufferTerrain->Get_VIBInfoDerived().m_iNumVerticesX;
	m_iIndices[1] = m_iIndex + pVIBufferTerrain->Get_VIBInfoDerived().m_iNumVerticesX + 1;
	m_iIndices[2] = m_iIndex + 1;
	m_iIndices[3] = m_iIndex;

	VerArray[0].y = pVertices[m_iIndices[0]].vPosition.y;
	VerArray[1].y = pVertices[m_iIndices[1]].vPosition.y;
	VerArray[2].y = pVertices[m_iIndices[2]].vPosition.y;
	VerArray[3].y = pVertices[m_iIndices[3]].vPosition.y;

	VB->Unlock();

	Safe_Release(pVIBufferTerrain);
	Safe_Release(pTerrain);
	Safe_Release(pInstance);

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

	LPDIRECT3DVERTEXBUFFER9 RectVB = m_pVIBufferCom->Get_VB();
	VTXTEX* pRectVertices = nullptr;
	RectVB->Lock(0, 0, (void**)&pRectVertices, 0);

	_float3 a = pRectVertices[0].vPosition;
	a = pRectVertices[1].vPosition;
	a = pRectVertices[2].vPosition;
	a = pRectVertices[3].vPosition;

	RectVB->Unlock();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CTerrainRect::SetUp_Components(void)
{
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), TEXT("Prototype_Component_Texture_Terrain"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), TEXT("Prototype_Component_VIBuffer_TerrainRect"), (CComponent**)&m_pVIBufferCom)))
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