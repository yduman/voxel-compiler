#define _CRT_SECURE_NO_DEPRECATE
#pragma once

#include <vector>
#include <iostream>
#include <stdint.h>
#include <unordered_map>
#include <cstring>

#include "util.h"
#include "ops.h"


#ifdef _WIN32
#define strdup _strdup
#endif

// mapping between function names and opcodes
static std::unordered_map<std::string, Op> opcodes = {
	{ "sphere", OP_SPHERE },
	{ "box", OP_BOX },
	{ "heart", OP_HEART }
};

struct Arg {
	uint32_t val;

	inline Arg(uint32_t x) : val(x)
	{
	}
};

struct Args {
	std::vector<Arg*> args;

	inline Args()
	{
	}

	inline ~Args()
	{
		for (int i = 0; i < size(); ++i) {
			delete args[i];
		}
	}

	inline Args(const Args &other)
	{
		for (int i = 0; i < other.size(); ++i) {
			add(new Arg(other[i]));
		}
	}

	inline Args &operator=(Args other)
	{
		args.swap(other.args);
		return *this;
	}

	template <typename S>
	int operator[](S i) const
	{
		return args[i]->val;
	}

	inline void add(Arg *arg)
	{
		args.push_back(arg);
	}

	inline size_t size() const
	{
		return args.size();
	}
};

/**
* This class represents a leaf in the syntax tree
*/
struct Fn {
	char* name; // Name of the function
	Args* args; // Arguments given to this function

	inline Fn(char *_name, Args *_args) : name(_name), args(_args)
	{
	}

	inline ~Fn()
	{
		free(name);
		delete args;
	}

	inline Fn(const Fn &other)
	{
		name = strdup(other.name);
		args = new Args(*other.args);
	}

	inline Fn &operator=(Fn other)
	{
		std::swap(name, other.name);
		std::swap(args, other.args);
		return *this;
	}

	inline friend std::ostream &operator<<(std::ostream &os, const Fn &expr);

	inline bool check(uint32_t &dim)
	{
		// Task 3 - add your code here

		if (strcmp(name, "sphere") != 0 && strcmp(name, "heart") != 0 && strcmp(name, "box") != 0)
		{
			printf("your function name was: %s\n", name);
			std::cout << "invalid function name!" << std::endl;
			return false;
		}

		if (name == "sphere" || name == "heart")
		{
			if (args->size() != 5)
			{
				std::cout << "invalid number of arguments!" << std::endl;
				return false;
			}
		}

		if (name == "box")
		{
			if (args->size() != 7)
			{
				std::cout << "invalid number of arguments!" << std::endl;
				return false;
			}
		}

		return true;
	}

	inline void code(uint32_t reg, Writer &writer)
	{

	}
};


inline std::ostream &operator<<(std::ostream &os, const Fn &fn)
{
	os << fn.name << "(";
	for (int i = 0; i < fn.args->size(); ++i) {
		if (i) os << ", ";
		os << (*fn.args)[i];
	}
	return os << ")";
}

/**
 * This class represents a node in the syntax tree.
 */
struct Expr {
	Op op; 		// operation code
	Expr *lhs; 	// left hand argument (optional)
	Expr *rhs; 	// right hand argument (optional)
	Fn *fn; 	   // function (optional)

	// unary
	inline Expr(Op _op, Expr *_lhs) : op(_op), lhs(_lhs), rhs(NULL), fn(NULL)
	{
	}

	// binary
	inline Expr(Op _op, Expr *_lhs, Expr *_rhs) : op(_op), lhs(_lhs), rhs(_rhs), fn(NULL)
	{
	}

	// fn
	inline Expr(Op _op, Fn *_lhs) : op(_op), lhs(NULL), rhs(NULL), fn(_lhs)
	{
	}

	inline ~Expr()
	{
		delete lhs;
		delete rhs;
		delete fn;
	}

	inline Expr(const Expr &other)
	{
		op = other.op;
		if (other.lhs)
			lhs = new Expr(*other.lhs);
		if (other.rhs)
			rhs = new Expr(*other.rhs);
		if (other.fn)
			fn = new Fn(*other.fn);
	}

	inline Expr &operator=(Expr other)
	{
		std::swap(op, other.op);
		std::swap(lhs, other.lhs);
		std::swap(rhs, other.rhs);
		std::swap(fn, other.fn);
		return *this;
	}

	inline friend std::ostream &operator<<(std::ostream &os, const Expr &expr);

	inline bool check(uint32_t &dim)
	{
		// Task 3 - add your code here
		if (op == 0x10 || op == 0x11) {
			return lhs->check(dim) && rhs->check(dim);
		} else if (op == 0x12) {
			return lhs->check(dim);
		} else {
			return fn->check(dim);
		}
	}

	inline uint32_t code(uint32_t reg, Writer &writer)
	{
		// Task 5 - add your code here
		if (op == OP_CONJ || op == OP_DISJ) {
			uint32_t left_code = lhs->code(reg, writer);
			uint32_t right_code = rhs->code(reg, writer);
			reg = left_code + right_code;
		} else if (op == OP_NEG) {
			return lhs->code(0, writer); // Negation set Color to 0x0
		} else {
			fn->code(0, writer);
		}

		return reg;
	}
};


inline std::ostream &operator<<(std::ostream &os, const Expr &expr)
{
	switch (expr.op) {
	case OP_FN:
		return os << *expr.fn;
	case OP_NEG:
		return os << "(!" << *expr.lhs << ")";
	case OP_DISJ:
		return os << "(" << *expr.lhs << " | " << *expr.rhs << ")";
	case OP_CONJ:
		return os << "(" << *expr.lhs << " & " << *expr.rhs << ")";
	default:
		return os;
	}
}

// Abstract Syntax Tree
struct Ast {
	Expr *root;

	Ast(Expr *_root) : root(_root)
	{
	}

	inline ~Ast()
	{
		delete root;
	}

	inline Ast(const Ast &other)
	{
		root = new Expr(*root);
	}

	inline Ast &operator=(Ast other)
	{
		std::swap(root, other.root);
		return *this;
	}

	inline friend std::ostream &operator<<(std::ostream &os, const Ast &ast);

	inline bool check(uint32_t &dim)
	{
		// Task 3 - modify if you can't solve the optional task, see note below.
		// NOTE: dim is used to determine the dimensions of the loop.
		// We can use a constant here (e.g. 128)
		// or the maximum of the x, y and z values of each function.
		return root->check(dim);
	}

	inline void code(uint32_t dim, Writer &writer)
	{
		// Task 5
		writer << OP_LOOP << static_cast<uint32_t>(-1) << 0 << 0 << 0 << dim << dim << dim;
		uint32_t reg = root->code(0, writer);
		writer << OP_DRAW << reg;
		writer << OP_JUMP << 0;
	}
};

inline std::ostream &operator<<(std::ostream &os, const Ast &ast)
{
	return os << *ast.root;
}

Ast parse(FILE *fp);
