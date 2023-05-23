set(grail-xdl
    std.cc
    SymbolTable.cc
    XDLCompiler.cc
)

list(TRANSFORM grail-xdl PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(grail-xdl ${grail-xdl} PARENT_SCOPE)