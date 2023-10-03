#ifndef PYQUARK_IMODEL_H
#define PYQUARK_IMODEL_H

#include "../Serialization/ISerializer.h"

namespace Quark
{
    namespace Neurons
    {
        class IModel : public ISerializer
        {
        public:
            ~IModel() override = default;
        };
    }
}

#endif //PYQUARK_IMODEL_H
