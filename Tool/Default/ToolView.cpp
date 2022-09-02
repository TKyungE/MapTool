
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "MyTerrain.h"
#include "Camera_Dynamic.h"
#include "PlayerSpawn.h"
#include "MyForm.h"
#include "MonsterSpawn.h"
#include "BackGruondObj.h"
#include "TerrainRect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd;
HINSTANCE g_hInst;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
	// TODO: 여기에 생성 코드를 추가합니다.
	Safe_AddRef(m_pGameInstance);
}

CToolView::~CToolView()
{
}

#pragma region 불필요
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG
#pragma endregion 불필요

// CToolView 메시지 처리기

void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT rcWnd{};

	pMainFrm->GetWindowRect(&rcWnd);

	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT rcView{};

	GetClientRect(&rcView);

	float fRowFrm = float(rcWnd.right - rcView.right);
	float fColFrm = float(rcWnd.bottom - rcView.bottom);

	pMainFrm->SetWindowPos(nullptr, 0, 0, int(g_iWinSizeX + fRowFrm), int(g_iWinSizeY + fColFrm), SWP_NOZORDER);

	g_hWnd = m_hWnd;
	g_hInst = AfxGetInstanceHandle();
	GRAPHIC_DESC Graphic_Desc;
	ZeroMemory(&Graphic_Desc, sizeof(GRAPHIC_DESC));

	Graphic_Desc.hWnd = g_hWnd;
	Graphic_Desc.iWinSizeX = g_iWinSizeX;
	Graphic_Desc.iWinSizeY = g_iWinSizeY;
	Graphic_Desc.eWinMode = GRAPHIC_DESC::MODE_WIN;

	ZeroMemory(&m_SavePos.m_vPlayerPos, sizeof(_float3));

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst,Graphic_Desc, &m_pGraphic_Device)))
	{
		ERR_MSG(TEXT("Initialize_Engine Failed"));
		return;
	}

	if (FAILED(SetUp_SamplerState()))
	{
		ERR_MSG(TEXT("SetUp_SamplerState Failed"));
		return;
	}
		
	if (FAILED(SetUp_RenderState()))
	{
		ERR_MSG(TEXT("SetUp_RenderState Failed"));
		return;
	}

	if (FAILED(Ready_Prototype_Component()))
	{
		ERR_MSG(TEXT("Ready_Prototype_Component Failed"));
		return;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"), CMyTerrain::Create(m_pGraphic_Device))))
	{
		ERR_MSG(TEXT("Prototype_GameObject_Terrain Failed"));
		return;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TerrainRect"), CTerrainRect::Create(m_pGraphic_Device))))
	{
		ERR_MSG(TEXT("Prototype_GameObject_TerrainRect Failed"));
		return;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerSpawn"), CPlayerSpawn::Create(m_pGraphic_Device))))
	{
		ERR_MSG(TEXT("Prototype_GameObject_PlayerSpawn Failed"));
		return;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MonsterSpawn"), CMonsterSpawn::Create(m_pGraphic_Device))))
	{
		ERR_MSG(TEXT("Prototype_GameObject_MonsterSpawn Failed"));
		return;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGroundObj::Create(m_pGraphic_Device))))
	{
		ERR_MSG(TEXT("Prototype_GameObject_BackGround Failed"));
		return;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"),
		CCamera_Dynamic::Create(m_pGraphic_Device))))
		return;
		
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	{
		ERR_MSG(TEXT("Ready_Layer_BackGround Failed"));
		return;
	}

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
	{
		ERR_MSG(TEXT("Ready_Layer_Camera Failed"));
		return;
	}
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CMyForm*		pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_MainSplitter.GetPane(0, 0));

	CMyTerrain* pTerrain1 = dynamic_cast<CMyTerrain*>(m_pGameInstance->Find_Object(TEXT("Layer_BackGround"), 0));
	if (pMyForm->m_ObejctListBox.GetCurSel() != -1)
		pTerrain1->Set_Check(true);
	else
		pTerrain1->Set_Check(false);

	m_pGameInstance->Tick_Engine();

	m_pGameInstance->Render_Begin();

	m_pRenderer->Render_GameObjects();

	m_pGameInstance->Render_End();


	if (m_bTrack)
	{
		Invalidate(FALSE);
	}	
}
	

void CToolView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Safe_Release(m_pRenderer);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pGameInstance);

	CGameInstance::Release_Engine();
}


HRESULT CToolView::Ready_Prototype_Component(void)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	//Prototype_Component_Renderer
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Component_Renderer"), m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	//Prototype_Component_VIBuffer_Terrain
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Component_VIBuffer_Terrain"), CVIBuffer_Terrain::Create(m_pGraphic_Device, 2, 2))))
		return E_FAIL;
	
	/*For.Prototype_Component_VIBuffer_Rect*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Component_VIBuffer_TerrainRect"), CVIBuffer_TerrainRect::Create(m_pGraphic_Device))))
		return E_FAIL;

	//Prototype_Component_Transform
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Component_Transform"), CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/OBJ/OBJ/MAP/LookMap/Map%d.png"), 10))))
		return E_FAIL;

	m_strTexFilePath = TEXT("Map");
	m_iNumTex = 10;

	/*For.Prototype_Component_Texture_PlayerSpawn*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Component_Texture_PlayerSpawn"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/UseSkill/Effect%d.png"), 3))))
		return E_FAIL;

	///*For.Prototype_Component_Texture_MonsterSpawn*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Component_Texture_MonsterSpawn"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/UseSkill/Effect%d.png"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Terrain/grass_%d.png"), m_iIndex = 2))))
		return E_FAIL;

	m_iIndex -= 1;

	return S_OK;
}

HRESULT CToolView::SetUp_RenderState(void)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);

	return S_OK;
}

HRESULT CToolView::SetUp_SamplerState(void)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CToolView::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Terrain"), pLayerTag, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CToolView::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera_Dynamic::CAMERADESC_DERIVED				CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera_Dynamic::CAMERADESC_DERIVED));

	CameraDesc.CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	CameraDesc.CameraDesc.vAt = _float3(0.f, 0.f, 0.f);

	CameraDesc.CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.CameraDesc.fNear = 0.2f;
	CameraDesc.CameraDesc.fFar = 500.f;

	CameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}


void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMouseMove(nFlags, point);

	if (m_bTrack == false)
	{
		TRACKMOUSEEVENT MouseEvent;
		ZeroMemory(&MouseEvent, sizeof(TRACKMOUSEEVENT));

		MouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		MouseEvent.dwFlags = TME_LEAVE;
		MouseEvent.hwndTrack = g_hWnd;
		MouseEvent.dwHoverTime = 0;


		m_bTrack = _TrackMouseEvent(&MouseEvent);

		RedrawWindow();
	}
}


void CToolView::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMouseLeave();

	m_bTrack = false;

	RedrawWindow();
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);
}


void CToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonUp(nFlags, point);

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CMyForm*		pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_MainSplitter.GetPane(0, 0));

	const _tchar* pstr = (_tchar*)(LPCTSTR)m_strObjectName;

	if (pMyForm->m_ObejctListBox.GetCurSel() != -1)
	{
		if (nullptr == m_pGameInstance)
		{
			ERR_MSG(TEXT("Failed to Created"));
			return;
		}

		_tchar PrototypeTag[MAX_PATH] = TEXT("Prototype_GameObject_");
		_tchar LayerTag[MAX_PATH] = TEXT("Layer_");

		wsprintf(PrototypeTag, TEXT("%s%s"), PrototypeTag, pstr);
		wsprintf(LayerTag, TEXT("%s%s"), LayerTag, pstr);

		_float3 vPos = m_pGameInstance->Get_TargetPos();


		if (!lstrcmp(pstr, TEXT("PlayerSpawn")))
		{
			if (m_bObjectCheck == false)
				m_bObjectCheck = true;

			else if (m_bObjectCheck)
			{
				CPlayerSpawn* PlayerSpawn = (CPlayerSpawn*)m_pGameInstance->Find_Object(TEXT("Layer_PlayerSpawn"), 0);
				vPos.y += 0.01f;
				PlayerSpawn->Get_Transform()->Set_State(CTransform::STATE_POSITION, vPos);
				m_SavePos.m_vPlayerPos = vPos;

				if (!pMyForm->m_ResetX.GetCheck())
					pMyForm->m_ObejctListBox.SetCurSel(-1);

				return;
			}

		}
		if (!lstrcmp(pstr, TEXT("MonsterSpawn")))
		{
			if (FAILED(m_pGameInstance->Add_GameObject(PrototypeTag, TEXT("Layer_MonsterSpawn"), &vPos)))
			{
				ERR_MSG(TEXT("Failed to Cloned : MonsterSpawn"));
				return;
			}
		}
		else if(!lstrcmp(pstr, TEXT("PlayerSpawn")))
		{
			if (FAILED(m_pGameInstance->Add_GameObject(PrototypeTag, TEXT("Layer_PlayerSpawn"), &vPos)))
			{
				ERR_MSG(TEXT("Failed to Cloned : PlayerSpawn"));
				return;
			}
		}
		else if (!lstrcmp(pstr, TEXT("BackGround")))
		{
			m_Index.m_iIndex = pMyForm->m_iIndex1;
			m_Index.m_BackGroundPos = vPos;

			if (FAILED(m_pGameInstance->Add_GameObject(PrototypeTag, TEXT("Layer_BackGround"), &m_Index)))
			{
				ERR_MSG(TEXT("Failed to Cloned : PlayerSpawn"));
				return;
			}
		}

		if (!lstrcmp(pstr, TEXT("PlayerSpawn")))
			m_SavePos.m_vPlayerPos = vPos;

		else if(!lstrcmp(pstr, TEXT("MonsterSpawn")))
			m_SavePos.m_vMonsterPos.push_back(vPos);

		else if (!lstrcmp(pstr, TEXT("BackGround")))
			m_SavePos.m_IndexPos.push_back(m_Index);
		

		if (!pMyForm->m_ResetX.GetCheck())
			pMyForm->m_ObejctListBox.SetCurSel(-1);
	}

	if (pMyForm->m_TileList.GetCurSel() != -1)
	{
		if (nullptr == m_pGameInstance)
		{
			ERR_MSG(TEXT("Failed to Created"));
			return;
		}

		_float3 mPoint = m_pGameInstance->Get_TargetPos();
		CMyTerrain* pTerrain = (CMyTerrain*)m_pGameInstance->Find_Object(TEXT("Layer_BackGround"), 0);
		_uint iIndex = pTerrain->Get_VIBufferCom()->Get_VIBInfoDerived().m_iNumVerticesX * (_uint)mPoint.z + (_uint)mPoint.x;
		_uint iIndices[4] = {
			iIndex + pTerrain->Get_VIBufferCom()->Get_VIBInfoDerived().m_iNumVerticesX,
			iIndex + pTerrain->Get_VIBufferCom()->Get_VIBInfoDerived().m_iNumVerticesX + 1,
			iIndex + 1,
			iIndex
		};

		LPDIRECT3DVERTEXBUFFER9 VB = pTerrain->Get_VIBufferCom()->Get_VB();
		LPDIRECT3DINDEXBUFFER9 IB = pTerrain->Get_VIBufferCom()->Get_IB();

		VTXTEX* pVertices = nullptr;
		FACEINDICES32* pIndices = nullptr;

		VB->Lock(0, 0, (void**)&pVertices, 0);
		IB->Lock(0, 0, (void**)&pIndices, 0);

		CTerrainRect::RECTINFO m_tRectInfo;
		m_tRectInfo.vPos = pVertices[iIndex].vPosition;
		m_tRectInfo.iTex = pMyForm->m_TileList.GetCurSel();

		VB->Unlock();
		IB->Unlock();

		_uint LayerSize = m_pGameInstance->Get_LayerSize(TEXT("Layer_TerrainRect"));

		for (_uint i = 0; i < LayerSize; ++i)
		{
			CTerrainRect* pObject = (CTerrainRect*)m_pGameInstance->Find_Object(TEXT("Layer_TerrainRect"), i);

			CTransform* pTransform = (CTransform*)pObject->Find_Component(TEXT("Com_Transform"));

			_float3 ObjectWorldPos = *D3DXVec3TransformCoord(&ObjectWorldPos, &_float3(0.f, 0.f, 0.f), &pTransform->Get_WorldMatrix());

			if (pVertices[iIndex].vPosition.x == ObjectWorldPos.x && pVertices[iIndex].vPosition.z == ObjectWorldPos.z)
			{
				pObject->Set_RectTex(m_tRectInfo.iTex);
				return;
			}
		}

		if (FAILED(m_pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_TerrainRect"), TEXT("Layer_TerrainRect"), (CTerrainRect::RECTINFO*)&m_tRectInfo)))
		{
			ERR_MSG(TEXT("Failed to Cloned : TerrainRect"));
			return;
		}
	}
}
