#include <iostream>
#include <string>
#include<list>

#include "CompilerParser.h"
#include "ParseTree.h"
#include "Token.h"


using namespace std;


int main(void) {
   std::string s = "class main {function void test(){}}";
	std::cout << "Testing code - Expecting: Valid Tree\n" << s << std::endl<< std::endl;
    std::list<Token*> tokens;
    tokens.push_back(new Token("keyword", "class"));
    tokens.push_back(new Token("identifier", "main"));
    tokens.push_back(new Token("symbol", "{"));
    tokens.push_back(new Token("keyword", "function"));
    tokens.push_back(new Token("keyword", "void"));
    tokens.push_back(new Token("identifier", "test"));
    tokens.push_back(new Token("symbol", "("));
    tokens.push_back(new Token("symbol", ")"));
    tokens.push_back(new Token("symbol", "{"));
    tokens.push_back(new Token("symbol", "}"));
    tokens.push_back(new Token("symbol", "}"));
	bool error = false;


	CompilerParser parser = CompilerParser(tokens);
	try {

		ParseTree* tree = parser.compileClass();
		std::cout << tree->tostring();
	} catch (ParseException e) {
		error = true;
		std::cout << "Error while parsing!\n";
	}


    return 0;
}