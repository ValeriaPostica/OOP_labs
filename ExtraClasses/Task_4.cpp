#include <iostream>
#include <string>
#include <vector>
#include "include/TextData.hh"
#include "include/FileReader.hh"

using namespace std;

// Usage example:
// ./task4 first /full/path/a.txt second /full/path/b.txt third /full/path/c.txt
int main(int argc, char* argv[]) {
    if (argc < 3 || ((argc - 1) % 2) != 0) {
        cerr << "Usage: " << argv[0] << " <name1> <path1> [<name2> <path2> ...]\n";
        return 1;
    }

    FileReader fr;

    int reportIndex = 1;
    for (int i = 1; i < argc; i += 2) {
        string name = argv[i];
        string path = argv[i + 1];

        string txt = fr.readFileIntoString(path);
        TextData data(name, txt);

        cout << "\nReport #" << reportIndex++ << "\n";
        cout << "Name: " << data.getFilename() << "\n";
        cout << "Vowels: " << data.getNumberOfVowels() << "\n";
        cout << "Consonants: " << data.getNumberOfConsonants() << "\n";
        cout << "Letters: " << data.getNumberOfLetters() << "\n";
        cout << "Sentences: " << data.getNumberOfSentences() << "\n";
        cout << "Longest: " << data.getLongestWord() << "\n";
    }
    cout << endl;
    return 0;
}
