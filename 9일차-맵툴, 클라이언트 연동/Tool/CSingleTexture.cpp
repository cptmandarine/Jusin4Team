#include "pch.h"
#include "CSingleTexture.h"
#include "CDevice.h"

CSingleTexture::CSingleTexture()
{
}

CSingleTexture::~CSingleTexture()
{
	Release();
}

const TEXINFO* CSingleTexture::Get_Texture(const TCHAR* pStateKey, const int& iCount)
{
	return m_pTexInfo;
}

HRESULT CSingleTexture::Insert_Texture(const TCHAR* pFilePath, 
										const TCHAR* pStateKey, 
										const int& iCount)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		Safe_Delete(m_pTexInfo);
		AfxMessageBox(pFilePath);
		return E_FAIL;
	}

	// D3DPOOL_DEFAULT = 가장 적합한 메모리에 보관, 보통 그래픽 카드 메모리에 저장
	// D3DPOOL_MANAGED = 그래픽 메모리를 사용하지만 이 정보를 RAM에 백업
	// D3DPOOL_SYSTEMMEM = 시스템 메모리(RAM)을 사용,
	// D3DPOOL_SCRATCH = 시스템 메모리를 사용하지만 DX 장치로 접근 불가

	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),
											pFilePath,
											m_pTexInfo->tImgInfo.Width,
											m_pTexInfo->tImgInfo.Height,
											m_pTexInfo->tImgInfo.MipLevels,
											0,		// 일반적인 텍스처(D3DUSAGE_RENDERTARGET으로 용도 변경 가능)
											m_pTexInfo->tImgInfo.Format,
											D3DPOOL_MANAGED,
											D3DX_DEFAULT,
											D3DX_DEFAULT,
											0,	// 0을 넣어주면 컬러키 무효화
											NULL,
											NULL,
											&(m_pTexInfo->pTexture))))
	{
		Safe_Delete(m_pTexInfo);
		AfxMessageBox(L"Single Texture Failed");
		
		return E_FAIL;
	}

	return S_OK;
}

void CSingleTexture::Release()
{
	Safe_Release(m_pTexInfo->pTexture);
	Safe_Delete(m_pTexInfo);
}
