#ifndef PYQUARK_NOTDERIVABLEEXCEPTION_H
#define PYQUARK_NOTDERIVABLEEXCEPTION_H

#include <stdexcept>

namespace Quark
{
    class NotDerivableException : public std::exception
    {
    public:
        NotDerivableException(std::string message);

        const char *what() const noexcept override;

    private:
        std::string m_message;
    };
}

#endif //PYQUARK_NOTDERIVABLEEXCEPTION_H
