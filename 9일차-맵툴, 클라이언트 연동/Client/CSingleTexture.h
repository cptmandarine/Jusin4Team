#pragma once

#include "CTexture.h"

class CSingleTexture : public CTexture
{
public:
	CSingleTexture();
	virtual ~CSingleTexture();

public:
	virtual const TEXINFO* Get_Texture(const TCHAR* pStateKey = L"",
		const int& iCount = 0);

public:
	virtual HRESULT		Insert_Texture(const TCHAR* pFilePath,
										const TCHAR* pStateKey = L"",
										const int& iCount = 0)override;

	virtual void		Release()override;

private:
	TEXINFO* m_pTexInfo;

};

