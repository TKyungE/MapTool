// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, Edit_VALUE, m_Edit_Value);
	DDX_Control(pDX, SLIDER_VALUE, m_Slider);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_EN_CHANGE(Edit_VALUE, &CMyForm::OnValue)

	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.

void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_Slider.SetRange(0, 100);
	m_Slider.SetPos(0);

	CString	strPos;
	strPos.Format(L"%d", m_Slider.GetPos());
	m_Edit_Value.SetWindowText(strPos);

}


void CMyForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);

	if (SLIDER_VALUE == pScrollBar->GetDlgCtrlID())
	{
		CString	strPos;
		strPos.Format(L"%d", m_Slider.GetPos());
		SetDlgItemInt(Edit_VALUE,m_Slider.GetPos(),0);
	}

	

}


void CMyForm::OnValue()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strPos;

	m_Edit_Value.GetWindowText(strPos);
	int iPos = _ttoi(strPos);

	m_Slider.SetPos(iPos);
}








