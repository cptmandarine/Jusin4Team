#pragma once
#include "afxdialogex.h"
#include "CDeco.h"

// CDecoTool 대화 상자
class CToolView;

class CDecoTool : public CDialog
{
	DECLARE_DYNAMIC(CDecoTool)

public:
	CDecoTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDecoTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDecoTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnControlListBox();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDestroy();
	afx_msg void OnListBox();
public:
	void Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }
	void Set_pDeco(CDeco* _pDeco) { m_pDeco = _pDeco; };
	void Add_ControlList(wstring _szElement);
public:
	//value
	CString m_strIndex;

	//control
	map<CString, CImage*>		m_MapPngImage;
	int							m_iDrawID;

	CDeco*	 m_pDeco;
	CListBox m_ListBox;
	CListBox m_ControllistBox;
	CStatic m_Picture;

public:
	CToolView* m_pMainView;
	afx_msg void OnBnClickedButton9();
};
