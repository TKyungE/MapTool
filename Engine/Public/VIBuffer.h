#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype(void) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(void);

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVB = nullptr;
	_uint m_iNumVertices = 0;
	_uint m_iStride = 0;
	_ulong m_dwFVF = 0;
	D3DPRIMITIVETYPE m_ePrimitiveType;
	_uint m_iNumPrimitive = 0;

protected:
	LPDIRECT3DINDEXBUFFER9 m_pIB = nullptr;
	_uint m_iIndicesByte = 0;
	D3DFORMAT m_eIndexFormat;

protected:
	HRESULT Ready_Vertex_Buffer(void);
	HRESULT Ready_Index_Buffer(void);

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free(void) override;
};

END