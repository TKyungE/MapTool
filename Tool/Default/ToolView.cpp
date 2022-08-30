
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd;
HINSTANCE g_hInst;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	Safe_AddRef(m_pGameInstance);
}

CToolView::~CToolView()
{
}

#pragma region ���ʿ�
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG
#pragma endregion ���ʿ�

// CToolView �޽��� ó����

void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerSpawn"), CPlayerSpawn::Create(m_pGraphic_Device))))
	{
		ERR_MSG(TEXT("Prototype_GameObject_PlayerSpawn Failed"));
		return;
	}

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MonsterSpawn"), CMonsterSpawn::Create(m_pGraphic_Device))))
	{
		ERR_MSG(TEXT("Prototype_GameObject_PlayerSpawn Failed"));
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

// CToolView �׸���

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
	
	//Prototype_Component_Transform
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Component_Transform"), CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.tga"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_PlayerSpawn*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Component_Texture_PlayerSpawn"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/UseSkill/Effect%d.png"), 3))))
		return E_FAIL;

	///*For.Prototype_Component_Texture_MonsterSpawn*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Component_Texture_MonsterSpawn"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/UseSkill/Effect%d.png"), 3))))
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

HRESULT CToolView::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (nullptr == m_pGameInstance || FAILED(m_pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Terrain"), pLayerTag, nullptr)))
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnMouseLeave();

	m_bTrack = false;

	RedrawWindow();
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnLButtonDown(nFlags, point);

}


void CToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnLButtonUp(nFlags, point);
	
	_tchar pStr[MAX_PATH] = TEXT("");
	memcpy(&pStr,m_strObjectName, sizeof(TCHAR) * MAX_PATH);
	
	//lstrcpy(pStr, m_strObjectName.GetString());

	if (pStr != TEXT(""))
	{
		if (nullptr == m_pGameInstance)
		{
			ERR_MSG(TEXT("Failed to Created"));
			return;
		}

		_tchar PrototypeTag[MAX_PATH] = TEXT("Prototype_GameObject_");
		_tchar LayerTag[MAX_PATH] = TEXT("Layer_");

		wsprintf(PrototypeTag, TEXT("%s%s"), PrototypeTag, pStr);
		wsprintf(LayerTag, TEXT("%s%s"), LayerTag, pStr);

		_float3 vPos = m_pGameInstance->Get_TargetPos();


 		auto iter = find_if(m_mapSpawn.begin(), m_mapSpawn.end(), CTag_Finder(TEXT("PlayerSpawn")));
		
		if (iter != m_mapSpawn.end())
		{
			CPlayerSpawn* PlayerSpawn = (CPlayerSpawn*)m_pGameInstance->Find_Object(TEXT("Layer_PlayerSpawn"),0);
			vPos.y += 0.01f;
			PlayerSpawn->Get_Transform()->Set_State(CTransform::STATE_POSITION, vPos);
			m_mapSpawn.insert({ pStr,vPos });
			return;
		}

		if (FAILED(m_pGameInstance->Add_GameObject(PrototypeTag, LayerTag, &vPos)))
		{
			ERR_MSG(TEXT("Failed to Cloned : CPlayerSpawn"));
			return;
		}
		m_mapSpawn.insert({ pStr,vPos });
	}
}
