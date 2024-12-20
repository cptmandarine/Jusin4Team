﻿// CDlgTab1.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CDlgTab1.h"


// CDlgTab1 대화 상자

IMPLEMENT_DYNAMIC(CDlgTab1, CDialogEx)

CDlgTab1::CDlgTab1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TAB1, pParent)
{

}

CDlgTab1::~CDlgTab1()
{
}

void CDlgTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTab1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTab1::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgTab1::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgTab1::OnBnClickedButton3)
END_MESSAGE_MAP()


// CDlgTab1 메시지 처리기


void CDlgTab1::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_MapTool.GetSafeHwnd())
	{
		m_MapTool.Create(IDD_CMapTool);
	}
	m_MapTool.ShowWindow(SW_SHOW);
}


void CDlgTab1::OnBnClickedButton2()
{
	if (nullptr == m_BGTool.GetSafeHwnd())
	{
		m_BGTool.Create(IDD_CBackGroundTool);
	}
	m_BGTool.ShowWindow(SW_SHOW);
}


void CDlgTab1::OnBnClickedButton3()
{

	if (nullptr == m_DecoTool.GetSafeHwnd())
	{
		m_DecoTool.Create(IDD_CDecoTool);
	}
	m_DecoTool.ShowWindow(SW_SHOW);
}
