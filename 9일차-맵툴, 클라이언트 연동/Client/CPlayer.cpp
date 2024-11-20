#include "pch.h"
#include "CPlayer.h"
#include "CDevice.h"
#include "CTextureMgr.h"
#include "TimeMgr.h"
#include "CMyAstarMgr.h"
CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
	Release();
}

HRESULT CPlayer::Initialize(void)
{
	m_bRight = true;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 6;
	m_tFrame.ullTime = GetTickCount64();
	m_tFrame.ullSpeed = 100;

	m_pObjKey = L"Player";
	m_pFrameKey = L"Stand_D";

	m_eCurState = CPlayer::STAND;
	m_tInfo.vPos = { 611.f, 552.f, 0.f };
	return S_OK;
}

int CPlayer::Update(void)
{
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		CMyAstarMgr::Get_Instance()->Start_Astar(m_tInfo.vPos, Get_Mouse() - CObj::m_vScroll);
	}
	//Best길을 역추적
	Move_Route();
	D3DXMATRIX	matScale, matTrans;

	if(m_bRight) D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);
	else D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x + CObj::m_vScroll.x + TILECX /2,
		m_tInfo.vPos.y + CObj::m_vScroll.y - TILECY / 2,
		0.f);

	m_tInfo.matWorld = matScale * matTrans;
	return 0;
}

void CPlayer::Late_Update(void)
{
	Motion_Change();
	CObj::Move_Frame();
}

void CPlayer::Render(void)
{
	

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_pObjKey, m_pFrameKey, m_tFrame.iFrameStart);

	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
	D3DXVECTOR3	vTemp = { fCenterX, fCenterY, 0.f };

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr, // 출력할 이미지 영역의 RECT 주소, NULL인 경우 0, 0 기준으로 출력
		&vTemp, // 출력할 이미지의 중심 좌표, VEC3주소, NULL 인 경우 0, 0이 중심 좌표
		nullptr, // 텍스처를 출력할 위치 좌표, VEC3주소, 스크린상 0, 0 좌표에 출력
		D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 원본 이미지와 섞을 색상, 0xffffffff를 넘겨주면 원본색 유지
}

void CPlayer::Release(void)
{
}

void CPlayer::Motion_Change()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CPlayer::STAND:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.ullTime = GetTickCount64();
			m_tFrame.ullSpeed = 100;
			m_pFrameKey = L"Stand_D";
			m_pObjKey = L"Player";
			m_bRight = false;
			break;
		case CPlayer::WALK_LD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.ullTime = GetTickCount64();
			m_tFrame.ullSpeed = 100;
			m_pFrameKey = L"Walk_LD";
			m_pObjKey = L"Player";
			m_bRight = false;
			break;
		case CPlayer::WALK_LU:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.ullTime = GetTickCount64();
			m_tFrame.ullSpeed = 100;
			m_pFrameKey = L"Walk_LU";
			m_pObjKey = L"Player";
			m_bRight = false;
			break;
		case CPlayer::WALK_RD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.ullTime = GetTickCount64();
			m_tFrame.ullSpeed = 100;
			m_pFrameKey = L"Walk_LD";
			m_pObjKey = L"Player";
			m_bRight = true;
			break;
		case CPlayer::WALK_RU:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.ullTime = GetTickCount64();
			m_tFrame.ullSpeed = 100;
			m_pFrameKey = L"Walk_LU";
			m_pObjKey = L"Player";
			m_bRight = true;
			break;
		case CPlayer::END:
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CPlayer::Move_Route()
{
	list<TILE*>& BestList = CMyAstarMgr::Get_Instance()->Get_BestList();

	if (!BestList.empty())
	{
		int a = BestList.size();
		
		D3DXVECTOR3  vDir = (BestList.front()->vPos) - m_tInfo.vPos;
	
		

		if (vDir.x < 0) // Left
		{
			if (vDir.y <0) // UP
			{
				m_eCurState = WALK_LU;
			}
		
			else if (vDir.y > 0) // DOWN
			{
				m_eCurState = WALK_LD;
			}
		}
	
		else if (vDir.x > 0) // RIGHT
		{
			if (vDir.y < 0) // UP
			{
				m_eCurState = WALK_RU;
			}
	
			else if (vDir.y > 0) // DOWN
			{
				m_eCurState = WALK_RD;
			}
		}
	
		float deltaSpeed = CTimeMgr::Get_Instance()->Get_TimeDelta() * 300.f;
		float    fDistance = D3DXVec3Length(&vDir);
		D3DXVec3Normalize(&vDir, &vDir);
	
		m_tInfo.vPos += (vDir * deltaSpeed);
		if (deltaSpeed >= fDistance)
		{
			m_tInfo.vPos = (BestList.front()->vPos);
			BestList.pop_front();
		}
	
	}
	
	else
	{
		m_eCurState = STAND;
		return;
	}
}
