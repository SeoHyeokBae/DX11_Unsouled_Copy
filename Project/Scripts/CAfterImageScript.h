#pragma once
#include <Engine\CScript.h>

#define POOLCOUNT 50

struct tAftInfo
{
	Ptr<CTexture>	Atlas;
	tAnimFrm		AnimFrm;
	Vec3			vPos;
	float			fAlhpa;
};

class CAfterImageScript :
    public CScript
{
private:
	CGameObject*								m_AfterImgObj;  // pool 관리 obj
	vector< std::pair<bool, CGameObject*>>		m_vSprite;		// 잔상 obj, 정보입력되면 true
	vector< tAftInfo>							m_vFrm;			// 애니메이션 현재 프레임 정보
	int											m_CurIdx;
	float										m_FadeSpeed;

public:
	int GetCurIdx() { return m_CurIdx; }
	void SetCurIdx(int _idx) { m_CurIdx = _idx; }

	void AddFrm();
	void Update();

	void CreateAfterImg(Ptr<CTexture> _tex, Vec3 _pos ,tAnimFrm _img)
	{
		tAftInfo info = {};
		info.Atlas = _tex;
		info.AnimFrm = _img;
		info.vPos = _pos;
		info.fAlhpa = 1.f;

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

