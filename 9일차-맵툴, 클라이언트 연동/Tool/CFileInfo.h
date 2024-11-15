#pragma once
#include "Include.h"

class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString	ConvertRealtivePath(CString strFullPath);
	static void		DirInfoExtraction(const wstring& _wstrPath, list<IMGPATH*>& _InfoList);
	static int		DirFileCount(const wstring& _wstrPath);
};

