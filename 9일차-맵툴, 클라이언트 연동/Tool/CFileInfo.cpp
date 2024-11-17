#include "pch.h"
#include "CFileInfo.h"

CFileInfo::CFileInfo()
{
}

CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRealtivePath(CString strFullPath)
{
	TCHAR	szRelativePath[MAX_PATH] = L"";
	TCHAR	szCurDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	// PathRelativePathTo : szCurDirPath에서 strFullPath로 가는 상대 경로를 구해서
	// 	szRelativePath에 저장하는 함수

	PathRelativePathTo(szRelativePath,
		szCurDirPath,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(),
		FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring& _wstrPath, list<IMGPATH*>& _InfoList)
{
	//해당 파일경로에 있는 모든 파일, 디렉토리, 시스템 파일
	wstring strFilePath = _wstrPath + L"\\*.*";
	//MFC에서 파일을 쉽게 찾을 수 있게 하는 클래스
	CFileFind Find;
	bool bContinue = Find.FindFile(strFilePath.c_str());
	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
		{
			continue;
		}
		
		if (Find.IsDirectory())
		{
			DirInfoExtraction(wstring(Find.GetFilePath()), _InfoList);
		}
		else 
		{
			if (Find.IsSystem())
			{
				continue;
			}
			TCHAR szDstPath[MAX_PATH] = L"";
			TCHAR szDirPath[MAX_PATH] = L"";
			lstrcpy(szDirPath, Find.GetFilePath().GetString());

			PathRemoveFileSpec(szDirPath);
			IMGPATH* pImgPath = new IMGPATH;

			//해당 폴더에서 파일이 몇개있는 지 알아야함
			pImgPath->iCount = DirFileCount(szDirPath);

			//파일 명만 가져와서 %d.png를 이용하여 나중에 wsprintf를 사용하여 붙임
			//getFileTitle을 하면 딱 파일의 이름만 가져옴 확장자 명 무시
			wstring wstrFileName = Find.GetFileTitle();
			wstrFileName = wstrFileName.substr(0, wstrFileName.size() - 1) + L"%d.png";

			PathCombine(szDstPath, szDirPath, wstrFileName.c_str());
			pImgPath->wstrPath = ConvertRealtivePath(szDstPath);


			PathRemoveFileSpec(szDirPath);
			pImgPath->wstrObjKey = PathFindFileName(szDirPath);
			
			PathRemoveFileSpec(szDirPath);
			pImgPath->wstrStateKey = PathFindFileName(szDirPath);

			_InfoList.push_back(pImgPath);
			bContinue = 0;
		}
	}
}

int CFileInfo::DirFileCount(const wstring& _wstrPath)
{
	int iCnt = 0;
	CFileFind Find;
	wstring wstrAllFilePath = _wstrPath + L"\\*.*";
	bool bContinue = Find.FindFile(wstrAllFilePath.c_str());

	while (bContinue)
	{
		bContinue = Find.FindNextFile();
		if (Find.IsDots() || Find.IsSystem())
		{
			continue;
		}
		++iCnt;
	}
	return iCnt;
}
