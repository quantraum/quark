
#ifndef PYQUARK_NODEGENE_H
#define PYQUARK_NODEGENE_H

#include <memory>

namespace Quark
{
    namespace Neurons
    {
        enum class GeneType
        {
            INPUT = 0,
            OUTPUT = 1,
            HIDDEN = 2,
        };

        class NodeGene : public std::enable_shared_from_this<NodeGene>
        {
        public:
            NodeGene(GeneType type);
            [[nodiscard]] inline GeneType GetType() const { return m_type; }
            bool operator==(GeneType type) const;
        private:
            GeneType m_type;
        };
    }
}

#endif //PYQUARK_NODEGENE_H
