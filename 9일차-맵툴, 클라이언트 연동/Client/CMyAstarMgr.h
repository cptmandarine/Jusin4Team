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
	// f�� index
	priority_queue<pair<int, int>>	m_OpenPQ;
	//visted�迭�� ����
	list<int>		m_CloseList;
	list<TILE*>		m_BestList;
	
	//�ε����� G�ڽ�Ʈ�� ���� �迭
	vector<int>		m_vecNodeG;

	int				m_iStartIdx;
};

