#pragma once
#include "CComponent.h"
class CMovement :
    public CComponent
{
private:
    // 누적된 힘
    Vec2    m_Force;

    // 현재 가속도
    Vec2    m_Accel;

    // 속도
    Vec2    m_Velocity;

    // 질량
    float   m_Mass;

    // 보장된 초기 속력
    float   m_InitSpeed;

    // 최대 속력
    float   m_MaxSpeed;

    // 마찰 크기
    float   m_FrictionScale;

    // 땅위인지 아닌지
    bool    m_Ground;


public:
    void AddForce(Vec2 _vForce)
    {
        m_Force += _vForce;
    }

    Vec2 GetForce()
    {
        return m_Force;
    }

    void SetVelocity(Vec2 _vVelocity)
    {
        m_Velocity = _vVelocity;
    }

    void AddVelocity(Vec2 _vAdd)
    {
        m_Velocity += _vAdd;
    }

    Vec2 GetVelocity()
    {
        return m_Velocity;
    }

    void SetMass(float _Mass)
    {
        m_Mass = _Mass;
    }

    float GetMass()
    {
        return m_Mass;
    }

    void SetInitSpeed(float _Speed)
    {
        m_InitSpeed = _Speed;
    }

    void SetMaxSpeed(float _Speed)
    {
        m_MaxSpeed = _Speed;
    }

    float GetInitSpeed()
    {
        return m_InitSpeed;
    }

    float GetMaxSpeed()
    {
        return m_MaxSpeed;
    }

    void SetFrictionScale(float _F)
    {
        m_FrictionScale = _F;
    }

    float GetFrictionScale()
    {
        return m_FrictionScale;
    }

    bool IsGround()
    {
        return m_Ground;
    }

    void SetGround(bool _Ground)
    {
        if (_Ground)
        {
            m_Velocity.y = 0.f;
        }

        m_Ground = _Ground;
    }



public:
    virtual void finaltick() override;
    //virtual void begin() {}
    //virtual void tick() {}
    //virtual void finaltick() = 0;
    //virtual void UpdateData() {};

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CMovement);
public:
    CMovement();
    ~CMovement();
};

