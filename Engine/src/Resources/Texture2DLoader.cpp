#include "Resources/Texture2DLoader.h"
#include "Resources/Texture2DResource.h"
#include "Core/Assert.h"
#include "stb_image.h"

namespace axiom
{
    SharedPtr<void> Texture2DLoader::CreateResource(const FileData& fileData)
    {
        int w, h, channels;
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* pixels = stbi_load_from_memory(
            fileData.buffer.data(), (int)fileData.buffer.size(),
            &w, &h, &channels, STBI_rgb_alpha);
        AX_ASSERT(pixels, "Failed to decode texture image");
        Vector<uint8> data(pixels, pixels + w * h * 4);
        stbi_image_free(pixels);
        return MakeShared<Texture2DResource>((uint32)w, (uint32)h, std::move(data));
    }
}
