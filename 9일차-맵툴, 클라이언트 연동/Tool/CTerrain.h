#pragma once

#include "Include.h"

class CToolView;
class CTerrain
{
public:
	CTerrain();
	~CTerrain();

public:
	vector<TILE*>& Get_VecTile() { return m_vecTile; }

public:
	void		Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);

public:
	void			Tile_Change(const D3DXVECTOR3& vPos, const int& iDrawID);
	int				Get_TileIndex(const D3DXVECTOR3& vPos);
	bool			Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool			Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);

public:
	void	Initialize();
	void	Update();
	void	Mini_Render();
	void	Render();
	void	Release();

private:
	vector<TILE*>		m_vecTile;
	CToolView*			m_pMainView;

};

// Invalidate(FALSE) : 화면을 갱신하는 함수

// 직선의 방정식을 이용하여 마우스를 클릭한 타일의 이미지를 변경해라.

// y = ax + b

// a : 기울기 (y 증가량 / x 증가량)
// b : y 절편 (x값이 0인 경우의 y 값)

// 2. 미니맵을 출력하라(마우스 피킹 연동)
// - 전체 타일을 미니맵 화면에 출력 할 것

// CMiniView* pMiniView->Invalidate(FALSE);