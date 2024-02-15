#pragma once
#include "CEntity.h"

#define GET_COMPONENT(Type, TYPE) class C##Type* Type() { return (C##Type*)m_arrCom[(UINT)COMPONENT_TYPE::##TYPE]; }

class CComponent;
class CRenderComponent;
class CScript;
class CCamera;
class CCollider2D;
class CLight2D;

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

    bool                    m_bDead;

public:
    void begin();
    void tick();
    void finaltick();
    void render();

public:
    void AddComponent(CComponent* _Component);
    CComponent* GetComponent(COMPONENT_TYPE _Type) { return m_arrCom[(UINT)_Type]; }
    CRenderComponent* GetRenderComponent() { return m_RenderCom; }

    GET_COMPONENT(Transform, TRANSFORM);     // Transform()
    GET_COMPONENT(MeshRender, MESHRENDER);   // MeshRnder()
    GET_COMPONENT(Camera, CAMERA);           // Camera()
    GET_COMPONENT(Collider2D, COLLIDER2D);   // Collider2D()
    GET_COMPONENT(Animator2D, ANIMATOR2D);   // Animator2D()
    GET_COMPONENT(Light2D, LIGHT2D);       
    GET_COMPONENT(TileMap, TILEMAP);       

    int GetLayerIdx() { return m_iLayerIdx; }

    CGameObject* GetParent() { return m_Parent; }
    vector<CGameObject*>& GetChild() { return m_vecChild; }

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

    void AddChild(CGameObject* _Child);
    bool IsDead() { return m_bDead; }

    void Destroy();

    bool IsAncestor(CGameObject* _Other);
public:
    CGameObject();
    ~CGameObject();

    friend class CLayer;
    friend class CTaskMgr;
};

