#ifndef _FUNC
#define _FUNC

#include "struct.fx"
#include "value.fx"

//inout = & 
void CalLight2D(float3 _WorldPos, int _LightIdx, inout tLightColor _output)
{
    
    // 빛을 적용시킬 광원의 정보
    tLightInfo info = g_Light2D[_LightIdx];
    
    // Directional Light
    if (0 == g_Light2D[0].LightType)
    {
        _output.vAmbient += info.Color.vAmbient;
    }
    
    // Point Light
    else if (1 == info.LightType)
    {
        float fAttenu = 1.f;
        
        float fDist = distance(info.vWorldPos.xy, _WorldPos.xy);
        if (fDist < info.fRadius)
        {
            if (g_int_0)
            {
                float fTheta = (fDist / info.fRadius) * (PI / 2.f);
                fAttenu = saturate(cos(fTheta));
            }
            else
            {
                fAttenu = saturate(1.f - fDist / g_Light2D[0].fRadius);
            }
            
            _output.vColor += info.Color.vColor * fAttenu;
        }
    }
    
    // Spot Light
    else
    {
        // Point Light 거의 유사
        // 내적을 활용, 각도 체크
        // 간단한 영상 찍어서 올리기
        // 광원을 회전시키기
    }
}

#endif