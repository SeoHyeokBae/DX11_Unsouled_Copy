#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE

#include "value.fx"
#include "struct.fx"


StructuredBuffer<tParticleModule> g_Module : register(t20);
RWStructuredBuffer<tParticle> g_ParticleBuffer : register(u0);
RWStructuredBuffer<uint3> g_SpawnCount : register(u1);

#define MAX_COUNT g_int_0
#define SpawnCount g_SpawnCount[0].x

[numthreads(1024, 1, 1)]
void CS_ParticleUpdate(uint3 id : SV_DispatchThreadID)
{
    if (0 < SpawnCount)
    {
       // Atomic ÇÔ¼ö
       // Interlock
        
        if (0 == g_ParticleBuffer[id.x].Active)
        {
            g_ParticleBuffer[id.x].Active = 1;
            SpawnCount -= 1;
        }
    }
    
    g_ParticleBuffer[id.x].
        vWorldPos.y += g_dt * 100.f;
 }





#endif