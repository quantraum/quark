#ifndef PYQUARK_ITERATIVEFEEDFORWARDMODEL_H
#define PYQUARK_ITERATIVEFEEDFORWARDMODEL_H

#include "Model.h"

namespace Quark
{
    namespace Models
    {
        class IterativeFeedForwardModel : public Model
        {
        public:
            IterativeFeedForwardModel() : m_errorSignals()
            {

            }

            explicit IterativeFeedForwardModel(std::vector<Layer> layers);

            void AddLayer(Layer layer) override;

            void Fit(Samples &samples, Labels &labels, const FitOptions& options) override;

            std::vector<double> Predict(Sample &sample) override;

            std::vector<double> GetQuadraticMeans() override;

        private:
            std::vector<std::vector<double>> m_errorSignals;

            std::vector<double> m_quadraticMeans;

            void ErrorBackPropagation(Sample &sample, Model::Label &label, int currentIteration);

            double ComputeErrorSignalPotential(std::vector<double> &successorErrorSignals, int currentNeuron, int currentLayer = 0);

            void WeightsCorrection(Sample &sample);

            std::vector<double> ComputeErrorSignals(std::vector<double> predictions, std::vector<double> potentials, Label &label, int currentLayer);
        };
    }
}


#endif //PYQUARK_ITERATIVEFEEDFORWARDMODEL_H
