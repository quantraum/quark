#ifndef PYQUARK_MODELDESERIALIZATIONEXCEPTION_H
#define PYQUARK_MODELDESERIALIZATIONEXCEPTION_H

#include <string>

namespace Quark
{
    class ModelDeserializationException : public std::exception
    {
    public:
        ModelDeserializationException(std::string message);
        const char* what() const noexcept override;

    private:
        std::string m_message;
    };

}

#endif //PYQUARK_MODELDESERIALIZATIONEXCEPTION_H
