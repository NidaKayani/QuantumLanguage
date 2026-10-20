# Embeds the Java prelude (src/java/prelude/**/*.jpl) into a C++ source file
# defining java::preludeSources().
#   cmake -DDIR=<prelude dir> -DOUT=<generated .cpp> -P EmbedJavaPrelude.cmake
file(GLOB_RECURSE files RELATIVE "${DIR}" "${DIR}/*.jpl")
list(SORT files)
set(out "// Generated from src/java/prelude by cmake/EmbedJavaPrelude.cmake. Do not edit.\n")
string(APPEND out "#include <string>\n#include <utility>\n#include <vector>\n\nnamespace java\n{\n")
string(APPEND out "const std::vector<std::pair<std::string, std::string>> &preludeSources()\n{\n")
string(APPEND out "    static const std::vector<std::pair<std::string, std::string>> sources = {\n")
foreach(f IN LISTS files)
    file(READ "${DIR}/${f}" content)
    get_filename_component(name "${f}" NAME_WE)
    string(APPEND out "        {\"${name}.java\", R\"JAVA(${content})JAVA\"},\n")
endforeach()
string(APPEND out "    };\n    return sources;\n}\n} // namespace java\n")
# rewrite only on change, so an unchanged prelude does not trigger a rebuild
set(old "")
if(EXISTS "${OUT}")
    file(READ "${OUT}" old)
endif()
if(NOT old STREQUAL out)
    file(WRITE "${OUT}" "${out}")
endif()
