// MyForm.cpp : ���� �����Դϴ�.
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
	DDX_Control(pDX, IDC_TILELIST, m_TileList);
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
	ON_LBN_SELCHANGE(IDC_TILELIST, &CMyForm::OnSelectTile)
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

		m_Slider_Value.SetTicFreq(1);

	CString	strPos;
	strPos.Format(L"%d", m_Slider_Value.GetPos());
	m_Edit_Slider.SetWindowText(strPos);

	UpdateData(TRUE);

	m_ObejctListBox.AddString(TEXT("PlayerSpawn"));
	m_ObejctListBox.AddString(TEXT("MonsterSpawn"));

	CGameInstance* pInstance = CGameInstance::Get_Instance();
	if (nullptr == pInstance)
		return;

	Safe_AddRef(pInstance);

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	CString strTexFilePath = pToolView->Get_TexFilePath();
	_uint iNumTex = pToolView->Get_NumTex();
	
	for (_uint i = 0; i < iNumTex; ++i)
	{
		CString strInt;
		strInt.Format(TEXT("%d"), i);

		m_TileList.AddString(strTexFilePath + strInt);
	}

	Safe_Release(pInstance);

	UpdateData(FALSE);
}

void CMyForm::OnSaveData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CFileDialog Dlg(FALSE, TEXT("dat"), TEXT("*.dat"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("Data File(*.dat) | *.dat||"), this);

	TCHAR szPath[MAX_PATH] = TEXT("");
	GetCurrentDirectory(MAX_PATH, szPath);

	PathRemoveFileSpec(szPath);

	lstrcat(szPath, TEXT("\\Data\\Terrain"));

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

#pragma region TerrainRect
		_int TerrainRectSize = pInstance->Get_LayerSize(TEXT("Layer_TerrainRect"));
		CString strSize;
		strSize.Format(TEXT("%d"), TerrainRectSize);

		_tchar szSize[MAX_PATH];
		_tcscpy_s(szSize, MAX_PATH, strSize.GetBuffer(0));

		WriteFile(hFile, &(szSize), sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);

		for (_int i = 0; i < TerrainRectSize; ++i)
		{
			CTerrainRect* pObject = (CTerrainRect*)pInstance->Find_Object(TEXT("Layer_TerrainRect"), i);

			WriteFile(hFile, &pObject->Get_RectInfo(), sizeof(CTerrainRect::RECTINFO), &dwByte, nullptr);

			LPDIRECT3DVERTEXBUFFER9 VB = pObject->Get_VB();

			VTXTEX* pVertices = nullptr;

			VB->Lock(0, 0, (void**)&pVertices, 0);

			for (_uint i = 0; i < 4; ++i)
			{
				WriteFile(hFile, &pVertices[i].vPosition, sizeof(_float3), &dwByte, nullptr);
				WriteFile(hFile, &pVertices[i].vTexture, sizeof(_float2), &dwByte, nullptr);
			}

			VB->Unlock();
		}
#pragma endregion TerrainRect
#pragma region Terrain
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
#pragma endregion Terrain

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

	lstrcat(szPath, TEXT("\\Data\\Terrain"));

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

#pragma region TerrainRect
		_tchar szSize[MAX_PATH];
		ReadFile(hFile, &szSize, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);
		_int TerrainRectSize = _wtoi(szSize);

		for (_int i = 0; i < TerrainRectSize; ++i)
		{
			CTerrainRect::RECTINFO tRectInfo;

			ReadFile(hFile, &tRectInfo, sizeof(tRectInfo), &dwByte, nullptr);

			if (FAILED(pInstance->Add_GameObject(TEXT("Prototype_GameObject_TerrainRect"), TEXT("Layer_TerrainRect"), &tRectInfo)))
			{
				ERR_MSG(TEXT("Failed to Cloned : CTerrainRect"));
				return;
			}

			CTerrainRect* pObject = (CTerrainRect*)pInstance->Find_Object(TEXT("Layer_TerrainRect"), i);

			LPDIRECT3DVERTEXBUFFER9 VB = pObject->Get_VB();

			VTXTEX* pVertices = nullptr;

			VB->Lock(0, 0, (void**)&pVertices, 0);

			for (_uint i = 0; i < 4; ++i)
			{
				_float3 vPos;
				_float2 vTex;

				ReadFile(hFile, &vPos, sizeof(_float3), &dwByte, nullptr);
				ReadFile(hFile, &vTex, sizeof(_float2), &dwByte, nullptr);

				pVertices[i].vPosition = vPos;
				pVertices[i].vTexture = vTex;
			}

			VB->Unlock();
		}
#pragma endregion TerrainRect
#pragma region Terrain
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
#pragma endregion Terrain
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	CString strFindName;

	int iSelect = m_ObejctListBox.GetCurSel();
	
	if (-1 == iSelect)
		return;
	
	m_ObejctListBox.GetText(iSelect, strFindName);

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	pToolView->Set_ObjectName(strFindName);

	UpdateData(FALSE);
}


void CMyForm::OnObjectSaveButton()
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
		CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

			WriteFile(hFile, pToolView->m_SavePos.m_vPlayerPos, sizeof(_float3), &dwByte, nullptr);
			
			for (auto& iter : pToolView->m_SavePos.m_vMonsterPos)
				WriteFile(hFile, iter, sizeof(_float3), &dwByte, nullptr);

		CloseHandle(hFile);
	}

}


void CMyForm::OnObjectLoadButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMyForm::OnResetXButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	UpdateData(FALSE);
}




void CMyForm::OnSelectTile()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	int iSelect = m_TileList.GetCurSel();

	if (-1 == iSelect)
		return;

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));


	UpdateData(FALSE);
}
