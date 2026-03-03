#pragma once

#include <filesystem>
#include "Types.h"

namespace axiom
{
    inline Path GetEngineAssetsPath()
    {
        #ifdef ENGINE_ROOT_PATH
            return Path(ENGINE_ROOT_PATH) / "Assets";
        #else
            throw std::runtime_error("Global variable ENGINE_ROOT not set. Unable to get EngineAssetsPath.");
        #endif
    }

    inline Path GetShaderPath(const String& shaderName)
    {
        return GetEngineAssetsPath() / "Shaders" / shaderName;
    }
}