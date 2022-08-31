#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

class CTerrainRect final : public CGameObject
{
public:
	typedef struct tagRectInfo
	{
		_float3 vPos;
		_float3 vUp;
		_uint iTex;
	}RECTINFO;

private:
	CTerrainRect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTerrainRect(const CTerrainRect& rhs);
	virtual ~CTerrainRect() = default;

public:
	void Set_RectInfo(RECTINFO& tInfo) { memcpy(&m_tInfo, &tInfo, sizeof(RECTINFO)); }

public:
	virtual HRESULT Initialize_Prototype(void) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(void) override;
	virtual HRESULT Render(void) override;

private:
	CRenderer* m_pRendererCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;

private:
	HRESULT SetUp_Components(void);

private:
	RECTINFO m_tInfo;

public:
	static CTerrainRect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;
};

