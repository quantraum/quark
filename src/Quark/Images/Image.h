#ifndef PYQUARK_IMAGE_H
#define PYQUARK_IMAGE_H

#include <cstddef>
#include <vector>
#include <string>
#include <tuple>
#include "../Exceptions/PixelOutOfBoundException.h"

namespace Quark
{
    class Image
    {
    public:
        explicit Image(std::string path);
        virtual ~Image() = default;

        inline std::vector<uint32_t>& GetPixels() { return m_pixels; }
        [[nodiscard]] inline size_t GetWidth() const { return m_width; }
        [[nodiscard]] inline size_t GetHeight() const { return m_height; }
        [[nodiscard]] inline std::tuple<int, int> GetDimensions() const { return { m_width, m_height }; }
        inline uint32_t GetPixel(int x, int y)
        {
            if(x < 0 || x >= m_width || y < 0 || y >= m_height)
                throw PixelOutOfBoundException("Pixel doesn't exist");

            return m_pixels[(y * m_width) + x];
        }

    private:
        std::vector<uint32_t> m_pixels;
        std::string m_path;
        int m_width;
        int m_height;
    };
}

#endif //PYQUARK_IMAGE_H
