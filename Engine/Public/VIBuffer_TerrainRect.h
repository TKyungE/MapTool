#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_TerrainRect final : public CVIBuffer
{
private:
	CVIBuffer_TerrainRect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_TerrainRect(const CVIBuffer_TerrainRect& rhs);
	virtual ~CVIBuffer_TerrainRect() = default;

public:
	virtual HRESULT Initialize_Prototype(void) override;
	virtual HRESULT Initialize(void* pArg) override;

	_float3 Get_VertexArray(_uint iIndex) { return m_VertexPosArray[iIndex]; }

private:
	_float3 m_VertexPosArray[4] = {};

public:
	static CVIBuffer_TerrainRect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free(void) override;
};

END