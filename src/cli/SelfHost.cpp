// Support for the Quantum-written compiler in selfhost/: the C++ front-end's
// tokens and bytecode to check it against, and a runner for the bytecode
// files it writes (qrun --dump-tokens / --emit-qbc / --exec).
#include "Cli.h"
#include "Dialect.h"
#include "Error.h"
#include "Lexer.h"
#include "Parser.h"
#include "Pipeline.h"
#include "Serializer.h"
#include "Vm.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

namespace
{
bool readText(const std::string &path, std::string &out)
{
    std::ifstream in(path); // text mode, as runFile reads a program
    if (!in)
        return false;
    std::ostringstream ss;
    ss << in.rdbuf();
    out = ss.str();
    return true;
}

// A token value on one line: \ \n \t \r and other control bytes escaped.
std::string escaped(const std::string &s)
{
    std::string out;
    for (unsigned char c : s)
    {
        if (c == '\\')
            out += "\\\\";
        else if (c == '\n')
            out += "\\n";
        else if (c == '\t')
            out += "\\t";
        else if (c == '\r')
            out += "\\r";
        else if (c < 32 || c == 127)
        {
            char buf[8];
            std::snprintf(buf, sizeof buf, "\\x%02X", c);
            out += buf;
        }
        else
            out += (char)c;
    }
    return out;
}
} // namespace

int dumpTokens(const std::string &path)
{
    std::string source;
    if (!readText(path, source))
    {
        std::cerr << "[Error] Cannot open file: " << path << "\n";
        return 1;
    }
    try
    {
        Lexer lexer(source);
        for (const Token &t : lexer.tokenize())
            std::cout << static_cast<int>(t.type) << "\t" << t.line << "\t" << t.col << "\t" << escaped(t.value)
                      << "\n";
    }
    catch (const QuantumError &e)
    {
        std::cout << "ERROR\t" << e.kind << ": " << e.what() << "\n";
        return 1;
    }
    return 0;
}

int emitBytecodeFile(const std::string &path, const std::string &outPath)
{
    std::string source;
    if (!readText(path, source))
    {
        std::cerr << "[Error] Cannot open file: " << path << "\n";
        return 1;
    }
    try
    {
        auto chunk = compileSource(applyDialect(source, path), path, false);
        auto bytes = Serializer::serialize(chunk);
        std::ofstream out(outPath, std::ios::binary);
        out.write(reinterpret_cast<const char *>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
        return out ? 0 : 1;
    }
    catch (const ParseError &e)
    {
        std::cerr << path << ":" << e.line << ": ParseError: " << e.what() << "\n";
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[Error] " << e.what() << "\n";
        return 1;
    }
}

int execBytecodeFile(const std::string &path)
{
    std::ifstream in(path, std::ios::binary);
    if (!in)
    {
        std::cerr << "[Error] Cannot open bytecode file: " << path << "\n";
        return 1;
    }
    std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    try
    {
        auto chunk = Serializer::deserialize(bytes);
        VM vm;
        vm.run(chunk);
        return 0;
    }
    catch (const QuantumError &e)
    {
        std::cerr << Colors::RED << Colors::BOLD << "\n  X " << e.kind << Colors::RESET;
        if (e.line > 0)
            std::cerr << " at line " << e.line;
        std::cerr << "\n    " << e.what() << "\n\n";
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[Fatal] " << e.what() << "\n";
        return 1;
    }
}
