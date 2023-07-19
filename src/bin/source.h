#pragma once

#include <filesystem>
#include <fstream>
#include <memory>

// Holds an input stream. Mostly for ease of use and for auto ifstream closing.
struct BSource
{
    std::unique_ptr<std::ifstream> data;

    // Create a new source.
    BSource(std::unique_ptr<std::ifstream> data) : data(std::move(data)) {}

    // New source from path.
    BSource(const std::string& path)
    {
        if (std::filesystem::exists(path))
        {
            data = std::make_unique<std::ifstream>(path, std::ios::in | std::ios::binary);
        }
        else throw std::runtime_error("File \"" + path + "\" does not exist!");
    }

    // Finally done with data.
    ~BSource()
    {
        data->close();
    }

};