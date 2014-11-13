#pragma once
#include <math.h>
#include <stdlib.h>
#include <gccore.h>
#define aabbHeapSize 512

typedef struct {
	double x, y, z;
} vector3;

#define vec3Dot(a,b) ((a.x * b.x) + (a.y * b.y) + (a.z * b.z))
inline vector3 vec3Normalize(vector3 a);

typedef struct {
	bool alloced;
	vector3 min;
	vector3 max;
} _aabb;

typedef int aabb;

aabb aabb_alloc(double x1, double y1, double z1, double x2, double y2, double z2);
void aabb_dealloc(aabb a);
void aabb_set(aabb a, double x1, double y1, double z1, double x2, double y2, double z2);
vector3 aabb_getMin(aabb a);
vector3 aabb_getMax(aabb a);
aabb aabb_copy(aabb _a);
void aabb_round(aabb _a);

//if equal then will move both a and b, else only move a
bool aabb_process(aabb _a, aabb _b, bool equal);
bool aabb_resolvecollisionwithworld(aabb _a);
