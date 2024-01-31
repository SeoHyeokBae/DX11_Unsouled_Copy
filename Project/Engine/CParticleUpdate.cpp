#include "pch.h"
#include "CParticleUpdate.h"

CParticleUpdate::CParticleUpdate()
	: CComputeShader(1024, 1 ,1)
{
	Create(L"shader\\particle_update.fx", "CS_ParticleUpdate");
}

CParticleUpdate::~CParticleUpdate()
{
}

int CParticleUpdate::UpdateData()
{
	return 0;
}

void CParticleUpdate::Clear()
{
}
