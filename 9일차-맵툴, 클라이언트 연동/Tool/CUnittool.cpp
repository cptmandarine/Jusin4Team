// CUnittool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CUnittool.h"
#include "CFileInfo.h"


// CUnittool 대화 상자

IMPLEMENT_DYNAMIC(CUnittool, CDialog)

CUnittool::CUnittool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CUnittool, pParent)
	, m_strUnitName(_T(""))
	, m_iAttack(0)
	, m_iHp(0)
	, m_iDrawID(0)
{

}

CUnittool::~CUnittool()
{
}

void CUnittool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON1, m_BitMap);
	DDX_Text(pDX, IDC_EDIT3, m_strUnitName);
	DDX_Text(pDX, IDC_EDIT4, m_iAttack);
	DDX_Text(pDX, IDC_EDIT5, m_iHp);
	DDX_Control(pDX, IDC_LIST3, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CUnittool, CDialog)
	
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON2, &CUnittool::OnSave)
	ON_BN_CLICKED(IDC_BUTTON3, &CUnittool::OnLoad)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST3, &CUnittool::OnListBox)
END_MESSAGE_MAP()


void CUnittool::OnDestroy()
{
	CDialog::OnDestroy();

	for_each(m_mapUnitData.begin(), m_mapUnitData.end(), [](auto& MyPair)
	{
		if (MyPair.second)
		{
			delete MyPair.second;
			MyPair.second = nullptr;
		}
	});

	m_mapUnitData.clear();
}


BOOL CUnittool::OnInitDialog()
{
	CDialog::OnInitDialog();

	HBITMAP	hBitmap = (HBITMAP)LoadImage(nullptr, 
		L"../Texture/JusinLogo1.bmp", 
		IMAGE_BITMAP, 170, 150, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_BitMap.SetBitmap(hBitmap);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}





void CUnittool::OnSave()
{
	// CFileDialog : 파일 열기 혹은 저장 작업에 필요한 대화 상자를 생성하는 객체

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

		DWORD	dwByte(0);
		DWORD	dwStrCnt(0);

		for (auto& MyPair : m_mapUnitData)
		{
			// key 값 저장
			dwStrCnt = sizeof(TCHAR) * (MyPair.first.GetLength() + 1);

			WriteFile(hFile, &dwStrCnt, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, MyPair.first.GetString(), dwStrCnt, &dwByte, nullptr);

			// value값 저장
			WriteFile(hFile, &(MyPair.second->byItem),		sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->byJobIndex),	sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->iAttack),		sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->iHp),			sizeof(int), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
}


void CUnittool::OnLoad()
{
	UpdateData(TRUE);

	CFileDialog	Dlg(TRUE,	// TRUE(불러오기), FALSE(다른 이름으로 저장)
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
		for (auto& MyPair : m_mapUnitData)
			delete MyPair.second;

		m_mapUnitData.clear();

		// ResetContent : 리스트 박스 목록 초기화 함수
		//m_ListBox.ResetContent();

		CString	str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath,
			GENERIC_READ,
			0, 0,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
			0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte(0);
		DWORD	dwStrCnt(0);
		UNITDATA	tData{};

		while (true)
		{
			// key
			ReadFile(hFile, &dwStrCnt, sizeof(DWORD), &dwByte, nullptr);

			TCHAR* pName = new TCHAR[dwStrCnt];
			ReadFile(hFile, pName, dwStrCnt, &dwByte, nullptr);

			// value
			ReadFile(hFile, &(tData.byItem), sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &(tData.byJobIndex), sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iAttack), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iHp), sizeof(int), &dwByte, nullptr);

			if (0 == dwByte)
			{
				delete[] pName;
				break;
			}

			UNITDATA* pUnit = new UNITDATA;
			pUnit->strName = pName;
			
			delete[]pName;
			pName = nullptr;

			pUnit->byItem = tData.byItem;
			pUnit->byJobIndex = tData.byJobIndex;
			pUnit->iAttack = tData.iAttack;
			pUnit->iHp = tData.iHp;

			m_mapUnitData.insert({ pUnit->strName, pUnit });

			//m_ListBox.AddString(pUnit->strName);
		}
		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}


void CUnittool::OnDropFiles(HDROP hDropInfo)
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


void CUnittool::OnListBox()
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
