#pragma once

#include <stdint.h>
#include <iostream>
#include <vector>
#include <fstream>

#include "util.h"

inline uint32_t coord2index(uint32_t x, uint32_t y, uint32_t z)
{
	return (z << 2) | (y << 1) | x;
}
inline uint32_t coord2index2(uint32_t nx, uint32_t ny, uint32_t nz, uint32_t x, uint32_t y, uint32_t z)
{
	return (coord2index(nx, ny, nz) << 3) | coord2index(x, y, z);
}
inline void setindex(uint32_t &idx, uint32_t i, uint32_t v)
{
	idx &= ~(1 << i);
	idx |= v << i;
}
inline uint32_t index2x(uint32_t index)
{
	return index & 1;
}
inline uint32_t index2y(uint32_t index)
{
	return (index >> 1) & 1;
}
inline uint32_t index2z(uint32_t index)
{
	return (index >> 2) & 1;
}


template <typename T>
struct Node {
	T data;
	Node<T> *children[8];
	Node<T> *parent;
	uint32_t size;

	Node(T _data) : data(_data), parent(NULL), size(0)
	{
		children[0] = NULL;
	}

	Node(T _data, Node<T> *_parent) : data(_data), parent(_parent), size(0)
	{
		children[0] = NULL;
	}

	~Node()
	{
		if (!leaf()) {
			for (int i = 0; i < 8; ++i) {
				delete children[i];
			}
		}
	}

	Node(const Node<T> &other)
	{
		data = other.data;
		if (!leaf()) {
			for (int i = 0; i < 8; ++i) {
				children[i] = new Node<T>(*other.children[i]);
				children[i]->parent = this;
			}
		} else {
			children[0] = NULL;
		}
// 		parent = other.parent;
		size = other.size;
	}

	Node<T> &operator=(Node<T> other)
	{
		std::swap(data, other.data);
		for (int i = 0; i < 8; ++i) {
			std::swap(children[i], other.children[i]);
		}
		std::swap(parent, other.parent);
		std::swap(size, other.size);
		return *this;
	}

	const Node<T> *descent(uint32_t x, uint32_t y, uint32_t z, uint32_t size) const
	{
		if (leaf())
			return this;

		size /= 2;
		uint32_t index = coord2index(x & size ? 1 : 0, y & size ? 1 : 0, z & size ? 1 : 0);

		return children[index]->descent(x, y, z, size);
	}
	Node *descent_create(uint32_t x, uint32_t y, uint32_t z, uint32_t size, T d)
	{
		if (size == 1)
			return this;
		if (leaf()) {
			if (data == d)
				return this;
			else
				fill();
		}

		size /= 2;
		uint32_t index = coord2index(x & size ? 1 : 0, y & size ? 1 : 0, z & size ? 1 : 0);

		return children[index]->descent_create(x, y, z, size, d);
	}
	const Node<T> *descentn(uint32_t x, uint32_t y, uint32_t z, uint32_t &size) const
	{
		if (leaf())
			return this;

		size /= 2;
		uint32_t index = coord2index(x & size ? 1 : 0, y & size ? 1 : 0, z & size ? 1 : 0);

		return children[index]->descentn(x, y, z, size);
	}
	const Node<T> *_neigh(Node<T> *root, uint32_t os, int32_t x, int32_t y, int32_t z, int32_t dx, int32_t dy, int32_t dz, int32_t size) const
	{
		int32_t xx = x + dx, yy = y + dy, zz = z + dz;

		if (xx < 0 || yy < 0 || zz < 0 || xx >= (int32_t)os || yy >= (int32_t)os || zz >= (int32_t)os)
			return NULL;

		const Node<T> *v = root->descentn(xx, yy, zz, os);
		return v;
	}
	bool neigh(Node<T> *root, uint32_t os, int32_t x, int32_t y, int32_t z, int32_t dx, int32_t dy, int32_t dz, int32_t size, uint32_t vis) const
	{
#ifndef NONEIGHCHECKS
		bool s = true;
		for (int zz = 0; zz < size; ++zz) {
			for (int yy = 0; yy < size; ++yy) {
				for (int xx = 0; xx < size; ++xx) {
					const Node<T> *n = _neigh(root, os, x + xx, y + yy, z + zz, dx, dy, dz, size);
					s &= n && n->data != -1;
				}
			}
		}
		return s;
#else
		return false;
#endif
	}
	T tesselate(Node<T> *root, uint32_t os, uint32_t x, uint32_t y, uint32_t z, uint32_t size, std::vector<uint32_t> &vertices, std::vector<uint32_t> &faces, std::vector<int> &colors) const
	{
		if (leaf()) {
			if (data != -1) {
				// render full vertex without neighbor checks
				uint32_t idx = static_cast<uint32_t>(vertices.size()) / 3;
				bool i = false;

				uint32_t p0 = idx + 0, p1 = idx + 1, p2 = idx + 2, p3 = idx + 3,
				         p4 = idx + 4, p5 = idx + 5, p6 = idx + 6, p7 = idx + 7;

				// -z
				if (!neigh(root, os, x, y, z, 0, 0, -1, size, data != -1 ? 1 : 0)) {
					i = true;
					faces.push_back(p6); faces.push_back(p4); faces.push_back(p0);
					faces.push_back(p2); faces.push_back(p6); faces.push_back(p0);
				}
				// z
				if (!neigh(root, os, x, y, z, 0, 0, 1, size, data != -1 ? 1 : 0)) {
					i = true;
					faces.push_back(p1); faces.push_back(p5); faces.push_back(p7);
					faces.push_back(p1); faces.push_back(p7); faces.push_back(p3);
				}

				// -y
				if (!neigh(root, os, x, y, z, 0, -1, 0, size, data != -1 ? 1 : 0)) {
					i = true;
					faces.push_back(p4); faces.push_back(p1); faces.push_back(p0);
					faces.push_back(p4); faces.push_back(p5); faces.push_back(p1);
				}
				// y
				if (!neigh(root, os, x, y, z, 0, 1, 0, size, data != -1 ? 1 : 0)) {
					i = true;
					faces.push_back(p2); faces.push_back(p3); faces.push_back(p6);
					faces.push_back(p3); faces.push_back(p7); faces.push_back(p6);
				}

				// -x
				if (!neigh(root, os, x, y, z, -1, 0, 0, size, data != -1 ? 1 : 0)) {
					i = true;
					faces.push_back(p0); faces.push_back(p1); faces.push_back(p3);
					faces.push_back(p0); faces.push_back(p3); faces.push_back(p2);
				}
				// x
				if (!neigh(root, os, x, y, z, 1, 0, 0, size, data != -1 ? 1 : 0)) {
					i = true;
					faces.push_back(p7); faces.push_back(p5); faces.push_back(p4);
					faces.push_back(p6); faces.push_back(p7); faces.push_back(p4);
				}
				if (i) {
					colors.push_back(data); colors.push_back(data); colors.push_back(data); colors.push_back(data);
					colors.push_back(data); colors.push_back(data); colors.push_back(data); colors.push_back(data);

					vertices.push_back(x);        vertices.push_back(y);        vertices.push_back(z);        // p0
					vertices.push_back(x);        vertices.push_back(y);        vertices.push_back(z + size); // p1
					vertices.push_back(x);        vertices.push_back(y + size); vertices.push_back(z);        // p2
					vertices.push_back(x);        vertices.push_back(y + size); vertices.push_back(z + size); // p3
					vertices.push_back(x + size); vertices.push_back(y);        vertices.push_back(z);        // p4
					vertices.push_back(x + size); vertices.push_back(y);        vertices.push_back(z + size); // p5
					vertices.push_back(x + size); vertices.push_back(y + size); vertices.push_back(z);        // p6
					vertices.push_back(x + size); vertices.push_back(y + size); vertices.push_back(z + size); // p7
				}
			}
			return data;
		} else {
			size /= 2;
			T r = 0;
			for (int i = 0; i < 8; ++i) {
				T v = children[i]->tesselate(root, os, x + size * index2x(i), y + size * index2y(i), z + size * index2z(i), size, vertices, faces, colors);
				r |= v;
			}
			return r;
		}
	}

	void inctoroot()
	{
		size += 8;
		if (parent)
			parent->inctoroot();
	}
	void dectoroot()
	{
		size -= 8;
		if (parent)
			parent->inctoroot();
	}

	void fill()
	{
		for (int i = 0; i < 8; ++i) {
			Node<T> *child = new Node<T>(data, this);
			children[i] = child;
		}
		inctoroot();
	}

	Node<T> *clean_parent()
	{
		if (!parent)
			return this;

		for (int i = 0; i < 8; ++i) {
			if (!parent->children[i]->leaf() || parent->children[i]->data != data)
				return this;
		}

		// parent is uniform
		Node<T> * _parent = parent;
		_parent->data = data;
		for (int i = 0; i < 8; ++i) {
			delete _parent->children[i];
		}
		_parent->children[0] = NULL;
		_parent->dectoroot();

		return _parent->clean_parent();;
	}

	bool leaf() const
	{
		return children[0] == NULL;
	}
};

template <typename T>
struct Octree {
	Node<T> *root;
	uint32_t size;

	Octree(uint32_t _size, T _data) : size(_size), root(new Node<T>(_data))
	{
	}

	~Octree()
	{
		delete root;
	}

	Octree(const Octree &other)
	{
		root = new Node<T>(*other.root);
		root->parent = NULL;
		size = other.size;
	}

	Octree &operator=(Octree other)
	{
		std::swap(root, other.root);
		std::swap(size, other.size);
		return *this;
	}

	T get(uint32_t x, uint32_t y, uint32_t z) const
	{
		const Node<T> *leaf = root->descent(x, y, z, size);
		return leaf->data;
	}

	void set(uint32_t x, uint32_t y, uint32_t z, T data)
	{
		Node<T> *leaf = root->descent_create(x, y, z, size, data);
		leaf->data = data;
		leaf = leaf->clean_parent();
	}

	void tesselate(std::vector<uint32_t> &vertices, std::vector<uint32_t> &faces, std::vector<int> &colors) const
	{
		root->tesselate(root, size, 0, 0, 0, size, vertices, faces, colors);
	}
};