#pragma once
#include "common_head.h"

class aabb {
public:
	aabb() {}
	aabb(const point3& a, const point3& b) { minimum = a; maximum = b; }

	point3 min() const { return minimum; }
	point3 max() const { return maximum; }

	bool hit(const ray& r, double t_min, double t_max) const {

	}

private:
	point3 minimum;
	point3 maximum;
};