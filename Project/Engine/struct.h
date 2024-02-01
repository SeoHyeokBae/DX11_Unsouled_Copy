#pragma once

// 3차원 공간에 배치되는 정점
struct Vtx
{
	Vec3 vPos;		// 정점의 좌표
	Vec4 vColor;	// 정점의 색상 정보
	Vec2 vUV;		// UV 좌표계 or Texture Coodinate
};

struct tDebugShapeInfo
{
	DEBUG_SHAPE	eShape;

	Vec3		vWorldPos;
	Vec3		vWorldScale;
	Vec3		vWorldRot;
	Matrix		matWorld;

	Vec3		vColor;
	float		fLifeTime;
	float		fDuration;
	bool		bDepthTest;
};

struct tLightInfo
{
	Vec4	vColor;		// 빛의 색상	- 광원의 순수 색상
	Vec4	vSpecular;	// 반사광	- 광원이 물체 표변에 반사된 색상
	Vec4	vAmbient;	// 환경광(주변광) - 광원에 의해서 보장되는 최소한의 빛

	Vec3	vWorldPos;	// 광원의 위치
	Vec3	vWorldDir;	// 광원이 향하는 방향
	float	fRadius;	// 광원의 영향 반경, 거리 정보
	float	fAngle;		// 광원의 범위 각도

	int		LightType;	// 광원 타입

	Vec3	vPadding;
};

struct tPixel
{
	BYTE r, g, b, a;
};

struct tParticle
{
	Vec4	vWorldPos;		// 위치
	Vec4	vWorldScale;	// 크기
	Vec4	vWorldRotation;	// 회전값
	Vec4	vVelocity;		// 속도
	Vec4	vColor;			// 색상

	float	Mass;			// 질량
	float	Age;			// 현재 나이
	float	Life;			// 수명
	int		Active;			// 활성화, 비활성화 여부
};

struct tParticleModule
{
	// Sapwn 모듈
	Vec4	vSpawnColor;	// 초기 컬러
	Vec4	vSpawnMinScale;	// 초기 최소 크기
	Vec4	vSpawnMaxScale;	// 초기 최대 크기

	float	MinLife;		// 최소 수명
	float	MaxLife;		// 최대 수명
	int		SpawnRate;		// 초당 생성 개수
	int		SpaceType;		// 좌표계(0 : LocalSpace, 1 : WorldSpace)
	int		SpawnShape;		// 스폰 범위 (0 : Sphere, 1 : Box)
	float	Radius;			// SpawnShape 가 Sphere 인 경우, 반지름 길이
	Vec4	vSpawnBoxScale;	// SpawnShape 가 Box 인 경우, Box의 크기
	Vec2	padding;

	int arrModuleCheck[(UINT)PARTICLE_MODULE::END];
};

struct tSpawnCount
{
	int	SpawnCount;
	int vPadding[3];
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

	int	bTex[TEX_PARAM::END];

	int iPadding[2];
};

struct tAnimData2D
{
	Vec2 vLeftTop;
	Vec2 vSliceSize;
	Vec2 vBackGround;
	Vec2 vOffset;
	int  UseAnim2D;
	Vec3 iPadding;
};

struct tGlobalData
{
	Vec2	g_RenderResolution; // 렌더링 해상도
	float	g_dt;				// DeltaTime
	float	g_time;				// 누적 시간
	int		g_Light2DCount;		// 2D 광원 개수
	int		g_Light3DCount;		// 3D 광원 개수
	Vec2	g_vPadding;
};

extern tGlobalData g_global;