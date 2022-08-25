// MyForm.cpp : ���� �����Դϴ�.
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


// CMyForm �����Դϴ�.

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


// CMyForm �޽��� ó�����Դϴ�.

void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_Slider.SetRange(0, 100);
	m_Slider.SetPos(0);

	CString	strPos;
	strPos.Format(L"%d", m_Slider.GetPos());
	m_Edit_Value.SetWindowText(strPos);

}


void CMyForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CFormView::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString strPos;

	m_Edit_Value.GetWindowText(strPos);
	int iPos = _ttoi(strPos);

	m_Slider.SetPos(iPos);
}








