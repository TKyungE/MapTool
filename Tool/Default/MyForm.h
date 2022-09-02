#pragma once

#include "afxwin.h"
#include "afxcmn.h"

// CMyForm 폼 뷰입니다.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

public:
	typedef struct tagVIBInfo
	{
		unsigned int iNumVer;
		unsigned int iStride;
		unsigned long dwFVF;
		D3DPRIMITIVETYPE ePrimitiveType;
		unsigned int iNumPrimitive;

		unsigned int iIndicesByte;
		D3DFORMAT eIndexFormat;
	}VIBINFO;

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCEc
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	float m_fSpin_Value = 0.f;
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;
public:
	virtual void OnInitialUpdate();

public:
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();
	afx_msg void OnCreateButton();
	afx_msg void OnEdit_Value();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnListBox();

public:
	CEdit m_Edit_Slider;
	CSliderCtrl m_Slider_Value;

	VIBINFO m_tVIBInfo;

	CEdit m_Edit_VerticesX;
	CEdit m_Edit_VerticesZ;
	
	CStatic m_StaticX;
	CStatic m_StaticY;
	CStatic m_StaticZ;

	CListBox m_ObejctListBox;
	afx_msg void OnObjectSaveButton();
	afx_msg void OnObjectLoadButton();
	
	CButton m_ResetX;
	afx_msg void OnResetXButton();

	CEdit m_EditIndex;
	int	m_iIndex1 = 0;
	CSpinButtonCtrl m_SpinIndex;
	afx_msg void OnSpinIndex(NMHDR *pNMHDR, LRESULT *pResult);

	
	afx_msg void OnSelectTile();

	CEdit m_EditSizeX;
	CEdit m_EditSizeY;
	CEdit m_EditSizeZ;

	afx_msg void OnScaleButton();
	CListBox m_TileList;
	CEdit m_EditTrun;
};


