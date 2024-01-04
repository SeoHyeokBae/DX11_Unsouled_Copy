#ifndef _STD2D  //#indef #define #endif == #pragma once
#define _STD2D

#include "value.fx"


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
    
    // ����(��) ��ǥ�� -> ���� -> �� -> ���� ��ǥ��� ���������� ��ȯ
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP); // ����
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
    
    if (g_btex_0)
    {
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        
        //saturate 0 ~ 1 �� ���� �ʰ� ����
        float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f);
        
        if (fAlpha < 0.1f)
        {
            // �ȼ� ���̴��� �߰��� ���ó��
            discard; //clip(-1);            
        }
    }
    
    if (g_btex_1)
    {
        vColor = g_tex_1.Sample(g_sam_1, _in.vUV);
        
        //saturate 0 ~ 1 �� ���� �ʰ� ����
        float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f);
        
        if (fAlpha < 0.1f)
        {
            // �ȼ� ���̴��� �߰��� ���ó��
            discard; //clip(-1);            
        }
    }
    
    if (g_int_0)
    {
        vColor.r *= 2.f;
    }
    
    //�ȼ� ���̴����� ���ϰ��� �������� �Ҷ� �ȼ������� ���� �־��� ���������� ���������ȴ�
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
