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
	Matrix	matWorld;
	Matrix	matView;
	Matrix	matProj;
	//Vec4	vWorldPos;
	//Vec4    vWorldScale;
	//Vec4	vWorldRotation;
};

extern tTransform g_Transform;