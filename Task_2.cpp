#include <iostream>
#include <string>
#include "include/TextData.hh"
#include "include/FileReader.hh"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <logical-name> <full-path-to-txt>\n";
        return 1;
    }

    string name = argv[1];
    string path = argv[2];

    FileReader fr;
    string text = fr.readFileIntoString(path);

    TextData data(name, text);

    cout << "Text Report\n";
    cout << "Name: " << data.getFilename() << "\n";
    cout << "Content:\n" << data.getText() << "\n";
    cout << "Vowels: " << data.getNumberOfVowels() << "\n";
    cout << "Consonants: " << data.getNumberOfConsonants() << "\n";
    cout << "Letters: " << data.getNumberOfLetters() << "\n";
    cout << "Sentences: " << data.getNumberOfSentences() << "\n";
    cout << "Longest word: " << data.getLongestWord() << "\n";

    return 0;
}
