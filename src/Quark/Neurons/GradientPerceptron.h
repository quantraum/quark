#ifndef QUARK_GRADIENTPERCEPTRON_H
#define QUARK_GRADIENTPERCEPTRON_H

#include <cstddef>
#include <string>
#include <vector>
#include "Perceptron.h"

namespace Quark
{
    namespace Neurons
    {
        class GradientPerceptron : public Perceptron<double>
        {
        public:
            GradientPerceptron() = default;
            GradientPerceptron(size_t inputs, double learningRate, bool randomize = false, std::shared_ptr<Functions::Function> activation = std::make_shared<Functions::Linear>());
            GradientPerceptron(size_t inputs, double learningRate, std::vector<double> weights, std::shared_ptr<Functions::Function> activation = std::make_shared<Functions::Linear>());
            ~GradientPerceptron() override = default;

            nlohmann::json Serialize() const override;
            void Deserialize(const nlohmann::json &document) override;

        protected:
            std::tuple<int, double> Epoch(Samples &samples, Labels &labels) override;

        private:
            mutable std::vector<double> m_corrections;

            void UpdateCorrections(Sample &vector, double error);

            void UpdateWeights();
        };
    }
}

#endif //QUARK_GRADIENTPERCEPTRON_H
