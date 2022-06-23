#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include "common_head.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "moving_sphere.h"

color ray_color(const ray& r, const hittable& world, int depth) {
	hit_record rec;

	if (depth < 0)
		return color(0, 0, 0);

	if (world.hit(r, 0.0001, DBL_MAX, rec)) {
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return color(0, 0, 0);
	}

	vec3 unit_direction = unit_vector(r.direction());
	double t = 0.5 * (unit_direction.y() + 1.0);
	return color(1.0, 1.0, 1.0) * (1 - t) + color(0.5, 0.7, 1.0) * t;
}

hittable_list random_scene() {
	hittable_list world;

	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					auto center2 = center + vec3(0, random_double(0, 0.5), 0);
					world.add(make_shared<moving_sphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	return world;
}

using namespace cv;
int main() {
	Mat img = Mat::zeros(600, 800, CV_8UC3);

	int samples_per_pixel = 50;
	int max_depth = 50; 

	point3 lookfrom(13, 2, 3);
	point3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.1;
	camera cam(lookfrom, lookat, vup, 20, aperture, dist_to_focus, 0, 1);

	hittable_list world = random_scene();

	for (double x = 0; x < img.rows; x++) {
		std::cerr << "\rScanlines remaining: " << img.rows - x << ' ' << std::flush;
		for (double y = 0; y < img.cols; y++) {
			color pixel_color(0, 0, 0);
			for (int i = 0; i < samples_per_pixel; i++) {
				ray r = cam.get_ray((x + random_double()) / (img.rows - 1), (y + random_double()) / (img.cols - 1));
				pixel_color += ray_color(r, world, max_depth);
			}
			pixel_color = pixel_color / samples_per_pixel;
			pixel_color.gamma(1 / 2.2);
			Vec3b a = pixel_color.to_Vec3b();
			img.at<Vec3b>(img.rows - x - 1, y) = a;
		}
	}
	imshow("a", img);
	waitKey();
	return 0;
}