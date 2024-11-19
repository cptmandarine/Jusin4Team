#pragma once

#include "Include.h"

class CToolView;
class CDeco
{
public:
	CDeco();
	~CDeco();
public:
	void		Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);
	vector<DECO*>& Get_DecoVector() { return m_vecDeco; };
public:
	void Initialize();
	void Update();
	bool Create_Deco(D3DXVECTOR3 vPos, int _iDrwaID, int _iIndex);
	void Mini_Render();
	void Render();
	void Release();
private:
	int					m_DrawID;
	vector<DECO*>		m_vecDeco;
	CToolView* m_pMainView;
};

