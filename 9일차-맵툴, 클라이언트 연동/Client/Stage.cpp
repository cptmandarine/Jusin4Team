#include "pch.h"
#include "Stage.h"
#include "CTextureMgr.h"
#include "ObjMgr.h"
#include "CPlayer.h"
#include "MyTerrain.h"

CStage::CStage()
{
}

CStage::~CStage()
{
	Release_Scene();
}

HRESULT CStage::Ready_Scene()
{
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Map/Map%d.png",
		TEX_MULTI, L"BackGround", L"Map", 6)))
	{
		ERR_MSG(L"BG Img Insert Failed");
		return E_FAIL;
	}
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Tile/Tile%d.png", 
		TEX_MULTI, L"Terrain", L"Tile", 5)))
	{
		ERR_MSG(L"Tile Img Insert Failed");
		return E_FAIL;
	}
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Object/Deco/Deco%d.png",
		TEX_MULTI, L"Object", L"Deco", 35)))
	{
		ERR_MSG(L"Deco Img Insert Failed");
		return E_FAIL;
	}

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Player/Stand_D/Stand_D%d.png",
		TEX_MULTI, L"Player", L"Stand_D", 7)))
	{
		ERR_MSG(L"Tile Img Insert Failed");
		return E_FAIL;
	}

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Player/Walk_LD/Walk_LD%d.png",
		TEX_MULTI, L"Player", L"Walk_LD", 10)))
	{
		ERR_MSG(L"Tile Img Insert Failed");
		return E_FAIL;
	}

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Player/Walk_LU/Walk_LU%d.png",
		TEX_MULTI, L"Player", L"Walk_LU", 10)))
	{
		ERR_MSG(L"Tile Img Insert Failed");
		return E_FAIL;
	}


	CObj*	pObj = new CMyTerrain;
	
	if (nullptr == pObj)
		return E_FAIL;
	
	pObj->Initialize();
	CObjMgr::Get_Instance()->Add_Object(CObjMgr::TERRAIN, pObj);

	// 플레이어
	pObj = new CPlayer;
	if (nullptr == pObj)
		return E_FAIL;

	pObj->Initialize();

	CObjMgr::Get_Instance()->Add_Object(CObjMgr::PLAYER, pObj);
	
	return S_OK;
}

void CStage::Update_Scene()
{
	CObjMgr::Get_Instance()->Update();
}

void CStage::Late_Update_Scene()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage::Render_Scene()
{
	CObjMgr::Get_Instance()->Render();
}

void CStage::Release_Scene()
{
}
