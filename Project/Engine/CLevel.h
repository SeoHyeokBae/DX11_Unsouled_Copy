#pragma once
#include"CEntity.h"

class CLayer;
class CGameObject;

enum class LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
};

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
	
	void GetLayerName(vector<string>& _Out);
	void GetObjectName(vector<string>& _Out); // 모든 레이어에서의 obj name

private:
	void clear();

	CLONE(CLevel);
public:
	CLevel();
	CLevel(const CLevel& _OriginLevel);
	~CLevel();

	friend class CLevelMgr;
};

