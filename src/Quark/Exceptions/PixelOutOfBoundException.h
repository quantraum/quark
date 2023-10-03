#ifndef PYQUARK_PIXELOUTOFBOUNDEXCEPTION_H
#define PYQUARK_PIXELOUTOFBOUNDEXCEPTION_H

#include <stdexcept>

namespace Quark
{
    class PixelOutOfBoundException : public std::exception
    {
    public:
        PixelOutOfBoundException(std::string message);

        const char *what() const noexcept override;

    private:
        std::string m_message;
    };
}

#endif //PYQUARK_PIXELOUTOFBOUNDEXCEPTION_H
