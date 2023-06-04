#include "CompilerParser.h"
#include "ParseTree.h"
#include "Token.h"
#include <vector>
#include <unordered_set>
#include <iostream>

using namespace std;

/*
    ALL VALIDATORS FOR METHODS HERE
 */
    bool classValidator(ParseTree* pTree) {
        std::list<ParseTree *> children = pTree->getChildren();

    if (pTree->getType() != "class" || pTree->getValue() != "") {
        return false;
    }
    //if first element is not class or keyword class
    if (children.front()->getType() != "keyword" || children.front()->getValue() != "class") {
        return false;
    }

    // if second element isnt an identifier
    auto it = std::next(children.begin());
    if ((*it)->getType() != "identifier") {
        return false;
    }
    ++it;
    if ((*it)->getType() != "symbol" || (*it)->getValue() != "{") {
        return false;
    }

    ++it;
    while (it != children.end() && ((*it)->getType() != "symbol" && (*it)->getValue() != "}")) {
        if ((*it)->getType() != "classVarDec" && (*it)->getType() != "subroutine") {
            return false;
        }
        ++it;
    }

    if (it != std::prev(children.end()) || (*it)->getValue() != "}") {
        return false;
    }

    return true;
    }

    bool classVarDecValidator(ParseTree* pTree) {
        return true;
    }
    bool subroutineValidator(ParseTree* pTree) {
        return true;
    }
    bool parameterListValidator(ParseTree* pTree) {
        return true;
    }
    bool subroutineBodyValidator(ParseTree* pTree) {
        return true;
    }
    bool varDecValidator(ParseTree* pTree) {
        return true;
    }
    bool statementsValidator(ParseTree* pTree) {
        return true;
    }
    bool letValidator(ParseTree* pTree) {
        return true;
    }
    bool ifValidator(ParseTree* pTree) {
        return true;
    }
    bool whileValidator(ParseTree* pTree) {
        return true;
    }
    bool doValidator(ParseTree* pTree) {
        return true;
    }
    bool returnValidator(ParseTree* pTree) {
        return true;
    }
    bool expressionValidator(ParseTree* pTree) {
        return true;
    }
    bool termValidator(ParseTree* pTree) {
        return true;
    }
    bool expressionListValidator(ParseTree* pTree) {
        return true;
    }

//methods

//List of allTokens Methods

Token* CompilerParser::popToken() {
    //process token by checking through grammarMaps against typing
    Token * curr = this->tokens.front();
    if (curr == nullptr) throw ParseException();

    if (curr->getType() == "keyword") {
        if (grammarMaps::keyWords.find(curr->getValue()) == grammarMaps::keyWords.end()) {
            throw ParseException();
        }
    } else if (curr->getType() == "keyword") {
        if (grammarMaps::symbols.find(curr->getValue()) == grammarMaps::keyWords.end()) {
            throw ParseException();
        }
    } else if (curr->getType() == "integerConstant") {
        int intConstant = stoi(curr->getValue());
        if (intConstant < 0 || intConstant > 32767) {
            throw ParseException();
        }
    } else if (curr->getType() == "stringConstant" && (curr->getValue().front() != '\"' || curr->getValue().back() != '\"')) {
        throw ParseException();
    } else if (curr->getType() == "identifier") {
        
        if (curr->getValue().front() >= '0' && curr->getValue().front() <= '9') {
            throw ParseException();
        }
        if (curr->getValue().find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_()") == string::npos) {
            throw ParseException();
        }
    }
    //pop front of tokens
    tokens.pop_front();
    return curr;
}

Token* CompilerParser::top() {
    return tokens.front();
}
string CompilerParser::popVal(int i) {
    std::list<Token*>::iterator it;
	it = tokens.begin();
	advance(it,i);

	return (*it)->getValue();
}


/**
 * Constructor for the CompilerParser
 * @param tokens A linked list of tokens to be parsed
 */
CompilerParser::CompilerParser(std::list<Token*> tokens) {
    this->tokens = tokens;
}

/**
 * Generates a parse tree for a single program
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileProgram() {
    //first check for a class
    if (popVal(0) == "class" && (popVal(1) == "Main" || popVal(1) == "main")){
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

    pTree->addChild(popToken());
    pTree->addChild(popToken());
    pTree->addChild(popToken());

    Token* curr = top();
    while (curr != nullptr && curr->getValue() != "}") {

        //checkiung for classVariableDeclaration
        if (curr->getType() == "keyword" && (curr->getValue() == "static" || curr->getValue() == "field")) {
            pTree->addChild(compileClassVarDec());
        } else if (curr->getType() == "keyword" && (curr->getValue() == "constructor" || curr->getValue() == "function" || curr->getValue() == "method")) {
            pTree->addChild(compileSubroutine());
        } else {
            pTree->addChild(popToken());
        }
    }

    // ADD VALIDATE CLASS

    if (classValidator(pTree) == false) throw ParseException();

    return pTree;
}

/**
 * Generates a parse tree for a static variable declaration or field declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClassVarDec() {
    auto checkComma = [](ParseTree* pTree) {
        if (pTree==nullptr) return true;
        if (pTree->getValue() == ",") return true;
        return false;
    };

    ParseTree * pTree = new ParseTree("classVarDec", "");
    pTree->addChild(popToken());
    pTree->addChild(popToken());
    pTree->addChild(popToken());


   ParseTree *curr = top();

   while (checkComma(curr)) {
       pTree->addChild(popToken());
       pTree->addChild(popToken());
       curr = top();
   }

   //add semicol
    pTree->addChild(popToken());

    //validate classvardec

    return pTree;
}

/**
 * Generates a parse tree for a method, function, or constructor
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
    ParseTree *pTree = new ParseTree("subroutine", "");
    
    pTree->addChild(popToken()); // subtype
    pTree->addChild(popToken()); // return type
    pTree->addChild(popToken()); // add (

    pTree->addChild(compileParameterList()); // params

    pTree->addChild(popToken()); // add )

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
    
    Token * curr = top();

    while (curr != nullptr && curr->getValue() != ")") {
        pTree->addChild(popToken());
        curr = top();
    }

    //add validation

    return pTree;
}

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() {
    /* ParseTree *pTree = new ParseTree("subroutineBody", "");
    
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

    //return pTree;
    return NULL; */
}

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
   /*  ParseTree *pTree = new ParseTree("subroutineBody", "");
    
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

    return NULL;
    //return pTree; */
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
