#include "PixelOutOfBoundException.h"

namespace Quark
{
    PixelOutOfBoundException::PixelOutOfBoundException(std::string message)
            : m_message(std::move(message))
    {
    }

    const char* PixelOutOfBoundException::what() const noexcept
    {
        return m_message.c_str();
    }
}