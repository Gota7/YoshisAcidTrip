#pragma once

#include "../jsystem/mem.hpp"
#include "../yaml/helper.hpp"
#include <string>
#include <variant>
#include <vector>

#define VERSION_INVALID 0xFFFF7777

// Forward declare.
class FConversionFileStep;
class FRomfs;

// Types.
using FConversionFileInput = std::variant<std::string, FConversionFileStep>; // Vectors store memory on heap, so using this in our class is fine despite infinite recursion.

// Mode.
enum class FConversionFileInputMode
{
    String,
    Step
};

// Conversion file step. TODO: MAKE THIS CLOSER TO WHAT ACTUALLY HAPPENS?
class FConversionFileStep
{
public:
    std::vector<FConversionFileInput> inputFiles;
    std::vector<FConversionFileInputMode> inputSteps;
    std::vector<std::string> outputFiles;
    std::vector<std::string> args;
    std::string command;

    // Make a new step.
    FConversionFileStep(const std::vector<FConversionFileInput>& inputFiles, const std::vector<FConversionFileInputMode>& inputSteps, const std::vector<std::string>& outputFiles, const std::vector<std::string>& args, const std::string& command) : inputFiles(inputFiles), inputSteps(inputSteps), outputFiles(outputFiles), args(args), command(command) {}

    // Load from a file.
    FConversionFileStep(YAML::Node node);

    // Emit a conversion.
    void Emit(YAML::Emitter& node, bool top = true);

};

// Utilities for dealing with the conversion file.
class FConversionFile
{
public:
    std::vector<FConversionFileStep> steps;
    unsigned int version = VERSION_INVALID;

    // Load conversion file.
    FConversionFile(const FRomfs& romfs, bool allowNew = false);

    // Save conversion file.
    void Save(const FRomfs& romfs);

};