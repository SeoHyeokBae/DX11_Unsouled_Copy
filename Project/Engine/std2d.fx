#ifndef _STD2D  //#indef #define #endif == #pragma once
#define _STD2D

// 4096 크기제한
cbuffer TRANSFORM : register(b0)
{
    row_major Matrix g_matWorld;
    row_major Matrix g_matWorldInv;

    row_major Matrix g_matView;
    row_major Matrix g_matViewInv;

    row_major Matrix g_matProj;
    row_major Matrix g_matProjInv;

    row_major Matrix g_matWV;
    row_major Matrix g_matWVP;
}

Texture2D g_tex_0 : register(t0);

// 샘플러 : 텍스쳐 자원의 데이터를 뽑아냄
SamplerState g_sam_0 : register(s0);

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
    //픽셀 쉐이더에서 리턴값을 정점으로 할때 픽셀정점의 값은 넣어준 정점값으로 선형보간된다
    
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    //if (vColor.a <= 0.1f)
    //{
    //    vColor.rgba = float4(1.f, 0.f, 0.f, 1.f);
    //}
    
    //float Aver = (vColor.r + vColor.g + vColor.b) / 3.f;
    //vColor.rgb = float3(Aver, Aver, Aver);    
    //vColor.a = 1.f;
    
    return vColor;
}

#endif
