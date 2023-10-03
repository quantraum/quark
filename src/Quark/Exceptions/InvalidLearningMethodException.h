#ifndef PYQUARK_PIXELOUTOFBOUNDEXCEPTION_H
#define PYQUARK_PIXELOUTOFBOUNDEXCEPTION_H

#include <stdexcept>

namespace Quark
{
    namespace Exceptions
    {
        class InvalidLearningMethodException : public std::exception
        {
        public:
            InvalidLearningMethodException(std::string message);

            const char *what() const noexcept override;

        private:
            std::string m_message;
        };
    }
}

#endif //PYQUARK_PIXELOUTOFBOUNDEXCEPTION_H
