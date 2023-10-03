#ifndef PYQUARK_IMODELDESERIALIZER_H
#define PYQUARK_IMODELDESERIALIZER_H

#include "../Neurons/IModel.h"
#include <string>

namespace Quark
{
    class IModelLoader
    {
    public:
        virtual ~IModelLoader() = default;
        virtual void Load(const std::string &path, Neurons::IModel* model) = 0;
    };
}

#endif //PYQUARK_IMODELDESERIALIZER_H
