#ifndef LEXER
#define LEXER

#include "includes.h"

#define TAB_WIDTH 4

//#define DEBUG

// Every type of token in the language
enum TokenType {

	// Operators
	LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE,
    DOT,
    COLON,
    SEMI_COLON,
	COMMA,
    ASSIGNMENT,
    LESS_THAN,
    GREATER_THAN,
    LOGICAL_NOT,
    ADDITION,
    SUBTRACT,
    ASTERISK,
    SLASH,
    MODULO,
    BITWISE_NOT,
    BITWISE_OR,
    BITWISE_XOR,
    AMPERSAND,
    ARROW,
    ADD_ASSIGN,
    SUBTRACT_ASSIGN,
    MULTIPLY_ASSIGN,
    DIV_ASSIGN,
    MOD_ASSIGN,
    OR_ASSIGN,
    XOR_ASSIGN,
    AND_ASSIGN,
    INCREMENT,
    DECREMENT,
    LOGICAL_OR,
    LOGICAL_XOR,
    LOGICAL_AND,
    LEFT_SHIFT,
    RIGHT_SHIFT,
    COMPARE,
    NOT_EQUAL,
    LESS_EQUAL,
    GREATER_EQUAL,
    LEFT_SHIFT_ASSIGN,
    RIGHT_SHIFT_ASSIGN,

	// Keywords
    CAST,
    RETURN,
    IF,
    ELSE,
    WHILE,
    FOR,
	FOREACH,
	IN,
	INDEX,
	SWITCH,
	CASE,
	CONT,
	BREAK,
    STRUCT,
    CLASS,
    UNION,
    INTERFACE,
    IMPL,
    FUNC,
    PROC,
    VAR,
	EXPORT,
	MODULE,
    IMPORT,
    FROM,
    GLOBALLY,
    AS,
	NAMESPACE,
    ALIAS,
    ENUM,
    CONST,
    STATIC,
	INSTANCE,

    IDENTIFIER,

	// Literals
	INTEGER,
	FLOAT,
    CHARACTER_LITERAL,
    STRING_LITERAL,

	// Miscelaneous
    LINE_COMMENT,
	BLOCK_COMMENT,
	OPEN_COMMENT,
	END_OF_FILE,
	ERROR
};


// All of the states that the lexer can be in.
enum State {
	start,
	haveEqual,
	haveLess,
	haveGreat,
	haveBang,
	havePlus,
	haveMinus,
	haveStar,
	haveSlash,
	haveMod,
	havePipe,
	haveCarot,
	haveAmper,
	haveSingleQuote,
	haveZero,
	inDecimalLeft,
	inId,
	leftShift,
	rightShift,
	singleLineComment,
	blockComment,
	escapeCharLit,
	haveCharLit,
	escapeString,
	stringLiteral,
	inDecimalRight,
	inBinaryLeft,
	inOctalLeft,
	inHexLeft,
	startingExponent,
	inExponent,
	leavingComment,
	escapeCharOctal,
	escapeCharHex,
	escapeStringOctal,
	escapeStringHex,
	inBinaryRight,
	inOctalRight,
	inHexRight,
	done
};


// Class to represent tokens.
class Token {

	public:

		std::string *filename;
		void *data;
		TokenType type;
		unsigned startLinenum;
		unsigned startColnum;
		unsigned endLinenum;
		unsigned endColnum;

		Token(TokenType t = ERROR, unsigned sl = 0, unsigned sc = 0, unsigned el = 0, unsigned ec = 0, std::string *fn = nullptr, void *d = nullptr) {
			type = t;
			data = d;
			startLinenum = sl;
			startColnum = sc;
			endLinenum = el;
			endColnum = ec;
			filename = fn;
		}

		~Token() {
			if (data != nullptr) {
				switch (type) {
					case (IDENTIFIER) :
					case (STRING_LITERAL) :
					case (LINE_COMMENT) :
					case (BLOCK_COMMENT) :
					case (OPEN_COMMENT) :
					case (ERROR) :
						delete (std::string*) data;
						break;
					case (CHARACTER_LITERAL) :
						delete (char*) data;
						break;
					case (INTEGER) :
						delete (unsigned long long*) data;
						break;
					case (FLOAT) :
						delete (double*) data;
						break;
					default : {}
				}
			}
		}			
};


// The lexer.
class Lexer {

	public:

		std::string filename;		// The name of the file being lexed.

		Token *nextToken;	// The next token in the input stream.

		unsigned linenum;	// The number of lines in the file that have been lexed.

		unsigned colnum;	// The number of characters on the current line that have been lexed.

		// Constructor.
		Lexer(std::string *file) {
			filename = *file;
			f = fopen(filename.c_str(), "r");
			linenum = 0;
			colnum = 0;
			getNextToken();
			if (f == nullptr) {
				printf("Error: could not open %s\n", filename.c_str());
			}
		}

		// Destructor.
		// filename is not freed because the tokens that the lexer created have copies of the pointer.
		~Lexer() {
			if (f != nullptr) {
				fclose(f);
			}
		}


		// Function to lex the next token.
		Token* getNextToken() {

			Token *ret = nextToken;

			State state = start;

			std::string *stringVal;
			unsigned long long intVal = 0;
			unsigned charVal = 0;
			double floatVal = 0.0;

			unsigned startLine = linenum;
			unsigned startCol = colnum;
			unsigned prevLineLen = 0;
			bool ready = true;
			bool negative = false;

			while (state != done) {

				bool repented = false;
				char in = getc(f);
	

				switch(state) {

					case (start) : {		

						switch (in) {

							case ('(') : {
								nextToken = new Token(LEFT_PAREN, startLine, startCol, linenum, colnum, &filename);
								state = done;
								break;
							}
							case (')') : {
								nextToken = new Token(RIGHT_PAREN, startLine, startCol, linenum, colnum, &filename);
								state = done;
								break;
							}
							case ('[') : {
								nextToken = new Token(LEFT_BRACKET, startLine, startCol, linenum, colnum, &filename);
								state = done;
								break;
							}
							case (']') : {
								nextToken = new Token(RIGHT_BRACKET, startLine, startCol, linenum, colnum, &filename);
								state = done;
								break;
							}
							case ('{') : {
								nextToken = new Token(LEFT_BRACE, startLine, startCol, linenum, colnum, &filename);
								state = done;
								break;
							}
							case ('}') : {
								nextToken = new Token(RIGHT_BRACE, startLine, startCol, linenum, colnum, &filename);
								state = done;
								break;
							}
							case ('.') : {
								nextToken = new Token(DOT, startLine, startCol, linenum, colnum, &filename);
								state = done;
								break;
							}
							case (':') : {
								nextToken = new Token(COLON, startLine, startCol, linenum, colnum, &filename);
								state = done;
								break;
							}
							case (';') : {
								nextToken = new Token(SEMI_COLON, startLine, startCol, linenum, colnum, &filename);
								state = done;
								break;
							}
							case (',') : {
								nextToken = new Token(COMMA, startLine, startCol, linenum, colnum, &filename);
								state = done;
								break;
							}
							case ('~') : {
								nextToken = new Token(BITWISE_NOT, startLine, startCol, linenum, colnum, &filename);
								state = done;
								break;
							}
							case ('=') : {
								state = haveEqual;
								break;
							}
							case ('<') : {
								state = haveLess;
								break;
							}
							case ('>') : {
								state = haveGreat;
								break;
							}
							case ('!') : {
								state = haveBang;
								break;
							}
							case ('+') : {
								state = havePlus;
								break;
							}
							case ('-') : {
								state = haveMinus;
								break;
							}
							case ('*') : {
								state = haveStar;
								break;
							}
							case ('/') : {
								state = haveSlash;
								break;
							}
							case ('%') : {
								state = haveMod;
								break;
							}
							case ('|') : {
								state = havePipe;
								break;
							}
							case ('^') : {
								state = haveCarot;
								break;
							}
							case ('&') : {
								state = haveAmper;
								break;
							}
							case ('\'') : {
								state = haveSingleQuote;
								break;
							}
							case ('\"') : {
								stringVal = new std::string();
								state = stringLiteral;
								break;
							}
							case ('0') : {
								state = haveZero;
								break;
							}
							case (EOF) : {
								nextToken = new Token(END_OF_FILE, startLine, startCol, linenum, colnum, &filename);
								state = done;
								break;
							}

							default : {
								if (isdigit(in)) {
									intVal = in - '0';
									state = inDecimalLeft;
								}
								else if (isspace(in)) {
									if (in == '\n') {
										startLine++;
										startCol = 0;
									}
									else if (in == '\t') {
										startCol += (TAB_WIDTH - (colnum & (TAB_WIDTH - 1)));
									}
									else {
										startCol++;
									}
								}

								// Any character that's not used for anything else can be in an identifier.
								else {
									stringVal = new std::string(1, in);
									state = inId;
								}
								break;
							}
						}
						break;
					}

					// =
					case (haveEqual) : {
						if (in == '=') {
							nextToken = new Token(COMPARE, startLine, startCol, linenum, colnum, &filename);
						}
						else {
							ungetc(in, f);
							nextToken = new Token(ASSIGNMENT, startLine, startCol, linenum, colnum - 1, &filename);
							repented = true;
						}
						state = done;
						break;
					}

					// <
					case (haveLess) : {
						if (in == '=') {
							nextToken = new Token(LESS_EQUAL, startLine, startCol, linenum, colnum, &filename);
							state = done;
						}
						else if (in == '<') {
							state = leftShift;
						}
						else {
							ungetc(in, f);
							nextToken = new Token(LESS_THAN, startLine, startCol, linenum, colnum - 1, &filename);
							repented = true;
							state = done;
						}
						break;
					}

					// >
					case (haveGreat) : {
						if (in == '=') {
							nextToken = new Token(GREATER_EQUAL, startLine, startCol, linenum, colnum, &filename);
							state = done;
						}
						else if (in == '<') {
							state = rightShift;
						}
						else {
							ungetc(in, f);
							nextToken = new Token(GREATER_THAN, startLine, startCol, linenum, colnum - 1, &filename);
							repented = true;
							state = done;
						}
						break;
					}

					// !
					case (haveBang) : {
						if (in == '=') {
							nextToken = new Token(NOT_EQUAL, startLine, startCol, linenum, colnum, &filename);
						}
						else {
							ungetc(in, f);
							nextToken = new Token(LOGICAL_NOT, startLine, startCol, linenum, colnum - 1, &filename);
							repented = true;
						}
						state = done;
						break;
					}

					// +
					case (havePlus) : {
						if (in == '+') {
							nextToken = new Token(INCREMENT, startLine, startCol, linenum, colnum, &filename);
						}
						else if (in == '=') {
							nextToken = new Token(ADD_ASSIGN, startLine, startCol, linenum, colnum, &filename);
						}
						else {
							ungetc(in, f);
							nextToken = new Token(ADDITION, startLine, startCol, linenum, colnum - 1, &filename);
							repented = true;
						}
						state = done;
						break;
					}

					// -
					case (haveMinus) : {
						if (in == '-') {
							nextToken = new Token(DECREMENT, startLine, startCol, linenum, colnum, &filename);
						}
						else if (in == '>') {
							nextToken = new Token(ARROW, startLine, startCol, linenum, colnum, &filename);
						}
						else if (in == '=') {
							nextToken = new Token(SUBTRACT_ASSIGN, startLine, startCol, linenum, colnum, &filename);
						}
						else {
							ungetc(in, f);
							nextToken = new Token(SUBTRACT, startLine, startCol, linenum, colnum - 1, &filename);
							repented = true;
						}
						state = done;
						break;
					}

					// *
					case (haveStar) : {
						if (in == '=') {
							nextToken = new Token(MULTIPLY_ASSIGN, startLine, startCol, linenum, colnum, &filename);
						}
						else {
							ungetc(in, f);
							nextToken = new Token(ASTERISK, startLine, startCol, linenum, colnum - 1, &filename);
							repented = true;
						}
						state = done;
						break;
					}

					// /
					case (haveSlash) : {
						if (in == '/') {
							stringVal = new std::string();
							state = singleLineComment;
						}
						else if (in == '*') {
							stringVal = new std::string();
							state = blockComment;
						}
						else if (in == '=') {
							nextToken = new Token(DIV_ASSIGN, startLine, startCol, linenum, colnum, &filename);
							state = done;
						}
						else {
							ungetc(in, f);
							nextToken = new Token(SLASH, startLine, startCol, linenum, colnum - 1, &filename);
							repented = true;
							state = done;
						}
						break;
					}

					// %
					case (haveMod) : {
						if (in == '=') {
							nextToken = new Token(MOD_ASSIGN, startLine, startCol, linenum, colnum, &filename);
						}
						else {
							ungetc(in, f);
							nextToken = new Token(MODULO, startLine, startCol, linenum, colnum - 1, &filename);
							repented = true;
						}
						state = done;
						break;
					}

					// |
					case (havePipe) : {
						if (in == '|') {
							nextToken = new Token(LOGICAL_OR, startLine, startCol, linenum, colnum, &filename);
						}
						else if (in == '=') {
							nextToken = new Token(OR_ASSIGN, startLine, startCol, linenum, colnum, &filename);
						}
						else {
							ungetc(in, f);
							nextToken = new Token(BITWISE_OR, startLine, startCol, linenum, colnum - 1, &filename);
							repented = true;
						}
						state = done;
						break;
					}

					// ^
					case (haveCarot) : {
						if (in == '^') {
							nextToken = new Token(LOGICAL_XOR, startLine, startCol, linenum, colnum, &filename);
						}
						else if (in == '=') {
							nextToken = new Token(XOR_ASSIGN, startLine, startCol, linenum, colnum, &filename);
						}
						else {
							ungetc(in, f);
							nextToken = new Token(BITWISE_XOR, startLine, startCol, linenum, colnum - 1, &filename);
							repented = true;
						}
						state = done;
						break;
					}

					// &
					case (haveAmper) : {
						if (in == '&') {
							nextToken = new Token(LOGICAL_AND, startLine, startCol, linenum, colnum, &filename);
						}
						else if (in == '=') {
							nextToken = new Token(AND_ASSIGN, startLine, startCol, linenum, colnum, &filename);
						}
						else {
							ungetc(in, f);
							nextToken = new Token(AMPERSAND, startLine, startCol, linenum, colnum - 1, &filename);
							repented = true;
						}
						state = done;
						break;
					}

					// '
					case (haveSingleQuote) : {
						if (in == '\\') {
							state = escapeCharLit;
						}
						else if (in == EOF) {
							stringVal = new std::string("Lex Error: unterminated character literal in ");
							makeError(stringVal, startLine, startCol);
							state = done;
						}
						else if (in == '\'') {
							stringVal = new std::string("Lex Error: empty character literal in ");
							makeError(stringVal, startLine, startCol);
							state = done;
						}
						else {
							charVal = (unsigned) in;
							state = haveCharLit;
						}
						break;
					}

					// In a string literal
					case (stringLiteral) : {

						if (in == '\"') {
							nextToken = new Token(STRING_LITERAL, startLine, startCol, linenum, colnum, &filename, (void*)stringVal);
							state = done;
						}
						else if (in == '\\') {
							state = escapeString;
						}
						else if (in == EOF) {
							*stringVal = "Lex Error: unterminated string literal in ";
							makeError(stringVal, startLine, startCol);
							state = done;
						}
						else {
							*stringVal += in;
						}
						break;
					}

					// 0
					case (haveZero) : {

						switch (in) {
							case ('.') : {
								state = inDecimalRight;
								intVal = 1;
								ready = false;
								break;
							}
							case ('b') : {
								state = inBinaryLeft;
								ready = false;
								break;
							}
							case ('o') : {
								state = inOctalLeft;
								ready = false;
								break;
							}
							case ('d') : {
								state = inDecimalLeft;
								ready = false;
								break;
							}
							case ('x') : {
								state = inHexLeft;
								ready = false;
								break;
							}
							default : {
								if (isdigit(in)) {
									intVal = in - '0';
									ready = true;
									state = inDecimalLeft;
								}
								else if (in == 'e' || in == 'E') {
									state = startingExponent;
								}
								else {
									unsigned long long *val = new unsigned long long(0);
									ungetc(in, f);
									nextToken = new Token(INTEGER, startLine, startCol, linenum, colnum, &filename, (void*)val);
									repented = true;
									state = done;
								}
								break;
							}
						}
						break;
					}

					// In what, so far, appears to be a decimal integer literal.
					case (inDecimalLeft) : {
						if (!(ready || isdigit(in))) {
							stringVal = new std::string("Lex Error: Numeric literal missing integral part in ");
							makeError(stringVal, startLine, startCol);
							state = done;
						}
						else if (in == '.') {
							floatVal = (double) intVal;
							intVal = 1;
							ready = false;
							state = inDecimalRight;
						}
						else if (in == 'e' || in == 'E') {
							floatVal = (double) intVal;
							intVal = 0;
							state = startingExponent;
						}
						else if (isdigit(in)) {
							intVal *= 10;
							intVal += (in - '0');
							ready = true;
						}
						else {
							unsigned long long *val = new unsigned long long(intVal);
							nextToken = new Token(INTEGER, startLine, startCol, linenum, colnum, &filename, (void*)val);
							ungetc(in, f);
							repented = true;
							state = done;
						}
						break;
					}

					// In either an identifier or a keyword.
					case (inId) : {

						// These are all of the characters that are not allowed in identifiers.
						if (in == '(' || in == ')' || in == '[' || in == ']' || in == '{' || in == '}' || in == '.' || in == ':' || in == ';' ||
							in == ',' || in == '~' || in == '!' || in == '=' || in == '<' || in == '>' || in == '+' || in == '-' || in == '*' ||
							in == '/' || in == '%' || in == '|' || in == '^' || in == '&' || in == '\'' || in == '\"' || in == EOF || isspace(in)) {

							if (*stringVal == "CAST") {
								delete stringVal;
								nextToken = new Token(CAST, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "return") {
								delete stringVal;
								nextToken = new Token(RETURN, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "if") {
								delete stringVal;
								nextToken = new Token(IF, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "else") {
								delete stringVal;
								nextToken = new Token(ELSE, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "while") {
								delete stringVal;
								nextToken = new Token(WHILE, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "for") {
								delete stringVal;
								nextToken = new Token(FOR, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "foreach") {
								delete stringVal;
								nextToken = new Token(FOREACH, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "in") {
								delete stringVal;
								nextToken = new Token(IN, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "index") {
								delete stringVal;
								nextToken = new Token(INDEX, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "switch") {
								delete stringVal;
								nextToken = new Token(SWITCH, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "case") {
								delete stringVal;
								nextToken = new Token(CASE, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "continue") {
								delete stringVal;
								nextToken = new Token(CONT, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "break") {
								delete stringVal;
								nextToken = new Token(BREAK, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "struct") {
								delete stringVal;
								nextToken = new Token(STRUCT, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "class") {
								delete stringVal;
								nextToken = new Token(CLASS, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "union") {
								delete stringVal;
								nextToken = new Token(UNION, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "interface") {
								delete stringVal;
								nextToken = new Token(INTERFACE, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "impl") {
								delete stringVal;
								nextToken = new Token(IMPL, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "func") {
								delete stringVal;
								nextToken = new Token(FUNC, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "proc") {
								delete stringVal;
								nextToken = new Token(PROC, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "var") {
								delete stringVal;
								nextToken = new Token(VAR, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "export") {
								delete stringVal;
								nextToken = new Token(EXPORT, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "module") {
								delete stringVal;
								nextToken = new Token(MODULE, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "import") {
								delete stringVal;
								nextToken = new Token(IMPORT, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "from") {
								delete stringVal;
								nextToken = new Token(FROM, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "globally") {
								delete stringVal;
								nextToken = new Token(GLOBALLY, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "as") {
								delete stringVal;
								nextToken = new Token(AS, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "namespace") {
								delete stringVal;
								nextToken = new Token(NAMESPACE, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "alias") {
								delete stringVal;
								nextToken = new Token(ALIAS, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "enum") {
								delete stringVal;
								nextToken = new Token(ENUM, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "const") {
								delete stringVal;
								nextToken = new Token(CONST, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "static") {
								delete stringVal;
								nextToken = new Token(STATIC, startLine, startCol, linenum, colnum, &filename);
							}
							else if (*stringVal == "instance") {
								delete stringVal;
								nextToken = new Token(INSTANCE, startline, startCol, linenum, colnum, &filename);
							}
							else {
								nextToken = new Token(IDENTIFIER, startLine, startCol, linenum, colnum, &filename, (void*)stringVal);
							}

							ungetc(in, f);
							repented = true;
							state = done;
						}
						else {
							*stringVal += in;
						}
						break;
					}

					// <<
					case (leftShift) : {
						if (in == '=') {
							nextToken = new Token(LEFT_SHIFT_ASSIGN, startLine, startCol, linenum, colnum, &filename);
						}
						else {
							nextToken = new Token(LEFT_SHIFT, startLine, startCol, linenum, colnum, &filename);
							ungetc(in, f);
							repented = true;
						}
						state = done;
						break;
					}

					// >>
					case (rightShift) : {
						if (in == '=') {
							nextToken = new Token(RIGHT_SHIFT_ASSIGN, startLine, startCol, linenum, colnum, &filename);
						}
						else {
							nextToken = new Token(RIGHT_SHIFT, startLine, startCol, linenum, colnum, &filename);
							ungetc(in, f);
							repented = true;
						}
						state = done;
						break;
					}

					// In a single line comment.
					case (singleLineComment) : {
						if (in == '\n' || in == EOF) {
							nextToken = new Token(LINE_COMMENT, startLine, startCol, linenum, colnum, &filename, (void*)stringVal);
							state = done;
							if (in == EOF) {
								ungetc(in, f);
								repented = true;
							}
						}
						else {
							*stringVal += in;
						}
						break;
					}

					/* In a block comment */
					case (blockComment) : {
						if (in == '*') {
							*stringVal += in;
							state = leavingComment;
						}
						else if (in == EOF) {
							printf("Warning: unterminated comment in %s: (%u:%u).\n", filename.c_str(), linenum, colnum);
							nextToken = new Token(OPEN_COMMENT, startLine, startCol, linenum, colnum, &filename, (void*)stringVal);
							state = done;
						}
						else {
							*stringVal += in;
						}
						break;
					}

					// In an character literal that happens to be an escape character.
					case (escapeCharLit) : {
						if ('0' <= in && in < '8') {
							charVal = in - '0';
							intVal = 2;
							state = escapeCharOctal;
						}
						else if (in == 'x') {
							intVal = 2;
							state = escapeCharHex;
						}
						else if (in == 'u') {
							intVal = 4;
							state = escapeCharHex;
						}
						else if (in == 'U') {
							intVal = 8;
							state = escapeCharHex;
						}
						else if (in == EOF) {
							stringVal = new std::string("Lex Error: unterminated character literal in ");
							makeError(stringVal, startLine, startCol);
							state = done;
						}
						else {
							charVal = escapeCharacter(in);
							state = haveCharLit;
						}
						break;
					}

					// Wrapping up a character literal and need a closing '
					case (haveCharLit) : {
						if (in == '\'') {
							unsigned *val = new unsigned(charVal);
							nextToken = new Token(CHARACTER_LITERAL, startLine, startCol, linenum, colnum, &filename, (void*)val);
							state = done;
						}
						else {
							stringVal = new std::string("Lex Error: unterminated character literal in ");
							makeError(stringVal, startLine, startCol);
							state = done;
						}
						break;
					}

					// In a string literal and just found a back-slash 
					case (escapeString) : {
						switch(in) {
							case ('x') : {
								intVal = 2;
								charVal = 0;
								state = escapeStringHex;
								break;
							}
							case ('u') : {
								intVal = 4;
								charVal = 0;
								state = escapeStringHex;
								break;
							}
							case ('U') : {
								intVal = 8;
								charVal = 0;
								state = escapeStringHex;
								break;
							}
							case (EOF) : {
								*stringVal = ("Lex Error: unterminated string literal in ");
								makeError(stringVal, startLine, startCol);
								state = done;
								break;
							}
							default : {
								if ('0' <= in  && in < '8') {
									intVal = 2;
									charVal = in - '0';
									state = escapeStringOctal;
								}
								else {
									*stringVal += escapeCharacter(in);
									state = stringLiteral;
								}
							}
						}
						break;
					}

					// After the decimal point in a decimal floating point number
					case (inDecimalRight) : {
						if (!(ready || isdigit(in))) {
							stringVal = new std::string("Lex Error: Numeric literal missing fractional part in ");
							makeError(stringVal, startLine, startCol);
							state = done;
						}
						else if (isdigit(in)) {
							ready = true;
							floatVal += (double(in - '0')) / intVal;
							intVal *= 10;
						}
						else if (in == 'e' || in == 'E') {
							intVal = 0;
							state = startingExponent;
						}
						else {
							double *val = new double(floatVal);
							nextToken = new Token(FLOAT, startLine, startCol, linenum, colnum, &filename, (void*)val);
							ungetc(in, f);
							repented = true;
							state = done;
						}
						break;
					}

					// In what, so far,  appears to be a binary integer
					case (inBinaryLeft) : {
						if (!(ready || in == '0' || in == '1')) {
							stringVal = new std::string("Lex Error: Numeric literal missing integral part in ");
							makeError(stringVal, startLine, startCol);
							state = done;
						}
						else if (in == '.') {
							floatVal = (double) intVal;
							intVal = 1;
							ready = false;
							state = inBinaryRight;
						}
						else if (in == '0' || in == '1') {
							intVal <<= 1;
							ready = true;
							intVal += (in - '0');
						}
						else {
							unsigned long long *val = new unsigned long long(intVal);
							nextToken = new Token(INTEGER, startLine, startCol, linenum, colnum, &filename, (void*)val);
							ungetc(in, f);
							repented = true;
							state = done;
						}
						break;
					}

					// In what, so far, appears to be an octal integer
					case (inOctalLeft) : {
						if (!ready && (in < '0' || in > '7')) {
							stringVal = new std::string("Lex Error: Numeric literal missing integral part in ");
							makeError(stringVal, startLine, startCol);
							state = done;
						}
						else if (in == '.') {
							floatVal = (double) intVal;
							ready = false;
							intVal = 1;
							state = inOctalRight;
						}
						else if ('0' <= in && in  < '8') {
							ready = true;
							intVal <<= 3;
							intVal += (in - '0');
						}
						else {
							unsigned long long *val = new unsigned long long(intVal);
							nextToken = new Token(INTEGER, startLine, startCol, linenum, colnum, &filename, (void*)val);
							ungetc(in, f);
							repented = true;
							state = done;
						}
						break;
					}

					// In what, so far, appears to be a hexadecimal integer
					case (inHexLeft) : {
						if (!(ready || isxdigit(in))) {
							stringVal = new std::string("Lex Error: Numeric literal missing integral part in ");
							makeError(stringVal, startLine, startCol);
							state = done;
						}
						if (in == '.') {
							floatVal = (double) intVal;
							ready = false;
							intVal = 1;
							state = inHexRight;
						}
						else if (isdigit(in)) {
							intVal <<= 4;
							intVal += (in - '0');
							ready = true;
						}
						else if ('a' <= in && in < 'g') {
							intVal <<= 4;
							intVal += (in - 'a' + 0xa);
							ready = true;
						}
						else if ('A' <= in && in < 'G') {
							intVal <<= 4;
							intVal += (in - 'A' + 0xA);
							ready = true;
						}
						else {
							unsigned long long *val = new unsigned long long(intVal);
							nextToken = new Token(INTEGER, startLine, startCol, linenum, colnum, &filename, (void*)val);
							ungetc(in, f);
							repented = true;
							state = done;
						}
						break;
					}

					// Just encountered e or E in a float literal
					case (startingExponent) : {
						if (in == '-') {
							state = inExponent;
							ready = false;
							negative = true;
						}
						else if (in == '+') {
							state = inExponent;
							ready = false;
							negative = false;
						}
						else if (isdigit(in)) {
							state = inExponent;
							ready = true;
							negative = false;
							intVal = in - '0';
						}
						else {
							stringVal = new std::string("Lex Error: Numeric literal missing exponent in ");
							makeError(stringVal, startLine, startCol);
							state = done;
						}
						break;
					}

					// In the exponent of a float literal that uses scientific notation
					case (inExponent) : {
						if (!(ready || isdigit(in))) {
							stringVal = new std::string("Lex Error: Numeric literal missing exponent in ");
							makeError(stringVal, startLine, startCol);
							state = done;
						}
						else if (isdigit(in)) {
							ready = true;
							intVal *= 10;
							intVal += (in - '0');
						}
						else {
							if (negative) {
								intVal = -intVal;
							}
							double *val = new double(pow(floatVal, (double)intVal));
							nextToken = new Token(FLOAT, startLine, startCol, linenum, colnum, &filename, (void*)val);
							ungetc(in, f);
							repented = true;
							state = done;
						}
						break;
					}

					// Just found an asterisk in a block comment
					case (leavingComment) : {
						if (in == '/') {
							stringVal->pop_back();
							nextToken = new Token(BLOCK_COMMENT, startLine, startCol, linenum, colnum, &filename, (void*)stringVal);
							state = done;
						}
						else if (in == EOF) {
							printf("Warning: unterminated comment in %s: (%u:%u).\n", filename.c_str(), linenum, colnum);
							nextToken = new Token(OPEN_COMMENT, startLine, startCol, linenum, colnum, &filename, (void*)stringVal);
							state = done;
						}
						else {
							*stringVal += in;
						}
						break;
					}

					// In an octal code escape sequence in a character literal.
					case (escapeCharOctal) : {
						if ('0' <= in && in  < '8') {
							charVal <<= 3;
							charVal += (in - '0');
							intVal--;
						}
						else {
							printf("Warning: Non-standard escape character in %s: (%u:%u).\n", filename.c_str(), linenum, colnum);
							intVal = 0;
							ungetc(in, f);
							repented = true;
						}
						if (intVal == 0) {
							state = haveCharLit;
						}
						break;
					}

					// In a hex code escape sequence in a character literal.
					case (escapeCharHex) : {
						if (isdigit(in)) {
							charVal <<= 4;
							charVal += (in - '0');
							intVal--;
						}
						else if ('a' <= in && in < 'g') {
							charVal <<= 4;
							charVal += (in - 'a' + 0xa);
							intVal--;
						}
						else if ('A' <= in && in < 'G') {
							charVal <<= 4;
							charVal += (in - 'A' + 0xA);
							intVal--;
						}
						else {
							printf("Warning: Non-standard escape character in %s: (%u:%u).\n", filename.c_str(), linenum, colnum);
							intVal = 0;
							ungetc(in, f);
							repented = true;
						}
						if (intVal == 0) {
							state = haveCharLit;
						}
						break;
					}

					// In an octal code escape sequence in a string literal
					case (escapeStringOctal) : {
						if ('0' <= in && in  < '8') {
							charVal <<= 3;
							charVal += (in - '0');
							intVal--;
						}
						else {
							printf("Warning: Non-standard escape character in %s: (%u:%u).\n", filename.c_str(), linenum, colnum);
							intVal = 0;
							ungetc(in, f);
							repented = true;
						}
						if (intVal == 0) {
							stringVal += (char)charVal;
							state = stringLiteral;
						}
						break;
					}

					// In a hex code escape sequence in a string literal
					case (escapeStringHex) : {
						if (isdigit(in)) {
							charVal <<= 4;
							charVal += (in - '0');
							intVal--;
						}
						else if ('a' <= in && in < 'g') {
							charVal <<= 4;
							charVal += (in - 'a' + 0xa);
							intVal--;
						}
						else if ('A' <= in && in < 'G') {
							charVal <<= 4;
							charVal += (in - 'A' + 0xA);
							intVal--;
						}
						else {
							printf("Warning: Non-standard escape character in %s: (%u:%u).\n", filename.c_str(), linenum, colnum);
							intVal = 0;
							ungetc(in, f);
							repented = true;
						}
						if (intVal == 0) {
							if (charVal > 0xff) {
								stringVal += (char)((charVal & 0xff000000) >> 24);
								stringVal += (char)((charVal & 0x00ff0000) >> 16);
								stringVal += (char)((charVal & 0x0000ff00) >> 8);
							}
							stringVal += (char)charVal;
							state = haveCharLit;
						}
						break;
					}

					// After the binary point in a binary float literal
					case (inBinaryRight) : {
						if (!(ready || in == '0' || in == '1')) {
							stringVal = new std::string("Lex Error: Numeric literal missing fractional part in ");
							makeError(stringVal, startLine, startCol);
							state = done;
						}
						else if (in == '0' || in == '1') {
							ready = true;
							floatVal += ((double)(in - '0')) / intVal;
							intVal <<= 1;
						}
						else {
							double *val = new double(floatVal);
							nextToken = new Token(FLOAT, startLine, startCol, linenum, colnum, &filename, (void*)val);
							ungetc(in, f);
							repented = true;
							state = done;
						}
						break;
					}

					// After the octal point in an octal float literal
					case (inOctalRight) : {
						if (!ready && (in < '0' || in > '7')) {
							stringVal = new std::string("Lex Error: Numeric literal missing fractional part in ");
							makeError(stringVal, startLine, startCol);
							state = done;
						}
						else if ('0' <= in && in < '8') {
							ready = true;
							floatVal += ((double)(in - '0')) / intVal;
							intVal <<= 3;
						}
						else {
							double *val = new double(floatVal);
							nextToken = new Token(FLOAT, startLine, startCol, linenum, colnum, &filename, (void*)val);
							ungetc(in, f);
							repented = true;
							state = done;
						}
						break;
					}

					// After the hexadecimal point in a hexadecimal float literal
					case (inHexRight) : {
						if (!(ready || isxdigit(in))) {
							stringVal = new std::string("Lex Error: Numeric literal missing fractional part in ");
							makeError(stringVal, startLine, startCol);
							state = done;
						}
						else if (isdigit(in)) {
							ready = true;
							floatVal += ((double)(in - '0')) / intVal;
							intVal <<= 4;
						}
						else if ('a' <= in && in < 'g') {
							ready = true;
							floatVal += ((double)(in - 'a' + 0xa)) / intVal;
							intVal <<= 4;
						}
						else if ('A' <= in && in < 'G') {
							ready = true;
							floatVal += ((double)(in - 'A' + 0xA)) / intVal;
							intVal <<= 4;
						}
						else {
							double *val = new double(floatVal);
							nextToken = new Token(FLOAT, startLine, startCol, linenum, colnum, &filename, (void*)val);
							ungetc(in, f);
							repented = true;
							state = done;
						}
						break;
					}

				}

				if (!repented) {
					if (in == '\n') {
						linenum++;
						colnum = 0;
					}
					else if (in == '\t') {
						colnum += (TAB_WIDTH - (colnum & (TAB_WIDTH - 1)));
					}
					else {
						colnum++;
					}
				}
			}
			return ret;
		}

	private:

		FILE *f;

		unsigned escapeCharacter(char c) {
			switch (c) {
				case ('a') : {
					return (unsigned) '\a';
				}
				case ('b') : {
					return (unsigned) '\b';
				}
				case ('e') : {
					return (unsigned) '\e';
				}
				case ('f') : {
					return (unsigned) '\f';
				}
				case ('n') : {
					return (unsigned) '\n';
				}
				case ('r') : {
					return (unsigned) '\r';
				}
				case ('t') : {
					return (unsigned) '\t';
				}
				case ('v') : {
					return (unsigned) '\v';
				}
				case ('0') : {
					return (unsigned) '\0';
				}
				default : {
					return (unsigned) c;
				}
			}
		}

		void makeError(std::string *message, unsigned startLine, unsigned startCol) { 
			*message += (filename + " (" + std::to_string(linenum) + ":" + std::to_string(colnum) + ").\n");
			nextToken = new Token(ERROR, startLine, startCol, linenum, colnum, &filename, (void*)message);
		}

};


void printToken(Token *t) {
	switch(t->type) {
		case (LEFT_PAREN) : {
			printf("(");
			break;
		}
		case (RIGHT_PAREN) : {
			printf(")");
			break;
		}
		case (LEFT_BRACKET) : {
			printf("[");
			break;
		}
		case (RIGHT_BRACKET) : {
			printf("]");
			break;
		}
		case (LEFT_BRACE) : {
			printf("{");
			break;
		}
		case (RIGHT_BRACE) : {
			printf("}");
			break;
		}
		case (DOT) : {
			printf(".");
			break;
		}
		case (COLON) : {
			printf(":");
			break;
		}
		case (SEMI_COLON) : {
			printf(";");
			break;
		}
		case (COMMA) : {
			printf(",");
			break;
		}
		case (ASSIGNMENT) : {
			printf("=");
			break;
		}
		case (LESS_THAN) : {
			printf("<");
			break;
		}
		case (GREATER_THAN) : {
			printf(">");
			break;
		}
		case (LOGICAL_NOT) : {
			printf("!");
			break;
		}
		case (ADDITION) : {
			printf("+");
			break;
		}
		case (SUBTRACT) : {
			printf("-");
			break;
		}
		case (ASTERISK) : {
			printf("*");
			break;
		}
		case (SLASH) : {
			printf("/");
			break;
		}
		case (MODULO) : {
			printf("%%");
			break;
		}
		case (BITWISE_NOT) : {
			printf("~");
			break;
		}
		case (BITWISE_OR) : {
			printf("|");
			break;
		}
		case (BITWISE_XOR) : {
			printf("^");
			break;
		}
		case (AMPERSAND) : {
			printf("&");
			break;
		}
		case (ARROW) : {
			printf("->");
			break;
		}
		case (ADD_ASSIGN) : {
			printf("+=");
			break;
		}
		case (SUBTRACT_ASSIGN) : {
			printf("-=");
			break;
		}
		case (MULTIPLY_ASSIGN) : {
			printf("*=");
			break;
		}
		case (DIV_ASSIGN) : {
			printf("/=");
			break;
		}
		case (MOD_ASSIGN) : {
			printf("%%=");
			break;
		}
		case (OR_ASSIGN) : {
			printf("|=");
			break;
		}
		case (XOR_ASSIGN) : {
			printf("^=");
			break;
		}
		case (AND_ASSIGN) : {
			printf("&=");
			break;
		}
		case (INCREMENT) : {
			printf("++");
			break;
		}
		case (DECREMENT) : {
			printf("--");
			break;
		}
		case (LOGICAL_OR) : {
			printf("||");
			break;
		}
		case (LOGICAL_XOR) : {
			printf("^^");
			break;
		}
		case (LOGICAL_AND) : {
			printf("&&");
			break;
		}
		case (LEFT_SHIFT) : {
			printf("<<");
			break;
		}
		case (RIGHT_SHIFT) : {
			printf(">>");
			break;
		}
		case (COMPARE) : {
			printf("==");
			break;
		}
		case (NOT_EQUAL) : {
			printf("!=");
			break;
		}
		case (LESS_EQUAL) : {
			printf("<=");
			break;
		}
		case (GREATER_EQUAL) : {
			printf(">=");
			break;
		}
		case (LEFT_SHIFT_ASSIGN) : {
			printf("<<=");
			break;
		}
		case (RIGHT_SHIFT_ASSIGN) : {
			printf(">>=");
			break;
		}
		case (CAST) : {
			printf("CAST");
			break;
		}
		case (RETURN) : {
			printf("return");
			break;
		}
		case (IF) : {
			printf("if");
			break;
		}
		case (ELSE) : {
			printf("else");
			break;
		}
		case (WHILE) : {
			printf("while");
			break;
		}
		case (FOR) : {
			printf("for");
			break;
		}
		case (FOREACH) : {
			printf("foreach");
			break;
		}
		case (IN) : {
			printf("in");
			break;
		}
		case (INDEX) : {
			printf("index");
			break;
		}
		case (SWITCH) : {
			printf("switch");
			break;
		}
		case (CASE) : {
			printf("case");
			break;
		}
		case (CONT) : {
			printf("continue");
			break;
		}
		case (BREAK) : {
			printf("break");
			break;
		}
		case (STRUCT) : {
			printf("struct");
			break;
		}
		case (CLASS) : {
			printf("class");
			break;
		}
		case (UNION) : {
			printf("union");
			break;
		}
		case (INTERFACE) : {
			printf("interface");
			break;
		}
		case (IMPL) : {
			printf("impl");
			break;
		}
		case (PROC) : {
			printf("proc");
			break;
		}
		case (FUNC) : {
			printf("func");
			break;
		}
		case (VAR) : {
			printf("var");
			break;
		}
		case (EXPORT) : {
			printf("export");
			break;
		}
		case (MODULE) : {
			printf("module");
			break;
		}
		case (IMPORT) : {
			printf("import");
			break;
		}
		case (FROM) : {
			printf("from");
			break;
		}
		case (GLOBALLY) : {
			printf("globally");
			break;
		}
		case (AS) : {
			printf("as");
			break;
		}
		case (ALIAS) : {
			printf("alias");
			break;
		}
		case (ENUM) : {
			printf("enum");
			break;
		}
		case (CONST) : {
			printf("const");
			break;
		}
		case (STATIC) : {
			printf("static");
			break;
		}
		case (END_OF_FILE) : {
			printf("EOF");
			break;
		}
		case (NAMESPACE) : {
			printf("namespace");
			break;
		}
		case (INSTANCE) : {
			printf("instance");
			break;
		}
		case (STRING_LITERAL) : {
			std::cout << '\"' << *(std::string*)(t->data) << '\"';
			break;
		}
		case (BLOCK_COMMENT) : {
			std::cout << "/*" << *(std::string*)(t->data) << "*/";
			break;
		}
		case (OPEN_COMMENT) : {
			std::cout << "/*" << *(std::string*)(t->data);
			break;
		}
		case (LINE_COMMENT) : {
			printf("//");
		}
		case (IDENTIFIER) :
		case (ERROR) :
			std::cout << *(std::string*)(t->data);
			break;
		case (CHARACTER_LITERAL) :
			printf("\'%c\'", *(char*)(t->data));
			break;
		case (INTEGER) :
			printf("%llu", *(unsigned long long*)(t->data));
			break;
		case (FLOAT) :
			printf("%lf", *(double*)(t->data));
			break;
		default : {}
	}
}

#endif
