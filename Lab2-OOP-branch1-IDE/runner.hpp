#pragma once
#include <string>
#include <filesystem>
#include <cstdio>
#include <array>
#include <sstream>
#include <system_error>

// result of running a program
struct RunResult 
{ 
        bool ok{false}; 
        std::string output; 
};

class DirGuard {
public:
    // change current working directory to the given path
    explicit DirGuard(const std::filesystem::path& p){
        old_ = std::filesystem::current_path();
        std::filesystem::current_path(p);
    }
    // restore old working directory
    // On destruction, always restores the old directory (even if exceptions occur), thanks to RAII
    ~DirGuard(){ try{ std::filesystem::current_path(old_); } catch(...){ } }
private:
    std::filesystem::path old_;
};

class Runner {
public:
    Runner(std::string language, std::string workdir): language_(language), workdir_(workdir) {}

    // builds and runs the given file in the given project, returns the result
    RunResult run(const std::string& project, const std::string& filename) const 
    {
        auto projDir = std::filesystem::path(workdir_) / project;
        std::filesystem::create_directories(projDir);
        if(lower(language_)=="python"){
            return runPython(projDir, filename);
        }
        return runC(projDir, filename);
    }

private:
    std::string language_;
    std::string workdir_;

    static std::string lower(std::string s){ std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); }); return s; }

        // executes the given command in the given working directory, captures output
    static RunResult execCapture(const std::string& cmd, const std::filesystem::path& cwd)
    {
        // change to the given directory for the duration of this function
        DirGuard g(cwd);
        std::array<char, 4096> buf{}; // buffer for reading output
        std::ostringstream out; // to accumulate output

        // open pipe to the command
#ifdef _WIN32
        FILE* pipe = _popen(cmd.c_str(), "r");
#else
        FILE* pipe = popen(cmd.c_str(), "r");
#endif
        if(!pipe){
            return {false, "Failed to start command: " + cmd + "\n"};
        }
        // read output until EOF
        while(true){
            size_t n = fread(buf.data(), 1, buf.size(), pipe); // read up to buf.size() bytes
            if(n>0) out.write(buf.data(), n);
            if(n<buf.size()) break;
        }
#ifdef _WIN32
// close pipe and get exit code
        int code = _pclose(pipe);
#else
        int code = pclose(pipe);
#endif
        return {code==0, out.str()};
    }

        // check if an executable exists in PATH
    static bool which(const std::string& exe)
    {
#ifdef _WIN32
        std::string cmd = "where " + exe + " >nul 2>nul";
#else
        std::string cmd = "command -v " + exe + " >/dev/null 2>&1";
#endif
        int code = std::system(cmd.c_str());
        return code==0;
    }

    // runs a Python script
    RunResult runPython(const std::filesystem::path& dir, const std::string& filename) const 
    {
#ifdef _WIN32
        std::string py = which("python")? "python" : (which("py")? "py -3" : "");
#else
        std::string py = which("python3")? "python3" : (which("python")? "python" : "");
#endif
        if(py.empty()) return {false, "Python interpreter not found.\n"};
        std::string cmd = py + " " + filename;
        return execCapture(cmd, dir);
    }

    // runs a C program
    RunResult runC(const std::filesystem::path& dir, const std::string& filename) const 
    {
#ifdef _WIN32
        std::string cc = which("gcc")? "gcc" : (which("clang")? "clang" : "");
        std::string prog = "program.exe";
#else
        std::string cc = which("gcc")? "gcc" : (which("clang")? "clang" : "");
        std::string prog = "program.out";
#endif
        if(cc.empty()) return {false, "C compiler (gcc/clang) not found.\n"};
        std::string build = cc + " " + filename + " -o " + prog;
        auto b = execCapture(build, dir);
        if(!b.ok) return b;
#ifdef _WIN32
        auto r = execCapture(prog, dir);
#else
        auto r = execCapture("./"+prog, dir);
#endif
        return r;
    }
};
