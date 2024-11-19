#include "pch.h"
#include "CUnit.h"
#include "CTextureMgr.h"
#include "CDevice.h"
#include "ToolView.h"

CUnit::CUnit()
{
}

CUnit::~CUnit()
{
	Release();
}

void CUnit::Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY)
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

void CUnit::Initialize()
{
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Monster/Unit/Unit%d.png", TEX_MULTI, L"Monster", L"Unit", 5)))
	{
		AfxMessageBox(L"Tile Texture Insert Failed");
		return;
	}
}

void CUnit::Update()
{
}

bool CUnit::Create_Unit(D3DXVECTOR3 vPos, int _iDrawID, int _iIndex)
{
	if (_iDrawID < 0)
	{
		return false;
	}
	for (int i = 0; i < m_vecDeco.size(); ++i)
	{
		if (m_vecDeco[i]->byIndex == _iIndex)
		{
			return false;
		}
	}

	DECO* pDeco = new DECO;
	pDeco->vPos = vPos;
	pDeco->byDrawID = _iDrawID;
	pDeco->byIndex = _iIndex;

	m_vecDeco.push_back(pDeco);
	return true;
}

void CUnit::Mini_Render()
{
}

void CUnit::Render()
{
	//y-sorting 利侩 募募
	sort(m_vecDeco.begin(), m_vecDeco.end(),
		[](auto& v1, auto& v2)->bool
		{
			if (v1->vPos.y == v2->vPos.y)
			{
				return v1->vPos.x < v2->vPos.x;
			}
			return v1->vPos.y < v2->vPos.y;
		});

	for (auto& pDeco : m_vecDeco)
	{
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Monster", L"Unit", pDeco->byDrawID);

		D3DXMATRIX	matWorld, matScale, matTrans;

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, pDeco->vPos.x - m_pMainView->GetScrollPos(0), // x 胶农费
			pDeco->vPos.y - m_pMainView->GetScrollPos(1), // y 胶农费
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
	}
}

void CUnit::Release()
{
	for_each(m_vecDeco.begin(), m_vecDeco.end(), [](auto& pDeco)
		{
			if (nullptr != pDeco)
			{
				delete pDeco;
				pDeco = nullptr;
			}
		});
	m_vecDeco.clear();
}
