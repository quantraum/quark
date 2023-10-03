#ifndef PYQUARK_PERCEPTRONSAVERLOADER_H
#define PYQUARK_PERCEPTRONSAVERLOADER_H

#include "../IModelSaver.h"
#include "../IModelLoader.h"
#include "../../Neurons/SimplePerceptron.h"

namespace Quark
{
    class PerceptronSaverLoader : public IModelLoader, public IModelSaver
    {
    public:
        ~PerceptronSaverLoader() override = default;

        void Load(const std::string &path, Neurons::IModel* model) override;
        void Save(const std::string &path, const Neurons::IModel &model) override;
    };
}

#endif //PYQUARK_PERCEPTRONSAVERLOADER_H
