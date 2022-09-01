#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
public:
	typedef struct VIBufferInfo
	{
		_uint m_iNumVertices;
		_uint m_iStride;
		_ulong m_dwFVF;
		D3DPRIMITIVETYPE m_ePrimitiveType;
		_uint m_iNumPrimitive;
		_uint m_iIndicesByte;
		D3DFORMAT m_eIndexFormat;
	}VIBINFO;

protected:
	CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	LPDIRECT3DVERTEXBUFFER9 Get_VB(void) { return m_pVB; }
	LPDIRECT3DINDEXBUFFER9 Get_IB(void) { return m_pIB; }
	VIBINFO Get_VIBInfo(void) { return m_tVIBInfo; }
	void Set_VIBInfo(VIBINFO& tVIBInfo);

public:
	virtual HRESULT Initialize_Prototype(void) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(void);
public:
	virtual _bool Picking(_float4x4 WorldMatrix, _float3* vPickPoint = nullptr) { return true; };

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVB = nullptr;
	_float3*					m_pVerticesPos = nullptr;

protected:
	LPDIRECT3DINDEXBUFFER9 m_pIB = nullptr;

protected:
	VIBINFO m_tVIBInfo;

protected:
	HRESULT Ready_Vertex_Buffer(void);
	HRESULT Ready_Index_Buffer(void);

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free(void) override;
};

END