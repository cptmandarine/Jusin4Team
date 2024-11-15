#pragma once


#include "CUnittool.h"
#include "CMapTool.h"
#include "CPathFinder.h"
class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

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
	CUnittool	m_UnitTool;
	CMapTool	m_MapTool;
	CPathFinder m_PathFinder;

public:
	virtual void OnInitialUpdate();
	afx_msg void OnUnittool();
	afx_msg void OnMapTool();
	afx_msg void OnPathFinder();
};


