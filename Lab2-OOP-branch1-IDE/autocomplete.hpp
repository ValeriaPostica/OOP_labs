#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <regex>
#include <algorithm>

class AutoCompleter {
public:
    // initialize with language("python" or "c")
    explicit AutoCompleter(std::string language): language_(std::move(language)) {}

    // learns identifiers from the given text
    void learnFromBuffer(const std::string& text)
    {
        // find all identifiers (at least 3 chars, starting with letter or _)
        std::unordered_set<std::string> ids;
        std::regex rx(R"([A-Za-z_][A-Za-z0-9_]{2,})");
        // iterate over all matches of the regex in the code
        for(std::sregex_iterator it(text.begin(), text.end(), rx), end; it!=end; ++it)
        {
            auto tok = it->str();
            // if token is not a reserved keyword, add to ids
            if(reserved().count(tok)==0) ids.insert(tok);
        }
        // add to learned set
        learned_.insert(ids.begin(), ids.end());
    }

    // suggests completions for the given prefix
    std::vector<std::string> complete(const std::string& prefix, size_t maxItems=6) const
    {
        auto p = lower(prefix);
        // gather candidates from learned identifiers and base snippets
        std::vector<std::string> candidates(learned_.begin(), learned_.end());
        auto snips = baseSnippets(); 
        candidates.insert(candidates.end(), snips.begin(), snips.end());

        // filter candidates that start with the prefix (case-insensitive)
        std::vector<std::string> out;
        for(const auto& c : candidates)
        {
            if(c.size()>=p.size() && lower(c).rfind(p,0)==0) out.push_back(c);
        }
        // make unique, learned first
        std::vector<std::string> uniq;
        for(const auto& s : out)
        { 
            if(std::find(uniq.begin(), uniq.end(), s)==uniq.end()) uniq.push_back(s); 
        }
        if(uniq.size()>maxItems) uniq.resize(maxItems);
        return uniq;
    }

private:
    std::string language_;
    std::unordered_set<std::string> learned_;

    static std::string lower(std::string s){ std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); }); return s; }

    // some base snippets for each language
    static const std::unordered_set<std::string>& reserved()
    {
        static std::unordered_set<std::string> r = {"def","class","import","return","if","else","elif","for","while","try","except","with","as"};
        return r;
    }

    std::vector<std::string> baseSnippets() const 
    {
        auto l = lower(language_);
        if(l=="python"){
            return {"def ","class ","import ","from ","for ","while ","if ","elif ","else:","try:","except ","with ","return ","print(","range(","in ","and ","or ","not ","True","False","None"};
        }
        // C
        return {"#include <stdio.h>","int main()","printf(","for (int i = 0; i < ","while (","if (","else","return ","struct ","typedef ","const ","static "};
    }
};
