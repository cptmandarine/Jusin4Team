#pragma once
#include "afxdialogex.h"
#include "Include.h"

// CPathFinder 대화 상자

class CPathFinder : public CDialog
{
	DECLARE_DYNAMIC(CPathFinder)

public:
	CPathFinder(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CPathFinder();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CPathFinder };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnListBox();
	afx_msg void OnDestroy();
	afx_msg void OnDropFiles(HDROP hDropInfo);

public:
	CListBox m_ListBox;
	list<IMGPATH*> m_PathInfoList;
};
