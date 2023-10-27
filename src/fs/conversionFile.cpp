#include "conversionFile.hpp"

#include "../common.hpp"
#include "romfs.hpp"
#include <filesystem>

FConversionFileStep::FConversionFileStep(YAML::Node node)
{
    ZoneScopedN("FConversionFileStep::FConversionFileStep");
    command = node["Command"].as<std::string>();
    if (node["Args"].IsDefined()) args = node["Args"].as<std::vector<std::string>>();
    for (auto input : node["Inputs"])
    {
        if (input.IsMap())
        {
            inputFiles.emplace_back(input);
            inputSteps.push_back(FConversionFileInputMode::Step);
        }
        else
        {
            inputFiles.emplace_back(input.as<std::string>());
            inputSteps.push_back(FConversionFileInputMode::String);
        }
    }
    if (node["Outputs"].IsDefined()) outputFiles = node["Outputs"].as<std::vector<std::string>>();
}

void FConversionFileStep::Emit(YAML::Emitter& node, bool top)
{
    ZoneScopedN("FConversionFileStep::Emit");
    node << YAML::BeginMap;
    YAML::KeyValue(node, "Command", command);
    if (args.size()) YAML::KeyValueVec(node, "Args", args);
    node << YAML::Value << "Inputs" << YAML::BeginSeq;
    for (std::size_t i = 0; i < inputFiles.size(); i++)
    {
        if (inputSteps[i] == FConversionFileInputMode::String) node << std::get<std::string>(inputFiles[i]);
        else std::get<FConversionFileStep>(inputFiles[i]).Emit(node, false);
    }
    node << YAML::EndSeq;
    if (top) YAML::KeyValueVec(node, "Outputs", outputFiles); // Only root has output files.
    node << YAML::EndMap;
}

FConversionFile::FConversionFile(const FRomfs& romfs, bool allowNew)
{
    ZoneScopedN("FConversionFile::FConversionFile");

    // See if file exits.
    std::filesystem::path path(romfs.patchDir.fullPath + "/LSD_CONVERSIONS.yaml");
    if (!std::filesystem::exists(path))
    {
        if (!allowNew)
        {
            DBG_PRINT("ROMFS@LSD: Unable to find \"" << path.string() << "\".");
            return;
        }

        // Write initial version and setup conversions list.
        std::ofstream outFile = JFileSystem::SaveFile(JResPath(path.string(), true));
        YAML::Emitter root(outFile);
        root << YAML::BeginMap;
        YAML::KeyValue(root, "Version", 0u);
        root << YAML::Key << "Conversions" << YAML::Value << YAML::BeginSeq;

        // Write each file.
        for (std::filesystem::recursive_directory_iterator i(romfs.baseDir.fullPath), end; i != end; i++)
        {
            if (i->is_directory()) continue;
            std::string file = i->path().string().erase(0, romfs.baseDir.fullPath.size() + 1); // Cheap hack that gets rid of base path.
            steps.emplace_back(FConversionFileStep({ file }, { FConversionFileInputMode::String }, { file }, {}, "copy"));
            steps.back().Emit(root);
        }
        root << YAML::EndSeq << YAML::EndMap;

        // It is important to also make sure we can not mix and match base and patch folder versions, so write version.
        {
            std::ofstream outFile = JFileSystem::SaveFile(JResPath(romfs.baseDir.fullPath + "/LSD_VERSION.yaml", true));
            YAML::Emitter root(outFile);
            root << YAML::BeginMap;
            YAML::KeyValue(root, "Version", 0u);
            root << YAML::EndMap;
        }
        version = 0;

    }

    // File does exist, load it.
    else
    {
        {
            YAML::Node root = YAML::LoadFile(romfs.baseDir.fullPath + "/LSD_VERSION.yaml");
            version = root["Version"].as<unsigned int>();
        }
        YAML::Node root = YAML::LoadFile(path.string());
        if (root["Version"].as<unsigned int>() != version)
        {
            version = VERSION_INVALID;
            return;
        }
        for (auto node : root["Conversions"])
        {
            steps.emplace_back(node);
        }
    }

}

void FConversionFile::Save(const FRomfs& romfs)
{
    ZoneScopedN("FConversionFile::Save");

    // Write version.
    std::ofstream outFile = JFileSystem::SaveFile(JResPath(romfs.patchDir.fullPath + "/LSD_CONVERSIONS.yaml", true));
    YAML::Emitter root(outFile);
    root << YAML::BeginMap;
    YAML::KeyValue(root, "Version", version);
    root << YAML::Key << "Conversions" << YAML::Value << YAML::BeginSeq;

    // Write each step.
    for (auto& step : steps)
    {
        step.Emit(root);
    }
    root << YAML::EndSeq << YAML::EndMap;

    // It is important to also make sure we can not mix and match base and patch folder versions, so write version.
    {
        std::ofstream outFile = JFileSystem::SaveFile(JResPath(romfs.baseDir.fullPath + "/LSD_VERSION.yaml", true));
        YAML::Emitter root(outFile);
        root << YAML::BeginMap;
        YAML::KeyValue(root, "Version", version);
        root << YAML::EndMap;
    }

}