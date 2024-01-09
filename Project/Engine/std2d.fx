#ifndef _STD2D  //#indef #define #endif == #pragma once
#define _STD2D

#include "value.fx"

StructuredBuffer<float4> g_Data : register(t14);


struct VS_IN
{
    float3 vPos : POSITION; // Sementic
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // 로컬(모델) 좌표를 -> 월드 -> 뷰 -> 투영 좌표계로 순차적으로 변환
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP); // 월드
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    
    //uint width = 0;
    //uint height = 0;
    //g_tex_1.GetDimensions(width, height);
    
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (g_UseAnim2D)
    {
        float2 vBackgroundLeftTop = g_vLeftTop + (g_vSliceSize / 2.f) - (g_vBackground / 2.f);
        vBackgroundLeftTop -= g_vOffset;
        float2 vUV = vBackgroundLeftTop + (g_vBackground * _in.vUV);
        
        if (vUV.x < g_vLeftTop.x || (g_vLeftTop.x + g_vSliceSize.x) < vUV.x
            || vUV.y < g_vLeftTop.y || (g_vLeftTop.y + g_vSliceSize.y) < vUV.y)
        {
            //vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
        else
        {
            vColor = g_anim2d_tex.Sample(g_sam_1, vUV);
        }
        
        
        // background 제거
        float fAlpha = vColor.a;
        if (fAlpha < 0.1f)
        {
            // 픽셀 쉐이더를 중간에 폐기처리
            discard; //clip(-1);            
        }
    }
    else
    {
        if (g_btex_0)
        {
            vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
            
            // StructedBuffer 예시
           // vColor = g_Data[2];
           // vColor.a = 1.f;
        
            //saturate 0 ~ 1 을 넘지 않게 보정
            float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f);
        
            if (fAlpha < 0.1f)
            {
            // 픽셀 쉐이더를 중간에 폐기처리
                discard; //clip(-1);            
            }
        }
    }
    
    if (g_int_0)
    {
        vColor.r *= 2.f;
    }
    
    // 광원 처리
    // 광원의 타입별 처리
    // 광원이 여러개일 때 처리
    vColor.rgb *= g_Light2D[0].vAmbient.rgb;
    
    return vColor;
}

#endif
