#ifndef QUARK_ADALINEPERCEPTRON_H
#define QUARK_ADALINEPERCEPTRON_H

#include <cstddef>
#include <string>
#include <vector>
#include "Perceptron.h"

namespace Quark
{
    namespace Neurons
    {
        class AdalinePerceptron : public Perceptron<double>
        {
        public:

            AdalinePerceptron() = default;
            AdalinePerceptron(size_t inputs, double learningRate, bool randomize = false, std::shared_ptr<Functions::Function> activation = std::make_shared<Functions::Linear>());
            AdalinePerceptron(size_t inputs, double learningRate, std::vector<double> weights, std::shared_ptr<Functions::Function> activation = std::make_shared<Functions::Linear>());
            ~AdalinePerceptron() override = default;

            nlohmann::json Serialize() const override;
            void Deserialize(const nlohmann::json &document) override;

        protected:
            std::tuple<int, double> Epoch(Samples &samples, Labels &labels) override;

        private:
            mutable std::vector<double> m_corrections;

            double ComputeQuadtraticMean(Samples &samples, Labels &labels);
        };
    }
}

#endif //QUARK_ADALINEPERCEPTRON_H
