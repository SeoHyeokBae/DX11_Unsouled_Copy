#pragma once
#include "singleton.h"

#include "CAsset.h"


class CMesh;
class CGraphicsShader;


class CAssetMgr :
	public CSingleton<CAssetMgr>
{
	SINGLE(CAssetMgr);
private:
	map<wstring, CAsset*>	m_mapAsset[(UINT)ASSET_TYPE::END];



public:
	void init();


public:
	template<typename T>
	void AddAsset(const wstring& _strKey, T* _Asset);

	template<typename T>
	T* FindAsset(const wstring& _strKey);



};

template<typename T>
inline void CAssetMgr::AddAsset(const wstring& _strKey, T* _Asset)
{
	const type_info& info = typeid(T);

	ASSET_TYPE Type = ASSET_TYPE::END;

	if (&info == &typeid(CMesh))
		Type = ASSET_TYPE::MESH;
	else if (&info == &typeid(CGraphicsShader))
		Type = ASSET_TYPE::GRAPHICS_SHADER;

	map<wstring, CAsset*>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);
	assert(iter == m_mapAsset[(UINT)Type].end());

	m_mapAsset[(UINT)Type].insert(make_pair(_strKey, _Asset));
}

template<typename T>
inline T* CAssetMgr::FindAsset(const wstring& _strKey)
{

	return nullptr;
}