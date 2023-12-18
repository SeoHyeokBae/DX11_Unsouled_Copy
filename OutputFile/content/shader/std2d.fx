#ifndef _STD2D  //#indef #define #endif == #pragma once
#define _STD2D

// 4096 크기제한
cbuffer TRANSFORM : register(b0)
{
    row_major float4x4 g_matWorld;
}

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
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    // return float4(1.f, 0.f, 0.f, 1.f);
    
    //픽셀 쉐이더에서 리턴값을 정점으로 할때 픽셀정점의 값은 넣어준 정점값으로 선형보간된다
    return _in.vColor;
}

#endif
