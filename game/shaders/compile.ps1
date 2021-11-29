dxc -T vs_6_0 -E VSMain shader.hlsl -Fc shader.dxc.vs.assembly
dxc -T vs_6_0 -E VSMain -spirv -fspv-reflect shader.hlsl -Fc shader.spirv.vs.assembly

dxc -T ps_6_0 -E PSMain shader.hlsl -Fc shader.dxc.ps.assembly
dxc -T ps_6_0 -E PSMain -spirv -fspv-reflect shader.hlsl -Fc shader.spirv.ps.assembly