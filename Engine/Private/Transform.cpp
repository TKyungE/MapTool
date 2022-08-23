#include "..\Public\Transform.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
}

_float3 CTransform::Get_Scale(void)
{
	_float3 vRight = Get_State(CTransform::STATE_RIGHT);
	_float3 vUp = Get_State(CTransform::STATE_UP);
	_float3 vLook = Get_State(CTransform::STATE_LOOK);

	return _float3(D3DXVec3Length(&vRight), D3DXVec3Length(&vUp), D3DXVec3Length(&vLook));
}

void CTransform::Set_Scaled(_float3 vScale)
{
	_float3 vRight = Get_State(CTransform::STATE_RIGHT);
	_float3 vUp = Get_State(CTransform::STATE_UP);
	_float3 vLook = Get_State(CTransform::STATE_LOOK);

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale.z);
}

HRESULT CTransform::Initialize_Prototype(void)
{
	D3DXMatrixIdentity(&m_WorldMatrix);

	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));
	
	return S_OK;
}

HRESULT CTransform::Bind_OnGraphicDev(void)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);

	return S_OK;
}

void CTransform::Go_Straight(void)
{
	_float3 vPosition = Get_State(CTransform::STATE_POSITION);
	_float3 vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * 0.001f;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(void)
{
	_float3 vPosition = Get_State(CTransform::STATE_POSITION);
	_float3 vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * 0.001f;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Right(void)
{
	_float3 vPosition = Get_State(CTransform::STATE_POSITION);
	_float3 vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * m_TransformDesc.fSpeedPerSec * 0.001f;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Left(void)
{
	_float3 vPosition = Get_State(CTransform::STATE_POSITION);
	_float3 vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * m_TransformDesc.fSpeedPerSec * 0.001f;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::LookAt(_float3 vPoint)
{
	_float3 vPosition = Get_State(CTransform::STATE_POSITION);

	_float3 vLook = vPoint - vPosition;

	_float3 vRight = *D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);

	_float3 vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);

	_float3 vScale = Get_Scale();

	Set_State(CTransform::STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScale.x);
	Set_State(CTransform::STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScale.y);
	Set_State(CTransform::STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScale.z);
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform* pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free(void)
{
	__super::Free();
}
