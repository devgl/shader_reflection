#pragma once

#ifndef SHADER_REFLECTION_IMPL
#define SHADER_REFLECTION_API __declspec(dllexport)
#else
#define SHADER_REFLECTION_API __declspec(dllimport)
#endif