#ifndef _FUNC
#define _FUNC

#include "struct.fx"
#include "value.fx"

//inout = & 
void CalLight2D(float3 _WorldPos, int _LightIdx, inout tLightColor _output)
{
    
    // ���� �����ų ������ ����
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
        // Point Light ���� ����
        // ������ Ȱ��, ���� üũ
        // ���� �������� �������� ���̰�������
        // cos(theta) -> acos(cos(theta)) = theta
        // degrees(theta) : ���̰������� ���� ��ȯ
        
        
        float fAttenu = 1.f;
        
        float HalfAngle = info.fAngle / 2.f;
        
        // ������ �Ÿ�����, ���⺤��
        float2 TargetDir = normalize(_WorldPos.xy - info.vWorldPos.xy);
        float3 TargetVector = _WorldPos - info.vWorldPos;
        
        // ������ ������ �Ÿ�
        float CenterDis = dot(TargetVector.xy, info.vWorldDir.xy);
        
        // ������ ������ ����
        float LightAngle = degrees(acos(dot(TargetDir, info.vWorldDir.xy)));
        
        // case 2 180������������ ����
        //if (CenterDis < 0.f || CenterDis > info.fRadius ) // �Ÿ��� ���
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
        
        // case 1 360�� ����
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