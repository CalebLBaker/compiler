#ifndef PARSER
#define PARSER

#include "includes.h"


struct Import {
};


struct Namespace {
};

struct VariableDeclaration {
};

struct TypeDeclaraion {
};

struct Routine {
};

struct Program {
	std::vector<Import> imports;
	std::vector<Namespace> namespaces;
	std::vector<Routine> routines;
	std::vector<VariableDeclaration> varDecls;
	std::vector<TypeDeclaration> typeDecls;
};

class Parser {
	public:
		Parser(std::string *filename) : lex(filename) {}
		~Parser(){}

		Import parseImport() {
			return Import();
		}

		Namespace parseNamespace() {
			return Namespace();
		}

		VariableDeclaration parseVarDecl() {
			return VariableDeclaration();
		}

		TypeDeclaration parseTypeDecl() {
			return TypeDeclaration();
		}

		Routine parseRoutine() {
			return Routine();
		}

		Program* parse() {
			Program *ret = new Program();
			
			while (lex.nextToken->type != END_OF_FILE && lex.nextToken->type != ERROR) {
				Token *nToken = lex.nextToken;
				switch(nToken->type) {
					case (LEFT_BRACKET):
					case (AMPERSAND):
					case (VAR):
					case (CONST):
					case (STATIC):
					case (IDENTIFIER) : {
						ret->varDecls.push_back(parseVarDecl());
						break;
					}
					case (ALIAS):
					case (STRUCT):
					case (ENUM):
					case (CLASS):
					case (UNION):
					case (INTERFACE):
					case (INSTANCE) : {
						ret->typeDecls.push_back(parseTypeDecl());
						break;
					}
					case (FUNC):
					case (PROC): {
						ret->routines.push_back(parseRoutine());
					}
					case (NAMESPACE):
					case (MODULE) : {
						ret->namespaces.push_back(parseNamespace());
						break;
					}
					case (IMPORT) : {
						ret->imports.push_back(parseImport());
						break;
					}
					case (LINE_COMMENT):
					case (BLOCK_COMMENT):
					case (OPEN_COMMENT):{
						lex.getNextToken();
						break;
					}
					default : {
						printf("Syntax error: unexpected token \'");
						printToken(nToken);
						printf("\' in global scope in %s (%u:%u).\n",  nToken->filename->c_str(), nToken->startLinenum, nToken->startColnum);
						delete ret;
						return nullptr;
					}
				}
			}
			if (lex.nextToken->type == END_OF_FILE) {
				return ret;
			}
			else {
				printToken(lex.nextToken);
				delete ret;
				return nullptr;
			}
		}

	private:
		Lexer lex;
};

Program *parse(std::string *filename) {
	Parser p(filename);
	return p.parse();
}

#endif
