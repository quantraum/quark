#ifndef FIRSTCEPTRON_SIZEMISMATCHEXCEPTION_H
#define FIRSTCEPTRON_SIZEMISMATCHEXCEPTION_H

#include <stdexcept>

namespace Quark
{
    namespace Exceptions
    {
        class SizeMismatchException : public std::exception
        {
        public:
            SizeMismatchException(std::string message);

            const char *what() const noexcept override;

        private:
            std::string m_message;
        };
    }
}

#endif //FIRSTCEPTRON_SIZEMISMATCHEXCEPTION_H
