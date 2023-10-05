#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "Quark/Images/Image.h"
#include "Quark/Models/Genome.h"

using namespace Quark;

TEST_CASE("Genome Initialization")
{
    // Given
    std::vector<Neurons::NodeGene> genes
    {
            Neurons::GeneType::INPUT,
            Neurons::GeneType::INPUT,
            Neurons::GeneType::INPUT,
            Neurons::GeneType::HIDDEN,
            Neurons::GeneType::OUTPUT,
    };

    std::vector<Neurons::ConnectionGene> connections
    {
        { 1, 5, 0.7 },
        { 3, 5, 0.5 },
        { 2, 4, 0.2 },
        { 4, 5, 0.4 },
        { 1, 4, 0.6 }
    };

    // When
    Quark::Models::Genome genome(genes, connections);
    auto output = genome.Evaluate({0.3, 0.4, 1});

    // Then
    const auto& genomeConnections = genome.GetConnections();

    REQUIRE(genomeConnections[3][0] == connections[2]);
    REQUIRE(genomeConnections[3][1] == connections[4]);
    REQUIRE(genomeConnections[4][0] == connections[0]);
    REQUIRE(genomeConnections[4][1] == connections[1]);
    REQUIRE(genomeConnections[4][2] == connections[3]);

    REQUIRE(output.size() == 1);
    REQUIRE((1 >= output[0] && output[0] >= 0.99));
}
