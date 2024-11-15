// CPathFinder.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CPathFinder.h"
#include "CFileInfo.h"


// CPathFinder 대화 상자

IMPLEMENT_DYNAMIC(CPathFinder, CDialog)

CPathFinder::CPathFinder(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CPathFinder, pParent)
{

}

CPathFinder::~CPathFinder()
{
}

void CPathFinder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CPathFinder, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CPathFinder::OnListBox)
	ON_WM_DESTROY()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CPathFinder 메시지 처리기


void CPathFinder::OnListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


}
void CPathFinder::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 파일을 올리면 파일의 오브젝트 키  | 스테이트 키 | 상대 경로 | 개수 이렇게 리스트에 올림
	// 그러기 위해서는 적절한 파싱이 필요
	// 드롭된 파일의 개수를 판단하고 각 파일마다 파싱을 진행함 
	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);

	TCHAR szFilePath[MAX_PATH] = L"";

	int iDropFileNum = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);
	for (int i = 0; i < iDropFileNum; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		//이후 파싱 시작
		CFileInfo::DirInfoExtraction(szFilePath, m_PathInfoList);
	}

	m_ListBox.ResetContent();

	wstring wstrCombined = L"";
	TCHAR szBuf[MAX_STR] = L"";

	for (auto& Info : m_PathInfoList)
	{
		_itow_s(Info->iCount, szBuf, 10);
		wstrCombined =
			Info->wstrObjKey + L"|" +
			Info->wstrStateKey + L"|" +
			Info->wstrPath + L"|" +
			szBuf;
		m_ListBox.AddString(wstrCombined.c_str());
	}


	UpdateData(FALSE);
}

void CPathFinder::OnDestroy()
{
	CDialog::OnDestroy();
	for_each(m_PathInfoList.begin(), m_PathInfoList.end(),
		[](auto& PathInfo)
		{
			if (PathInfo != nullptr)
			{
				delete PathInfo;
				PathInfo = nullptr;
			}
		}
	);
	m_PathInfoList.clear();
}