#ifndef __GLTFPBR_HLSL__
#define __GLTFPBR_HLSL__

#include "root_sig.hlsl"

struct VSInput
{
    float4 position : POSITION;
    float4 color : COLOR;
    float4 normal : NORMAL;
    float4 uv0 : TEXCOORD0;
    float4 uv1 : TEXCOORD1;
};

struct PSInput
{
    float4 position : SV_Position;
    float4 color : TEXCOORD0;
    float4 uv : TEXCOORD1;
    float4 normal : TEXCOORD2;
};

[RootSignature(GLTFRootSignature)]
PSInput VSMain(VSInput input)
{
    PSInput output;
    output.position = input.position;
    output.color = input.color;
    output.uv = input.uv0;
    output.normal = input.normal;
    return output;
}

[RootSignature(GLTFRootSignature)]
float4 PSMain(PSInput input) : SV_Target0
{
    float4 baseTexColor = _BaseTex.Sample(_BaseTex_Sampler, input.uv.xy);
    baseTexColor.r += _Lights[0].colorAndRange.r;

    return input.color + baseTexColor;
}


#endif