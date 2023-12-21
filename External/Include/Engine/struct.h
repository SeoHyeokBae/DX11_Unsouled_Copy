#pragma once

// 3���� ������ ��ġ�Ǵ� ����
struct Vtx
{
	Vec3 vPos;		// ������ ��ǥ
	Vec4 vColor;	// ������ ���� ����
	Vec2 vUV;		// UV ��ǥ�� or Texture Coodinate
};


// ==================
// ������� ���� ����ü
// ==================
struct tTransform
{
	// ����(��) ��ǥ�� -> ���� -> �� -> ���� ��ǥ��� ���������� ��ȯ
	Matrix	matWorld;
	Matrix	matWorldInv;

	Matrix	matView;
	Matrix	matViewInv;

	Matrix	matProj;
	Matrix	matProjInv;

	Matrix	matWV;		// World + View 
	Matrix	matWVP;		// World + View + Proj
};

extern tTransform g_Transform;