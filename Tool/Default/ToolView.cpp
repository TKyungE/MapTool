
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

	ON_WM_DESTROY()
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

	GRAPHIC_DESC Graphic_Desc;
	ZeroMemory(&Graphic_Desc, sizeof(GRAPHIC_DESC));

	Graphic_Desc.hWnd = g_hWnd;
	Graphic_Desc.iWinSizeX = g_iWinSizeX;
	Graphic_Desc.iWinSizeY = g_iWinSizeY;
	Graphic_Desc.eWinMode = GRAPHIC_DESC::MODE_WIN;

	if (FAILED(m_pGameInstance->Initialize_Engine(Graphic_Desc, &m_pGraphic_Device)))
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

	
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	m_pGameInstance->Render_Begin();

	m_pGameInstance->Render_End();
}

void CToolView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pGameInstance);

	CGameInstance::Release_Engine();
}


HRESULT CToolView::Ready_Prototype_Component(void)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;
	
	//Prototype_Component_VIBuffer_Terrain
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Component_VIBuffer_Terrain"), CVIBuffer_Terrain::Create(m_pGraphic_Device, 200, 200))))
		return E_FAIL;
	
	//Prototype_Component_Transform
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Component_Transform"), CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

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
