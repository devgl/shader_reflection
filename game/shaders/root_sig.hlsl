#ifndef __GLTFROOTSIG_HLSL__
#define __GLTFROOTSIG_HLSL__

/*
 * space 0 : per frame data
 * space 1 : per camera data
 * space 2 : per material data
 * space 3 : per object data
*/


#define GLTFRootSignature \
"RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
"CBV(b0, space=0)," \
"DescriptorTable(CBV(b0, space=1), CBV(b1, numDescriptors=8, space=1), CBV(b20, space=1))," \
"DescriptorTable(SRV(t0, space=2), CBV(b0, space=2)), " \
"CBV(b0, space=3), " \
"StaticSampler(s5, addressU = TEXTURE_ADDRESS_CLAMP, filter = FILTER_MIN_MAG_MIP_LINEAR)"

#define MAX_LIGHT_PER_CAMERA 8

// per-frame data
struct Frame
{

};
ConstantBuffer<Frame> _FrameData : register(b0, space0);

// per-camera data
struct Camera
{
    float4x4 ProjMatrix;
    float4x4 ViewMatrix;
    float4x4 _pad0;
    float4x4 _pad1;
};
ConstantBuffer<Camera> _CameraData : register(b0, space1);
struct LightInfo
{
    float4 position;
    float4 colorAndRange;
    float4 spotDir;
    float4 spotRange;
};
ConstantBuffer<LightInfo> _Lights[MAX_LIGHT_PER_CAMERA] : register(b1, space1);
struct LightData
{
    uint lightCount;
};
ConstantBuffer<LightData> _LightData : register(b2, space1);

// per-material data
Texture2D<float4> _BaseTex : register(t0, space2);
SamplerState _BaseTex_Sampler : register(s5);
struct Material
{
    float4 BaseColor;
};
ConstantBuffer<Material> _MaterialData : register(b0, space2);

// per-object data
struct Object
{
    float4x4 ObjectToWorldMatrix;
};
ConstantBuffer<Object> _ObjectData : register(b0, space3);

#endif
