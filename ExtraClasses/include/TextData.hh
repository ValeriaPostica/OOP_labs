#ifndef TEXTDATA_HH
#define TEXTDATA_HH

#include <iostream>
#include <string>

using namespace std;

class TextData {
private:
    string fileName;
    string text;

    int numberOfVowels;
    int numberOfConsonants;
    int numberOfLetters;
    int numberOfSentences;
    string longestWord;

    static bool isLetter(char c) {
        c = (char)tolower(c);
        return (c >= 'a' && c <= 'z');
    }

    static bool isVowel(char c) {
        c = (char)tolower(c);
        return (c=='a'||c=='e'||c=='i'||c=='o'||c=='u');
    }

    void computeStats() {
        numberOfVowels = numberOfConsonants = numberOfLetters = numberOfSentences = 0;
        longestWord.clear();

        string cur;
        for (size_t i = 0; i < text.size(); ++i) {
            char c = text[i];

            // sentence-ish delimiters kept simple
            if (c == '.' || c == '!' || c == '?' || c == '\n') {
                numberOfSentences++;
            }

            if (isLetter(c)) {
                numberOfLetters++;
                if (isVowel(c)) numberOfVowels++; else numberOfConsonants++;
                cur.push_back(c);
            } else {
                if (!cur.empty() && cur.size() > longestWord.size()) longestWord = cur;
                cur.clear();
            }
        }
        // handle last token if text doesn't end in delimiter
        if (!cur.empty() && cur.size() > longestWord.size()) longestWord = cur;
    }

public:
    TextData(string name, string txt) : fileName(name), text(txt) {
        computeStats();
    }

    string getFilename() const { return fileName; }
    string getText() const { return text; }
    int getNumberOfVowels() const { return numberOfVowels; }
    int getNumberOfConsonants() const { return numberOfConsonants; }
    int getNumberOfLetters() const { return numberOfLetters; }
    int getNumberOfSentences() const { return numberOfSentences; }
    string getLongestWord() const { return longestWord; }
};

#endif // TEXTDATA_HH
