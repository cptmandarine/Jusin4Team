// CBackGroundTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CBackGroundTool.h"
#include "CMiniView.h"
#include "CDevice.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "CTerrain.h"

// CBackGroundTool 대화 상자

IMPLEMENT_DYNAMIC(CBackGroundTool, CDialog)

CBackGroundTool::CBackGroundTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CBackGroundTool, pParent)
{

}

CBackGroundTool::~CBackGroundTool()
{
}

void CBackGroundTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);
	DDX_Control(pDX, IDC_RADIO4, m_Radio[3]);
	DDX_Control(pDX, IDC_RADIO5, m_Radio[4]);
	DDX_Control(pDX, IDC_RADIO6, m_Radio[5]);

	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}

void CBackGroundTool::Change_BackGround()
{
	int iIdx = 0;
	for (int i = 0; i < 6; ++i)
	{
		if (m_Radio[i].GetCheck())
		{
			iIdx = i;
		}
	}
	//이미지 크기를 조정해버림...
	CRect pictureRect;
	m_Picture.GetClientRect(&pictureRect);

	CClientDC dc(&m_Picture);
	m_vecBGImg[iIdx]->StretchBlt(dc.m_hDC, 0, 0,
		pictureRect.Width(), pictureRect.Height(),
		0, 0, m_vecBGImg[iIdx]->GetWidth(), m_vecBGImg[iIdx]->GetHeight());
	//아래거 적용하면 크기가 적용이 안됨
	//m_Picture.SetBitmap(*(m_vecBGImg[iIdx]));
}


BEGIN_MESSAGE_MAP(CBackGroundTool, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CBackGroundTool::OnBnClickedButton1)
END_MESSAGE_MAP()


// CBackGroundTool 메시지 처리기


BOOL CBackGroundTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_vecBGImg.reserve(6);
	for (int i = 0; i < 6; ++i)
	{
		TCHAR szBuf[MAX_PATH];
		swprintf_s(szBuf, L"../Texture/Map/Map%d.png", i);
		CImage* pPngImg = new CImage;
		HRESULT hr = pPngImg->Load(szBuf);
		if (SUCCEEDED(hr))
		{
			m_vecBGImg.push_back(pPngImg);
		}
		else
		{
			delete pPngImg;
			pPngImg = nullptr;
			AfxMessageBox(L"error Insert BG IMG");
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CBackGroundTool::OnDestroy()
{
	CDialog::OnDestroy();

	for (int i = 0; i < m_vecBGImg.size(); ++i)
	{
		m_vecBGImg[i]->Destroy();
		delete (m_vecBGImg[i]);
		m_vecBGImg[i] = nullptr;
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CBackGroundTool::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_RADIO1:
	case IDC_RADIO2:
	case IDC_RADIO3:
	case IDC_RADIO4:
	case IDC_RADIO5:
	case IDC_RADIO6:
		Change_BackGround();
		break;
	}
	return CDialog::OnCommand(wParam, lParam);
}


void CBackGroundTool::OnBnClickedButton1()
{

	//terrain을 가져와서 
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	CTerrain* pTerrain = pMainView->m_pTerrain;
	int iIdx = 0;
	for (int i = 0; i < 6; ++i)
	{
		if (m_Radio[i].GetCheck())
		{
			iIdx = i;
		}
	}
	pTerrain->Set_BGID(iIdx);
	pMainView->Invalidate(FALSE);
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
	pMiniView->Invalidate(FALSE);
}
