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


public:
	virtual void OnInitialUpdate();

public:
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();

private:
	typedef struct tag;
public:

	CEdit m_Edit_Slider;
	CSliderCtrl m_Slider_Value;

	afx_msg void OnEdit_Value();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	VIBINFO m_tVIBInfo;

	CEdit m_Edit_VerticesX;
	CEdit m_Edit_VerticesZ;
	afx_msg void OnCreateButton();
	CStatic m_StaticX;
	CStatic m_StaticY;
	CStatic m_StaticZ;
};


