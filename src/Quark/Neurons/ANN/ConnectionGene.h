#ifndef PYQUARK_CONNECTIONGENE_H
#define PYQUARK_CONNECTIONGENE_H

#include <memory>
#include "NodeGene.h"

namespace Quark
{
    namespace Neurons
    {
        class ConnectionGene
        {
        public:
            ConnectionGene();
            ConnectionGene(unsigned int from, unsigned int to, double weight);
            [[nodiscard]] inline unsigned int GetFrom() const { return m_from; }
            [[nodiscard]] inline unsigned int GetTo() const { return m_to; }
            [[nodiscard]] inline double GetWeight() const { return m_weight; }
            bool operator== (const ConnectionGene &other);
        private:
            /*std::shared_ptr<NodeGene> m_from;
            std::shared_ptr<NodeGene> m_to;*/
            unsigned int m_from;
            unsigned int m_to;
            double m_weight;
            bool m_enabled;
        };
    }
}

#endif //PYQUARK_CONNECTIONGENE_H
