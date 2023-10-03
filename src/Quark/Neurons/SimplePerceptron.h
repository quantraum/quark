#ifndef QUARK_PERCEPTRON_H
#define QUARK_PERCEPTRON_H

#include <cstddef>
#include <string>
#include <vector>
#include "Perceptron.h"
#include "../Functions/BinaryStep.h"

namespace Quark
{
    namespace Neurons
    {
        class SimplePerceptron : public Perceptron<int>
        {
        public:
            SimplePerceptron() = default;
            SimplePerceptron(size_t inputs, double learningRate, bool randomize = false, std::shared_ptr<Functions::Function> activation = std::make_shared<Functions::Linear>());
            SimplePerceptron(size_t inputs, double learningRate, std::vector<double> weights, std::shared_ptr<Functions::Function> activation = std::make_shared<Functions::Linear>());
            ~SimplePerceptron() override = default;

            nlohmann::json Serialize() const override;

            void Deserialize(const nlohmann::json &document) override;

        protected:

            std::tuple<int, double> Epoch(Samples &samples, Labels &labels) override;

        private:

            void UpdateWeights(Sample & sample, int error);

        };
    }
}

#endif //QUARK_PERCEPTRON_H
