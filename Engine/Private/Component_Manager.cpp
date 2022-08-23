#include "..\Public\Component_Manager.h"

IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{
}

HRESULT CComponent_Manager::Add_Prototype(const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr != Find_Component(pPrototypeTag))
		return E_FAIL;
	
	m_Prototypes.emplace(pPrototypeTag, pPrototype);

	return S_OK;
}

CComponent * CComponent_Manager::Clone_Component(const _tchar * pPrototypeTag, void * pArg)
{
	CComponent* pPrototype = Find_Component(pPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;
	
	CComponent* pComponent = pPrototype->Clone(pArg);
	if (nullptr == pComponent)
		return nullptr;	
	
	return pComponent;
}

void CComponent_Manager::Clear(void)
{
	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);
	
	m_Prototypes.clear();
}

CComponent * CComponent_Manager::Find_Component(const _tchar * pPrototypeTag)
{
	auto iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTag_Finder(pPrototypeTag));

	if (iter == m_Prototypes.end())
		return nullptr;
	
	return iter->second;
}

void CComponent_Manager::Free(void)
{
	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);
	
	m_Prototypes.clear();
}
