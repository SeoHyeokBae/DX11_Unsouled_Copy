#pragma once
#include "CComponent.h"
class CMovement :
    public CComponent
{
private:
    // ������ ��
    Vec2    m_Force;

    // ���� ���ӵ�
    Vec2    m_Accel;

    // �ӵ�
    Vec2    m_Velocity;

    // ����
    float   m_Mass;

    // ����� �ʱ� �ӷ�
    float   m_InitSpeed;

    // �ִ� �ӷ�
    float   m_MaxSpeed;

    // ���� ũ��
    float   m_FrictionScale;

    // �������� �ƴ���
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

