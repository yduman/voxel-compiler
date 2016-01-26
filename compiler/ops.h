#pragma once

#include <string>

enum Op { OP_DRAW = 0x00, OP_LOOP = 0x01, OP_JUMP = 0x02, OP_CONJ = 0x10, OP_DISJ = 0x11, OP_NEG = 0x12, OP_SET = 0x20, OP_SPHERE = 0xa0, OP_BOX = 0xa1, OP_HEART = 0xa2, OP_FN = 0xff };

inline std::string op2str(int op)
{
	switch (op) {
	case OP_DRAW:
		return "DRAW";
	case OP_LOOP:
		return "LOOP";
	case OP_JUMP:
		return "JUMP";
	case OP_DISJ:
		return "DISJ";
	case OP_CONJ:
		return "CONJ";
	case OP_SET:
		return "SET";
	case OP_NEG:
		return "NEG";
	case OP_SPHERE:
		return "SPHERE";
	case OP_BOX:
		return "BOX";
	case OP_HEART:
		return "HEART";
	}
	return "UNKNOWN";
}