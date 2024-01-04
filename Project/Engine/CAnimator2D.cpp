#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim.h"

CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
{
}

CAnimator2D::~CAnimator2D()
{
}

void CAnimator2D::finaltick()
{
}

void CAnimator2D::Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _LeftTop,
	Vec2 _vSliceSize, Vec2 _OffsetSize, int _FrmCount, float _FPS)
{
	CAnim* pAnim = FindAnim(_strKey);
	assert(!pAnim);

	pAnim = new CAnim;
	pAnim->Create(this, _AtlasTex, _LeftTop, _vSliceSize, _OffsetSize, _FrmCount, _FPS);
	m_mapAnim.insert(make_pair(_strKey, pAnim));
}

CAnim* CAnimator2D::FindAnim(const wstring& _strKey)
{
	map<wstring, CAnim*>::iterator iter = m_mapAnim.find(_strKey);

	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}