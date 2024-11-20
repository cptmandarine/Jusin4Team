// CDecoTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CDecoTool.h"
#include "CFileInfo.h"
#include "ToolView.h"
#include "MainFrm.h"

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
	ON_BN_CLICKED(IDC_BUTTON1, &CDecoTool::OnBnClickedButton1)
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
	if (!m_ControllistBox.m_hWnd)
	{
		return;
	}

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


//데코 저장쓰
void CDecoTool::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog	Dlg(FALSE,	// TRUE(불러오기), FALSE(다른 이름으로 저장)
		L"dat",	// Default 파일 확장자명	
		L"*.dat", // 대화 상자에 표시될 최초의 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(읽기 전용 체크박스 숨김), OFN_OVERWRITEPROMPT(중복 파일 저장 시, 경고 메세지 띄움)
		L"Data File(*.dat)|*.dat||",  // 대화 상자에 표시될 파일 형식 "콤보 박스에 출력될 문자열 | 실제 사용할 필터링 문자열"
		this);	// 부모 윈도우 주소

	TCHAR	szPath[MAX_PATH] = L"";

	// GetCurrentDirectory : 현재 프로젝트의 경로를 얻어오는 함수
	GetCurrentDirectory(MAX_PATH, szPath);

	// PathRemoveFileSpec : 전체 경로 중 파일 이름만 잘라내는 함수
	// 경로 상 파일 이름이 없는 상태라면 그중 맨 마지막 폴더명을 잘라냄
	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;


	if (IDOK == Dlg.DoModal())
	{
		// GetPathName : 선택된 경로를 반환
		// GetString : 원시 버퍼 형태의 자료값을 반환하는 함수
		CString	str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath,
			GENERIC_WRITE,
			0, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
			0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CToolView* pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
		CDeco* pMyDeco = pMainView->m_pDeco;
		vector<DECO*>& vecDeco = pMyDeco->Get_DecoVector();

		if (vecDeco.empty())
			return;

		DWORD	dwByte(0);

		for (auto& pDeco : vecDeco)
			WriteFile(hFile, pDeco, sizeof(DECO), &dwByte, NULL);

		CloseHandle(hFile);
	}
}
