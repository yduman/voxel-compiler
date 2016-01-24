#pragma once

#define INVALID static_cast<unsigned int>(-1)

inline uint32_t sphere(uint32_t x, uint32_t y, uint32_t z, uint32_t mx, uint32_t my, uint32_t mz, uint32_t r, uint32_t c)
{
	uint32_t dx = x - mx, dy = y - my, dz = z - mz;
	uint32_t dd = dx * dx + dy * dy + dz * dz;
	uint32_t rr = r * r;

	return dd < rr ? c : INVALID;
}

inline uint32_t box(uint32_t x, uint32_t y, uint32_t z, uint32_t bx, uint32_t by, uint32_t bz, uint32_t w, uint32_t h, uint32_t d, uint32_t c)
{
	return x >= bx && x < bx + w &&
	       y >= by && y < by + h &&
	       z >= bz && z < bz + d ? c : INVALID;
}

inline uint32_t heart(uint32_t e, uint32_t l, uint32_t a, uint32_t mx, uint32_t my, uint32_t mz, uint32_t ss, uint32_t c)
{
	// Source: https://anhngq.wordpress.com/2011/11/05/mupad-heart-in-3d/
	// (x^2+9/4*y^2+z^2-1)^3-x^2*z^3-9/80*y^2*z^3=0

	float x = (float)e - (float)mx, y = (float)l - (float)my, z = (float)a - (float)mz, s = (float)ss;

	x /= s; y /= s; z /= s;

	float q = x * x + 9.0f / 4.0f * y * y + z * z - 1;
	return q * q * q < x * x * z * z * z - 9.0f / 80.0f * y * y * z * z * z ? c : INVALID;
}