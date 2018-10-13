#include "includes.h"

int main(int argc,  char **argv) {
	if (argc > 1) {
		std::string filename(argv[1]);
		Program *ast = parse(&filename);
	}
}
