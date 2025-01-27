#pragma once
#include "CEntity.h"

#define GET_COMPONENT(Type, TYPE) class C##Type* Type() { return (C##Type*)m_arrCom[(UINT)COMPONENT_TYPE::##TYPE]; }

class CComponent;
class CRenderComponent;
class CScript;
class CCamera;
class CCollider2D;
class CLight2D;
class CStateMachine;

class CGameObject :
    public CEntity
{
private:
    CComponent*             m_arrCom[(UINT)COMPONENT_TYPE::END];
    CRenderComponent*       m_RenderCom;

    vector<CScript*>        m_vecScript;
    vector<CGameObject*>    m_vecChild;

    CGameObject*            m_Parent;

    int                     m_iLayerIdx;    // 오브젝트가 소속되어있는 Layer의 Idx

    bool                    m_bShadow;
    bool                    m_bAfterAct;    // 잔상여부 (스크립트필요)
    bool                    m_bDead;

    eDIR                    m_Dir;

public:
    void begin();
    void tick();
    virtual void finaltick();
    void render();

public:
    void AddComponent(CComponent* _Component);
    CComponent* GetComponent(COMPONENT_TYPE _Type) { return m_arrCom[(UINT)_Type]; }
    CRenderComponent* GetRenderComponent() { return m_RenderCom; }

    GET_COMPONENT(Transform, TRANSFORM);     // Transform()
    GET_COMPONENT(MeshRender, MESHRENDER);   // MeshRnder()
    GET_COMPONENT(Camera, CAMERA);           // Camera()
    GET_COMPONENT(StateMachine, STATEMACHINE);
    GET_COMPONENT(Collider2D, COLLIDER2D);   // Collider2D()
    GET_COMPONENT(Animator2D, ANIMATOR2D);   // Animator2D()
    GET_COMPONENT(Light2D, LIGHT2D);       
    GET_COMPONENT(TileMap, TILEMAP);       
    GET_COMPONENT(Movement, MOVEMENT);       

    int GetLayerIdx() { return m_iLayerIdx; }

    CGameObject* GetParent() { return m_Parent; }
    vector<CGameObject*>& GetChild() { return m_vecChild; }

    CGameObject* GetShadow() 
    {
        for (size_t i = 0; i < m_vecChild.size(); i++)
        {
            if (m_vecChild[i]->GetName() == L"Shadow")
                return m_vecChild[i];
        }

        return nullptr;
    }

    CGameObject* GetChildObj(const wstring& _strName)
    {
        for (size_t i = 0; i < m_vecChild.size(); i++)
        {
            if (m_vecChild[i]->GetName() == _strName)
                return m_vecChild[i];
        }

        return nullptr;
    }

    const vector<CScript*>& GetScripts() { return m_vecScript; }
    CRenderComponent* GetRenderComopnent() { return m_RenderCom; }

    template<typename T>
    T* GetScript()
    {
        for (size_t i = 0; i < m_vecScript.size(); ++i)
        {
            if (dynamic_cast<T*>(m_vecScript[i]))
                return (T*)m_vecScript[i];
        }
        return nullptr;
    }

    int DisconnectWithParent();
    int DisconnectWithLayer();


public:
    void AddChild(CGameObject* _Child);

    bool IsDead() { return m_bDead; }

    void Destroy();

    bool IsAncestor(CGameObject* _Other);

    eDIR GetDir() { return m_Dir; }
    void SetDir(eDIR _dir) { m_Dir = _dir; }

    void SelfShadow(bool _bool) { m_bShadow = _bool; }
    bool IsShadow() { return m_bShadow; }
    bool IsAfterImgAct() { return m_bAfterAct; }
    void SetAfterImgAct(bool _active) { m_bAfterAct = _active; }

    CLONE(CGameObject);
public:
    CGameObject();
    CGameObject(const CGameObject& _OriginObject);
    ~CGameObject();

    friend class CLayer;
    friend class CTaskMgr;
};

