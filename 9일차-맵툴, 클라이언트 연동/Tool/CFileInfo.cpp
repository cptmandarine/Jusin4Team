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

	// PathRelativePathTo : szCurDirPath���� strFullPath�� ���� ��� ��θ� ���ؼ�
	// 	szRelativePath�� �����ϴ� �Լ�

	PathRelativePathTo(szRelativePath,
		szCurDirPath,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(),
		FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring& _wstrPath, list<IMGPATH*>& _InfoList)
{
	//�ش� ���ϰ�ο� �ִ� ��� ����, ���丮, �ý��� ����
	wstring strFilePath = _wstrPath + L"\\*.*";
	//MFC���� ������ ���� ã�� �� �ְ� �ϴ� Ŭ����
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

			//�ش� �������� ������ ��ִ� �� �˾ƾ���
			pImgPath->iCount = DirFileCount(szDirPath);

			//���� �� �����ͼ� %d.png�� �̿��Ͽ� ���߿� wsprintf�� ����Ͽ� ����
			//getFileTitle�� �ϸ� �� ������ �̸��� ������ Ȯ���� �� ����
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
