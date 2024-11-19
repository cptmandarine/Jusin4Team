// CDlgTab3.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CDlgTab3.h"


// CDlgTab3 대화 상자

IMPLEMENT_DYNAMIC(CDlgTab3, CDialogEx)

CDlgTab3::CDlgTab3(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TAB3, pParent)
{

}

CDlgTab3::~CDlgTab3()
{
}

void CDlgTab3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTab3, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTab3::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgTab3 메시지 처리기


void CDlgTab3::OnBnClickedButton1()
{
	if (nullptr == m_PathFinder.GetSafeHwnd())
	{
		m_PathFinder.Create(IDD_CPathFinder);
	}
	m_PathFinder.ShowWindow(SW_SHOW);
}
