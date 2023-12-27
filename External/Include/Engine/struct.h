#pragma once

// 3차원 공간에 배치되는 정점
struct Vtx
{
	Vec3 vPos;		// 정점의 좌표
	Vec4 vColor;	// 정점의 색상 정보
	Vec2 vUV;		// UV 좌표계 or Texture Coodinate
};


// ==================
// 상수버퍼 대응 구조체
// ==================
struct tTransform
{
	// 로컬(모델) 좌표를 -> 월드 -> 뷰 -> 투영 좌표계로 순차적으로 변환
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

struct tMtrlConst
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];
};