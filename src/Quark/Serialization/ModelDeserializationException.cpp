#include "ModelDeserializationException.h"

namespace Quark
{
    ModelDeserializationException::ModelDeserializationException(std::string message)
            : m_message(std::move(message))
    {
    }

    const char* ModelDeserializationException::what() const noexcept
    {
        return m_message.c_str();
    }
} // Quark