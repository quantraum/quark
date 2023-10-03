#include "InvalidLearningMethodException.h"

namespace Quark::Exceptions
{
    InvalidLearningMethodException::InvalidLearningMethodException(std::string message)
            : m_message(std::move(message))
    {
    }

    const char *InvalidLearningMethodException::what() const noexcept
    {
        return m_message.c_str();
    }
}
