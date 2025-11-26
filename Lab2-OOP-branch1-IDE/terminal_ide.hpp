#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <regex>
#include <optional>
#include <algorithm>
#include "highlighter.hpp"
#include "autocomplete.hpp"
#include "project_store.hpp"
#include "runner.hpp"

// string to lower
static inline std::string toLower(std::string s){ std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); }); return s; }

// class for the terminal IDE
class TerminalIDE 
{
public:
    TerminalIDE(std::string workspace="projects")
        : workspace_(workspace), store_(workspace_), language_("python"), filename_("main.py"),
          highlighter_(language_), completer_(language_) {}

    // main working loop for writing code
    void loop()
    {
        openOrCreateBanner();
        // infinite loop until user exits
        for(;;)
        {
            std::cout << "> " << std::flush;
            std::string line;
            
            // if EOF or error, exit
            if(!std::getline(std::cin, line)) 
            { 
                std::cout << "\nExiting.\n"; break; 
            }

            // Pre defined commands(they start with ':')
            if(!line.empty() && line.front()==':')
            {
                // lowercase
                auto cmd = toLower(line.substr(1));
                // process commands
                if(cmd=="help") printHelp();
                else if(cmd=="open" || cmd=="new") openOrCreate();
                else if(cmd=="lang") setLanguage(ask("Language (python/c)", language_, {"python","c"}));
                else if(cmd=="save") save();
                else if(cmd=="undo") undo();
                else if(cmd=="clear") clear();
                else if(cmd=="show") showBuffer();
                else if(cmd=="run") run();
                else if(cmd=="quit") break;
                else if(isDigits(cmd)) acceptSuggestion(std::stoi(cmd));
                else std::cout << "Unknown command. Type :help\n";
                continue;
            }

            // Add line to buffer
            buffer_.push_back(line);
            auto text = join(buffer_, "\n");

            // Update highlighter and completer
            completer_.learnFromBuffer(text);
            std::string prefix = lastToken(line);

            // make 6 suggestions and print them
            auto suggestions = prefix.empty()? std::vector<std::string>{} : completer_.complete(prefix, 6);
            std::cout << highlighter_.highlight(text) << "\n";
            // print suggestions as [1] xxx, [2] yyy, ...
            if(!suggestions.empty())
            {
                std::cout << "Suggestions: ";
                for(size_t i=0;i<suggestions.size();++i)
                {
                    std::cout << "[" << (i+1) << "] " << suggestions[i];
                    if(i+1<suggestions.size()) std::cout << ", ";
                }
                std::cout << "\n";
            }
            // store last suggestions
            lastSuggestions_ = {prefix, suggestions};
        }
    }

private:
    // Workspace root folder
    std::string workspace_;
    // Persistent project store
    ProjectStore store_;
    // Current project name
    std::string projectName_;
    std::string language_;
    std::string filename_;
    std::vector<std::string> buffer_;
    // Syntax highlighter
    Highlighter highlighter_;
    // Code completer
    AutoCompleter completer_;
    // Last token and its suggestions
    std::pair<std::string, std::vector<std::string>> lastSuggestions_;

    // check if a string is all digits
    static bool isDigits(const std::string& s)
    { 
        return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit); 
    }

    // join vector of strings with a separator("Hello", "world" with ", " -> "Hello, world")
    static std::string join(const std::vector<std::string>& v, const std::string& sep)
    {
        std::string out; for(size_t i=0;i<v.size();++i){ out+=v[i]; if(i+1<v.size()) out+=sep; } return out;
    }

    // get the last token in a line (for autocompletion)
    static std::string lastToken(const std::string& line)
    {
        std::regex rx("([A-Za-z_][A-Za-z0-9_]*)$");
        std::smatch m; if(std::regex_search(line, m, rx)) return m[1].str();
        return {};
    }

    // a beautiful print function to ask user
    static std::string ask(const std::string& prompt, const std::string& def, const std::vector<std::string>& options)
    {
        for(;;){
            std::cout << prompt << " ["<<def<<"]: " << std::flush;
            std::string ans; std::getline(std::cin, ans);
            if(ans.empty()) ans = def;
            auto al = toLower(ans);
            bool ok=false; for(auto& o: options) if(al==o) ok=true;
            if(ok) return al;
            std::cout << "Choose from {"; for(size_t i=0;i<options.size();++i){ std::cout<<options[i]; if(i+1<options.size()) std::cout<<",";} std::cout<<"}\n";
        }
    }

    // All of available commands
    void printHelp() const 
    {
        std::cout << R"(Commands:
  :help           Show this help
  :open           Open existing project
  :new            Create new project
  :lang           Change language (python|c)
  :save           Save file
  :undo           Remove last line
  :clear          Clear buffer
  :show           Show full highlighted buffer
  :run            Build & run
  :quit           Exit
Accept suggestions:
  :1 or :2 ...    Accept suggestion number 1, 2, ...
)" << std::flush;
    }

    // set language and change filename, highlighter, completer accordingly
    // only "python" or "c" allowed
    void setLanguage(const std::string& lang)
    {
        if(lang!="python" && lang!="c"){ std::cout<<"Only 'python' or 'c' allowed.\n"; return; }
        language_ = lang;
        filename_ = (lang=="python")? "main.py" : "main.c";
        highlighter_ = Highlighter(language_);
        completer_ = AutoCompleter(language_);
        completer_.learnFromBuffer(join(buffer_,"\n"));
        std::cout << "[IDE] Language set to " << language_ << ". File: " << filename_ << "\n";
    }

    // open or create a project
    void openOrCreate()
    {
        // setting up project
        // ask for open or new
        auto mode = ask("Open or new project? (open/new)", "new", {"open","new"});
        // ask for project name
        std::cout << "Project name [demo]: " << std::flush;
        std::string name; std::getline(std::cin, name); if(name.empty()) name="demo";
        // ask for language
        auto lang = ask("Language (python/c)", "python", {"python","c"});
        setLanguage(lang);
        projectName_ = name;

        // if open but not exists, create it
        if(mode=="open" && !store_.exists(name))
        {
            std::cout << "[IDE] Project '"<<name<<"' not found, creating it.\n";
        }

        // load content
        auto content = store_.load(name, filename_);
        buffer_.clear();
        
        // split into buffer lines and show them
        if(!content.empty())
        {
            buffer_ = splitLines(content);
            std::cout << "[IDE] Loaded " << buffer_.size() << " lines from " << filename_ << "\n";
            showBuffer();
        }else{
            std::cout << "[IDE] New file " << filename_ << ". Start typing. Type :help for commands.\n";
        }
    }

    // show current buffer with highlighting
    void showBuffer() const 
    {
        auto text = join(buffer_, "\n");
        std::cout << highlighter_.highlight(text) << "\n";
    }

    // saving program to the project store
    void save()
    {
        if(projectName_.empty())
        { 
            std::cout<<"No project.\n"; 
            return; 
        }
        store_.save(projectName_, filename_, join(buffer_,"\n"));
        std::cout << "[IDE] Saved to " << projectName_ << "/" << filename_ << "\n";
    }

    void undo()
    {
        if(buffer_.empty())
        {
            std::cout << "[IDE] Buffer is empty.\n";
            return;
        }
        // .back() Returns a read/write reference to the data at the last element of the %vector.
        auto removed = buffer_.back();
        // .pop_back() Removes the last element in the %vector
        buffer_.pop_back();
        // thus we removed the last line
        std::cout << "[IDE] Removed: '"<< removed << "'\n";
    }

    void clear()
    {
        buffer_.clear();
        std::cout << "[IDE] Buffer cleared.\n";
    }

    void run()
    {
        // auto save first
        save();
        // build and run
        Runner r(language_, workspace_);
        auto res = r.run(projectName_, filename_);
        std::cout << std::string(60,'=') << "\n" << res.output << std::string(60,'=') << "\n";
        if(!res.ok) std::cout << "[IDE] Build/Run failed.\n"; else std::cout << "[IDE] Done.\n";
    }

    // accept a suggestion by its index (1-based)
    void acceptSuggestion(int idx)
    {
        if(idx<=0)
        {
            std::cout << "[IDE] Invalid suggestion index.\n";
            return;
        }
        if(buffer_.empty())
        {
            std::cout << "[IDE] Buffer is empty.\n";
            return;
        }
        auto& suggs = lastSuggestions_.second; // only the suggestions
        if((size_t)idx > suggs.size())
        {
            std::cout << "[IDE] Invalid suggestion index.\n";
            return;
        }

        //Finds the trailing identifier in the last line
        // if present, replaces just that token with the chosen completion 
        // otherwise appends the suggestion.
        auto choice = suggs[idx-1];
        auto& last = buffer_.back();
        std::regex rx("([A-Za-z_][A-Za-z0-9_]*)$");
        std::smatch m;
        if(std::regex_search(last, m, rx)){
            last = last.substr(0, (size_t)m.position()) + choice;
        }else{
            last += choice;
        }
        std::cout << "[IDE] Accepted suggestion: " << choice << "\n";
        showBuffer();
    }

    static std::vector<std::string> splitLines(const std::string& s)
    {
        std::vector<std::string> out; 
        std::string cur;
        for(char c: s)
        { 
            if(c=='\n')
            { 
                out.push_back(cur); 
                cur.clear(); 
            } else cur.push_back(c); 
        }
        out.push_back(cur); return out;
    }

    void openOrCreateBanner()
    {
        std::cout << "=== Terminal IDE (C++17) ===\nType :help for commands.\n";
        openOrCreate();
    }
};
