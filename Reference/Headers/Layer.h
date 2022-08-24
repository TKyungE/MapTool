#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Initialize(void);
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	void Tick();
private:
	list<class CGameObject*> m_GameObjects;
	typedef list<class CGameObject*> GAMEOBJECTS;

public:
	static CLayer* Create(void);
	virtual void Free(void) override;
};

END