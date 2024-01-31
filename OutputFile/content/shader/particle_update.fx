#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE

#include "value.fx"
#include "struct.fx"

RWStructuredBuffer<tParticle> g_ParticleBuffer : register(u0);

#define MAX_COUNT g_int_0

[numthreads(1024, 1, 1)]
void CS_ParticleUpdate(uint3 id : SV_DispatchThreadID)
{
    g_ParticleBuffer[id.x].vWorldPos.y = 2000.f;
}





#endif