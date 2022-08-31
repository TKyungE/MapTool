// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

#include "GameInstance.h"
#include "MyTerrain.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "PlayerSpawn.h"
#include "Transform.h"

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
	DDX_Control(pDX, IDC_EDIT2, m_Edit_VerticesX);
	DDX_Control(pDX, IDC_EDIT3, m_Edit_VerticesZ);
	DDX_Control(pDX, IDC_MouseX, m_StaticX);
	DDX_Control(pDX, IDC_MouseY, m_StaticY);
	DDX_Control(pDX, IDC_MouseZ, m_StaticZ);
	DDX_Control(pDX, IDC_OBJECTLIST, m_ObejctListBox);
	DDX_Control(pDX, IDC_CHECK1, m_ResetX);
	DDX_Control(pDX, IDC_EDIT4, m_EditIndex);
	DDX_Control(pDX, IDC_SPIN1, m_SpinIndex);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_SAVE, &CMyForm::OnSaveData)
	ON_BN_CLICKED(IDC_LOAD, &CMyForm::OnLoadData)
	ON_EN_CHANGE(IDC_EDIT1, &CMyForm::OnEdit_Value)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnCreateButton)
	ON_LBN_SELCHANGE(IDC_OBJECTLIST, &CMyForm::OnListBox)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnObjectSaveButton)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyForm::OnObjectLoadButton)
	ON_BN_CLICKED(IDC_CHECK1, &CMyForm::OnResetXButton)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CMyForm::OnSpinIndex)
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
		m_Slider_Value.SetRange(0, 100);
		m_Slider_Value.SetPos(0);

		m_Slider_Value.SetTicFreq(1);

	CString	strPos;
	strPos.Format(L"%d", m_Slider_Value.GetPos());
	m_Edit_Slider.SetWindowText(strPos);

	UpdateData(TRUE);

	m_ObejctListBox.AddString(TEXT("PlayerSpawn"));
	m_ObejctListBox.AddString(TEXT("MonsterSpawn"));
	m_ObejctListBox.AddString(TEXT("BackGround"));

	m_EditIndex.SetWindowText(TEXT("0"));
	m_SpinIndex.SetRange(0, 100);
	m_SpinIndex.SetPos(0);
	UpdateData(FALSE);
}

void CMyForm::OnSaveData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.


	CString strPos;

	m_Edit_Slider.GetWindowText(strPos);
	int iPos = _ttoi(strPos);

	m_Slider_Value.SetPos(iPos);

	float fPos = _float(_wtof(strPos));

	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);

	CMyTerrain* pTerrain = dynamic_cast<CMyTerrain*>(pInstance->Find_Object(TEXT("Layer_BackGround"), 0));

	if (nullptr == pTerrain)
		return;

	pTerrain->Set_Value(fPos);

	Safe_Release(pInstance);
}




void CMyForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
	if (IDC_SLIDER1 == pScrollBar->GetDlgCtrlID())
	{
		CString	strPos, strEditPos;
		strPos.Format(L"%d", m_Slider_Value.GetPos());

		float fPos = _float(_wtof(strPos));
		fPos /= 10.f;

		strEditPos.Format(L"%f", fPos);

		strEditPos.TrimRight('0');
		strEditPos.TrimRight('.');

		m_Edit_Slider.SetWindowText(strEditPos);

		int iPos = _ttoi(strPos);

		m_Slider_Value.SetPos(iPos);
		
		CGameInstance* pInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pInstance);

		CMyTerrain* pTerrain = dynamic_cast<CMyTerrain*>(pInstance->Find_Object(TEXT("Layer_BackGround"), 0));

		if (nullptr == pTerrain)
		return;

		pTerrain->Set_Value(fPos);

		Safe_Release(pInstance);
	}
	
}


void CMyForm::OnCreateButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);


	/*if (FAILED(pInstance->Add_GameObject(TEXT("Prototype_GameObject_Terrain"), TEXT("Layer_BackGround"), nullptr)))
		return;*/



	CMyTerrain* pTerrain = dynamic_cast<CMyTerrain*>(pInstance->Find_Object(TEXT("Layer_BackGround"), 0));
	if (nullptr == pTerrain)
	{
		ERR_MSG(TEXT("Failed to Load"));
		return;
	}

	LPDIRECT3DVERTEXBUFFER9 SaveVB = pTerrain->Get_VIBufferCom()->Get_VB();
	LPDIRECT3DINDEXBUFFER9 SaveIB = pTerrain->Get_VIBufferCom()->Get_IB();


	CVIBuffer::VIBINFO SaveVIBInfo;
	CVIBuffer_Terrain::VIBINFO_DERIVED SaveVIBInfo_Derived;
	ZeroMemory(&SaveVIBInfo, sizeof(CVIBuffer::VIBINFO));
	ZeroMemory(&SaveVIBInfo_Derived, sizeof(CVIBuffer_Terrain::VIBINFO_DERIVED));

	SaveVIBInfo = pTerrain->Get_VIBufferCom()->Get_VIBInfo();
	SaveVIBInfo_Derived = pTerrain->Get_VIBufferCom()->Get_VIBInfoDerived();


	pTerrain->Get_VIBufferCom()->Release_Buffer();

	//================================================================

	CVIBuffer::VIBINFO VIBInfo1;
	CVIBuffer_Terrain::VIBINFO_DERIVED VIBInfo_Derived1;
	ZeroMemory(&VIBInfo1, sizeof(CVIBuffer::VIBINFO));
	ZeroMemory(&VIBInfo_Derived1, sizeof(CVIBuffer_Terrain::VIBINFO_DERIVED));


	CString strVerticesX, strVerticesZ;
	m_Edit_VerticesX.GetWindowText(strVerticesX);
	m_Edit_VerticesZ.GetWindowText(strVerticesZ);

	int iVerticesX = _ttoi(strVerticesX);
	int iVerticesZ = _ttoi(strVerticesZ);


	VIBInfo1.m_iNumVertices = iVerticesX * iVerticesZ;
	VIBInfo1.m_iStride = sizeof(VTXTEX);
	VIBInfo1.m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	VIBInfo1.m_ePrimitiveType = D3DPT_TRIANGLELIST;
	VIBInfo1.m_iNumPrimitive = (iVerticesX - 1) * (iVerticesZ - 1) * 2;
	VIBInfo1.m_iIndicesByte = sizeof(FACEINDICES32);
	VIBInfo1.m_eIndexFormat = D3DFMT_INDEX32;

	VIBInfo_Derived1.m_iNumVerticesX = iVerticesX;
	VIBInfo_Derived1.m_iNumVerticesZ = iVerticesZ;


	pTerrain->Get_VIBufferCom()->Set_VIBInfo(VIBInfo1);
	pTerrain->Get_VIBufferCom()->Set_VIBInfoDerived(VIBInfo_Derived1);

	if (FAILED(pTerrain->Get_VIBufferCom()->Load_Terrain()))
	{
		ERR_MSG(TEXT("Failed to Load Terrain"));
		return;
	}

	CMyTerrain* pTerrain1 = dynamic_cast<CMyTerrain*>(pInstance->Find_Object(TEXT("Layer_BackGround"), 0));
	if (nullptr == pTerrain1)
	{
		ERR_MSG(TEXT("Failed to Load"));
		return;
	}

	LPDIRECT3DVERTEXBUFFER9 VB = pTerrain1->Get_VIBufferCom()->Get_VB();
	LPDIRECT3DINDEXBUFFER9 IB = pTerrain1->Get_VIBufferCom()->Get_IB();

	VTXTEX* pVertices = nullptr;
	VTXTEX* pSaveVertices = nullptr;

	FACEINDICES32* pIndices = nullptr;
	FACEINDICES32* pSaveIndices = nullptr;

	VB->Lock(0, 0, (void**)&pVertices, 0);
	SaveVB->Lock(0, 0, (void**)&pSaveVertices, 0);

	_uint i = 0;

	_uint NumZ = 0;
	_uint NumX = 0;

	if (SaveVIBInfo_Derived.m_iNumVerticesZ > VIBInfo_Derived1.m_iNumVerticesZ)
	{
		NumZ = SaveVIBInfo_Derived.m_iNumVerticesZ - VIBInfo_Derived1.m_iNumVerticesZ;
	}
	if (SaveVIBInfo_Derived.m_iNumVerticesX > VIBInfo_Derived1.m_iNumVerticesX)
	{
		NumX = SaveVIBInfo_Derived.m_iNumVerticesX - VIBInfo_Derived1.m_iNumVerticesX;
	}

	for (i; i < SaveVIBInfo_Derived.m_iNumVerticesZ - NumZ; ++i)
	{
		_uint j = 0;
		for (j; j < SaveVIBInfo_Derived.m_iNumVerticesX - NumX; ++j)
		{
			_uint iIndex = i *  VIBInfo_Derived1.m_iNumVerticesX + j;
			_uint iSaveIndex = i * SaveVIBInfo_Derived.m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = pSaveVertices[iSaveIndex].vPosition;
			pVertices[iIndex].vTexture = pSaveVertices[iSaveIndex].vTexture;
		}
		for (j; j < VIBInfo_Derived1.m_iNumVerticesX; ++j)
		{
			_uint iIndex = i *  VIBInfo_Derived1.m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = pVertices[iIndex].vPosition = _float3((_float)j, 0.f, (_float)i);
			pVertices[iIndex].vTexture = pVertices[iIndex].vTexture = _float2((_float)j, (_float)i);
		}
	}
	for (i; i < VIBInfo_Derived1.m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < VIBInfo_Derived1.m_iNumVerticesX; ++j)
		{
			_uint iIndex = i *  VIBInfo_Derived1.m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = pVertices[iIndex].vPosition = _float3((_float)j, 0.f, (_float)i);
			pVertices[iIndex].vTexture = pVertices[iIndex].vTexture = _float2((_float)j, (_float)i);
		}
	}

	VB->Unlock();
	SaveVB->Unlock();

	IB->Lock(0, 0, (void**)&pIndices, 0);

	_uint iNumFaces = 0;

	for (_uint i = 0; i < VIBInfo_Derived1.m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < VIBInfo_Derived1.m_iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * VIBInfo_Derived1.m_iNumVerticesX + j;

			_uint iIndices[4] = {
				iIndex + VIBInfo_Derived1.m_iNumVerticesX,
				iIndex + VIBInfo_Derived1.m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumFaces]._0 = iIndices[0];
			pIndices[iNumFaces]._1 = iIndices[1];
			pIndices[iNumFaces]._2 = iIndices[2];
			++iNumFaces;

			pIndices[iNumFaces]._0 = iIndices[0];
			pIndices[iNumFaces]._1 = iIndices[2];
			pIndices[iNumFaces]._2 = iIndices[3];
			++iNumFaces;
		}
	}

	IB->Unlock();

	Safe_Release(pInstance);
}


void CMyForm::OnListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CString strFindName;

	int iSelect = m_ObejctListBox.GetCurSel();
	
	if (-1 == iSelect)
		return;
	
	m_ObejctListBox.GetText(iSelect, strFindName);

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	pToolView->Set_ObjectName(strFindName);

	if (strFindName == TEXT("BackGround"))
	{
		SetDlgItemInt(IDC_STATIC1, pToolView->m_iIndex);
	}
	else
	{
		SetDlgItemInt(IDC_STATIC1, 0);
		SetDlgItemInt(IDC_EDIT4, 0);
		m_iIndex1 = 0;
	}

	UpdateData(FALSE);
}


void CMyForm::OnObjectSaveButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
		CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

		for (auto& iter : pToolView->m_SavePos.m_vMonsterPos)
			++pToolView->m_SavePos.m_iMSize;
		for (auto& iter : pToolView->m_SavePos.m_IndexPos)
			++pToolView->m_SavePos.m_IndexSize;
		

			WriteFile(hFile, pToolView->m_SavePos.m_vPlayerPos, sizeof(_float3), &dwByte, nullptr);
			WriteFile(hFile, &pToolView->m_SavePos.m_iMSize, sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, &pToolView->m_SavePos.m_IndexSize, sizeof(_uint), &dwByte, nullptr);

			
			for (auto& iter : pToolView->m_SavePos.m_vMonsterPos)
				WriteFile(hFile, iter, sizeof(_float3), &dwByte, nullptr);
			
			for (auto& iter : pToolView->m_SavePos.m_IndexPos)
			{
				WriteFile(hFile, iter.m_BackGroundPos, sizeof(_float3), &dwByte, nullptr);
				WriteFile(hFile, &iter.m_iIndex, sizeof(_uint), &dwByte, nullptr);
			}

		CloseHandle(hFile);
	}

}


void CMyForm::OnObjectLoadButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMyForm::OnResetXButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	UpdateData(FALSE);
}


void CMyForm::OnSpinIndex(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	UpdateData(TRUE);

	CString strFindName;

	int iSelect = m_ObejctListBox.GetCurSel();

	if (-1 == iSelect)
		return;

	m_ObejctListBox.GetText(iSelect, strFindName);


	if (strFindName == TEXT("BackGround"))
	{
		if (pNMUpDown->iDelta > 0)
		{
			if (m_iIndex1 >= GetDlgItemInt(IDC_STATIC1))
				return;

			++m_iIndex1;
		}
		else
		{
			if (m_iIndex1 <= 0)
				return;

			--m_iIndex1;
			
		}
		SetDlgItemInt(IDC_EDIT4, m_iIndex1);
	}
	*pResult = 0;

	UpdateData(FALSE);
}



