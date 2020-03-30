#include "semantic.h"
#include "tree.h"
#include <assert.h>
#include <string.h>

struct Variable *variableTable[TABLE_SIZE];
struct Structure *structureTable[TABLE_SIZE];
struct Func *funcTable[TABLE_SIZE];

unsigned hash(char *name)
{
    unsigned val = 0, i;
    for (; *name; ++name)
    {
        val = (val << 2) + *name;
        if (i = val & ~TABLE_SIZE)
            val = (val ^ (i >> 12)) & TABLE_SIZE;
    }
    return val;
}

void insertFuncTable(struct Func* func){
    assert(func != NULL);
    assert(func->name != NULL);
    unsigned idx = hash(func->name);
    struct Func* head = funcTable[idx];
    //头部插入
    func->next = head;
    funcTable[idx] = func;
}

void insertStructureTable(struct Structure*structure){
    assert(structure != NULL);
    assert(structure->structureInfo->kind == STRUCTURE);
    assert(structure->name != NULL);
    unsigned idx = hash(structure->name);
    struct Structure* head = structureTable[idx];
    //头部插入
    structure->next = head;
    structureTable[idx] = structure;
}

void insertVariableTable(struct Variable*variable){
    assert(variable != NULL);
    assert(variable->name != NULL);
    unsigned idx = hash(variable->name);
    struct Variable* head = variableTable[idx];
    //头部插入
    variable->next = head;
    variableTable[idx] = variable;
}

struct Func* searchFuncTable(char*name){
    assert(name != NULL);
    unsigned idx = hash(name);
    struct Func* head = funcTable[idx];
    while (head != NULL && (strcmp(name,head->name) != 0))
    {
        head = head->next;
    }
    return head;
}

struct Structure* searchStructureTable(char*name){
    assert(name != NULL);
    unsigned idx = hash(name);
    struct Structure* head = structureTable[idx];
    while (head != NULL && (strcmp(name,head->name) != 0))
    {
        head = head->next;
    }
    return head;
}

struct Variable* searchVariableTable(char*name){
    assert(name != NULL);
    unsigned idx = hash(name);
    struct Variable* head = variableTable[idx];
    while (head != NULL && (strcmp(name,head->name) != 0))
    {
        head = head->next;
    }
    return head;
}