#pragma once

#include"common_head.h"
#include"hittable.h"

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
	hittable_list() {};
	hittable_list(shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<hittable>object) { objects.push_back(object); }
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
private:
	std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (const auto& object : objects) {
		if (object->hit(r, t_min, closest_so_far, rec)) {
			hit_anything = true;
			closest_so_far = rec.t;
		}
	}
	return hit_anything;
}