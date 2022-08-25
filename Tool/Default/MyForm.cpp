// MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

#include "GameInstance.h"
#include "MyTerrain.h"

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
	DDX_Control(pDX, IDC_EDIT1, m_Edit_Slider);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider_Value);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_SAVE, &CMyForm::OnSaveData)
	ON_BN_CLICKED(IDC_LOAD, &CMyForm::OnLoadData)
	ON_EN_CHANGE(IDC_EDIT1, &CMyForm::OnEdit_Value)
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
		m_Slider_Value.SetRange(0, 100);
		m_Slider_Value.SetPos(0);

	CString	strPos;
	strPos.Format(L"%d", m_Slider_Value.GetPos());
	m_Edit_Slider.SetWindowText(strPos);
}














void CMyForm::OnSaveData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CFileDialog Dlg(FALSE, TEXT("dat"), TEXT("*.dat"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("Data File(*.dat) | *.dat||"), this);

	TCHAR szPath[MAX_PATH] = TEXT("");
	GetCurrentDirectory(MAX_PATH, szPath);

	PathRemoveFileSpec(szPath);

	lstrcat(szPath, TEXT("\\Data"));

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		CString str = Dlg.GetPathName();

		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;

		CGameInstance* pInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pInstance);

		CMyTerrain* pTerrain = dynamic_cast<CMyTerrain*>(pInstance->Find_Object(TEXT("Layer_BackGround"), 0));
		if (nullptr == pTerrain)
		{
			ERR_MSG(TEXT("Failed to Save"));
			return;
		}

		WriteFile(hFile, &pTerrain->Get_VIBufferCom()->Get_VIBInfo(), sizeof(CVIBuffer::VIBINFO), &dwByte, nullptr);
		WriteFile(hFile, &pTerrain->Get_VIBufferCom()->Get_VIBInfoDerived(), sizeof(CVIBuffer_Terrain::VIBINFO_DERIVED), &dwByte, nullptr);

		LPDIRECT3DVERTEXBUFFER9 VB = pTerrain->Get_VIBufferCom()->Get_VB();
		LPDIRECT3DINDEXBUFFER9 IB = pTerrain->Get_VIBufferCom()->Get_IB();

		VTXTEX* pVertices = nullptr;
		FACEINDICES32* pIndices = nullptr;

		VB->Lock(0, 0, (void**)&pVertices, 0);

		for (_uint i = 0; i < pTerrain->Get_VIBufferCom()->Get_VIBInfo().m_iNumVertices; ++i)
		{
			WriteFile(hFile, &pVertices[i].vPosition, sizeof(_float3), &dwByte, nullptr);
			WriteFile(hFile, &pVertices[i].vTexture, sizeof(_float2), &dwByte, nullptr);
		}

		VB->Unlock();

		IB->Lock(0, 0, (void**)&pIndices, 0);

		//WriteFile(hFile, pIndices, sizeof(FACEINDICES32), &dwByte, nullptr);
		for (_uint i = 0; i < pTerrain->Get_VIBufferCom()->Get_VIBInfo().m_iNumPrimitive; ++i)
		{
			WriteFile(hFile, &pIndices[i]._0, sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, &pIndices[i]._1, sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, &pIndices[i]._2, sizeof(_uint), &dwByte, nullptr);
		}

		IB->Unlock();

		Safe_Release(pInstance);

		CloseHandle(hFile);
	}

}


void CMyForm::OnLoadData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(TRUE, TEXT("dat"), TEXT("*.dat"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("Data File(*.dat) | *.dat||"), this);

	TCHAR szPath[MAX_PATH] = TEXT("");
	GetCurrentDirectory(MAX_PATH, szPath);

	PathRemoveFileSpec(szPath);

	lstrcat(szPath, TEXT("\\Data"));

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		CString str = Dlg.GetPathName();

		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;
		
		DWORD dwByte = 0;

		CGameInstance* pInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pInstance);

		CMyTerrain* pTerrain = dynamic_cast<CMyTerrain*>(pInstance->Find_Object(TEXT("Layer_BackGround"), 0));
		if (nullptr == pTerrain)
		{
			ERR_MSG(TEXT("Failed to Load"));
			return;
		}

		CVIBuffer::VIBINFO VIBInfo;
		CVIBuffer_Terrain::VIBINFO_DERIVED VIBInfo_Derived;
		ZeroMemory(&VIBInfo, sizeof(CVIBuffer::VIBINFO));
		ZeroMemory(&VIBInfo_Derived, sizeof(CVIBuffer_Terrain::VIBINFO_DERIVED));

		ReadFile(hFile, &VIBInfo, sizeof(CVIBuffer::VIBINFO), &dwByte, nullptr);
		ReadFile(hFile, &VIBInfo_Derived, sizeof(CVIBuffer_Terrain::VIBINFO_DERIVED), &dwByte, nullptr);

		pTerrain->Get_VIBufferCom()->Release_Buffer();

		pTerrain->Get_VIBufferCom()->Set_VIBInfo(VIBInfo);
		pTerrain->Get_VIBufferCom()->Set_VIBInfoDerived(VIBInfo_Derived);

		if (FAILED(pTerrain->Get_VIBufferCom()->Load_Terrain()))
		{
			ERR_MSG(TEXT("Failed to Load Terrain"));
			return;
		}

		LPDIRECT3DVERTEXBUFFER9 VB = pTerrain->Get_VIBufferCom()->Get_VB();
		LPDIRECT3DINDEXBUFFER9 IB = pTerrain->Get_VIBufferCom()->Get_IB();

		VTXTEX* pVertices = nullptr;
		FACEINDICES32* pIndices = nullptr;

		VB->Lock(0, 0, (void**)&pVertices, 0);

		for (_uint i = 0; i < pTerrain->Get_VIBufferCom()->Get_VIBInfo().m_iNumVertices; ++i)
		{
			_float3 vPos;
			_float2 vTex;
			ReadFile(hFile, vPos, sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, vTex, sizeof(_float2), &dwByte, nullptr);

			pVertices[i].vPosition = vPos;
			pVertices[i].vTexture = vTex;
		}
		
		VB->Unlock();

		IB->Lock(0, 0, (void**)&pIndices, 0);

		for (_uint i = 0; i < pTerrain->Get_VIBufferCom()->Get_VIBInfo().m_iNumPrimitive; ++i)
		{
			ReadFile(hFile, &pIndices[i]._0, sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &pIndices[i]._1, sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &pIndices[i]._2, sizeof(_uint), &dwByte, nullptr);
		}

		IB->Unlock();

		Safe_Release(pInstance);

		CloseHandle(hFile);
	}

	
}


void CMyForm::OnEdit_Value()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CFormView::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.


	CString strPos;

	m_Edit_Slider.GetWindowText(strPos);
	int iPos = _ttoi(strPos);

	m_Slider_Value.SetPos(iPos);





	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);

	CMyTerrain* pTerrain = dynamic_cast<CMyTerrain*>(pInstance->Find_Object(TEXT("Layer_BackGround"), 0));

	pTerrain->Set_Value(iPos);

	Safe_Release(pInstance);
}




void CMyForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
	if (IDC_SLIDER1 == pScrollBar->GetDlgCtrlID())
	{
		CString	strPos;
		strPos.Format(L"%d", m_Slider_Value.GetPos());
		m_Edit_Slider.SetWindowText(strPos);

		int iPos = _ttoi(strPos);

		m_Slider_Value.SetPos(iPos);

		CGameInstance* pInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pInstance);

		CMyTerrain* pTerrain = dynamic_cast<CMyTerrain*>(pInstance->Find_Object(TEXT("Layer_BackGround"), 0));

		pTerrain->Set_Value(iPos);

		Safe_Release(pInstance);
	}
	
}
