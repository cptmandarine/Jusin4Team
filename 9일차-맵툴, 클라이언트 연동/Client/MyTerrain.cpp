#include "pch.h"
#include "MyTerrain.h"
#include "CDevice.h"
#include "CTextureMgr.h"
#include "TimeMgr.h"

CMyTerrain::CMyTerrain()
{
}

CMyTerrain::~CMyTerrain()
{
	Release();
}

HRESULT CMyTerrain::Initialize(void)
{
	m_vecTile.reserve(TILEX * TILEY);

	//Ÿ�� �ε�, ���� �ε�...
	if (FAILED(Load_Tile(L"../Data/MyMap.dat")))
	{
		return E_FAIL;
	}
	if (FAILED(Load_Deco(L"../Data/MyDeco.dat")))
	{
		return E_FAIL;
	}
	m_wstrObjKey = L"Terrain";
	m_wstrStateKey = L"Tile";

	Ready_Adj();
	return S_OK;
}

int CMyTerrain::Update(void)
{
	if (0.f > Get_Mouse().x)
	{
		m_vScroll.x += 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();
	}
	if (WINCX < Get_Mouse().x)
	{
		m_vScroll.x -= 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();
	}
	if (0.f > Get_Mouse().y)
	{
		m_vScroll.y += 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();
	}
	if (WINCY < Get_Mouse().y)
	{
		m_vScroll.y -= 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();
	}
	return 0;
}

void CMyTerrain::Late_Update(void)
{


}

void CMyTerrain::Render(void)
{
	TCHAR	szBuf[MIN_STR] = L"";
	int		iIndex(0);

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"BackGround", L"Map", 3);
	D3DXMATRIX	matWorld, matScale, matTrans;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans,
		pTexInfo->tImgInfo.Width * 0.5f + m_vScroll.x, // x ��ũ��
		pTexInfo->tImgInfo.Height * 0.5f + m_vScroll.y, // y ��ũ��
		0.f);

	matWorld = matScale * matTrans;

	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	D3DXVECTOR3	vTemp = { fCenterX, fCenterY, 0.f };
	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&vTemp,
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	//Ÿ�� �׸���
	//for (auto pTile : m_vecTile)
	//{
	//	D3DXMatrixIdentity(&matWorld);
	//	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	//	D3DXMatrixTranslation(&matTrans,
	//		pTile->vPos.x + m_vScroll.x,
	//		pTile->vPos.y + m_vScroll.y, 0.f);
	//	matWorld = matScale * matTrans;
	//	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	//	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", pTile->byDrawID);
	//	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	//	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
	//	D3DXVECTOR3	vTemp = { fCenterX, fCenterY, 0.f };
	//	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
	//		nullptr, // ����� �̹��� ������ RECT �ּ�, NULL�� ��� 0, 0 �������� ���
	//		&vTemp, // ����� �̹����� �߽� ��ǥ, VEC3�ּ�, NULL �� ��� 0, 0�� �߽� ��ǥ
	//		nullptr, // �ؽ�ó�� ����� ��ġ ��ǥ, VEC3�ּ�, ��ũ���� 0, 0 ��ǥ�� ���
	//		D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� ���� �̹����� ���� ����, 0xffffffff�� �Ѱ��ָ� ������ ����
	//}

	//���� �׷��ֱ� 

	for (auto pDeco : m_vecDeco)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			pDeco->vPos.x + m_vScroll.x,
			pDeco->vPos.y + m_vScroll.y, 0.f);
		matWorld = matScale * matTrans;
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Object", L"Deco", pDeco->byDrawID);
		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
		D3DXVECTOR3	vTemp = { fCenterX, fCenterY, 0.f };
		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr, // ����� �̹��� ������ RECT �ּ�, NULL�� ��� 0, 0 �������� ���
			&vTemp, // ����� �̹����� �߽� ��ǥ, VEC3�ּ�, NULL �� ��� 0, 0�� �߽� ��ǥ
			nullptr, // �ؽ�ó�� ����� ��ġ ��ǥ, VEC3�ּ�, ��ũ���� 0, 0 ��ǥ�� ���
			D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� ���� �̹����� ���� ����, 0xffffffff�� �Ѱ��ָ� ������ ����
	}
}

void CMyTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<TILE*>);
	m_vecTile.clear();
}

HRESULT CMyTerrain::Load_Tile(const TCHAR* pTilePath)
{
	HANDLE hFile = CreateFile(pTilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte(0);

	TILE* pTile = nullptr;

	while (true)
	{
		pTile = new TILE;

		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pTile);
			break;
		}

		m_vecTile.push_back(pTile);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CMyTerrain::Load_Deco(const TCHAR* pDecoPath)
{
	HANDLE hFile = CreateFile(pDecoPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte(0);

	DECO* pDeco = nullptr;

	while (true)
	{
		pDeco = new DECO;

		ReadFile(hFile, pDeco, sizeof(DECO), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pDeco);
			break;
		}

		m_vecDeco.push_back(pDeco);
	}

	CloseHandle(hFile);

	return S_OK;
}

void CMyTerrain::Ready_Adj()
{
	m_vecAdj.resize(m_vecTile.size());

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			int iIndex = i * TILEX + j;

			// ���� ���� Ȯ��
			if (iIndex >= m_vecTile.size()) continue;

			// �� ���

			// �� ���� && �� �������� �ƴ� ���
			if ((0 != i) && (0 != iIndex % (TILEX * 2)))
			{
				if ((0 != i % 2) && (iIndex - TILEX >= 0) && iIndex - TILEX < m_vecTile.size() && m_vecTile[iIndex - TILEX] && (!m_vecTile[iIndex - TILEX]->byDrawID))
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - TILEX]);
				}
				else if ((0 == i % 2) && (iIndex - (TILEX + 1) >= 0) && iIndex - (TILEX + 1) < m_vecTile.size() && m_vecTile[iIndex - (TILEX + 1)] && (!m_vecTile[iIndex - (TILEX + 1)]->byDrawID))
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - (TILEX + 1)]);
				}
			}

			// �� ���� && �� ������ ���� �ƴ� ���
			if ((0 != i) && ((TILEX * 2 - 1) != iIndex % (TILEX * 2)))
			{
				if ((0 != i % 2) && (iIndex - (TILEX - 1) >= 0) && iIndex - (TILEX - 1) < m_vecTile.size() && m_vecTile[iIndex - (TILEX - 1)] && (!m_vecTile[iIndex - (TILEX - 1)]->byDrawID))
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - (TILEX - 1)]);
				}
				else if ((0 == i % 2) && (iIndex - TILEX >= 0) && iIndex - TILEX < m_vecTile.size() && m_vecTile[iIndex - TILEX] && (!m_vecTile[iIndex - TILEX]->byDrawID))
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - TILEX]);
				}
			}

			// �� �Ʒ� �� && �� �������� �ƴ� ���
			if ((TILEY - 1 != i) && (0 != iIndex % (TILEX * 2)))
			{
				if ((0 != i % 2) && (iIndex + TILEX < m_vecTile.size()) && m_vecTile[iIndex + TILEX] && (!m_vecTile[iIndex + TILEX]->byDrawID))
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + TILEX]);
				}
				else if ((0 == i % 2) && (iIndex + TILEX - 1 < m_vecTile.size()) && m_vecTile[iIndex + (TILEX - 1)] && (!m_vecTile[iIndex + (TILEX - 1)]->byDrawID))
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + (TILEX - 1)]);
				}
			}

			// �� �Ʒ� �� && �� ������ ���� �ƴ� ���
			if ((TILEY - 1 != i) && (TILEX * 2 - 1) != iIndex % (TILEX * 2))
			{
				if ((0 != i % 2) && (iIndex + TILEX + 1 < m_vecTile.size()) && m_vecTile[iIndex + TILEX + 1] && (!m_vecTile[iIndex + TILEX + 1]->byDrawID))
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + TILEX + 1]);
				}
				else if ((0 == i % 2) && (iIndex + TILEX < m_vecTile.size()) && m_vecTile[iIndex + TILEX] && (!m_vecTile[iIndex + TILEX]->byDrawID))
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + TILEX]);
				}
			}
		}
	}
}
