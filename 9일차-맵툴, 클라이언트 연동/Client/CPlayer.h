#pragma once
#include "Obj.h"
class CPlayer : public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();
public:
	enum PLAYERSTATE { STAND, WALK_LU, WALK_LD, WALK_RU, WALK_RD, END };
public:
	// CObj��(��) ���� ��ӵ�
	HRESULT Initialize(void) override;
	int Update(void) override;
	void Late_Update(void) override;
	void Render(void) override;
	void Release(void) override;
private:
	void Motion_Change();
	void Move_Route();
private:
	bool m_bRight;
	CONST TCHAR* m_pObjKey;
	CONST TCHAR* m_pFrameKey;
	PLAYERSTATE m_eCurState;
	PLAYERSTATE m_ePreState;
	bool m_Test;
};

