#include "NotDerivableException.h"

namespace Quark
{
    NotDerivableException::NotDerivableException(std::string message)
            : m_message(std::move(message))
    {
    }

    const char* NotDerivableException::what() const noexcept
    {
        return m_message.c_str();
    }
}