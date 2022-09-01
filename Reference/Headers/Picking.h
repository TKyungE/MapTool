#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CPicking final : public CBase
{
	DECLARE_SINGLETON(CPicking)
private:
	CPicking();
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(const GRAPHIC_DESC & GraphicDesc, LPDIRECT3DDEVICE9* ppOut);
	void Tick();
	HRESULT Intersect(_float4x4 InvWorld, _float3* LU, _float3* RU, _float3* RD);
	_float3 Get_TargetPos(void) { return m_TargetPos; }
public:
	void Transform_ToLocalSpace(_float4x4 WorldMatrixInverse);
	_bool Intersect_InWorldSpace(_float3 vPointA, _float3 vPointB, _float3 vPointC, _float3* pOut);
	_bool Intersect_InLocalSpace(_float3 vPointA, _float3 vPointB, _float3 vPointC, _float3* pOut);

private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;
	GRAPHIC_DESC m_GraphicDesc;
	_float3 m_MouseRayPos;
	_float3 m_MouseRayDir;
	_float3 m_TargetPos;

	_float3					m_vRayDir, m_vRayPos;
	_float3					m_vRayDir_Local, m_vRayPos_Local;

public:
	virtual void Free(void) override;
};

END