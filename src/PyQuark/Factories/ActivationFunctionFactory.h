#ifndef PYQUARK_ACTIVATIONFUNCTIONFACTORY_H
#define PYQUARK_ACTIVATIONFUNCTIONFACTORY_H

#include <Quark/Functions/Tanh.h>

namespace ActivationFunctionFactory
{
    std::shared_ptr<Quark::Functions::Function> Create(const std::string &functionName, double sigmoidCParam)
    {
        if (functionName == "Linear")
            return std::make_shared<Quark::Functions::Linear>();
        else if (functionName == "BinaryStep")
            return std::make_shared<Quark::Functions::BinaryStep>();
        else if (functionName == "Step")
            return std::make_shared<Quark::Functions::Step>();
        else if (functionName == "Tanh")
            return std::make_shared<Quark::Functions::Tanh>();
        else if (functionName == "Sigmoid")
            return std::make_shared<Quark::Functions::Sigmoid>(sigmoidCParam);

        return nullptr;
    }

    void RaisePythonError()
    {
        PyErr_SetString(PyExc_RuntimeError, R"(Argument error: Activation Function not recognized (use "Linear", "Step", "BinaryStep", "Tanh" or "Sigmoid"))");
    }
}

#endif //PYQUARK_ACTIVATIONFUNCTIONFACTORY_H
