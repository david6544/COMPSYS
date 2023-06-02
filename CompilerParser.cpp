#include "CompilerParser.h"
#include "ParseTree.h"
#include "Token.h"
#include <vector>
#include <unordered_set>
#include <iostream>

using namespace std;

//List of allTokens Methods
allTokens::allTokens(){}

allTokens::allTokens(std::list<Token*> tokens){
    for (auto i = tokens.begin(); i != tokens.end(); ++i){
        this->tokens.push_back(*i);
    }
}

Token* allTokens::popToken() {
    //process token by checking through grammarMaps against typing
    Token * curr = this->tokens.front();
    if (curr == nullptr) throw ParseException();

    if (curr->getType() == "keyword" && grammarMaps::keyWords.find(curr->getValue()) == grammarMaps::keyWords.end()) {
        throw ParseException();
    } else if (curr->getType() == "keyword" && grammarMaps::symbols.find(curr->getValue()) == grammarMaps::symbols.end()) {
        throw ParseException();
    } else if (curr->getType() == "integerConstant") {
        int intConstant = stoi(curr->getValue());
        if (intConstant < 0 || intConstant > 32767) {
            throw ParseException();
        }
    } else if (curr->getType() == "stringConstant" && (curr->getValue().front() != '\"' || curr->getValue().back() != '\"')) {
        throw ParseException();
    } else if (curr->getType() == "identifier") {
        
        if (curr->getValue().find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_()") == string::npos) {
            throw ParseException();
        }
        if (curr->getValue().front() >= '0' && curr->getValue().front() <= '9') {
            throw ParseException();
        }
    }
    //pop front of tokens
    this->tokens.erase(this->tokens.begin());
    return curr;
}

Token* allTokens::top() {
    return tokens[0];
} 

Token* allTokens::top(int i) {
    return tokens[i];
}

string allTokens::popVal(int i) {
    return tokens[i]->getValue();
}

string allTokens::popType(int i) {
    return tokens[i]->getType();
}

string allTokens::toString() {
    string result = "";
    for (auto i = tokens.begin(); i != tokens.end(); ++i){
        result +=  (*i)->tostring();
    }
    return result;
}



/**
 * Constructor for the CompilerParser
 * @param tokens A linked list of tokens to be parsed
 */
CompilerParser::CompilerParser(std::list<Token*> tokens) {
    this->tokens = allTokens(tokens);
}

/**
 * Generates a parse tree for a single program
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileProgram() {
    //first check for a class
    if (tokens.popVal(0) == "class" && (tokens.popVal(1) == "Main" || tokens.popVal(1) == "main")){
        return compileClass();
    } else {
        throw ParseException();
    }
    return NULL;
}

/**
 * Generates a parse tree for a single class
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClass() {
    
    ParseTree *pTree = new ParseTree("class", "");

    pTree->addChild(tokens.popToken());
    pTree->addChild(tokens.popToken());
    pTree->addChild(tokens.popToken());

    Token* curr = tokens.top();
    while (curr != nullptr && curr->getValue() != "}") {

        //checkiung for classVariableDeclaration
        if (curr->getType() == "keyword" && (curr->getValue() == "static" || curr->getValue() == "field")) {
            pTree->addChild(compileClassVarDec());
        } else if (curr->getType() == "keyword" && (curr->getValue() == "constructor" || curr->getValue() == "function" || curr->getValue() == "method")) {
            pTree->addChild(compileSubroutine());
        } else {
            pTree->addChild(tokens.popToken());
        }
    }

    // ADD VALIDATE CLASS

    return pTree;
}

/**
 * Generates a parse tree for a static variable declaration or field declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClassVarDec() {
    ParseTree * pTree = new ParseTree("classVarDec", "");
    pTree->addChild(tokens.popToken());
    pTree->addChild(tokens.popToken());
    pTree->addChild(tokens.popToken());

   ParseTree * curr = tokens.top();

   while (curr != nullptr && curr->getValue() == ";") {
       pTree->addChild(tokens.popToken());
       pTree->addChild(tokens.popToken());
       curr = tokens.top();
   }

   //add semicol
    pTree->addChild(tokens.popToken());

    //validate classvardec

    return pTree;
}

/**
 * Generates a parse tree for a method, function, or constructor
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
    ParseTree *pTree = new ParseTree("subroutine", "");
    
    pTree->addChild(tokens.popToken()); // subtype
    pTree->addChild(tokens.popToken()); // return type
    pTree->addChild(tokens.popToken()); // add (

    pTree->addChild(compileParameterList()); // params

    pTree->addChild(tokens.popToken()); // add )

    pTree->addChild(compileSubroutineBody());

    //validate subroutine

    return pTree;
}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
    ParseTree *pTree = new ParseTree("parameterList", "");
    
    Token * curr = tokens.top();

    while (curr != nullptr && curr->getValue() != ")") {
        pTree->addChild(tokens.popToken());
        curr = tokens.top();
    }

    //add validation

    return pTree;
}

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() {
    ParseTree *pTree = new ParseTree("subroutineBody", "");
    
    pTree->addChild(tokens.popToken()); // add {

    Token * curr = tokens.top();

    while (curr != nullptr  && curr->getValue() != "}") {
        if (curr->getType() == "keyword" && curr->getValue() == "var") {
            pTree->addChild(compileVarDec());
        } else {
            pTree->addChild(compileStatements());
        }
        curr = tokens.top();
    }

    pTree->addChild(tokens.popToken()); // add }
    
    // add validation

    return pTree;
}

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
    ParseTree *pTree = new ParseTree("subroutineBody", "");
    
    pTree->addChild(tokens.popToken()); // var
    pTree->addChild(tokens.popToken()); // type
    pTree->addChild(tokens.popToken()); // first ident

    Token * curr = tokens.top();

    while(curr != nullptr && curr->getValue() != ";") {
        pTree->addChild(tokens.popToken()); // ,
        pTree->addChild(tokens.popToken()); // ident
        curr = tokens.top();
    }

    pTree->addChild(tokens.popToken()); // ;

    //validate

    
    return pTree;
}

/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileStatements() {
    return NULL;
}

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() {
    return NULL;
}

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() {
    return NULL;
}

/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {
    return NULL;
}

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() {
    return NULL;
}

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() {
    return NULL;
}

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() {
    return NULL;
}

/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileTerm() {
    return NULL;
}

/**
 * Generates a parse tree for an expression list
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpressionList() {
    return NULL;
}

/**
 * Advance to the next token
 */
void CompilerParser::next(){
    return;
}

/**
 * Return the current token
 * @return the Token
 */
Token* CompilerParser::current(){
    return NULL;
}

/**
 * Check if the current token matches the expected type and value.
 * @return true if a match, false otherwise
 */
bool CompilerParser::have(std::string expectedType, std::string expectedValue){
    return false;
}

/**
 * Check if the current token matches the expected type and value.
 * If so, advance to the next token, returning the current token, otherwise throw a ParseException.
 * @return the current token before advancing
 */
Token* CompilerParser::mustBe(std::string expectedType, std::string expectedValue){
    return NULL;
}

/**
 * Definition of a ParseException
 * You can use this ParseException with `throw ParseException();`
 */
const char* ParseException::what() {
    return "An Exception occurred while parsing!";
}
