#pragma once
#include "CComponent.h"

enum class eLight2DDir
{
    RIGHT,
    LEFT,
    UP,
    DOWN
};

class CLight2D :
    public CComponent
{
private:
    tLightInfo      m_Info;
    Vec3            m_OffsetPos;
    eLight2DDir     m_Dir;

public:
    virtual void finaltick() override;

public:
    const tLightInfo& GetLightInfo() { return m_Info; }
    void SetLightColor(Vec3 _vColor) { m_Info.vColor = _vColor; }
    void SetSpecular(Vec3 _vSpec) { m_Info.vSpecular = _vSpec; }
    void SetAmbient(Vec3 _vAmb) { m_Info.vAmbient = _vAmb; }

    Vec4 GetLightColor(Vec3 _vColor) { return m_Info.vColor; }
    Vec4 GetSpecular(Vec3 _vSpec) { return m_Info.vSpecular; }
    Vec4 GetAmbient(Vec3 _vAmb) { return m_Info.vAmbient; }


    void SetLightType(LIGHT_TYPE _type);
    void SetLightDir(eLight2DDir _dir) { m_Dir = _dir; }
    void SetOffset(Vec3 _vOffset) { m_OffsetPos = _vOffset; }
    void SetRadius(float _Radius);
    void SetAngle(float _Angle);


    LIGHT_TYPE GetLightType() { return (LIGHT_TYPE)m_Info.LightType; }
    eLight2DDir GetLightDir() { return m_Dir; }
    Vec3 GetOffsetPos() { return m_OffsetPos; }
    float GetRadius() { return m_Info.fRadius; }
    float GetAngle() { return m_Info.fAngle; }

    void UpdateLightDir(eLight2DDir);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CLight2D);
public:
    CLight2D();
    ~CLight2D();

};

