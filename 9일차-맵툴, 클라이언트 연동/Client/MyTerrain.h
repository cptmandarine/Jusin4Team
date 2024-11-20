#pragma once

#include "Obj.h"

class CMyTerrain :	public CObj
{
public:
	CMyTerrain();
	virtual ~CMyTerrain();
public:
	vector<TILE*>& Get_VecTile() { return m_vecTile; };
	vector<list<TILE*>>& Get_VecAdj() { return m_vecAdj; }
public:
	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	HRESULT		Load_Tile(const TCHAR* pTilePath);
	HRESULT		Load_Deco(const TCHAR* pDecoPath);

	void		Ready_Adj();
private:
	//그냥 배경은 디폴트로 잡음 귀찮..
	vector<TILE*>		m_vecTile;
	vector<list<TILE*>> m_vecAdj;
	vector<DECO*>		m_vecDeco;
};