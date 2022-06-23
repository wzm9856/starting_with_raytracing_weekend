#pragma once

#include "common_head.h"
class camera {
public:
	camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aperture, double focus_dist, double _time0 = 0, double _time1 = 0) {
		double theta = degrees_to_radians(vfov);
		double viewport_height = 2.0 * tan(theta / 2);
		double viewport_width = viewport_height / 3.0 * 4.0;

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		origin = lookfrom;
		horizontal = focus_dist * viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

		lens_radius = aperture / 2;
		time0 = _time0;
		time1 = _time1;
	}
	ray get_ray(double x, double y) const {
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, 
			lower_left_corner + x * vertical + y * horizontal - origin - offset, 
			random_double(time0, time1));
	}
private:
	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	double lens_radius;
	double time0, time1;
};
