#ifndef PYQUARK_IMODELSERIALIZER_H
#define PYQUARK_IMODELSERIALIZER_H

#include "../Neurons/IModel.h"
#include <string>

namespace Quark
{
    class IModelSaver
    {
    public:
        virtual ~IModelSaver() = default;
        virtual void Save(const std::string &path, const Neurons::IModel &model) = 0;
    };
}

#endif //PYQUARK_IMODELSERIALIZER_H
