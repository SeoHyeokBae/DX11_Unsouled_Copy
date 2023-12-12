#pragma once

#define SINGLE(ClassType)	private:\
								ClassType();\
								~ClassType();\
							friend class CSingleton<ClassType>; // �����ڸ� private�� ���Ƽ� �̱��濡�� ������ ������ ����

#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()

#define KEY_CHECK(Key, State) CKeyMgr::GetInst()->GetKeyState(Key) == State
#define DT	CTimeMgr::GetInst()->GetDeltaTime()

#define KEY_TAP(Key) KEY_CHECK(Key, TAP)
#define KEY_PRESSED(Key) KEY_CHECK(Key, PRESSED)
#define KEY_RELEASED(Key) KEY_CHECK(Key, RELEASED)
#define KEY_NONE(Key) KEY_CHECK(Key, NONE)


enum class ASSET_TYPE
{
	MESH,
	MESHDATA,
	TEXTURE,
	MATERIAL,
	SOUND,
	COMPUTE_SHADER,
	GRAPHICS_SHADER,

};

enum class COMPONENT_TYPE
{
	TRANSFORM,  // ������Ʈ�� ��ġ, ũ��, ȸ��

	COLLIDER2D, // 2���� �浹ü
	COLLIDER3D, // 3���� �浹ü

	ANIMATOR2D, // ��������Ʈ Animation
	ANIMATOR3D, // Bone Skinning Animation

	LIGHT2D,	// 2���� ����
	LIGHT3D,	// 3���� ����

	CAMERA,		// ī�޶� ���
	
	
	// Render Component
	MESHRENDER,
	TILEMAP,
	PARTICLESYSTEM,
	SKYBOX,
	DECAL,
	LANDSCAPE,

	END,
};
