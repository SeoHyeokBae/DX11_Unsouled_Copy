#pragma once
#include "CComponent.h"

enum class PROJ_TYPE
{
    ORTHOGRAPHIC,   // ���� ����
    PERSPECTIVE,    // ���� ����
};

class CCamera :
    public CComponent
{
private:
	PROJ_TYPE   m_ProjType;		//�������

	// ��������(Perspective)
	float		m_FOV;          // �þ� ��
			
	// ��������(Orthographic)
	float		m_Width;        // �������� ���� ����
	float		m_Scale;        // �������� ����

	// Both
	float		m_AspectRatio;  // ��Ⱦ��, ���� ���μ��� ����
	float		m_Far;          // ���� �ִ� �Ÿ�

	// ��ȯ ���
	Matrix		m_matView;      // ī�޶� �� 
	Matrix		m_matProj;

	UINT		m_LayerCheck;

public:
	PROJ_TYPE GetProjType() { return m_ProjType; }
	void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }

	float GetScale() { return m_Scale; }
	void SetScale(float _Scale) { m_Scale = _Scale; }

	float GetFOV() { return m_FOV; }
	void SetFOV(float _FOV) { m_FOV = _FOV; }

	void SetCameraPriority(int _Priority);
	void LayerCheck(UINT _LayerIdx, bool _bCheck);
	void LayerCheck(const wstring& _strLayerName, bool _bCheck);
	void LayerCheckAll() { m_LayerCheck = 0xffffffff; }

public:
    virtual void finaltick() override;
	void render();

public:
    CCamera();
    ~CCamera();
};

