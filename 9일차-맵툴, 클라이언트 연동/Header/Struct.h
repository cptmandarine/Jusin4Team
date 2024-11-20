#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vLook;
	D3DXVECTOR3		vSize;

	D3DXMATRIX		matWorld;

}INFO;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9		pTexture;   // �� ��ü

	D3DXIMAGE_INFO			tImgInfo;	// ����ü

}TEXINFO;



typedef struct tagTile
{
	D3DXVECTOR3	vPos;				// Ÿ���� ���� ��ǥ
	D3DXVECTOR2 vSize;				// Ÿ���� ����, ���� ������

	BYTE		byOption;			// 0, 1��(��ֹ�)
	BYTE		byDrawID;			// �� �� Ÿ�� �̹���

	int			iIndex;
	int			iParentIdx;
}TILE;

typedef struct tagDeco
{
	D3DXVECTOR3	vPos;
	int	 byIndex;
	BYTE byDrawID;
}DECO;

typedef	struct tagUnitData
{
#ifdef _AFX
	CString strName;
#else
	wstring	strName;
#endif

	int		iHp;
	int		iAttack;

}UNITDATA;

typedef struct tagTexturePath
{
	wstring		wstrObjKey		= L"";
	wstring		wstrStateKey	= L"";
	wstring		wstrPath		= L"";
	int			iCount			= 0;

}IMGPATH;

//typedef	struct tagFrame
//{
//	float		fFrame; // �������� ���� ���� ����
//	float		fMax;	// �ִ� �̹����� ������ ��
//}FRAME;

typedef struct tagFrame
{
	int iFrameStart;
	int iFrameEnd;

	ULONGLONG ullTime;
	ULONGLONG ullSpeed;
}FRAME;

static D3DXVECTOR3		Get_Mouse()
{
	POINT	Pt{};

	GetCursorPos(&Pt);
	ScreenToClient(g_hWnd, &Pt);

	return D3DXVECTOR3((float)Pt.x, (float)Pt.y, 0.f);
}