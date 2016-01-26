#define _CRT_SECURE_NO_DEPRECATE

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <vector>

#include "ops.h"
#include "util.h"

struct Program : std::vector<uint32_t> {
	using std::vector<uint32_t>::vector;

	template <typename T>
	inline uint32_t operator[](T i)
	{
#ifndef NODEBUG
		if (i >= size() || i < 0) {
			std::cout << "Invalid offset " << i << std::endl;
			std::exit(EXIT_FAILURE);
		}
#endif
		return std::vector<uint32_t>::operator[](i);
	}
};

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " [in.txt] [out.gdi]" << std::endl;
		return EXIT_FAILURE;
	}

	FILE *fp = fopen(argv[1], "rb"); FileGuard fg(fp);
	if (!fp) {
		std::cout << "Cannot open file" << std::endl;
		return EXIT_FAILURE;
	}
	fseek(fp, 0, SEEK_END);
	int filesize = ftell(fp);
	if (filesize % sizeof(uint32_t) != 0) {
		std::cout << "Invalid file size" << std::endl;
		return EXIT_FAILURE;
	}
	int size = filesize / sizeof(uint32_t);
	fseek(fp, 0, SEEK_SET);
	Program ops(size);
	fread(ops.data(), sizeof(uint32_t), ops.size(), fp);

	uint32_t pc = 0;
	while (pc < ops.size()) {
		uint32_t i = pc;
		uint32_t op = ops[pc++];
		std::cout << "#" << i << ": " << op2str(op) << ' ';
		switch (op) {
		case OP_DRAW:
		{
			uint32_t reg = ops[pc++];

			std::cout << "r" << reg << ' ';
			break;
		}
		case OP_LOOP:
		{
			uint32_t end = ops[pc++];
			uint32_t startx = ops[pc++];
			uint32_t starty = ops[pc++];
			uint32_t startz = ops[pc++];
			uint32_t endx = ops[pc++];
			uint32_t endy = ops[pc++];
			uint32_t endz = ops[pc++];

			std::cout << "#" << end << ' ' << startx << ' ' << starty << ' ' << startz << ' ' << endx << ' ' << endy << ' ' << endz;
			break;
		}
		case OP_JUMP:
		{
			uint32_t offset = ops[pc++];

			std::cout << "#" << offset;
			break;
		}
		case OP_CONJ:
		{
			uint32_t dest = ops[pc++];
			uint32_t lhs = ops[pc++];
			uint32_t rhs = ops[pc++];

			std::cout << "r" << dest << " r" << lhs << " r" << rhs;
			break;
		}
		case OP_DISJ:
		{
			uint32_t dest = ops[pc++];
			uint32_t lhs = ops[pc++];
			uint32_t rhs = ops[pc++];

			std::cout << "r" << dest << " r" << lhs << " r" << rhs;
			break;
		}
		case OP_NEG: {
			uint32_t dest = ops[pc++];
			uint32_t lhs = ops[pc++];

			std::cout << "r" << dest << " r" << lhs;
			break;
		}
		case OP_SET: {
			uint32_t dest = ops[pc++];
			uint32_t val = ops[pc++];

			std::cout << val;
			break;
		}
		case OP_SPHERE: {
			uint32_t dest = ops[pc++];
			uint32_t x = ops[pc++];
			uint32_t y = ops[pc++];
			uint32_t z = ops[pc++];
			uint32_t radius = ops[pc++];
			uint32_t color = ops[pc++];

			std::cout << "r" << dest << ' ' << x << ' ' << y << ' ' << z << ' ' << radius << " 0x" << std::hex << std::setfill('0') << std::setw(6) << color << std::dec;
			break;
		}
		case OP_BOX: {
			uint32_t dest = ops[pc++];
			uint32_t x = ops[pc++];
			uint32_t y = ops[pc++];
			uint32_t z = ops[pc++];
			uint32_t w = ops[pc++];
			uint32_t h = ops[pc++];
			uint32_t d = ops[pc++];
			uint32_t color = ops[pc++];

			std::cout << "r" << dest << ' ' << x << ' ' << y << ' ' << z << ' ' << w << ' ' << h << ' ' << d << " 0x" << std::hex << std::setfill('0') << std::setw(6) << color << std::dec;
			break;
		}
		case OP_HEART: {
			uint32_t dest = ops[pc++];
			uint32_t x = ops[pc++];
			uint32_t y = ops[pc++];
			uint32_t z = ops[pc++];
			uint32_t radius = ops[pc++];
			uint32_t color = ops[pc++];

			std::cout << "r" << dest << ' ' << x << ' ' << y << ' ' << z << ' ' << radius << " 0x" << std::hex << std::setfill('0') << std::setw(6) << color << std::dec;
			break;
		}
		}
		std::cout << std::endl;
	}

	return EXIT_SUCCESS;
}