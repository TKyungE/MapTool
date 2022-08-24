#include "..\Public\Layer.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Initialize(void)
{
	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;
	
	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

void CLayer::Tick()
{
	for (auto& iter = m_GameObjects.begin();
		iter != m_GameObjects.end(); ++iter)
		(*iter)->Tick();
	
}

CLayer * CLayer::Create(void)
{
	CLayer* pInstance = new CLayer();

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLayer::Free(void)
{
	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);
	
	m_GameObjects.clear();
}
