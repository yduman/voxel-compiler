FILE(REMOVE_RECURSE
  "parser.cc"
  "parser.hh"
  "lexer.cc"
  "CMakeFiles/compiler.dir/compiler/main.cc.o"
  "CMakeFiles/compiler.dir/parser.cc.o"
  "CMakeFiles/compiler.dir/lexer.cc.o"
  "compiler.pdb"
  "compiler"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang CXX)
  INCLUDE(CMakeFiles/compiler.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
