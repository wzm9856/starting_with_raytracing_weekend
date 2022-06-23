#pragma once
#include "vec3.h"
#include <opencv2/opencv.hpp>

class color : public vec3 {
public:
	color() :vec3() {};
	color(double e0, double e1, double e2) :vec3(e0, e1, e2) {};
	color(vec3 v) :vec3(v) {};
	cv::Vec3b to_Vec3b() const {
		return cv::Vec3b(this->operator[](2) * 255, this->operator[](1) * 255, this->operator[](0) * 255);
	}
	void gamma(double g) {
		this->operator[](0) = pow(this->operator[](0), g);
		this->operator[](1) = pow(this->operator[](1), g);
		this->operator[](2) = pow(this->operator[](2), g);
	}
};
