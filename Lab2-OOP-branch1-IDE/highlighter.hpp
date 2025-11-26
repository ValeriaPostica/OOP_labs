#pragma once
#include <string>
#include <vector>
#include <regex>
#include <algorithm>

namespace ansi 
{
    static const std::string RESET = "\033[0m";
    static const std::string BOLD = "\033[1m";
    static const std::string DIM = "\033[2m";
    static const std::string RED = "\033[31m";
    static const std::string GREEN = "\033[32m";
    static const std::string YELLOW = "\033[33m";
    static const std::string BLUE = "\033[34m";
    static const std::string MAGENTA = "\033[35m";
    static const std::string CYAN = "\033[36m";
    static const std::string GRAY = "\033[90m";
}

struct Span 
{ 
    size_t s{}, e{}; 
    std::string type; 
};

class Highlighter 
{
public:
    // initialize with language("python" or "c")
    explicit Highlighter(std::string lang): language_(lang)
    {
        auto l = toLower(language_);
        if(l=="python")
        {
            rules_.push_back({"keyword",  std::regex(R"(\b(def|class|import|from|return|if|elif|else|for|while|try|except|with|as|pass|yield|lambda|in|not|and|or|True|False|None)\b)")});
            rules_.push_back({"string",   std::regex(R"((\"[^\"]*\"|'[^']*'))")});
            rules_.push_back({"comment",  std::regex(R"(#.*)")});
            rules_.push_back({"number",   std::regex(R"(\b\d+(?:\.\d+)?\b)")});
        } else if(l=="c")
        {
            rules_.push_back({"directive",std::regex(R"((^|\n)\s*#\w+.*)")});
            rules_.push_back({"keyword",  std::regex(R"(\b(int|float|double|char|void|if|else|for|while|return|struct|typedef|enum|static|const)\b)")});
            rules_.push_back({"string",   std::regex(R"((\"[^\"]*\"|'[^']*'))")});
            rules_.push_back({"comment",  std::regex(R"(//.*|/\*[\s\S]*?\*/)")});
            rules_.push_back({"number",   std::regex(R"(\b\d+(?:\.\d+)?\b)")});
        }
    }

    // highlights the given code and returns it with ANSI color codes
    std::string highlight(const std::string& code) const 
    {
        if(rules_.empty()) return code;

        // span means a range with a token type
        std::vector<Span> spans;
        // apply all rules
        for(const auto& r : rules_)
        {
            // iterate over all matches of the regex in the code
            for(std::sregex_iterator it(code.begin(), code.end(), r.pattern), end; it!=end; ++it)
            {
                // adds a new span for the matched token by computing its start and end positions
                spans.push_back(Span{(size_t)it->position(), (size_t)(it->position()+it->length()), r.tokenType});
            }
        }
        // sort spans by start position, then by end position
        // if starts are equal, shorter span comes first
        // if starts are different, earlier start comes first
        std::sort(spans.begin(), spans.end(), [](const Span&a, const Span&b)
        {
            return (a.s!=b.s)? a.s<b.s : a.e<b.e;
        });

        // now we have all spans, we need to merge them and apply colors

        std::string out; out.reserve(code.size()*1.2); // reserve some space to avoid too many reallocations
        size_t i=0;

        for(size_t pos=0; pos<code.size(); )
        {
            // skip spans that end before pos
            while(i<spans.size() && spans[i].e<=pos) ++i;

            // find the first span that contains pos
            const Span* chosen=nullptr;
            for(size_t j=i;j<spans.size();++j)
            {

                if(spans[j].s<=pos && pos<spans[j].e)
                { 
                    chosen=&spans[j]; break;
                }
                if(spans[j].s>pos) break;
            }
            if(chosen)
            {
                // apply color for the chosen span
                out += colorFor(chosen->type);
                out.append(code, pos, chosen->e-pos);
                out += ansi::RESET;
                pos = chosen->e;
            }else{
                // no span applies, just copy the character
                out.push_back(code[pos++]);
            }
        }
        return out;
    }

private:
    // A rule consists of a token type and a regex pattern to match that token
    struct Rule 
    { 
        std::string tokenType; 
        std::regex pattern; 
    };
    std::string language_;
    std::vector<Rule> rules_;

    // convert string to lower case
    static std::string toLower(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c)
        {
            return std::tolower(c);
        });
        return s;
    }

    // get ANSI color code for a token type
    static const std::string& colorFor(const std::string& t)
    {
        using namespace ansi;
        if(t=="keyword") return BLUE;
        if(t=="string")  return GREEN;
        if(t=="comment") return GRAY;
        if(t=="number")  return MAGENTA;
        if(t=="directive") return YELLOW;
        return RESET;
    }
};
