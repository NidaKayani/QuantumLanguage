# Embeds the C/C++ front-end's library headers (lib/cpp/*: <filesystem>,
# <regex>, ...) into a C++ source file defining cpp::libraryHeaders().
#   cmake -DDIR=<lib/cpp> -DOUT=<generated .cpp> -P EmbedCppLib.cmake
file(GLOB_RECURSE files RELATIVE "${DIR}" "${DIR}/*")
list(SORT files)
set(out "// Generated from lib/cpp by cmake/EmbedCppLib.cmake. Do not edit.\n")
string(APPEND out "#include <string>\n#include <utility>\n#include <vector>\n\nnamespace cpp\n{\n")
string(APPEND out "const std::vector<std::pair<std::string, std::string>> &libraryHeaders()\n{\n")
string(APPEND out "    static const std::vector<std::pair<std::string, std::string>> headers = {\n")
foreach(f IN LISTS files)
    file(READ "${DIR}/${f}" content)
    string(APPEND out "        {\"${f}\", R\"QCPPLIB(${content})QCPPLIB\"},\n")
endforeach()
string(APPEND out "    };\n    return headers;\n}\n} // namespace cpp\n")
# rewrite only on change, so unchanged headers do not trigger a rebuild
set(old "")
if(EXISTS "${OUT}")
    file(READ "${OUT}" old)
endif()
if(NOT old STREQUAL out)
    file(WRITE "${OUT}" "${out}")
endif()
