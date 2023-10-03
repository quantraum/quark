#include "SizeMismatchException.h"

namespace Quark::Exceptions
{
    SizeMismatchException::SizeMismatchException(std::string message)
        : m_message(std::move(message))
    {
    }

    const char* SizeMismatchException::what() const noexcept
    {
        return m_message.c_str();
    }
}
