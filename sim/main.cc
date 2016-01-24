#define _CRT_SECURE_NO_DEPRECATE
//#define USE_CHRONO

#include <cstdio>
#include <iostream>
#include <ostream>
#include <cstdlib>
#include <iomanip>
#ifdef USE_CHRONO
#include <chrono>
#endif
#include <cstring>
#include <vector>

#include "util.h"
#include "ops.h"
#include "fns.h"
#include "octree.h"

#define BLACK 0x0

typedef Octree<uint32_t> UIntOt;

void createply(const UIntOt &ot, std::ostream &out)
{
	std::cout << "Sparse size: " << ot.size * ot.size * ot.size * sizeof(int) << " bytes" << std::endl;
	std::cout << "Dense size:  " << ot.root->size * sizeof(Node<int>) << " bytes" << std::endl;
	std::vector<uint32_t> vertices;
	std::vector<int> colors;
	std::vector<uint32_t> faces;
	ot.tesselate(vertices, faces, colors);
	out << "ply\n"
		<< "format ascii 1.0\n"
		<< "element vertex " << vertices.size() / 3 << "\n"
		<< "property float x\n"
		<< "property float y\n"
		<< "property float z\n"
		<< "property uchar red\n"
		<< "property uchar green\n"
		<< "property uchar blue\n"
		<< "element face " << faces.size() / 3 << "\n"
		<< "property list uint8 int32 vertex_index\n"
		<< "end_header\n";
	for (int i = 0; i < vertices.size() / 3; ++i) {
		uint32_t color = colors[i];
		uint32_t r = (color >> 12) & 0xff, g = (color >> 8) & 0xff, b = color & 0xff;
		out << std::fixed << std::setprecision(1) << (double)vertices[i * 3] << " " << (double)vertices[i * 3 + 1] << " " << (double)vertices[i * 3 + 2] << " " << r << " " << g << " " << b << "\n";
	}
	for (int i = 0; i < faces.size() / 3; ++i) {
		out << "3 " << faces[i * 3] << " " << faces[i * 3 + 1] << " " << faces[i * 3 + 2] << "\n";
	}
}

void out3d(const UIntOt &ot)
{
	for (uint32_t z = 0; z < ot.size; ++z) {
		std::cout << "Layer " << z << ':' << std::endl;
		for (uint32_t y = 0; y < ot.size; ++y) {
			for (uint32_t x = 0; x < ot.size; ++x) {
				std::cout << (ot.get(x, y, z) != INVALID ? '#' : ' ');
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

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
	if (argc < 3) {
		std::cout << "Usage: " << argv[0] << " [in.gdi] [out.ply]" << std::endl;
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

	int pc = 0;
	uint32_t dim = 0;
	uint32_t maxreg = 0;
	// collect meta data
	while (pc < ops.size()) {
		uint32_t op = ops[pc++];
		switch (op) {
		case OP_DRAW:
			maxreg = maxreg > ops[pc + 0] ? maxreg : ops[pc + 0];
			pc += 1;
			break;
		case OP_LOOP:
			dim = dim > ops[pc + 4] ? dim : ops[pc + 4];
			dim = dim > ops[pc + 5] ? dim : ops[pc + 5];
			dim = dim > ops[pc + 6] ? dim : ops[pc + 6];
			pc += 7;
			break;
		case OP_JUMP:
			pc += 1;
			break;
		case OP_CONJ:
			maxreg = maxreg > ops[pc + 0] ? maxreg : ops[pc + 0];
			maxreg = maxreg > ops[pc + 1] ? maxreg : ops[pc + 1];
			maxreg = maxreg > ops[pc + 2] ? maxreg : ops[pc + 2];
			pc += 3;
			break;
		case OP_DISJ:
			maxreg = maxreg > ops[pc + 0] ? maxreg : ops[pc + 0];
			maxreg = maxreg > ops[pc + 1] ? maxreg : ops[pc + 1];
			maxreg = maxreg > ops[pc + 2] ? maxreg : ops[pc + 2];
			pc += 3;
			break;
		case OP_NEG:
			maxreg = maxreg > ops[pc + 0] ? maxreg : ops[pc + 0];
			maxreg = maxreg > ops[pc + 1] ? maxreg : ops[pc + 1];
			pc += 2;
			break;
		case OP_SET:
			maxreg = maxreg > ops[pc + 0] ? maxreg : ops[pc + 0];
			pc += 2;
			break;
		case OP_SPHERE:
			maxreg = maxreg > ops[pc + 0] ? maxreg : ops[pc + 0];
			pc += 6;
			break;
		case OP_BOX:
			maxreg = maxreg > ops[pc + 0] ? maxreg : ops[pc + 0];
			pc += 8;
			break;
		case OP_HEART:
			maxreg = maxreg > ops[pc + 0] ? maxreg : ops[pc + 0];
			pc += 6;
			break;
		default:
			std::cout << "Invalid opcode 0x" << std::hex << op << " at offset " << std::dec << pc << std::endl;
			return EXIT_FAILURE;
		}
	}

	uint32_t full = 1;
	while (dim > full) {
		full <<= 1;
	}
	dim = full;

	std::cout << "Dimensions: " << dim << std::endl << "Register count: " << maxreg << std::endl;

	std::vector<uint32_t> regs(maxreg + 1);

	UIntOt ot(dim, INVALID);

	bool regloopinited = false;
	uint32_t regx, regy, regz;
	// simulate
	std::cout << "Running simulation..." << std::endl;
#ifdef USE_CHRONO
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
#endif
	pc = 0;
	while (pc < ops.size()) {
		uint32_t op = ops[pc++];
		switch (op) {
		case OP_DRAW:
		{
			uint32_t reg = ops[pc++];
			ot.set(regx, regy, regz, regs[reg]);
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

			if (!regloopinited) {
				regloopinited = true;
				regx = startx;
				regy = starty;
				regz = startz;
			} else {
				++regx;
			}
			if (regx == endx) {
				regx = 0;
				++regy;
			}
			if (regy == endy) {
				regy = 0;
				++regz;
			}
			if (regz == endz) {
				regloopinited = false;
				pc = end;
			}
			break;
		}
		case OP_JUMP:
		{
			uint32_t offset = ops[pc++];

			pc = offset;
			break;
		}
		case OP_CONJ:
		{
			uint32_t dest = ops[pc++];
			uint32_t lhs = ops[pc++];
			uint32_t rhs = ops[pc++];

			uint32_t lhsv = regs[lhs];
			uint32_t rhsv = regs[rhs];
			regs[dest] = lhsv != INVALID ? lhsv : rhsv;
			break;
		}
		case OP_DISJ:
		{
			uint32_t dest = ops[pc++];
			uint32_t lhs = ops[pc++];
			uint32_t rhs = ops[pc++];

			uint32_t lhsv = regs[lhs];
			uint32_t rhsv = regs[rhs];
			regs[dest] = rhsv == INVALID ? INVALID : lhsv;
			break;
		}
		case OP_NEG: {
			uint32_t dest = ops[pc++];
			uint32_t lhs = ops[pc++];

			uint32_t lhsv = regs[lhs];
			regs[dest] = lhsv == INVALID ? BLACK : INVALID;
			break;
		}
		case OP_SET: {
			uint32_t dest = ops[pc++];
			uint32_t val = ops[pc++];

			regs[dest] = val;
			break;
		}
		case OP_SPHERE: {
			uint32_t dest = ops[pc++];
			uint32_t x = ops[pc++];
			uint32_t y = ops[pc++];
			uint32_t z = ops[pc++];
			uint32_t radius = ops[pc++];
			uint32_t color = ops[pc++];

			regs[dest] = sphere(regx, regy, regz, x, y, z, radius, color);
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

			regs[dest] = box(regx, regy, regz, x, y, z, w, h, d, color);
			break;
		}
		case OP_HEART: {
			uint32_t dest = ops[pc++];
			uint32_t x = ops[pc++];
			uint32_t y = ops[pc++];
			uint32_t z = ops[pc++];
			uint32_t radius = ops[pc++];
			uint32_t color = ops[pc++];

			regs[dest] = heart(regx, regy, regz, x, y, z, radius, color);
			break;
		}
		}
	}
#ifdef USE_CHRONO
	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "Finished simulation. Took " << elapsed_seconds.count() << "s" << std::endl;
#endif

	if (strcmp(argv[2], "-") == 0) {
		out3d(ot);
	} else {
		std::ofstream out(argv[2], std::ofstream::binary);
		createply(ot, out);
	}

	return EXIT_SUCCESS;
}