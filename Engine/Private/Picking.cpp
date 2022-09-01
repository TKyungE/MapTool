#include "..\Public\Picking.h"

IMPLEMENT_SINGLETON(CPicking)

CPicking::CPicking()
{
	ZeroMemory(&m_GraphicDesc, sizeof(GRAPHIC_DESC));
	ZeroMemory(&m_MouseRayPos, sizeof(_float3));
	ZeroMemory(&m_MouseRayDir, sizeof(_float3));
	ZeroMemory(&m_TargetPos, sizeof(_float3));
}

HRESULT CPicking::Initialize(const GRAPHIC_DESC & GraphicDesc, LPDIRECT3DDEVICE9 * ppOut)
{
	memcpy(&m_GraphicDesc, &GraphicDesc, sizeof(GRAPHIC_DESC));

	m_pGraphic_Device = *ppOut;
	Safe_AddRef(m_pGraphic_Device);

	return S_OK;
}

void CPicking::Tick()
{
	POINT		ptMouse;

	GetCursorPos(&ptMouse);

	ScreenToClient(m_GraphicDesc.hWnd, &ptMouse);

	D3DVIEWPORT9		Viewport;

	m_pGraphic_Device->GetViewport(&Viewport);

	_float3				vMousePoint;

	/* 투영스페이스 상의 마수으 좌료플 구했다. */
	vMousePoint.x = ptMouse.x / (Viewport.Width * 0.5f) - 1.f;
	vMousePoint.y = ptMouse.y / -(Viewport.Height * 0.5f) + 1.f;
	vMousePoint.z = 0.f;

	/* 뷰스페이스 상의 좌표를 구하자. */
	_float4x4			ProjMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);
	D3DXMatrixInverse(&ProjMatrix, nullptr, &ProjMatrix);
	D3DXVec3TransformCoord(&vMousePoint, &vMousePoint, &ProjMatrix);

	/* 마우스 레이, 마우스 레이포스(마우스 레이의 시작점) 를 구한다.  */


	m_vRayPos = _float3(0.f, 0.f, 0.f);
	m_vRayDir = vMousePoint - m_vRayPos;

	/* 월드스페이스 상의  vRayDir, vRayPos를 구한다. */
	_float4x4			ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &ViewMatrix);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &ViewMatrix);

	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
}

HRESULT CPicking::Intersect(_float4x4 InvWorld, _float3* LU, _float3* RU, _float3* RD)
{
	D3DXVec3TransformCoord(&m_MouseRayPos, &m_MouseRayPos, &InvWorld);
	D3DXVec3TransformNormal(&m_MouseRayDir, &m_MouseRayDir, &InvWorld);

	D3DXVec3Normalize(&m_MouseRayDir, &m_MouseRayDir);

	_float Dist = 0.f;

	if (D3DXIntersectTri(LU, RU, RD, &m_MouseRayPos, &m_MouseRayDir, nullptr, nullptr, &Dist))
		m_TargetPos = m_MouseRayPos + m_MouseRayDir * Dist;
	else
		return E_FAIL;

	return S_OK;
}

void CPicking::Transform_ToLocalSpace(_float4x4 WorldMatrixInverse)
{
	D3DXVec3TransformCoord(&m_vRayPos_Local, &m_vRayPos, &WorldMatrixInverse);
	D3DXVec3TransformNormal(&m_vRayDir_Local, &m_vRayDir, &WorldMatrixInverse);

	D3DXVec3Normalize(&m_vRayDir_Local, &m_vRayDir_Local);
}

_bool CPicking::Intersect_InWorldSpace(_float3 vPointA, _float3 vPointB, _float3 vPointC, _float3 * pOut)
{
	_float		fU, fV, fDist;

	if (true == D3DXIntersectTri(&vPointA, &vPointB, &vPointC, &m_vRayPos, &m_vRayDir, &fU, &fV, &fDist))
	{
		*pOut = m_vRayPos + m_vRayDir* fDist;

		return true;
	}

	return false;
}

_bool CPicking::Intersect_InLocalSpace(_float3 vPointA, _float3 vPointB, _float3 vPointC, _float3 * pOut)
{
	_float		fU, fV, fDist;

	if (true == D3DXIntersectTri(&vPointA, &vPointB, &vPointC, &m_vRayPos_Local, &m_vRayDir_Local, &fU, &fV, &fDist))
	{
		*pOut = m_vRayPos_Local + m_vRayDir_Local * fDist;

		return true;
	}

	return false;
}

void CPicking::Free(void)
{
	Safe_Release(m_pGraphic_Device);
}
