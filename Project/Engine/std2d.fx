#ifndef _STD2D  //#indef #define #endif == #pragma once
#define _STD2D

// 4096 크기제한
cbuffer TRANSFORM : register(b0)
{
    row_major matrix g_matWorld;
    row_major matrix g_matView;
    row_major matrix g_matProj;
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
    
    // 로컬(모델) 좌표를 -> 월드 -> 뷰 -> 투영 좌표계로 순차적으로 변환
    float4 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);  // 월드
    float4 vViewPos = mul(vWorldPos, g_matView);                // 카메라 뷰
    float4 vProjPos = mul(vViewPos, g_matProj);                 // 투영 좌표계
    
    output.vPosition = vProjPos;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    // return float4(1.f, 0.f, 0.f, 1.f);
    
    // 알파블렌딩 값
    _in.vColor.a = 0.3f;
    
    //픽셀 쉐이더에서 리턴값을 정점으로 할때 픽셀정점의 값은 넣어준 정점값으로 선형보간된다
    return _in.vColor;
}

#endif
