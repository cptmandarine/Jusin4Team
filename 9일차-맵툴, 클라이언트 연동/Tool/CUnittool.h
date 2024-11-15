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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnListBox();
	afx_msg void OnAdd();
	afx_msg void OnDestroy();
	afx_msg void OnDelete();
	afx_msg void OnSearch();

public: // value
	CString m_strTest;
	CString m_strResult;

	CString m_strName;
	int m_iAttack;
	int m_iHp;

	CString m_strFindName;


public: // control
	CListBox m_ListBox;
	CButton m_Radio[3];
	CButton m_Check[3];
	CButton m_Bitmap;

public:
	map<CString, UNITDATA*>		m_mapUnitData;

	virtual BOOL OnInitDialog();
	afx_msg void OnSave();
	afx_msg void OnLoad();
};
