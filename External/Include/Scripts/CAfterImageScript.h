#pragma once
#include <Engine\CScript.h>
//#include <Engine/CAnim.h>
//#include <Engine\CTexture.h>

#define POOLCOUNT 50

struct tAftInfo
{
	tAnimFrm	AnimFrm;
	Vec3		vPos;
	float		fAlhpa;
};

class CAfterImageScript :
    public CScript
{
private:
	CGameObject*								m_AfterImgObj;  // pool ���� obj
	vector< std::pair<bool, CGameObject*>>		m_vSprite;		// �ܻ� obj, �����ԷµǸ� true
	vector< tAftInfo>							m_vFrm;			// �ִϸ��̼� ���� ������ ����
	Ptr<CTexture>								m_pTexture;
	int											m_CurIdx;
	float										m_FadeSpeed;

public:
	int GetCurIdx() { return m_CurIdx; }
	void SetCurIdx(int _idx) { m_CurIdx = _idx; }

	void Update();

	void CreateAfterImg(Ptr<CTexture> _tex, Vec3 _pos ,tAnimFrm _img)
	{
		m_pTexture = _tex;
		tAftInfo info = {};
		info.AnimFrm = _img;
		info.vPos = _pos;
		info.fAlhpa = 0.8f;

		m_vFrm.push_back(info);
	}

	void Clear();

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

	CLONE(CAfterImageScript);
public:
	CAfterImageScript();
	~CAfterImageScript();
};

