#define _CRT_SECURE_NO_DEPRECATE

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>

#include "util.h"
#include "parser.h"

int main(int argc, char **argv)
{
	if (argc < 3) {
		std::cout << "Usage: " << argv[0] << " [in.txt] [out.gdi]" << std::endl;
		return EXIT_FAILURE;
	}

	FILE *fp = fopen(argv[1], "r"); FileGuard fg(fp);
	if (!fp) {
		std::cout << "Cannot open file" << std::endl;
		return EXIT_FAILURE;
	}

	Ast ast = parse(fp);

	uint32_t dim = 0;
	bool sem = ast.check(dim);
	std::cout << "Semantic check OK: " << (sem ? "Yes" : "No") << std::endl;
	if (!sem)
		return EXIT_FAILURE;

	std::cout << "Dimensions: " << dim << std::endl;

	std::cout << "Printing the AST: " << ast << std::endl;

	std::ofstream os(argv[2], std::ofstream::binary);
	Writer writer(os);
	ast.code(dim, writer);

	return EXIT_SUCCESS;
}