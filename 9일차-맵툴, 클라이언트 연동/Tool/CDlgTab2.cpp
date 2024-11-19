// CDlgTab2.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CDlgTab2.h"


// CDlgTab2 대화 상자

IMPLEMENT_DYNAMIC(CDlgTab2, CDialogEx)

CDlgTab2::CDlgTab2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TAB2, pParent)
{

}

CDlgTab2::~CDlgTab2()
{
}

void CDlgTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTab2, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTab2::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgTab2 메시지 처리기


void CDlgTab2::OnBnClickedButton1()
{
	if (nullptr == m_CUnitTool.GetSafeHwnd())
	{
		m_CUnitTool.Create(IDD_CUnittool);
	}
	m_CUnitTool.ShowWindow(SW_SHOW);
}
