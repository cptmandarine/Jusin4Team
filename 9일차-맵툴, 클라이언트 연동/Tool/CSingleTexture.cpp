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

	// D3DPOOL_DEFAULT = ���� ������ �޸𸮿� ����, ���� �׷��� ī�� �޸𸮿� ����
	// D3DPOOL_MANAGED = �׷��� �޸𸮸� ��������� �� ������ RAM�� ���
	// D3DPOOL_SYSTEMMEM = �ý��� �޸�(RAM)�� ���,
	// D3DPOOL_SCRATCH = �ý��� �޸𸮸� ��������� DX ��ġ�� ���� �Ұ�

	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),
											pFilePath,
											m_pTexInfo->tImgInfo.Width,
											m_pTexInfo->tImgInfo.Height,
											m_pTexInfo->tImgInfo.MipLevels,
											0,		// �Ϲ����� �ؽ�ó(D3DUSAGE_RENDERTARGET���� �뵵 ���� ����)
											m_pTexInfo->tImgInfo.Format,
											D3DPOOL_MANAGED,
											D3DX_DEFAULT,
											D3DX_DEFAULT,
											0,	// 0�� �־��ָ� �÷�Ű ��ȿȭ
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
