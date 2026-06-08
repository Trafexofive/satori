// src/codegen.h - AST to bytecode compiler

#ifndef SATORI_CODEGEN_H
#define SATORI_CODEGEN_H

#include "frontend/ast.h"
#include "runtime/vm.h"
#include "core/source.h"

bool codegen_compile(AstNode *ast, Chunk *chunk, SourceInfo *source);

#endif // SATORI_CODEGEN_H
