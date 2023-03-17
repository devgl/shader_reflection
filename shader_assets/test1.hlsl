#ifndef __TEST1_HLSL__
#define __TEST1_HLSL__

struct VSInput
{
    float3 PositionOS : POSITION;
    float3 NormalOS : NORMAL;
    float4 Tangent : TANGENT;
    float2 Texcoord0 : TEXCOORD0;
    float2 Texcoord1 : TEXCOORD1;
    float4 Color : COLOR0;
};

struct VSOutput
{
    float4 PositionCS : SV_POSITION;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
    float4 color : TEXCOORD2;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output = (VSOutput)0;
    output.PositionCS = float4(input.PositionOS, 1.0f);
    output.uv0 = input.Texcoord0;
    output.uv1 = input.Texcoord1;
    output.color = input.Color;
    return output;
}

struct PSInput
{
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
    float4 color : TEXCOORD2;
};

struct PSOutput
{
    float4 color : SV_TARGET;
};

struct MaterialProperty
{
    float4 base_color;
    uint albedo_texture_id;
    uint albedo_sampler_id;
    float roughness;
};
ConstantBuffer<MaterialProperty> material_uniforms : register(b0, space0);

SamplerState sampler0 : register(s0);

PSOutput PSMain(PSInput input)
{
    Texture2D<float4> albedo_texture = ResourceDescriptorHeap[material_uniforms.albedo_texture_id];
    SamplerState albedo_texture_sampler = SamplerDescriptorHeap[material_uniforms.albedo_sampler_id];

    float4 albedo_color = albedo_texture.Sample(albedo_texture_sampler, input.uv0);

    PSOutput output = (PSOutput)0;
    output.color = input.color * albedo_color;
    return output;
}

#endif // __TEST1_HLSL__