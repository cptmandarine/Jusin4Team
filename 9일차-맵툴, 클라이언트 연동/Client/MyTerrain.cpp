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

	//타일 로드, 데코 로드...
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
		pTexInfo->tImgInfo.Width * 0.5f + m_vScroll.x, // x 스크롤
		pTexInfo->tImgInfo.Height * 0.5f + m_vScroll.y, // y 스크롤
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

	//타일 그리기
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
	//		nullptr, // 출력할 이미지 영역의 RECT 주소, NULL인 경우 0, 0 기준으로 출력
	//		&vTemp, // 출력할 이미지의 중심 좌표, VEC3주소, NULL 인 경우 0, 0이 중심 좌표
	//		nullptr, // 텍스처를 출력할 위치 좌표, VEC3주소, 스크린상 0, 0 좌표에 출력
	//		D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 원본 이미지와 섞을 색상, 0xffffffff를 넘겨주면 원본색 유지
	//}

	//데코 그려주기 

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
			nullptr, // 출력할 이미지 영역의 RECT 주소, NULL인 경우 0, 0 기준으로 출력
			&vTemp, // 출력할 이미지의 중심 좌표, VEC3주소, NULL 인 경우 0, 0이 중심 좌표
			nullptr, // 텍스처를 출력할 위치 좌표, VEC3주소, 스크린상 0, 0 좌표에 출력
			D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 원본 이미지와 섞을 색상, 0xffffffff를 넘겨주면 원본색 유지
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

			// 벡터 범위 확인
			if (iIndex >= m_vecTile.size()) continue;

			// 좌 상단

			// 맨 윗줄 && 맨 왼쪽줄이 아닌 경우
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

			// 맨 윗줄 && 맨 오른쪽 줄이 아닌 경우
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

			// 맨 아랫 줄 && 맨 왼쪽줄이 아닌 경우
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

			// 맨 아랫 줄 && 맨 오른쪽 줄이 아닌 경우
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
