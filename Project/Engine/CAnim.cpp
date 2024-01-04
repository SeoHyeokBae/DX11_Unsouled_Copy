#include "pch.h"
#include "CAnim.h"


CAnim::CAnim()
	: m_Animator(nullptr)
	, m_CurFrmIdx(0)
	, m_bFinish(false)
{
}

CAnim::~CAnim()
{
}


void CAnim::Create(CAnimator2D* _Animator, Ptr<CTexture> _Atlas, Vec2 _vLeftTop, Vec2 _vSliceSize, Vec2 _vOffset, int _FrmCount, float _FPS)
{
	m_Animator = _Animator;
	m_AtlasTex = _Atlas;
	for (int i = 0; i < _FrmCount; ++i)
	{
		tAnimFrm frm = {};

		frm.vLeftTop;
		frm.vSlice;
		frm.vOffset;
		frm.Duration;

		m_vecFrm.push_back(frm);
	}
}