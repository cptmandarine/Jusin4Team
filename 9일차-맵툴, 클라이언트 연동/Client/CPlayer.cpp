#include "pch.h"
#include "CPlayer.h"
#include "CDevice.h"
#include "CTextureMgr.h"
#include "TimeMgr.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
	Release();
}

HRESULT CPlayer::Initialize(void)
{
	//wstrng을 어떻게 TCHAR로 변환...?
	//왜 wstring으로 만든 것일까?
	m_bRight = true;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 11;
	m_tFrame.ullTime = GetTickCount64();
	m_tFrame.ullSpeed = 100;

	m_pObjKey = L"Player_Stand";
	m_pFrameKey = L"Stand";

	m_eCurState = CPlayer::STAND;
	m_tInfo.vPos = { WINCX * 0.5f, WINCY * 0.5f, 0.f };
	return S_OK;
}

int CPlayer::Update(void)
{
	if (GetAsyncKeyState(VK_RIGHT))
	{
		m_bRight = true;
		m_tInfo.vPos.x += (300.f * CTimeMgr::Get_Instance()->Get_TimeDelta());
		m_eCurState = CPlayer::WALK;
	}
	else if (GetAsyncKeyState(VK_LEFT))
	{
		m_bRight = false;
		m_tInfo.vPos.x -= (300.f * CTimeMgr::Get_Instance()->Get_TimeDelta());
		m_eCurState = CPlayer::WALK;
	}
	else
	{
		m_eCurState = CPlayer::STAND;
	}
	return 0;
}

void CPlayer::Late_Update(void)
{
	Motion_Change();
	CObj::Move_Frame();
}

void CPlayer::Render(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans;

	D3DXMatrixIdentity(&matWorld);
	if(m_bRight) D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	else D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

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
			m_tFrame.iFrameEnd = 11;
			m_tFrame.ullTime = GetTickCount64();
			m_tFrame.ullSpeed = 100;
			m_pFrameKey = L"Stand";
			m_pObjKey = L"Player_Stand";
			break;
		case CPlayer::WALK:
			m_tFrame.iFrameStart = 1;
			m_tFrame.iFrameEnd = 12;
			m_tFrame.ullTime = GetTickCount64();
			m_tFrame.ullSpeed = 100;
			m_pFrameKey = L"Walk";
			m_pObjKey = L"Player_Walk";
			break;
		case CPlayer::END:
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}
