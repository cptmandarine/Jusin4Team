#include "pch.h"
#include "CDevice.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice()
	: m_pSDK(nullptr), m_pDevice(nullptr),
	m_pSprite(nullptr), m_pFont(nullptr)
{
}

CDevice::~CDevice()
{
	Release();
}

HRESULT CDevice::Init_Device()
{
	// 1. sdk 객체 생성

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	// 2. sdk 객체를 이용하여 그래픽 카드 성능 조사

	D3DCAPS9	DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps : 장치에 대한 지원 수준을 얻어오는 함수
	// D3DADAPTER_DEFAULT : 정보를 얻으려는 기본 그래픽 카드를 의미
	
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		AfxMessageBox(L"GetDeviceCaps Failed");
		return E_FAIL;
	}

	// 3. 그래픽 카드 담당(제어) 객체 생성

	// 버텍스 프로세싱 = 정점의 변환 + 조명 연산
	DWORD	vp(0);

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;

	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Parameters(d3dpp);

	// CreateDevice : 그래픽 장치를 제어할 객체를 생성
	// CreateDevice(어떤 그래픽 카드를 제어할 것인가, 어떤 정보로 장치에 접근할 것인가, 장치를 사용할 창 정보, 동작 방식, 사용 환경, 생성할 객체를 저장할 포인터)
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										g_hWnd,
										vp, 
										&d3dpp,
										&m_pDevice)))
	{
		AfxMessageBox(L"CreateDevice Failed");
		return E_FAIL;
	}

	// 스프라이트 컴 객체

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		AfxMessageBox(L"D3DXCreateSprite Failed");
		return E_FAIL;
	}

	// 폰트 컴 객체

	D3DXFONT_DESCW  tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 15;
	tFontInfo.Width  = 5;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"궁서");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		AfxMessageBox(L"D3DXCreateFontIndirect Failed");
		return E_FAIL;
	}

	
	return S_OK;
}

// 후면 버퍼의 동작 원리
void CDevice::Render_Begin()
{
	m_pDevice->Clear(0,			// 렉트의 개수
					nullptr,	// 렉트 중 첫 번째 주소
					D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, 
					D3DCOLOR_ARGB(255, 0, 0, 0),	// 백버퍼의 색상
					1.f,  // z버퍼 초기화 값
					0);	  // 스텐실 버퍼 초기화 값

	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

}

void CDevice::Render_End(HWND hWnd)
{

	m_pSprite->End();
	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, hWnd, NULL);

	// 1, 2, 4 매개변두 모두 D3DSWAPEFFECT_COPY 옵션이어야만 사용 가능
}

void CDevice::Release()
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pDevice);
	Safe_Release(m_pSDK);
}

void CDevice::Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp)
{
	d3dpp.BackBufferWidth  = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount  = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// D3DSWAPEFFECT_DISCARD : 스왑 체인 방식
	// D3DSWAPEFFECT_FLIP : 버퍼 하나로 뒤집으면서 사용
	// D3DSWAPEFFECT_COPY : 더블 버퍼링과 유사한 복사

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = g_hWnd;
	
	d3dpp.Windowed = TRUE; // 창 모드(TRUE) or 전체 화면 모드(FALSE) 실행 설정
	

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// 창 모드 시 모티너 재생률을 운영체제에서 관리
	// 전체화면 변경 시 , 현재 모니터 재생률을 토대로 알아서 수행
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// 재생률과 시연 간격 설정
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
