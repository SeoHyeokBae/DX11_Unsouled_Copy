#pragma once
#include "CAsset.h"

typedef void(*SAVE_TYPE)(CGameObject*, FILE*);
typedef CGameObject* (*LOAD_TYPE)(FILE*);

class CPrefab :
    public CAsset
{
public:
    static SAVE_TYPE GAMEOBJECT_SAVE;
    static LOAD_TYPE GAMEOBJECT_LOAD;

private:
    CGameObject* m_ProtoObj;

public:
    void SetGameObject(CGameObject* _Object);
    CGameObject* Instantiate();

    CGameObject* GetProtoGameObj() { return m_ProtoObj; }

    CLONE_DISABLE(CPrefab);

public:
    virtual int Save(const wstring& _strRelativePath);
    virtual int Load(const wstring& _strFilePath);

public:
    CPrefab(bool _Engine = false);
    CPrefab(CGameObject* _Proto, bool _Engine);
    ~CPrefab();
};

