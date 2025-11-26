# IDE

A **terminal-first IDE** written in **C++17**:
- Users **create/open** a project, choose **Python** or **C**.
- Type code **line-by-line**; after every Enter the buffer is **syntax-highlighted** and **autocomplete** suggestions appear.
- Accept suggestions with `:1`, `:2`, …, or keep typing.
- When ready, `:run` will **build & run**:
  - Python → system Python (`python3`/`python`/`py`).
  - C → `gcc` or `clang` to produce `program.out` (or `program.exe` on Windows).

## Commands
```
:help   show help
:open   open existing project
:new    create new project
:lang   switch between python|c
:save   save buffer to file
:undo   remove last line
:clear  clear buffer
:show   show highlighted buffer
:run    build & run
:quit   exit
:1 ..   accept suggestion #N
```

# Classes and description

![IDE system diagram](terminal_ide_cpp\System_implemented_diagram.drawio.png)

# TerminalIDE

**Attributes**
- `workspace_` : std::string  
- `store_` : ProjectStore  
- `projectName_` : std::string  
- `language_` : std::string  
- `filename_` : std::string  
- `buffer_` : std::vector<std::string>  
- `highlighter_` : Highlighter  
- `completer_` : AutoCompleter  
- `lastSuggestions_` : std::pair<std::string, std::vector<std::string>>

The difference of attributes from my initial class are: Instead of just currentFile I added workspace, store, ProjectName and filename. It is done because I implemented the suggestion that AI and my coleague said, I added the Project class thus for management of projects I need a workspace where I will create projects and store with a file that will run. Other changes that I removed the paths to compiler and editor because now the IDE class itself independently of others represents an editor(just text editor) so for that I added buffer where the text will be saved and can be edited or added. Reste remained the same but in order to add other classes to the system I added the completer, highlighter and lastSuggestions to implement this IDE features.

**Public methods**
- `void loop()`  
- `void openOrCreate()`  
- `void save()`  
- `void run()`  
- `void setLanguage(lang)`  
- `void acceptSuggestion(idx)`  
- `void undo()`  
- `void clear()`  
- `void showBuffer()`

Main methods remained the same as: run, save, open/create. loop() method is basically editCode + other features, this is just the main working loop. And for a better experience with editor working I added undo() - remove last line clear() clear the buffer and showBuffer so we can see our code. They are all public because we use them in the loop. Also I took the suggesstion from AI from last lab and added the build, but not to make the main class too big I created a separate class Runner for that.

**Relations**
- TerminalIDE "has-a" ProjectStore (`store_`)
- TerminalIDE "has-a" Highlighter (`highlighter_`)
- TerminalIDE "has-a" AutoCompleter (`completer_`)
- TerminalIDE uses `Runner` at runtime to build & run projects

Thus without the system is a text editor, in a system it is an actual IDE.

---

# ProjectStore

**Attributes**
- `root_` : std::string

**Public methods**
- `bool exists(project)`  
- `std::string pathFor(project, filename)`  
- `void save(project, filename, content)`  
- `std::string load(project, filename)`

This is a new class that I added to my systme from the coleague sugestion. The attributes contain only the root directory. Then in the methods we have function to check if the project directory exists. Save is saving the code we written in the buffer to the file in the specific project.
And by loading we just load the code from the project file to the buffer to edit or run.

---

# Runner

**Attributes**
- `language_` : std::string  
- `workdir_` : std::string

**Public / Private methods**
- `RunResult run(project, filename)`  (public)  
- `static RunResult execCapture(cmd, cwd)`  (private/static)  
- `static bool which(exe)`  (private/static)  
- `RunResult runC(dir, filename)`  (private)  
- `RunResult runPython(dir, filename)`  (private)

The runner needs the language to know how to build and run a program. And of course the work directory of project to find it. Now about the methods: runn(): Determine language/workdir, call the language-specific runner (runC/runPython), and return RunResult (success + captured output).
Then we have execCapture executes the given command in the given working directory, captures output
The which just check if an executable exists in PATH. And finnaly runC which compiles the C code and runPython which interprets the python code.

Uses:
- `DirGuard` to temporarily change working directory  
- `RunResult` struct to return `ok` and `output`

---

# Highlighter

**Attributes**
- `language_` : std::string  
- `rules_` : std::vector<Rule>

**Public methods**
- `std::string highlight(code) const`

The highlighter class basically remained the same as in the lab 1.
It based on specific language uses another class(in my is just a struct because it's easier)
And highligth the text on specifi regex pattern for example "" means that this is a string. 

---

# AutoCompleter

**Attributes**
- `language_` : std::string  
- `learned_` : std::unordered_set<std::string>

**Public methods**
- `void learnFromBuffer(text)`  
- `std::vector<std::string> complete(prefix, maxItems=6)`

Helpers (documented as functions)
- `std::vector<std::string> baseSnippets() const`  
- `static const std::unordered_set<std::string>& reserved()`  
- `static std::string lower(std::string s)`
This are simple yet usefull function to lower string and also to save the base tokens that "predict"

This class is exactly as in the diagram, I just removed update the sugestion because  it is included already in the complete that gives the suggestion. Other stuff remained the same, attributes are language and learned - set of learned identifiers. The function learnFromBuffer just tokenize the text and add new words that could be recomended. And finnaly complete just finds words that has same prefix and suggest them.

Behavior: learns identifiers >=3 chars; suggests learned identifiers + language snippets.

---

# DirGuard

**Attributes**
- `old_` : std::filesystem::path

**Public methods**
- `DirGuard(path)` (constructor)  
- `~DirGuard()` (destructor)

This actually was suggestet by Chat when I was trying to implement the other class.
I added this because it helps better to change temporarily working directory. 
Chat explains this better: "to safely run build/run commands inside a project's folder without leaking the working-directory change if exceptions or early returns occur."

## Post Note: I choosed the methods to be private when they where needed only and only inside of the class. Otherwise they were public.

---

# RunResult

**Attributes**
- `ok` : bool  
- `output` : std::string

# AI sugestion
Actually after each commit I gave the code to the AI and asked the feedback, mainly he suggested how to optimize or to extend but the extention was already in my mind(the lab 1 system) and I consider that my project does not need much optimization it is not a big or too complex one.
About the actual changes: I mainly implemented the AI sugestions from last lab(runner, builder, project manager). But also added the DirGuard class as he sugested. Also the first version of my highlighter was too primitive and many color were one above another so AI sugested me sort function and span struct so highlighter will only apply one color on a specific part of code.

For the current version of project he suggested: 
- Trim or skip blank/whitespace lines before adding to buffer_.
- Quote/escape filenames (or use spawn/execv) instead of concatenating shell commands.
- Add timeout/stream cap to execCapture to avoid hangs/huge output.
- Improve missing-tool errors (mention install hints).
- Add a couple of unit tests for AutoCompleter and Highlighter.
- Move language-specific options to a small config (compiler flags, interpreter path).

The 2, 3, 4 seems to me reasonable changes so I think I should implement them.

# Feedback
About the feedback, I gave the project in zip to Nichita, but it was not the final version, but after the 4-5 commit don't quite remember, I know he observed that the highlighter for C is not working right and sugested to change the regex part of my code. He was amazed, he said:
"This is amaziiiingg! I can really use it as an actual IDE, and I am glad you accepted my 
suggestion about the first lab"
And also:
"Just as a recomandation could you maybe improve the undo functin? I made a small mistake and want to go to a specific line and change it not delete every line till that one" 
This sugestion is quite usefull I will implement this later. It really would be better with a more robust editor, I didn't notice that because i teste with small programs.


