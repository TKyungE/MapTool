
// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once

#include "Tool_Defines.h"
#include "GameInstance.h"

class CToolDoc;
class CMyTerrain;
class CToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

private:
	HRESULT Ready_Prototype_Component(void);
	HRESULT SetUp_RenderState(void);
	HRESULT SetUp_SamplerState(void);
	
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
private:
	CGameInstance* m_pGameInstance = nullptr;
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;

	CRenderer* m_pRenderer = nullptr;
	
	bool m_bTrack = false;
	
	CString m_strObjectName = TEXT("");

	CString m_strTexFilePath = TEXT("");
	_uint m_iNumTex = 0;

public:
	typedef struct tagIndexPos
	{
		_float3 m_BackGroundPos;
		_float3 m_Scale;
		_uint m_iIndex;
		_uint m_iTurn;
		
	}INDEXPOS;

	typedef struct tagPos
	{
		_float3 m_vPlayerPos;
		vector<_float3> m_vMonsterPos;

		vector<INDEXPOS>	m_IndexPos;
		vector<INDEXPOS>	m_TreePos;
		vector<INDEXPOS>	m_HousePos;
		vector<INDEXPOS>	m_House2Pos;
		vector<INDEXPOS>	m_PortalPos;

		_uint		m_iMSize;
		_uint		m_IndexSize;
		_uint		m_TreeSize;
		_uint		m_HouseSize;
		_uint		m_House2Size;
		_uint		m_PortalSize;

	}SAVEPOS;

	
	SAVEPOS m_SavePos;

	INDEXPOS m_Index;
	_uint m_iBackIndex = 0;
	_uint m_iTreeIndex = 0;
	_uint m_iHouseIndex = 0;

	_bool m_bCheck = false;

	_bool m_bObjectCheck = false;

public:
	void Set_ObjectName(CString _strObjectName) { m_strObjectName = _strObjectName; }

	CString Get_TexFilePath(void) { return m_strTexFilePath; }
	_uint Get_NumTex(void) { return m_iNumTex; }

public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif
