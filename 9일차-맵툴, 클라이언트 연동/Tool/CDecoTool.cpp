// CDecoTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CDecoTool.h"
#include "CFileInfo.h"


// CDecoTool 대화 상자

IMPLEMENT_DYNAMIC(CDecoTool, CDialog)

CDecoTool::CDecoTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CDecoTool, pParent), m_iDrawID(-1)
	, m_strIndex(_T(""))
{

}

CDecoTool::~CDecoTool()
{
}

void CDecoTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ListBox);
	DDX_Control(pDX, IDC_LIST1, m_ControllistBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Text(pDX, IDC_EDIT2, m_strIndex);
}


BEGIN_MESSAGE_MAP(CDecoTool, CDialog)
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_LIST2, &CDecoTool::OnListBox)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDecoTool::OnControlListBox)
	ON_BN_CLICKED(IDC_BUTTON7, &CDecoTool::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON9, &CDecoTool::OnBnClickedButton9)
END_MESSAGE_MAP()


// CDecoTool 메시지 처리기






void CDecoTool::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);

	TCHAR	szFilePath[MAX_PATH] = L"";
	int iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);
	TCHAR		szFileName[MAX_STR] = L"";


	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString strRelativePath = CFileInfo::ConvertRealtivePath(szFilePath);
		CString	strFileName = PathFindFileName(strRelativePath);

		lstrcpy(szFileName, strFileName.GetString());

		PathRemoveExtension(szFileName);

		strFileName = szFileName;

		auto iter = m_MapPngImage.find(strFileName);

		if (iter == m_MapPngImage.end())
		{
			CImage* pPngImg = new CImage;

			pPngImg->Load(strRelativePath);

			m_MapPngImage.insert({ strFileName, pPngImg });

			m_ListBox.AddString(strFileName);
		}
	}
	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}


void CDecoTool::OnDestroy()
{
	CDialog::OnDestroy();
	for_each(m_MapPngImage.begin(), m_MapPngImage.end(), [](auto& MyPair)
		{
			MyPair.second->Destroy();
			Safe_Delete(MyPair.second);
		});

	m_MapPngImage.clear();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}



void CDecoTool::OnListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString	strFindName = L"";

	int		iIndex = m_ListBox.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	m_ListBox.GetText(iIndex, strFindName);

	auto	iter = m_MapPngImage.find(strFindName);

	if (iter == m_MapPngImage.end())
		return;

	m_Picture.SetBitmap(*(iter->second));

	int i(0);

	for (; i < strFindName.GetLength(); ++i)
	{
		if (0 != isdigit(strFindName[i]))
			break;
	}

	strFindName.Delete(0, i);

	m_iDrawID = _tstoi(strFindName);

	UpdateData(FALSE);
}

void CDecoTool::Add_ControlList(wstring wstr)
{
	if (!m_ControllistBox.m_hWnd) return;

	UpdateData(TRUE);
	m_ControllistBox.AddString(wstr.c_str());
	UpdateData(FALSE);
}




void CDecoTool::OnControlListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iListIndex = m_ControllistBox.GetCurSel();
	CString CStrFind;
	m_ControllistBox.GetText(iListIndex, CStrFind);
	
	int pos = CStrFind.Find(L"-");
	if (pos != -1)
	{
		CString token = CStrFind.Mid(pos + 1);
		int iIndex = _tstoi(token);
		m_strIndex.Format(L"%d", iIndex);
	}
	UpdateData(FALSE);

}


void CDecoTool::OnBnClickedButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iListIndex = m_ControllistBox.GetCurSel();
	CString CStrFind;
	m_ControllistBox.GetText(iListIndex, CStrFind);

	int pos = CStrFind.Find(L"-");
	if (pos != -1)
	{
		CString token = CStrFind.Mid(pos + 1);
		int iIndex = _tstoi(token);
		vector<DECO*>& vecDeco = m_pDeco->Get_DecoVector();
		for (int i = 0; i < vecDeco.size(); ++i)
		{
			if (vecDeco[i]->byIndex == iIndex)
			{
				vecDeco.erase(vecDeco.begin() + i);
				m_ControllistBox.DeleteString(iListIndex);
				//창 업데이트
				if(m_pMainView) ((CView*)m_pMainView)->Invalidate(FALSE);

				if (iListIndex == m_ControllistBox.GetCount() - 1)
				{
					m_ControllistBox.SetCurSel(0);
				}
				else
				{
					m_ControllistBox.SetCurSel(iListIndex);
				}
				break;
			}
		}
	}
}


//Delete ALL
void CDecoTool::OnBnClickedButton9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_ControllistBox.ResetContent();
	vector<DECO*>& vecDeco = m_pDeco->Get_DecoVector();
	if (!vecDeco.empty())
	{
		for_each(vecDeco.begin(), vecDeco.end(), [](auto& pDeco)
			{
				if (pDeco != nullptr)
				{
					delete pDeco;
					pDeco = nullptr;
				}
			});
		vecDeco.clear();
	}
	if(m_pMainView) ((CView*)m_pMainView)->Invalidate(FALSE);
	UpdateData(FALSE);
}
