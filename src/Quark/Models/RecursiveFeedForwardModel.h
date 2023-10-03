#ifndef PYQUARK_RECURSIVEFEEDFORWARDMODEL_H
#define PYQUARK_RECURSIVEFEEDFORWARDMODEL_H

#include "Model.h"

namespace Quark
{
    namespace Models
    {
        class RecursiveFeedForwardModel : public Model
        {
        public:
            RecursiveFeedForwardModel() : m_errorSignals()
            {

            }

            explicit RecursiveFeedForwardModel(std::vector<Layer> layers);

            void AddLayer(Layer layer) override;

            void Fit(Samples &samples, Labels &labels, const FitOptions& options) override;

            std::vector<double> Predict(Sample &sample) override;

            std::vector<double> GetQuadraticMeans() override;

        private:
            std::vector<std::vector<double>> m_errorSignals;

            std::vector<double> m_quadraticMeans;


            void RecursiveErrorBackPropagation(Sample &sample, Model::Label &label, int currentIteration, int currentLayer = 0);

            double ComputeErrorSignalPotential(std::vector<double> &successorErrorSignals, int currentNeuron, int currentLayer = 0);

            void RecursiveWeightsCorrection(Sample &sample, int currentLayer = 0);

            std::vector<double> ComputeErrorSignals(std::vector<double> predictions, std::vector<double> potentials, Label &label, int currentLayer);
        };
    }
}


#endif //PYQUARK_RECURSIVEFEEDFORWARDMODEL_H
