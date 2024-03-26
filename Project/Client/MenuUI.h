#pragma once
#include "UI.h"

enum class eCOMPONENT
{
       Transform,
       MeshRender,
       Collider2D,
       Movement,
       Animator2D,
       Light2D,
       TileMap,
       StateMachine,
       End,
};

class MenuUI :
    public UI
{

public:
    virtual void render() override;
    virtual void render_update() override;

private:
    void File();
    void Edit();
    void Level();
    void GameObject();
    void Asset();

public:
    MenuUI();
    ~MenuUI();
};

