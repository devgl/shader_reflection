#pragma once

#include "module.hpp"
#include <string>
#include <cstdint>

namespace shader_reflection
{
    struct SHADER_REFLECTION_API options
    {
        std::wstring_view file_path;
    };

    using result = uint64_t;
    constexpr result SR_OK = 0;
    constexpr result SR_FAILED = 1;

    SHADER_REFLECTION_API
    [[nodiscard]]
    result reflect(const options* options);
}



