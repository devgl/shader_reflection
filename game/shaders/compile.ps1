dxc -T vs_6_0 -E VSMain shader.hlsl -Fc shader.dxc.vs.assembly -Fo shader.dxc.vs.bin
dxc -T vs_6_0 -E VSMain -spirv -fspv-reflect shader.hlsl -Fc shader.spirv.vs.assembly -Fo shader.spirv.vs.bin

dxc -T ps_6_0 -E PSMain shader.hlsl -Fc shader.dxc.ps.assembly -Fo shader.dxc.ps.bin
dxc -T ps_6_0 -E PSMain -spirv -fspv-reflect shader.hlsl -Fc shader.spirv.ps.assembly -Fo shader.spirv.ps.bin