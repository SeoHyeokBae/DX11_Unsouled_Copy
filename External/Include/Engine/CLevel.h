#pragma once
#include"CEntity.h"

class CLayer;
class CGameObject;


class CLevel :
	public CEntity
{
private:
	CLayer* m_arrLayer[LAYER_MAX];


public:
	void begin();
	void tick();
	void finaltick();

	void AddObject(CGameObject* _Object, int _LayerIdx , bool _bChildeMove = true);
	void AddObject(CGameObject* _Object, const wstring& _strLayerName, bool _bChildMove = true);

	CLayer* GetLayer(int _iLayerIdx) { return m_arrLayer[_iLayerIdx]; }
	CLayer* GetLayer(const wstring& _strLayerName);

	CGameObject* FindObjectByName(const wstring& _strName);
	void FindObjectsByName(const wstring& _strName, vector<CGameObject*>& _vecObj);

private:
	void clear();

public:
	CLevel();
	~CLevel();

	friend class CLevelMgr;
};

