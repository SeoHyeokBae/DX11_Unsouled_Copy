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
        // 점과 광원과의 내적으로 사이각을구함
        // cos(theta) -> acos(cos(theta)) = theta
        // degrees(theta) : 사이각라디안을 도로 변환
        
        
        float fAttenu = 1.f;
        
        float HalfAngle = info.fAngle / 2.f;
        
        // 점과의 거리벡터, 방향벡터
        float2 TargetDir = normalize(_WorldPos.xy - info.vWorldPos.xy);
        float3 TargetVector = _WorldPos - info.vWorldPos;
        
        // 광원과 점과의 거리
        float CenterDis = dot(TargetVector.xy, info.vWorldDir.xy);
        
        // 광원과 점과의 각도
        float LightAngle = degrees(acos(dot(TargetDir, info.vWorldDir.xy)));
        
        // case 2 180도각도까지만 가능
        //if (CenterDis < 0.f || CenterDis > info.fRadius ) // 거리를 벗어남
        //    fAttenu = 0.f;
        //else if (LightAngle > HalfAngle)
        //    fAttenu = 0.f;
        //else
        //{
        //    float fDist = distance(info.vWorldPos.xy, _WorldPos.xy);

        //    float fTheta = (fDist / info.fRadius) * (PI / 2.f);
        //    fAttenu = saturate(cos(fTheta));
        //}
            
        //    _output.vColor += info.Color.vColor * fAttenu;
        
        // case 1 360도 가능
        float fDist = distance(info.vWorldPos.xy, _WorldPos.xy);
        if (fDist < info.fRadius)
        {
            if (LightAngle < HalfAngle)
            {
                float fTheta = (fDist / info.fRadius) * (PI / 2.f);
                fAttenu = saturate(cos(fTheta));
                _output.vColor += info.Color.vColor * fAttenu;
            }
            
        }
    }
}

#endif