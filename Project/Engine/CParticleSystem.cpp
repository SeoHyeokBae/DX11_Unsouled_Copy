#include "pch.h"
#include "CParticleSystem.h"
 
#include "CStructuredBuffer.h"


CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
{
	tParticle arrParticle[5] = {};

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), 5, SB_TYPE::READ_WRITE, true, arrParticle);
}

CParticleSystem::~CParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;
}


void CParticleSystem::finaltick()
{
	// 파티컬 업데이트 컴퓨트 쉐이드
}

void CParticleSystem::render()
{
}
