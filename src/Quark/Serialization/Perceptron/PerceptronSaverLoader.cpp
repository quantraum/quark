#include <fstream>
#include "PerceptronSaverLoader.h"
#include "nlohmann/json.hpp"
#include "../ModelDeserializationException.h"
#include "../../Neurons/Perceptron.h"
#include "../../Neurons/GradientPerceptron.h"

namespace Quark
{
    void PerceptronSaverLoader::Load(const std::string &path, Neurons::IModel *model)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            std::stringstream ss;
            ss << "File doesn't exist: " << path;
            throw ModelDeserializationException(ss.str());
        }

        std::stringstream buffer { std::string { std::istreambuf_iterator<char>{ file }, {} } };

        auto doc = nlohmann::json::parse(buffer);
        model->Deserialize(doc);
    }

    void PerceptronSaverLoader::Save(const std::string &path, const Neurons::IModel &model)
    {
        auto document = model.Serialize();
        std::ofstream file(path);
        file << document;
    }
}