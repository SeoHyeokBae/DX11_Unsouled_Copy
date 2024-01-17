#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"

// mesh : RectMesh
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

// ==========
// GrayFilter
// ==========
VS_OUT VS_GrayFilter(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_GrayFilter(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    vColor = g_postprocess.Sample(g_sam_0, _in.vUV);
    
    float aver = (vColor.r + vColor.g + vColor.b) / 3.f;
    
    vColor.rgb = float3(aver, aver, aver);
    
    return vColor;
}

// 커스텀
VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    // 픽셀쉐이더에 SV_Position 으로 입력된 값은 픽셀 쉐이더를 호출한 해당 픽셀의 좌표가 들어있다.
    // 월드/투영 처리된 좌표
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    
    // cos그래프로 일렁이는 효과
    //vScreenUV.y += cos((vScreenUV.x + (g_time * (속도) )) * (주파수)) * (진폭);
    //vScreenUV.y += cos((vScreenUV.x + (g_time *  0.1f))   *   40.f)  *  0.1f;
    
    // 노이즈효과
    // 투명 / 물
    if (g_btex_0)
    {
        vScreenUV = float2(_in.vPosition.x / g_RenderResolution.x, -(_in.vPosition.y / g_RenderResolution.y));
        float2 vUV = _in.vUV;
        vUV.x += g_time * 0.1f;
        vUV.y += g_time * 0.1f;
        
        float2 vNoise = g_tex_0.Sample(g_sam_0, vUV);
        vNoise = (vNoise.xy - 0.5f) * 0.02f; // 일렁임
        
        vScreenUV += vNoise;
    }
    
    vColor = g_postprocess.Sample(g_sam_0, vScreenUV);
    
    // Shock Wave
 //   float offset = (g_time - floor(g_time)) / g_time;
 //   float CurrentTime = (g_time) * (offset);
    
 //   float3 WaveParams = float3(10.0, 0.8, 0.1);
    
 //   //Use this if you want to place the centre with the mouse instead
	////vec2 WaveCentre = vec2( iMouse.xy / iResolution.xy );
       
 //   float2 WaveCentre = float2(0.5, 0.5);
 //   //WaveCentre = g_vec2_0 / g_RenderResolution;
 //   float2 texCoord = vScreenUV;
 //   float Dist = distance(texCoord, WaveCentre);
    
 //   float4 Color = g_postprocess.Sample(g_sam_0, texCoord);
    
 //   if ((Dist <= ((CurrentTime) + (WaveParams.z))) &&
	//(Dist >= ((CurrentTime) - (WaveParams.z))))
 //   {
 //       //The pixel offset distance based on the input parameters
 //       float Diff = (Dist - CurrentTime);
 //       float ScaleDiff = (1.0 - pow(abs(Diff * WaveParams.x), WaveParams.y));
 //       float DiffTime = (Diff * ScaleDiff);
        
 //       //The direction of the distortion
 //       float2 DiffTexCoord = normalize(texCoord - WaveCentre);
        
 //       //Perform the distortion and reduce the effect over time
 //       texCoord += ((DiffTexCoord * DiffTime) / (CurrentTime * Dist * 40.0));
 //       Color = g_postprocess.Sample(g_sam_0, texCoord);
        
 //       //Blow out the color and reduce the effect over time
 //       Color += (Color * ScaleDiff) / (CurrentTime * Dist * 40.0);
 //   }
    
 //   vColor = Color;
   /////////////     
   
    
    // Outline
    float2 pixelSize = (float2(1.f, 1.f) / g_RenderResolution);


    // uv좌표에서의 픽셀의 크기
    float4 pixelUp = g_postprocess.Sample(g_sam_0, float2(vScreenUV.x, vScreenUV.y + pixelSize.y));
    float4 pixelDown = g_postprocess.Sample(g_sam_0, float2(vScreenUV.x, vScreenUV.y - pixelSize.y));
    float4 pixelRight = g_postprocess.Sample(g_sam_0, float2(vScreenUV.x + pixelSize.x, vScreenUV.y));
    float4 pixelLeft = g_postprocess.Sample(g_sam_0, float2(vScreenUV.x - pixelSize.x, vScreenUV.y));
                		 
    
    if (pixelUp.a != vColor.a ||
        pixelDown.a != vColor.a ||
        pixelRight.a != vColor.a ||
        pixelLeft.a != vColor.a)
    {
        vColor.r += 1.0f;
    }
    

    return vColor;
}




#endif