#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "parser.h" 
#include "ppm.h"

using namespace parser;

typedef struct{
    Vec3f o, d;
    int depth;
} ray;

typedef struct{
    Vec3f intersection;
    Material material;
    Vec3f normal;
    float time;
    bool hit;
} hitRecord;

float dotP(Vec3f a, Vec3f b);

Vec3f crossP(Vec3f v, Vec3f w);

Vec3f componentP(Vec3f a, Vec3f b);

Vec3f multS(Vec3f a, float s);

Vec3f add(Vec3f a, Vec3f b);

Vec3f operator-(const Vec3f& a, const Vec3f& b) ;

ray generateRay(int i, int j, const Vec3f& w, const Vec3f& v, const Vec3f& u, const Vec3f& e, const Vec3f& m, const Vec3f& q, float suCoeff, float svCoeff);

Vec3f computerColor(ray ray, const Scene& scene, std::vector<Vec3f>& triangle_normals, std::vector<Vec3f>& mesh_normals);

Vec3f applyShading(ray ray, hitRecord& record, const Scene& scene, std::vector<Vec3f>& triangle_normals, std::vector<Vec3f>& mesh_normals);

bool inShadow(hitRecord record, PointLight light, const Scene& scene);

Vec3f diffuseTerm(hitRecord record, PointLight light);

Vec3f specularTerm(hitRecord record, ray generatedRay, PointLight light);

hitRecord closestHit(ray ray, const Scene& scene, std::vector<Vec3f>& triangle_normals, std::vector<Vec3f>& mesh_normals);

bool sphereIntersection(hitRecord& intersection, ray ray, Sphere sphere, Vec3f center, bool shadow = false );

bool triangleIntersection(hitRecord& record, ray ray, Vec3f a, Vec3f b, Vec3f c, bool shadow = false );


#endif
