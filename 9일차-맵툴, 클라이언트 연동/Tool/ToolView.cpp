
// ToolView.cpp: CToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "CMiniView.h"
#include "CMyForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HWND		g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// 스크로 바 생성
	// SetScrollSizes : 스크롤 바의 사이즈를 지정하는 cscrollView 멤버 함수
	// MM_TEXT : 픽셀 단위로 조정
	// CSize : mfc 클래스

	SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX, TILEY * TILECY / 2));

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWnd{};

	// GetWindowRect : 현재 윈도우의 RECT 정보를 얻어오는 함수
	pMainFrm->GetWindowRect(&rcWnd);

	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);
	// rcWnd.right = 창의 가로 길이
	// rcWnd.bottom = 창의 세로 길이

	RECT		rcMainView{};

	//GetClientRect : 현재 창의 view 창의 렉트 정보만 얻어오는 함수
	GetClientRect(&rcMainView);

	float	fRowFrm = float(rcWnd.right - rcMainView.right);
	float	fColFrm = float(rcWnd.bottom - rcMainView.bottom);


	// SetWindowPos : 매개 변수대로 새롭게 윈도우 위치와 크기를 조정하는 함수
	// SetWindowPos(배치할 윈도우의 Z순서에 대한 포인터, X좌표, Y좌표, 가로 크기, 세로 크기, 배치할 윈도우의 크기 및 위치 지정 옵션)

	pMainFrm->SetWindowPos(nullptr, // 순서 변경 안하겠음
		0, 0,
		int(WINCX + fRowFrm), int(WINCY + fColFrm),
		SWP_NOZORDER); // 현재 순서를 유지하는 옵션


	g_hWnd = m_hWnd;

	if (FAILED(CDevice::Get_Instance()->Init_Device()))
	{
		AfxMessageBox(L"Device Create Failed");
		return;
	}
	m_pTerrain = new CTerrain;
	m_pTerrain->Initialize();
	m_pTerrain->Set_MainView(this);

	m_pDeco = new CDeco;
	m_pDeco->Initialize();
	m_pDeco->Set_MainView(this);

	m_pUnit = new CUnit;
	m_pUnit->Initialize();
	m_pUnit->Set_MainView(this);
}




void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDevice::Get_Instance()->Render_Begin();

	m_pTerrain->Render();
	m_pDeco->Render();
	m_pUnit->Render();

	CDevice::Get_Instance()->Render_End();

}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{

	CScrollView::OnLButtonDown(nFlags, point);
}



void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnMouseMove(nFlags, point);

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));

		CMapTool* pMapTool = &(pMyForm->Get_Dig1()->m_MapTool);
		CDecoTool* pDecoTool = &(pMyForm->Get_Dig1()->m_DecoTool);
		CUnittool* pUnitTool = &(pMyForm->Get_Dig2()->m_CUnitTool);
		


		pDecoTool->Set_MainView(this);
		pDecoTool->Set_pDeco(m_pDeco);
		
		pUnitTool->Set_MainView(this);
		pUnitTool->Set_pUnit(m_pUnit);

		m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x + GetScrollPos(0)),
			float(point.y + GetScrollPos(1)),
			0.f),
			pMapTool->m_iDrawID);
		//m_pTerrain에서 타일의 인덱스를 불러와서
		//m_pDeco로 전달..
		int iIdx = m_pTerrain->Get_TileIndex(
			D3DXVECTOR3(
				float(point.x + GetScrollPos(0)),
				float(point.y + GetScrollPos(1)),
				0.f));
		D3DXVECTOR3 vDecoPos = m_pTerrain->Get_VecPos(iIdx);

		if (pMyForm->Get_Dig1()->IsWindowVisible())
		{
			if (m_pDeco->Create_Deco(vDecoPos, pDecoTool->m_iDrawID, iIdx))
			{
				TCHAR szBuf[MAX_STR];
				swprintf_s(szBuf, L"DECO%d-%d", pDecoTool->m_iDrawID, iIdx);
				pDecoTool->Add_ControlList(szBuf);
			}
		}
		if (pMyForm->Get_Dig2()->IsWindowVisible())
		{
			m_pUnit->Create_Unit(vDecoPos, pUnitTool->m_iDrawID, iIdx);
		}

		Invalidate(FALSE);

		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

		pMiniView->Invalidate(FALSE);
	}

}


void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();

	Safe_Delete(m_pTerrain);
	Safe_Delete(m_pDeco);
	CTextureMgr::Get_Instance()->Destroy_Instance();
	CDevice::Get_Instance()->Destroy_Instance();
}

#pragma region 생략
// CToolView 인쇄
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 그리기

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
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG



#pragma endregion








