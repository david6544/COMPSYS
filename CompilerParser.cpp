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
        list<ParseTree*> children = pTree->getChildren();
        list<ParseTree*>::iterator it = children.begin();

        if (pTree->getType() != "classVarDec" || pTree->getValue() != "") {
            throw ParseException();
        }
        if (children.front()->getType() != "keyword" || (children.front()->getValue() != "static" && children.front()->getValue() != "field")) {
            throw ParseException();
        }
        it = next(it);
        if ((*it)->getType() != "keyword" || grammarMaps::variableTypes.find((*it)->getValue()) == grammarMaps::variableTypes.end()) {
            throw ParseException();
        }

        it = next(it);

        for (int i = 2; std::next(it) != children.end(); i++) {
        if (i % 2 == 0) {
            if ((*it)->getType() != "identifier")
                return false;
        } else {
            if ((*it)->getType() != "symbol" || (*it)->getValue() != ",")
                return false;
        }
        ++it;
        }

        if ((*it)->getType() != "symbol" || (*it)->getValue() != ";")
            return false;

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

inline bool isValidStatement(ParseTree * curr) {
    if (curr == nullptr) return false;
    if (curr->getType() == "keyword" && curr->getValue() == "let") {
        return true;
    } else if (curr->getType() == "keyword" && curr->getValue() == "if") {
        return true;
    } else if (curr->getType() == "keyword" && curr->getValue() == "while") {
        return true;
    } else if (curr->getType() == "keyword" && curr->getValue() == "do") {
        return true;
    } else if (curr->getType() == "keyword" && curr->getValue() == "return") {
        return true;
    } else {
        return false;
    }
}

inline bool isValidToken(const string& type, const string& val, ParseTree* curr) {
    if (curr == nullptr) return false;
    if (curr ->getType() == type && curr->getValue() == val) return true;
    return false;
}

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
    if (popVal(0) == "class") {
        if  (popVal(1) == "Main" || popVal(1) == "main") {
        return compileClass();
        } 
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

    if (classVarDecValidator(pTree) == false) throw ParseException();

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
    ParseTree *pTree = new ParseTree("subroutineBody", "");
    
    pTree->addChild(popToken()); // add {

    Token * curr = top();

    while (curr != nullptr  && curr->getValue() != "}") {
        if (curr->getType() == "keyword" && curr->getValue() == "var") {
            pTree->addChild(compileVarDec());
        } else {
            pTree->addChild(compileStatements());
        }
        curr = top();
    }

    pTree->addChild(popToken()); // add }
    
    // add validation

    //return pTree;
    return pTree;
}

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
    ParseTree *pTree = new ParseTree("subroutineBody", "");
    
    pTree->addChild(popToken()); // var
    pTree->addChild(popToken()); // type
    pTree->addChild(popToken()); // first ident

    Token * curr = top();

    while(curr != nullptr && curr->getValue() != ";") {
        pTree->addChild(popToken()); // ,
        pTree->addChild(popToken()); // ident
        curr = top();
    }

    pTree->addChild(popToken()); // ;

    //validate

    return pTree;
    //return pTree;
}

/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileStatements() {
    ParseTree *pTree = new ParseTree("statements", "");
    ParseTree *curr = top();
    while (isValidStatement(curr)) {
        if (curr->getValue() == "do") {
            pTree->addChild(compileDo());
        } else if (curr->getValue() == "let") {
            pTree->addChild(compileLet());
        } else if (curr->getValue() == "return") {
            pTree->addChild(compileReturn());
        } else if (curr->getValue() == "if") {
            pTree->addChild(compileIf());
        } else if (curr->getValue() == "while") {
            pTree->addChild(compileWhile());
        }
        curr = top();
    }

    return pTree;
}

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() {
    ParseTree *pTree = new ParseTree("letStatement", "");

    pTree->addChild(popToken()); // let
    pTree->addChild(popToken()); // ident

    ParseTree *curr = top();
    if (curr->getValue() == "[") {
        pTree->addChild(popToken()); // [
        pTree->addChild(compileExpression()); // expression
        pTree->addChild(popToken()); // ]
    } else if (curr->getValue() == "=") {
        pTree->addChild(popToken()); // =
        pTree->addChild(compileExpression()); // expression
    } else {
        throw ParseException();
    }


    // add validator

    return pTree;
}

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() {
    ParseTree *pTree = new ParseTree ("ifStatement", "");

    pTree->addChild(popToken()); // if
    pTree->addChild(popToken()); // (
        pTree->addChild(compileExpression()); // expression
    pTree->addChild(popToken()); // )
    pTree->addChild(popToken()); // {
    

    ParseTree *curr = top();

    while (curr != nullptr && curr->getValue() != "}") {
        if (isValidStatement(curr)) {
            pTree->addChild(compileStatements());
        } else throw ParseException();
        curr = top();
    }

    if (pTree->getChildren().back() != nullptr && pTree->getChildren().back()->getValue()  == "{") {
        pTree->addChild(new ParseTree("statements","")); // }
    }

    pTree->addChild(popToken()); // }

    // check for an else block

    /* curr = top();
    if (curr != nullptr && curr->getValue() == "else") {
        pTree->addChild(popToken()); // else
        pTree->addChild(popToken()); // {
        curr = top();
        while (curr != nullptr && curr->getValue() != "}") {
            if (isValidStatement(curr)) {
                pTree->addChild(compileStatements());
            } else throw ParseException();
            curr = top();
        }
        if (curr->getValue() != "}") { throw ParseException();} 
        if (pTree->getChildren().back() != nullptr && pTree->getChildren().back()->getValue()  == "{") {
            pTree->addChild(new ParseTree("statements","")); // }
        }
        pTree->addChild(popToken()); // }
    }

    if (curr->getValue() == "{") { throw ParseException();} */

    // add validator;

    
    return pTree;
}

/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {
    ParseTree *pTree = new ParseTree("whileStatement", "");

    pTree->addChild(popToken()); // while
    pTree->addChild(popToken()); // (
        pTree->addChild(compileExpression()); // expression
    pTree->addChild(popToken()); // )
    pTree->addChild(popToken()); // {

    ParseTree *curr = top();

    while(curr != nullptr && curr->getValue() != "}") {
        if (isValidStatement(curr)) {
            pTree->addChild(compileStatements());
        }
        curr = top();
    }

    if (pTree->getChildren().back() != nullptr && pTree->getChildren().back()->getValue()  == "{") {
            pTree->addChild(new ParseTree("statements","")); // }
    }

    pTree->addChild(popToken()); // }

    // add validator
    return pTree;
}

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() {
    ParseTree * pTree = new ParseTree ("doStatement", "");

    pTree->addChild(popToken()); // do
    pTree->addChild(compileExpression()); // ident
    pTree->addChild(popToken()); // ;

    // add validator


    return pTree;
}

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() {
    ParseTree *pTree = new ParseTree("returnStatement", "");

    pTree->addChild(popToken()); // return

    if(top() != nullptr && top()->getValue() != ";") {
        pTree->addChild(compileExpression()); // expression
    }

    pTree->addChild(popToken()); // ;

    if (top() != nullptr && top()->getValue() != ";") {
        throw ParseException();
    }

    //ADD VALIDATOR

    return pTree;
}

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() {
    auto checkOperator = [](ParseTree* p) {
        if (p != nullptr) {
            if (p->getType() == "symbol") {
                if (grammarMaps::operators.find(p->getValue()) != grammarMaps::operators.end()) {
                    return true;
                }
            }
        }
        return false;
    };
    auto isEndOfStatement = [](ParseTree* p) {
        if (p == nullptr) return true;
        if (isValidToken("symbol", ")", p)) return true;
        if (isValidToken("symbol", "]", p)) return true;
        if (isValidToken("symbol", ";", p)) return true;
        if (isValidToken("symbol", ",", p)) return true;
        if (isValidToken("symbol", "}", p)) return true;
        return false;
    };
    ParseTree * pTree = new ParseTree("expression", "");

    while(!isEndOfStatement(top())) {
        if (checkOperator(top())) {
            pTree->addChild(popToken()); // operator
        } else {
            pTree->addChild(compileTerm()); // term
        }
    }

    // validator

    return pTree;
}

/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileTerm() {
    ParseTree *pTree = new ParseTree("term", "");
    ParseTree *curr = top();

    if (curr != nullptr && curr->getValue() != "(") {
        if (curr->getType() == "identifier") {
            pTree->addChild(popToken()); // ident
            curr = top();

            if (isValidToken("symbol","(",curr)) {
                pTree->addChild(popToken()); // (
                pTree->addChild(compileExpressionList()); // expression list
                pTree->addChild(popToken()); // )
            } else if (isValidToken("symbol", ".", curr)) {
                pTree->addChild(popToken()); // .
                pTree->addChild(popToken()); // ident
                pTree->addChild(popToken()); // (
                pTree->addChild(compileExpressionList()); // expression list
                pTree->addChild(popToken()); // )
            } else return pTree;
        } else if (curr->getType() == "integerConstant") {
            pTree->addChild(popToken()); // int
        } else if (curr->getType() == "stringConstant") {
            pTree->addChild(popToken()); // string
        } else if (curr->getType() == "keyword") {
            if (grammarMaps::keyWords.find(curr->getValue()) != grammarMaps::keyWords.end()) {
                pTree->addChild(popToken()); // keyword
            } else {
                throw ParseException();
            }
        }
    else if (isValidToken("symbol", "(", curr)) {
        pTree->addChild(popToken()); // (
        curr = top();

        while (curr != nullptr && curr->getValue() != ")") {
                pTree->addChild(compileExpression());
            curr = top();
        }
        
        if (isValidToken("symbol", ")",curr)) {
            pTree->addChild(popToken()); // )
        }
        } else {
            throw ParseException();
        }
    } else {
        throw ParseException();
    }
    return pTree;
}

/**
 * Generates a parse tree for an expression list
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpressionList() {
    ParseTree *pTree = new ParseTree("expressionList", "");
    auto isEndOfStatement = [](ParseTree* p) {
        if (p == nullptr) return true;
        if (isValidToken("symbol", ")", p)) return true;
        if (isValidToken("symbol", "]", p)) return true;
        if (isValidToken("symbol", ";", p)) return true;
        if (isValidToken("symbol", "}", p)) return true;
        return false;
    };

    ParseTree *curr = top();

    while (isEndOfStatement(curr) == false) {
        pTree->addChild(compileExpression());

        if (isValidToken("symbol",",",curr)) {
            pTree->addChild(popToken()); // ,
        }
        curr = top();
    }

    return pTree;
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
