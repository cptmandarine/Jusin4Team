﻿// CMyForm.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "CMyForm.h"

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_CMyForm)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyForm::OnSelchangeTab1)
END_MESSAGE_MAP()


// CMyForm 진단

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


// CMyForm 메시지 처리기


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CRect rect;
	m_tab.GetWindowRect(rect);
	m_tab.InsertItem(0, L"MapTool");
	m_tab.InsertItem(1, L"ObjectTool");
	m_tab.InsertItem(2, L"PathFinder");
	m_tab.SetCurSel(0);

	m_dlg1 = new CDlgTab1;
	if(nullptr == m_dlg1->GetSafeHwnd())
		m_dlg1->Create(IDD_DIALOG_TAB1, &m_tab);

	m_dlg1->MoveWindow(10, 40, rect.Width(), rect.Height());
	m_dlg1->ShowWindow(SW_SHOW);

	m_dlg2 = new CDlgTab2;
	if (nullptr == m_dlg2->GetSafeHwnd())
		m_dlg2->Create(IDD_DIALOG_TAB2, &m_tab);

	m_dlg2->MoveWindow(10, 40, rect.Width(), rect.Height());
	m_dlg2->ShowWindow(SW_HIDE);

	m_dlg3 = new CDlgTab3;
	if (nullptr == m_dlg3->GetSafeHwnd())
		m_dlg3->Create(IDD_DIALOG_TAB3, &m_tab);

	m_dlg3->MoveWindow(10, 40, rect.Width(), rect.Height());
	m_dlg3->ShowWindow(SW_HIDE);

	
}


void CMyForm::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (IDC_TAB1 == pNMHDR->idFrom)
	{
		int iSelect = m_tab.GetCurSel();
		switch (iSelect)
		{
		case 0:
			m_dlg1->ShowWindow(SW_SHOW);
			m_dlg2->ShowWindow(SW_HIDE);
			m_dlg3->ShowWindow(SW_HIDE);
			break;
		case 1:
			m_dlg1->ShowWindow(SW_HIDE);
			m_dlg2->ShowWindow(SW_SHOW);
			m_dlg3->ShowWindow(SW_HIDE);
			break;
		case 2:
			m_dlg1->ShowWindow(SW_HIDE);
			m_dlg2->ShowWindow(SW_HIDE);
			m_dlg3->ShowWindow(SW_SHOW);
			break;
		default:
			break;
		}
	}
	*pResult = 0;
}
