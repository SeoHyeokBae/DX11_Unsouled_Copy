#ifndef _STD2D  //#indef #define #endif == #pragma once
#define _STD2D

#include "value.fx"
#include "func.fx"



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
        
    float3 vWorldPos : POSITIONT;
};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // 로컬(모델) 좌표를 -> 월드 -> 뷰 -> 투영 좌표계로 순차적으로 변환
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP); // 월드
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    
    return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);

    if(g_UseAnim2D)
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
        
    }
    else if(g_btex_0)
    {
        
        //vColor = g_tex_0.Sample(g_sam_1, _in.vUV + float2(g_time * 0.1, 0.f));
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        
        //saturate 0 ~ 1 을 넘지 않게 보정
        float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f);
        
        if (fAlpha < 0.1f)
        {
            // 픽셀 쉐이더를 중간에 폐기처리
            discard; //clip(-1);            
        }
    }
    
    if (g_int_0 == 10) // 그림자
        vColor *= float4(0.f, 0.f, 0.f, 0.6f);
    
     // 광원 처리
    // 광원의 타입별 처리
    // 광원이 여러개일 때 처리
    //g_Light2DCount;    
    tLightColor LightColor = (tLightColor) 0.f;
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalLight2D(_in.vWorldPos, i, LightColor);
    }
    
    vColor.rgb *= (LightColor.vColor.rgb + LightColor.vAmbient.rgb);
    
    if (0.f == vColor.a)
        discard;
    
    //Paper Burn
    //float x = g_NoiseTex.Sample(g_sam_0, _in.vUV).x;
    //if (0 > x - g_float_1)
    //{
    //    discard;
    //}
    
    if (1 == g_int_3)
    {
        vColor.rg = 1.f;
    }
    
    return vColor;
}

// EffectShader
float4 PS_Std2D_Effect(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 0.f);
    
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
        
    }
    else if (g_btex_0)
    {
        
        //vColor = g_tex_0.Sample(g_sam_1, _in.vUV + float2(g_time * 0.1, 0.f));
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        
        //saturate 0 ~ 1 을 넘지 않게 보정
        float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f);
        
        if (fAlpha < 0.1f)
        {
            // 픽셀 쉐이더를 중간에 폐기처리
            discard; //clip(-1);            
        }
    }
    
    if (g_int_0 == 10) // 그림자
        vColor *= float4(0.f, 0.f, 0.f, 0.6f);
    
     // 광원 처리
    // 광원의 타입별 처리
    // 광원이 여러개일 때 처리
    //g_Light2DCount;    
    tLightColor LightColor = (tLightColor) 0.f;
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalLight2D(_in.vWorldPos, i, LightColor);
    }
    
    vColor.rgb *= (LightColor.vColor.rgb + LightColor.vAmbient.rgb);
    
    if (0.f == vColor.a)
        discard;
    
    return vColor;
}

// AfterImage
float4 PS_Std2D_AftImg(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 0.f);
    
    if (g_btex_1)
    {
        float2 vBackgroundLeftTop = g_vec2_0 + (g_vec2_2 / 2.f) - (g_vec2_1 / 2.f);
        vBackgroundLeftTop -= g_vec2_3;
        float2 vUV = vBackgroundLeftTop + (g_vec2_1 * _in.vUV);
        
        if (vUV.x < g_vec2_0.x || (g_vec2_0.x + g_vec2_2.x) < vUV.x
            || vUV.y < g_vec2_0.y || (g_vec2_0.y + g_vec2_2.y) < vUV.y)
        {
            discard;
        }
        else
        {
            vColor = g_tex_1.Sample(g_sam_1, vUV);
            vColor *= g_vec4_0;
        }
    }

    return vColor;
}

#endif
