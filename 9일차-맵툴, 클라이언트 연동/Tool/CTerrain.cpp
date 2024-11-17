#include "pch.h"
#include "CTerrain.h"
#include "CTextureMgr.h"
#include "CDevice.h"
#include "ToolView.h"

CTerrain::CTerrain()
{
	m_vecTile.reserve(TILEX * TILEY);
	m_BackID = 3;
}

CTerrain::~CTerrain()
{
	Release();
}

void CTerrain::Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY)
{
	pOut->_11 *= fRatioX;
	pOut->_21 *= fRatioX;
	pOut->_31 *= fRatioX;
	pOut->_41 *= fRatioX;

	pOut->_12 *= fRatioY;
	pOut->_22 *= fRatioY;
	pOut->_32 *= fRatioY;
	pOut->_42 *= fRatioY;
}

void CTerrain::Tile_Change(const D3DXVECTOR3& vPos, const int& iDrawID)
{
	int		iIndex = Get_TileIndex(vPos);
	
	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->byOption = 1;
	m_vecTile[iIndex]->byDrawID = iDrawID;
}

int CTerrain::Get_TileIndex(const D3DXVECTOR3& vPos)
{
	for (size_t index = 0; index < m_vecTile.size(); ++index)
	{
		if (Picking_Dot(vPos, index))
			return index;
	}

	return -1;
}

bool CTerrain::Picking(const D3DXVECTOR3& vPos, const int& iIndex)
{
	float fGradient[4]
	{
		(TILECY / 2.f) / (TILECX / 2.f) * -1.f, 
		(TILECY / 2.f) / (TILECX / 2.f),
		(TILECY / 2.f) / (TILECX / 2.f) * -1.f,
		(TILECY / 2.f) / (TILECX / 2.f)
	};

	// 12시, 3시, 6시, 9시
	D3DXVECTOR3	vPoint[4]
	{
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f),  0.f },
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y , 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f),  0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y , 0.f }
	};

	// y = ax + b;
	
	// b = y - ax

	float	fB[4]
	{
		vPoint[0].y - fGradient[0] * vPoint[0].x, 
		vPoint[1].y - fGradient[1] * vPoint[1].x,
		vPoint[2].y - fGradient[2] * vPoint[2].x,
		vPoint[3].y - fGradient[3] * vPoint[3].x
	};

	bool	bCheck[4]{ false };

	// 0 == ax + b - y : 직선 상에 놓여있는 상황
	// 0 > ax + b - y : 직선 위 쪽에 놓여있는 상황
	// 0 < ax + b - y : 직선 아래 쪽에 놓여있는 상황

	if (0 < fGradient[0] * vPos.x + fB[0] - vPos.y)
		bCheck[0] = true;

	if (0 > fGradient[1] * vPos.x + fB[1] - vPos.y)
		bCheck[1] = true;

	if (0 > fGradient[2] * vPos.x + fB[2] - vPos.y)
		bCheck[2] = true;

	if (0 < fGradient[3] * vPos.x + fB[3] - vPos.y)
		bCheck[3] = true;

	return bCheck[0] && bCheck[1] && bCheck[2] && bCheck[3];
}

bool CTerrain::Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex)
{

	// 12시, 3시, 6시, 9시
	D3DXVECTOR3	vPoint[4]
	{
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f),  0.f },
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y , 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f),  0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y , 0.f }
	};

	D3DXVECTOR3		vDir[4]
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	D3DXVECTOR3		vNormal[4]
	{
		{ -vDir[0].y, vDir[0].x, 0.f},
		{ -vDir[1].y, vDir[1].x, 0.f},
		{ -vDir[2].y, vDir[2].x, 0.f},
		{ -vDir[3].y, vDir[3].x, 0.f}

	};

	D3DXVECTOR3		vMouseDir[4]
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}

void CTerrain::Initialize()
{
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Map/Map%d.png", TEX_MULTI, L"BackGround", L"Map", 6)))
	{
		AfxMessageBox(L"Tile Texture Insert Failed");
		return;
	}

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Tile/Tile%d.png", TEX_MULTI, L"Terrain", L"Tile", 5)))
	{
		AfxMessageBox(L"Tile Texture Insert Failed");
		return;
	}

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			TILE* pTile = new TILE;

			float	fX = (TILECX * j) + (i % 2) * (TILECX / 2.f);
			float	fY = (TILECY / 2.f) * i;

			pTile->vPos  = {fX, fY, 0.f};
			pTile->vSize = { (float)TILECX, (float)TILECY };
			pTile->byOption = 0;
			pTile->byDrawID = 0;

			m_vecTile.push_back(pTile);
		}
	}


}

void CTerrain::Update()
{
	
}

void CTerrain::Mini_Render()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"BackGround", L"Map", m_BackID);
	D3DXMATRIX	matWorld, matScale, matTrans;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans,
		pTexInfo->tImgInfo.Width * 0.5f , // x 스크롤
		pTexInfo->tImgInfo.Height * 0.5f , // y 스크롤
		0.f);

	matWorld = matScale * matTrans;

	RECT rc{};
	GetClientRect(m_pMainView->m_hWnd, &rc);

	Set_Ratio(&matWorld, 0.3f, 0.3f);
	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));


	for (auto& pTile : m_vecTile)
	{
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", pTile->byDrawID);

		D3DXMATRIX	matWorld, matScale, matTrans;

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, pTile->vPos.x,
											pTile->vPos.y, 
											0.f);

		matWorld = matScale * matTrans;
	
		Set_Ratio(&matWorld, 0.3f, 0.3f);

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));	
	}
}

void CTerrain::Render()
{
	TCHAR	szBuf[MIN_STR] = L"";
	int		iIndex(0);

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"BackGround", L"Map", m_BackID);
	D3DXMATRIX	matWorld, matScale, matTrans;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans, 
			pTexInfo->tImgInfo.Width * 0.5f  - m_pMainView->GetScrollPos(0), // x 스크롤
			pTexInfo->tImgInfo.Height * 0.5f - m_pMainView->GetScrollPos(1), // y 스크롤
		0.f);

	matWorld = matScale * matTrans;

	RECT rc{};
	GetClientRect(m_pMainView->m_hWnd, &rc);

	float fX = WINCX / float(rc.right - rc.left);
	float fY = WINCY / float(rc.bottom - rc.top);

	Set_Ratio(&matWorld, fX, fY);

	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));


	for (auto& pTile : m_vecTile)
	{
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", pTile->byDrawID);

		D3DXMATRIX	matWorld, matScale, matTrans;

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, pTile->vPos.x - m_pMainView->GetScrollPos(0), // x 스크롤
										 pTile->vPos.y - m_pMainView->GetScrollPos(1), // y 스크롤
										 0.f);

		matWorld = matScale * matTrans;

		RECT rc{};
		GetClientRect(m_pMainView->m_hWnd, &rc);

		float fX = WINCX / float(rc.right - rc.left);
		float fY = WINCY / float(rc.bottom - rc.top);

		Set_Ratio(&matWorld, fX, fY);

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,								
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f),  
			nullptr,								
			D3DCOLOR_ARGB(255, 255, 255, 255));		

		swprintf_s(szBuf, L"%d", iIndex);
		CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
														szBuf, 
														lstrlen(szBuf),
														nullptr,	// 렉트의 주소
														DT_CENTER,			// 정렬 옵션(DT_CENTER)
														D3DCOLOR_ARGB(255, 0, 0, 0));	// 출력할 폰트 색상

		++iIndex;
	}	
}

void CTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), [](auto& pTile)
		{
			if (nullptr != pTile)
			{
				delete pTile;
				pTile = nullptr;
			}
		});

	m_vecTile.clear();

}
