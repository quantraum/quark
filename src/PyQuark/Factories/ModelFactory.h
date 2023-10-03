#ifndef PYQUARK_MODELFACTORY_H
#define PYQUARK_MODELFACTORY_H


#include <Quark/Models/RecursiveFeedForwardModel.h>
#include "Quark/Models/IterativeFeedForwardModel.h"

namespace ModelFactory
{
    std::unique_ptr<Quark::Models::Model> Create(const std::string& name)
    {
        if(name == "IterativeFeedForward")
            return std::make_unique<Quark::Models::IterativeFeedForwardModel>();

        if(name == "RecursiveFeedForward")
            return std::make_unique<Quark::Models::RecursiveFeedForwardModel>();

        return nullptr;
    }
}

#endif //PYQUARK_MODELFACTORY_H
