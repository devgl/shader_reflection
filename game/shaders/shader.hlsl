#ifndef __SHADER_HLSL__
#define __SHADER_HLSL__

struct VSInput
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

struct PSInput
{
    float4 wpos : SV_Position;
    float2 uv : TEXCOORD0;
    float4 color : TEXCOORD1;
};

struct PSOutput
{
    float4 color : SV_Target;
};

cbuffer PerFrame : register(b0)
{
    float4x4 _ViewMatrix;
    float4x4 _ProjMatrix;
    float4x4 _VPMatrix;
    float4x4 _INV_VPMatrix;
};

cbuffer PerObject : register(b1)
{
    float4x4 _ObjectToWorldMatrix;
};

float4 ObjectToWorld(float3 position)
{
    return mul(_ObjectToWorldMatrix, float4(position, 1.0));
}

#define MyRS0 \
"RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), "\
"CBV(b0),"\
"SRV(t0),"\
"UAV(u0),"\
"DescriptorTable("\
"    CBV(b1),"\
"    SRV(t1, numDescriptors = 8, flags = DESCRIPTORS_VOLATILE),"\
"    UAV(u1, numDescriptors = unbounded, flags = DESCRIPTORS_VOLATILE)"\
"),"\
"DescriptorTable("\
"    Sampler(s0, space=1, numDescriptors = 4)"\
"),"\
"RootConstants(num32BitConstants=3, b10),"\
"StaticSampler(s1),"\
"StaticSampler("\
"    s3, "\
"    addressU = TEXTURE_ADDRESS_CLAMP,"\
"    filter = FILTER_MIN_MAG_MIP_LINEAR"\
")"
 

[RootSignature(MyRS0)]
PSInput VSMain(VSInput input)
{
    PSInput output;
    //output.positoin = ObjectToWorld(input.position.xyz);
    output.wpos = mul(_ObjectToWorldMatrix, input.position);
    output.uv = input.uv;
    output.color = input.color;
    return output;
}

PSOutput PSMain(PSInput input)
{
    PSOutput output;
    output.color = input.color;
    return output;
}

#endif