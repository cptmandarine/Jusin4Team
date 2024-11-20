#pragma once
#include "MyTerrain.h"

class CMyAstarMgr
{
	DECLARE_SINGLETON(CMyAstarMgr)

private:
	CMyAstarMgr();
	~CMyAstarMgr();
public:
	list<TILE*>& Get_BestList() { return m_BestList; }

public:
	void	Start_Astar(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vGoal);

public:
	bool	Make_Route(int iPreIdx, int iGoalIdx);
	void	Make_BestList(int iStartIdx, int iGoalIdx);

	int		Get_TileIndex(const D3DXVECTOR3& vPos);
	bool	Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);
	void	Release();

private:
	bool	Check_Close(int iIndex);

private:
	// f와 index
	priority_queue<pair<int, int>>	m_OpenPQ;
	//visted배열과 동일
	list<int>		m_CloseList;
	list<TILE*>		m_BestList;
	
	//인덱스의 G코스트를 담을 배열
	vector<int>		m_vecNodeG;

	int				m_iStartIdx;
};

