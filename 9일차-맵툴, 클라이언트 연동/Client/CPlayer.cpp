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
	//wstrng�� ��� TCHAR�� ��ȯ...?
	//�� wstring���� ���� ���ϱ�?
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
		nullptr, // ����� �̹��� ������ RECT �ּ�, NULL�� ��� 0, 0 �������� ���
		&vTemp, // ����� �̹����� �߽� ��ǥ, VEC3�ּ�, NULL �� ��� 0, 0�� �߽� ��ǥ
		nullptr, // �ؽ�ó�� ����� ��ġ ��ǥ, VEC3�ּ�, ��ũ���� 0, 0 ��ǥ�� ���
		D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� ���� �̹����� ���� ����, 0xffffffff�� �Ѱ��ָ� ������ ����
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
