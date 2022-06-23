#pragma once
#include<cmath>
#include<iostream>

class vec3 {
public:
	explicit vec3() :e{ 0,0,0 } {};
	explicit vec3(double e0, double e1, double e2) :e{ e0, e1, e2 } {};
	vec3(const vec3 &v) :e{ v[0],v[1],v[2] } {};

	double x() { return e[0]; }
	double y() { return e[1]; }
	double z() { return e[2]; }

	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	vec3 operator+(const vec3& v) const { return vec3(e[0] + v[0], e[1] + v[1], e[2] + v[2]); }
	vec3 operator+(double t) const { return vec3(e[0] + t, e[1] + t, e[2] + t); }
	vec3& operator+=(const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}
	vec3 operator-(const vec3& v) const { return vec3(e[0] - v[0], e[1] - v[1], e[2] - v[2]); }
	vec3 operator-(double t) const { return vec3(e[0] - t, e[1] - t, e[2] - t); }
	vec3& operator-=(const vec3& v) {
		e[0] -= v.e[0];
		e[1] -= v.e[1];
		e[2] -= v.e[2];
		return *this;
	}
	vec3 operator*(const vec3& v) const { return vec3(e[0] * v[0], e[1] * v[1], e[2] * v[2]); }
	vec3 operator*(double t) const { return vec3(e[0] * t, e[1] * t, e[2] * t); }
	vec3& operator*=(const vec3& v) {
		e[0] *= v.e[0];
		e[1] *= v.e[1];
		e[2] *= v.e[2];
		return *this;
	}
	vec3 operator/(const vec3& v) const { return vec3(e[0] / v[0], e[1] / v[1], e[2] / v[2]); }
	vec3 operator/(double t) const { return vec3(e[0] / t, e[1] / t, e[2] / t); }
	vec3& operator/=(const vec3& v) {
		e[0] /= v.e[0];
		e[1] /= v.e[1];
		e[2] /= v.e[2];
		return *this;
	}

	double length() const {
		return std::sqrt(length_squared());
	}
	double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
		const int a = 1;
		int b = a + 1;
	}
	bool near_zero() const {
		const auto s = 1e-8;
		return (fabs(e[0] < s)) && (fabs(e[1] < s)) && (fabs(e[2] < s));
	}
	static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}
	static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

private:
	double e[3];
};

typedef vec3 point3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v[0] << ' ' << v[1] << ' ' << v[2];
}
inline double dot(const vec3& a, const vec3& b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
inline vec3 cross(const vec3& a, const vec3& b) {
	return vec3(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]);
}
inline vec3 unit_vector(const vec3& a) {
	return a / a.length();
}
inline vec3 operator+(double d, vec3 v) {
	return v + d;
}
inline vec3 operator-(double d, vec3 v) {
	return v - d;
}
inline vec3 operator*(double d, vec3 v) {
	return v * d;
}
inline vec3 operator/(double d, vec3 v) {
	return v / d;
}
vec3 random_in_unit_sphere() {
	while (1) {
		auto p = vec3::random(-1, 1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}
vec3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}
vec3 random_in_unit_disk() {
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}
vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}