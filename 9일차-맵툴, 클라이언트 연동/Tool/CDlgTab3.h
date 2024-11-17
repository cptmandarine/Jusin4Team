#pragma once
#include "afxdialogex.h"


// CDlgTab3 대화 상자

class CDlgTab3 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTab3)

public:
	CDlgTab3(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgTab3();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
