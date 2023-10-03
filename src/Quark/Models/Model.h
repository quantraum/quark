#ifndef PYQUARK_MODEL_H
#define PYQUARK_MODEL_H

#include <iostream>
#include <vector>
#include <functional>
#include "../Layers/Layer.h"
#include "FitOptions.h"

namespace Quark
{
    namespace Models
    {
        class Model
        {
        public:
            using Layer = std::unique_ptr<Layers::Layer>;
            using Samples = const std::vector<std::vector<double>>;
            using Sample = const std::vector<double>;
            using Labels = const std::vector<std::vector<double>>;
            using Label = const std::vector<double>;

            virtual ~Model() = default;
            virtual void AddLayer(Layer layer) = 0;
            virtual void Fit(Samples &samples, Labels &labels, const FitOptions& options) = 0;
            virtual std::vector<double> Predict(Sample &sample) = 0;
            virtual std::vector<double> GetQuadraticMeans() = 0;

        protected:
            std::vector<Layer> m_layers;
        };
    }
}

#endif //PYQUARK_MODEL_H
