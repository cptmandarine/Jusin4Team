#pragma once
#include "afxdialogex.h"


// CDlgTab2 대화 상자

class CDlgTab2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTab2)

public:
	CDlgTab2(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgTab2();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
