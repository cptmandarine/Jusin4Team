#include "pch.h"
#include "CMyAstarMgr.h"
#include "ObjMgr.h"

IMPLEMENT_SINGLETON(CMyAstarMgr)

CMyAstarMgr::CMyAstarMgr()
{
}

CMyAstarMgr::~CMyAstarMgr()
{
	Release();
}

void CMyAstarMgr::Start_Astar(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vGoal)
{
	Release();

	CObj* pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();

	m_iStartIdx = Get_TileIndex(vStart);

	int		iGoalIdx = Get_TileIndex(vGoal);

	if (0 > m_iStartIdx ||
		0 > iGoalIdx ||
		(size_t)m_iStartIdx >= vecTile.size() ||
		(size_t)iGoalIdx >= vecTile.size())
	{
		return;
	}

	//안찾아
	if (m_iStartIdx == iGoalIdx)
		return;
	if (0 != vecTile[iGoalIdx]->byDrawID)
		return;


	if (Make_Route(m_iStartIdx, iGoalIdx))
	{
		Make_BestList(m_iStartIdx, iGoalIdx);
	}
}

bool CMyAstarMgr::Make_Route(int iStartIdx, int iGoalIdx)
{

	CObj* pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();
	vector<list< TILE*>>& vecAdj = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecAdj();
	//Release();
	//부모들도 갱신
	for (int i = 0; i < vecTile.size(); ++i)
	{
		vecTile[i]->iParentIdx = -1;
	}
	vecTile[iStartIdx]->iParentIdx = iStartIdx;
	m_vecNodeG.reserve(vecTile.size());
	m_vecNodeG.resize(vecTile.size());
	fill(m_vecNodeG.begin(), m_vecNodeG.end(), 1e9);
	m_vecNodeG[iStartIdx] = 0;

	m_OpenPQ.push({ 0, iStartIdx });
	
	while (!m_OpenPQ.empty())
	{
		int iCurFCost = m_OpenPQ.top().first;
		int iCurIndex = m_OpenPQ.top().second;
		m_OpenPQ.pop();
		//도착지점을 찾으면 종료
		if (iCurIndex == iGoalIdx)
		{
			return true;
		}
		//방문한 배열이면 continue
		if (Check_Close(iCurIndex))
		{
			continue;
		}

		m_CloseList.push_back(iCurIndex);
		//해당 인접 인덱스로부터 f = g + h 구하기
		for (auto iter = vecAdj[iCurIndex].begin(); iter != vecAdj[iCurIndex].end(); ++iter)
		{
			D3DXVECTOR3 vHuristic = vecTile[iGoalIdx]->vPos - (*iter)->vPos;
			int hCost = D3DXVec3Length(&vHuristic);
			int gCost = m_vecNodeG[iCurIndex] + 1;
			int fCost = gCost + hCost;
			//갱신 시 넣기
			if (fCost < m_vecNodeG[(*iter)->iIndex])
			{
				m_vecNodeG[(*iter)->iIndex] = fCost;
				//갱신되면 부모 인덱스를 갱신해준다. 
				// 다음 인덱스의 뿌리는 바로 나야
				vecTile[(*iter)->iIndex]->iParentIdx = iCurIndex;
				m_OpenPQ.push({ -fCost, (*iter)->iIndex });
			}
		}
	}

	return false;
}

void CMyAstarMgr::Make_BestList(int iStartIdx, int iGoalIdx)
{
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->Get_VecTile();

	// 목표 지점부터 시작해서 부모를 따라가며 경로를 구함
	int iCurIndex = iGoalIdx;

	while (iCurIndex != iStartIdx) 
	{
		m_BestList.push_front(vecTile[iCurIndex]);

		iCurIndex = vecTile[iCurIndex]->iParentIdx; 
		if (iCurIndex == -1) break;
	}

	m_BestList.push_front(vecTile[iStartIdx]); // 시작 노드도 추가

	/*for (auto& iter : m_BestList)
	{
		int a = iter->iIndex;
	}*/
}

int CMyAstarMgr::Get_TileIndex(const D3DXVECTOR3& vPos)
{
	CObj* pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();

	for (size_t index = 0; index < vecTile.size(); ++index)
	{
		if (Picking_Dot(vPos, index))
			return index;
	}

	return -1;		// 잘못된 픽킹 처리
}

bool CMyAstarMgr::Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex)
{
	CObj* pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();

	// 12시, 3시, 6시, 9시
	D3DXVECTOR3	vPoint[4]
	{
		{ vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y + (TILECY / 2.f),  0.f },
		{ vecTile[iIndex]->vPos.x + (TILECX / 2.f), vecTile[iIndex]->vPos.y , 0.f },
		{ vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y - (TILECY / 2.f),  0.f },
		{ vecTile[iIndex]->vPos.x - (TILECX / 2.f), vecTile[iIndex]->vPos.y , 0.f }
	};

	D3DXVECTOR3		vDir[4]
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	D3DXVECTOR3		vNormal[4]
	{
		{ -vDir[0].y, vDir[0].x, 0.f},
		{ -vDir[1].y, vDir[1].x, 0.f},
		{ -vDir[2].y, vDir[2].x, 0.f},
		{ -vDir[3].y, vDir[3].x, 0.f}

	};

	D3DXVECTOR3		vMouseDir[4]
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}

void CMyAstarMgr::Release()
{
	m_CloseList.clear();
	while (!m_OpenPQ.empty()) {
		m_OpenPQ.pop();
	}
	m_BestList.clear();
}

bool CMyAstarMgr::Check_Close(int iIndex)
{
	for (int& iCloseIdx : m_CloseList)
	{
		if (iCloseIdx == iIndex)
			return true;
	}

	return false;
}
