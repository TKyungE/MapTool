#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final :	public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		_float fSpeedPerSec;
		_float fRotationPerSec;
	}TRANSFORMDESC;

private:
	CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_float3 Get_State(STATE eState) const { return *(_float3*)&m_WorldMatrix.m[eState][0]; }
	_float4x4 Get_WorldMatrix(void) const { return m_WorldMatrix; }
	const TRANSFORMDESC& Get_TransformDesc(void) const { return m_TransformDesc; }
	
	void Set_State(STATE eState, _float3 vState) { memcpy(&m_WorldMatrix.m[eState][0], &vState, sizeof(_float3)); }
	void Set_TransformDesc(const TRANSFORMDESC& TransformDesc) { m_TransformDesc = TransformDesc; }
	void Set_TransformDesc_Speed(_float fSpeed) { m_TransformDesc.fSpeedPerSec = fSpeed; }


	_float3 Get_Scale(void);
	void Set_Scaled(_float3 vScale);

public:
	virtual HRESULT Initialize_Prototype(void);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Bind_OnGraphicDev(void);

public:
	void Go_Straight(void);
	void Go_Backward(void);
	void Go_Right(void);
	void Go_Left(void);
	void Turn(_float3 vAxis ,_float fTimeDelta);
	void LookAt(_float3 vPoint);

private:
	_float4x4 m_WorldMatrix;
	TRANSFORMDESC m_TransformDesc;

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;
};

END