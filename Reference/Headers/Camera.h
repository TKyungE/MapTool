#pragma once

#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{
		_float3 vEye;
		_float3 vAt;

		_float fAspect;
		_float fFovy;
		_float fNear;
		_float fFar;

		CTransform::TRANSFORMDESC TransformDesc;
	}CAMERADESC;

protected:
	CCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype(void);
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render(void);

protected:
	static _tchar* m_pTransformTag;
	class CTransform* m_pTransform = nullptr;
	CAMERADESC m_CameraDesc;

protected:
	HRESULT Bind_OnGraphicDev(void);

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free(void) override;
};

END