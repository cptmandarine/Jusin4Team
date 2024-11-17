#pragma once


#include "CUnittool.h"
#include "CMapTool.h"
#include "CPathFinder.h"
#include "CDlgTab1.h"
#include "CDlgTab2.h"
#include "CDlgTab3.h"

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();
public:
	CDlgTab1* Get_Dig1() { return m_dlg1; };
	CDlgTab2* Get_Dig2() { return m_dlg2; };
	CDlgTab3* Get_Dig3() { return m_dlg3; };

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMyForm };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


private:
	CFont		m_Font;

public:
	CTabCtrl m_tab;

	/*CUnittool	m_UnitTool;
	CPathFinder m_PathFinder;*/

	CDlgTab1* m_dlg1;
	CDlgTab2* m_dlg2;
	CDlgTab3* m_dlg3;


public:
	virtual void OnInitialUpdate();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
};


