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

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(4 * sizeof(VTXTEX), 0, D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_MANAGED, &m_pVBuffer, 0)))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(2 * sizeof(FACEINDICES16), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIBuffer, nullptr)))
		return E_FAIL;

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
	
	VTXTEX* pVertices = nullptr;
	VTXTEX* pVertex = nullptr;

	FACEINDICES16* pIndex = nullptr;

	VB->Lock(0, 0, (void**)&pVertices, 0);
	m_pVBuffer->Lock(0, 0, (void**)&pVertex, 0);

	m_iIndex = pVIBufferTerrain->Get_VIBInfoDerived().m_iNumVerticesX * (_uint)m_tInfo.vPos.z + (_uint)m_tInfo.vPos.x;
	m_iIndices[0] = m_iIndex + pVIBufferTerrain->Get_VIBInfoDerived().m_iNumVerticesX;
	m_iIndices[1] = m_iIndex + pVIBufferTerrain->Get_VIBInfoDerived().m_iNumVerticesX + 1;
	m_iIndices[2] = m_iIndex + 1;
	m_iIndices[3] = m_iIndex;

	pVertex[0].vPosition = pVertices[m_iIndices[0]].vPosition;
	pVertex[0].vPosition.y += 0.001f;
	pVertex[0].vTexture = pVertices[m_iIndices[0]].vTexture;

	pVertex[1].vPosition = pVertices[m_iIndices[1]].vPosition;
	pVertex[1].vPosition.y += 0.001f;
	pVertex[1].vTexture = pVertices[m_iIndices[1]].vTexture;

	pVertex[2].vPosition = pVertices[m_iIndices[2]].vPosition;
	pVertex[2].vPosition.y += 0.001f;
	pVertex[2].vTexture = pVertices[m_iIndices[2]].vTexture;

	pVertex[3].vPosition = pVertices[m_iIndices[3]].vPosition;
	pVertex[3].vPosition.y += 0.001f;
	pVertex[3].vTexture = pVertices[m_iIndices[3]].vTexture;

	VB->Unlock();
	m_pVBuffer->Unlock();

	m_pIBuffer->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIBuffer->Unlock();

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

	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetStreamSource(0, m_pVBuffer, 0, sizeof(VTXTEX));
	m_pGraphic_Device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	m_pGraphic_Device->SetIndices(m_pIBuffer);

	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	return S_OK;
}

HRESULT CTerrainRect::SetUp_Components(void)
{
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), TEXT("Prototype_Component_Texture_Terrain"), (CComponent**)&m_pTextureCom)))
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
	Safe_Release(m_pIBuffer);
	Safe_Release(m_pVBuffer);

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);

	m_Components.clear();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);

	Safe_Release(m_pGraphic_Device);
}