#ifndef PYQUARK_ISERIALIZER_H
#define PYQUARK_ISERIALIZER_H

#include <sstream>
#include <nlohmann/json.hpp>

namespace Quark
{
    class ISerializer
    {
    public:
        virtual ~ISerializer() = default;
        [[nodiscard]] virtual nlohmann::json Serialize() const = 0;
        virtual void Deserialize(const nlohmann::json& document) = 0;
    };
};

#endif //PYQUARK_ISERIALIZER_H
