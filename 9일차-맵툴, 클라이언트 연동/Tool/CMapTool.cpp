// CMapTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CMapTool.h"
#include "CFileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "CTerrain.h"

// CMapTool 대화 상자

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CMapTool, pParent), m_iDrawID(0)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnListBox)
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnSave)
END_MESSAGE_MAP()



void CMapTool::OnListBox()
{
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
		// isdigit : 매개 변수로 전달받은 글자가 글자 형태의 문자인지 숫자 형태의 문자인지 구별하는 함수
		// 숫자 형태의 글자라 판명될 경우 0이 아닌 값을 리턴

		if (0 != isdigit(strFindName[i]))
			break;
	}

	strFindName.Delete(0, i);

	m_iDrawID = _tstoi(strFindName);

	UpdateData(FALSE);
}


void CMapTool::OnDropFiles(HDROP hDropInfo)
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

		lstrcpy(szFileName,strFileName.GetString());

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

	Horizontal_Scroll();

	UpdateData(FALSE);
}

void CMapTool::Horizontal_Scroll()
{
	CString		strName;
	CSize		Size;

	int			iWidth(0);

	CDC* pDC = m_ListBox.GetDC();

	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);

		// GetTextExtent : 문자열의 길이를 픽셀 단위로 변환
		Size = pDC->GetTextExtent(strName);

		if (Size.cx > iWidth)
			iWidth = Size.cx;
	}

	m_ListBox.ReleaseDC(pDC);

	// GetHorizontalExtent : 리스트 박스가 가로로 스크롤 할 수 있는 최대 범위를 얻어오는 함수
	if (iWidth > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iWidth);
}


void CMapTool::OnDestroy()
{
	CDialog::OnDestroy();

	for_each(m_MapPngImage.begin(), m_MapPngImage.end(), [](auto& MyPair)
		{
			MyPair.second->Destroy();
			Safe_Delete(MyPair.second);
		});
	
	m_MapPngImage.clear();
}


void CMapTool::OnSave()
{
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
		CTerrain* pMyTerrain = pMainView->m_pTerrain;
		vector<TILE*>& vecTile = pMyTerrain->Get_VecTile();

		if (vecTile.empty())
			return;

		DWORD	dwByte(0);

		for (auto& pTile : vecTile)
			WriteFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

		CloseHandle(hFile);
	}
}
