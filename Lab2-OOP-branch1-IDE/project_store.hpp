#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

class ProjectStore {
public:
    // explicit prevents accidental implicit conversions (e.g., passing a const char* automatically)
    explicit ProjectStore(std::string root): root_(std::move(root))
    {
        // create root directory if not exists
        std::filesystem::create_directories(root_);
    }

    // Combines the root path and project name to check if the project directory exists
    bool exists(const std::string& project) const 
    {
        return std::filesystem::exists(std::filesystem::path(root_) / project);
    }

    // Constructs the full file path for a given project and filename
    std::string pathFor(const std::string& project, const std::string& filename) const 
    {
        auto dir = std::filesystem::path(root_) / project;
        std::filesystem::create_directories(dir);
        return (dir / filename).string();
    }

    // Saves the content to the specified file within the project directory
    void save(const std::string& project, const std::string& filename, const std::string& content) const {
        auto p = pathFor(project, filename);
        std::ofstream f(p, std::ios::binary); f << content;
    }

    // Loads the content from the specified file within the project directory
    std::string load(const std::string& project, const std::string& filename) const 
    {
        auto p = pathFor(project, filename);
        std::ifstream f(p, std::ios::binary);
        if(!f) return {};
        std::ostringstream ss; ss << f.rdbuf(); return ss.str();
    }

private:
    std::string root_;
};
