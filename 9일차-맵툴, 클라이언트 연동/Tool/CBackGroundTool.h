#pragma once
#include "afxdialogex.h"


// CBackGroundTool 대화 상자

class CBackGroundTool : public CDialog
{
	DECLARE_DYNAMIC(CBackGroundTool)

public:
	CBackGroundTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBackGroundTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBackGroundTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	void Change_BackGround();
public:
	CButton m_Radio[6];
	CStatic m_Picture;
private:
	vector<CImage*> m_vecBGImg;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButton1();
};
