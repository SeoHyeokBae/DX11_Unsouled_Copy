#ifndef _SETCOLOR
#define _SETCOLOR

#include "value.fx"

#define TEX_WIDTH  g_int_0
#define TEX_HEIGHT g_int_1
RWTexture2D<float4> g_TargetTex : register(u0);

// 그룹 하나의 스레드 차원 수
// hlsl 5.0, 하나의 그룹이 가질수 있는 최대 스레드개수 1024개

// SV_GroupID : 소속 그룹 ID
// SV_GroupThreadID : 소속 그룹 내에서의 ID
// SV_GroupIndex : 소속 그룹내에서의 ID 를 1차원으로 변환
// SV_DispatchThreadID : 모든 그룹과 그룹내의 스레드를 통틀어서 전체 기준, 호출된 해당 스레드의 ID
[numthreads(32, 32, 1)]
void CS_SetColor(uint3 id : SV_DispatchThreadID)
{
    if (5 == id.x && 1 == id.y)
    {
        g_TargetTex[id.xy] = float4(0.f, 1.f, 1.f, 1.f);
    }
    else
    {
        g_TargetTex[id.xy] = float4(g_vec4_0.rgb, 1.f);
    }

}


#endif
