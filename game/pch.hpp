#pragma once

#define WidenHelper(x)  L##x
#define Widen(x)        WidenHelper(x)

#define ASSEMBLY_PATH(x) ShaderPath##x