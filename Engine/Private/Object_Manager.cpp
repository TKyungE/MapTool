#include "..\Public\Object_Manager.h"
#include "Layer.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

HRESULT CObject_Manager::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr != Find_Prototype(pPrototypeTag))
		return E_FAIL;
	
	m_Prototypes.emplace(pPrototypeTag, pPrototype);
	
	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject(const _tchar * pPrototypeTag, const _tchar * pLayerTag, void * pArg)
{
	CGameObject* pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;
	
	CGameObject* pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;
	
	CLayer* pLayer = Find_Layer(pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		pLayer->Add_GameObject(pGameObject);

		m_Layers.emplace(pLayerTag, pLayer);
	}
	else
		pLayer->Add_GameObject(pGameObject);

	return S_OK;
}

void CObject_Manager::Tick(void)
{
	for (auto& Pair : m_Layers)
		Pair.second->Tick();
}

void CObject_Manager::Clear(void)
{
	for (auto& Pair : m_Layers)
		Safe_Release(Pair.second);
	
	m_Layers.clear();
}

CGameObject * CObject_Manager::Find_Object(const _tchar * pLayerTag, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(pLayerTag);
	if (nullptr == pLayer)
		return nullptr;
	
 	CGameObject* pGameObject = pLayer->Find_GameObject(iIndex);
	if (nullptr == pGameObject)
		return nullptr;

	return pGameObject;
}

CLayer * CObject_Manager::Find_Layer(const _tchar * pLayerTag)
{
	auto iter = find_if(m_Layers.begin(), m_Layers.end(), CTag_Finder(pLayerTag));

	if (iter == m_Layers.end())
		return nullptr;

	return iter->second;
}

CGameObject * CObject_Manager::Find_Prototype(const _tchar * pPrototypeTag)
{
	auto iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTag_Finder(pPrototypeTag));

	if (iter == m_Prototypes.end())
		return nullptr;
	
	return iter->second;
}

void CObject_Manager::Free(void)
{
	for (auto& Pair : m_Layers)
		Safe_Release(Pair.second);
	
	m_Layers.clear();

	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);
	
	m_Prototypes.clear();
}
