#ifndef COMPILERPARSER_H
#define COMPILERPARSER_H

#include <list>
#include <exception>
#include <string>
#include<unordered_map>
#include<unordered_set>
#include<vector>


#include "ParseTree.h"
#include "Token.h"

struct allTokens
{
    private:
    std::vector<Token*> tokens;
    public:
    allTokens();
    allTokens(std::list<Token*>);

    Token* popToken();
    Token* top();
    std::string popVal(int i);

};


class CompilerParser {
    public:
        allTokens tokens;
        CompilerParser(std::list<Token*> tokens);

        ParseTree* compileProgram();
        ParseTree* compileClass();
        ParseTree* compileClassVarDec();
        ParseTree* compileSubroutine();
        ParseTree* compileParameterList();
        ParseTree* compileSubroutineBody();
        ParseTree* compileVarDec();

        ParseTree* compileStatements();
        ParseTree* compileLet();
        ParseTree* compileIf();
        ParseTree* compileWhile();
        ParseTree* compileDo();
        ParseTree* compileReturn();

        ParseTree* compileExpression();
        ParseTree* compileTerm();
        ParseTree* compileExpressionList();
        
        void next();
        Token* current();
        bool have(std::string expectedType, std::string expectedValue);
        Token* mustBe(std::string expectedType, std::string expectedValue);
};

class ParseException : public std::exception {
    public:
        const char* what();
};

namespace grammarMaps {
    const std::unordered_set<std::string> keyWords {
		"class", "constructor", "function", "method", "field", "static", "var",
		"int", "char", "boolean", "void", "true", "false", "null", "this",
		"let","do","if","else","while","return","skip"
	};

	const std::unordered_set<std::string> symbols {
		"{", "}", "(", ")", "[", "]", ".", ",", ";",
		"+", "-", "*", "/",
		"&", "|", "<", ">", "=", "~"
	};
    const std::unordered_set<std::string> variableTypes {
    "int", "char", "boolean"
  };

  const std::unordered_set<std::string> operators {
    "+", "-", "*", "/",
		"&", "|", "<", ">", "=", "~"
  };

  const std::unordered_map<std::string, std::string> keywordToToken {
    {"class", "class"},
    {"constructor","subroutine"},
    {"function","subroutine"},
    {"method","subroutine"},

    {"field","classVarDec"},
    {"static", "classVarDec"},

    {"var","varDec"},
    {"let","varDec"},

    {"let","letStatement"},
    {"if","ifStatement"},
    {"while","whileStatement"},
    {"do","doStatement"},
    {"return","returnStatement"},
    
    {"skip","expression"},
  };
}

#endif /*COMPILERPARSER_H*/