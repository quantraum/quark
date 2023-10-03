#define STB_IMAGE_IMPLEMENTATION
#include "Image.h"
#include <stb_image.h>
#include <cstring>

namespace Quark
{
    Image::Image(std::string path) : m_path(std::move(path))
    {
        int channels;
        auto* data = stbi_load(m_path.c_str(), &m_width, &m_height, &channels, 4);
        auto pixels = m_width * m_height;
        m_pixels.reserve(pixels);
        m_pixels.resize(pixels);
        std::memcpy(m_pixels.data(), data, pixels * sizeof(uint32_t));
        stbi_image_free(data);
    }
}