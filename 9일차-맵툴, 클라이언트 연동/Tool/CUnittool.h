#pragma once
#include "afxdialogex.h"
#include "Include.h"
// CUnittool 대화 상자

class CUnittool : public CDialog
{
	DECLARE_DYNAMIC(CUnittool)

public:
	CUnittool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CUnittool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUnittool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSave();
	afx_msg void OnLoad();
	afx_msg void OnListBox();

public: // value
	CString m_strUnitName;
	int m_iAttack;
	int m_iHp;


public: // control
	CButton m_BitMap;
	CListBox m_ListBox;
	CStatic m_Picture;

public:
	int m_iDrawID;
	map<CString, CImage*>		m_MapPngImage;
	map<CString, UNITDATA*>		m_mapUnitData;
};
