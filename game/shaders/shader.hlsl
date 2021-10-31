#ifndef __SHADER_HLSL__
#define __SHADER_HLSL__

struct VSInput
{
    float4 position : POSITIONT;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

struct PSInput
{
    
};

PSInput main(VSInput input)
{
    PSInput output;
    
    return output;
}

#endif