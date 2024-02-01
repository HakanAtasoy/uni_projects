#include <iostream>
#include "parser.h"
#include "ppm.h"
#include <limits>
#include "raytracer.h"
#include <algorithm>


float dotP(Vec3f a, Vec3f b){

    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3f crossP(Vec3f v, Vec3f w){

    Vec3f result;
    result.x = v.y*w.z - v.z*w.y;
    result.y = v.z*w.x - v.x*w.z;
    result.z = v.x*w.y - v.y*w.x;
    return result;
}

Vec3f componentP(Vec3f a, Vec3f b){
    
    Vec3f result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    return result;

}

Vec3f multS(Vec3f a, float s){

    Vec3f result;
    result.x = a.x*s;
    result.y = a.y*s;
    result.z = a.z*s; 
    return result;
}

Vec3f add(Vec3f a, Vec3f b){

    Vec3f result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

Vec3f operator-(const Vec3f& a, const Vec3f& b) {
    Vec3f result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

ray generateRay(int j, int i, const Vec3f& w, const Vec3f& v, const Vec3f& u, const Vec3f& e, const Vec3f& m, const Vec3f& q, float suCoeff, float svCoeff){

    ray result;
    float su, sv;

    su = (i+0.5)*suCoeff;
    sv = (j+0.5)*svCoeff;

    Vec3f s = add(q, add(multS(u, su), multS(v, -sv)));

    result.o =  e;
    result.d = add(s, multS(e, -1));
    return result;
}

Vec3f computeColor(ray ray, const Scene& scene, std::vector<Vec3f>& triangle_normals, std::vector<Vec3f>& mesh_normals){

    if (ray.depth > scene.max_recursion_depth)
        return Vec3f();

    hitRecord intersection = closestHit(ray, scene, triangle_normals, mesh_normals);
    if (intersection.hit)
        return applyShading(ray, intersection, scene, triangle_normals, mesh_normals);
  
    else if (ray.depth == 0){
        Vec3f background_color_float;
        background_color_float.x = scene.background_color.x;
        background_color_float.y = scene.background_color.y;
        background_color_float.z = scene.background_color.z;
        return background_color_float;
    }
 
    else 
        return Vec3f();
}

Vec3f applyShading(ray generatedRay, hitRecord& record, const Scene& scene, std::vector<Vec3f>& triangle_normals, std::vector<Vec3f>& mesh_normals) {

    Material mat = record.material;
    Vec3f color = componentP(mat.ambient,scene.ambient_light);
    if(mat.is_mirror)  {

        Vec3f normalized_ray = generatedRay.d;
        normalized_ray.normalize();
        ray reflection_ray;
        reflection_ray.o = record.intersection;
        reflection_ray.d = normalized_ray - multS(record.normal, 2*dotP(normalized_ray, record.normal));
        reflection_ray.depth = generatedRay.depth + 1;
        color = add(color, componentP(computeColor(reflection_ray, scene, triangle_normals, mesh_normals), mat.mirror));
    }

    

    for(const auto& light : scene.point_lights) {

        if(!inShadow(record, light, scene))
            color = add(color, add(diffuseTerm(record, light), specularTerm(record, generatedRay, light)));
    }

    return color;

}

bool inShadow(hitRecord record, PointLight light, const Scene& scene) {

    ray shadow_ray;
    shadow_ray.o = add(record.intersection, multS(record.normal, 1e-4));
    shadow_ray.d = light.position - shadow_ray.o;
    record.time = std::numeric_limits<float>::infinity(); 

    for (int i = 0; i < scene.triangles.size(); i++) {

        Triangle tri = scene.triangles[i];
        Vec3f a = scene.vertex_data[tri.indices.v0_id - 1], b = scene.vertex_data[tri.indices.v1_id - 1], c = scene.vertex_data[tri.indices.v2_id - 1] ; 
        if(triangleIntersection(record, shadow_ray, a, b, c, true))
            return true;
        
    }

    for (int i = 0; i < scene.meshes.size(); i++) {
        
        int face_count = scene.meshes[i].faces.size(); 

        for(int j = 0; j < face_count; j++){

            Face tri = scene.meshes[i].faces[j];
            Vec3f a = scene.vertex_data[tri.v0_id - 1], b = scene.vertex_data[tri.v1_id - 1], c = scene.vertex_data[tri.v2_id - 1] ; 
            if(triangleIntersection(record, shadow_ray, a, b, c, true))
                return true;
        }
    }

    for (int i = 0; i < scene.spheres.size(); i++) {

        Sphere sphere = scene.spheres[i];
        if(sphereIntersection(record, shadow_ray, sphere, scene.vertex_data[sphere.center_vertex_id - 1], true))
            return true;
    }

    return false;

}

Vec3f diffuseTerm(hitRecord record, PointLight light) {

    Vec3f light_distance = light.position - record.intersection;
    Vec3f intensity_at_intersection = multS(light.intensity, 1.0f/dotP(light_distance, light_distance));
    light_distance.normalize();
    float cosTheta = std::max(0.0f, dotP(record.normal, light_distance));
    Vec3f color = multS(componentP(intensity_at_intersection, record.material.diffuse), cosTheta);
    return color;
}

Vec3f specularTerm(hitRecord record, ray generatedRay, PointLight light) {

    Vec3f light_distance = light.position - record.intersection;
    Vec3f light_direction = light_distance;
    light_direction.normalize();
    float cosTheta = std::max(0.0f, dotP(record.normal, light_direction));
    
    if(cosTheta < 1e-4)
        return Vec3f();
    
    Vec3f intensity_at_intersection = multS(light.intensity, 1.0f/dotP(light_distance, light_distance));
    generatedRay.d.normalize();
    Vec3f viewer_direction = multS(generatedRay.d, -1);
    Vec3f halfway_vector = (add(viewer_direction,light_direction));
    halfway_vector.normalize();
    float cosAlpha = std::max(0.0f, dotP(record.normal, halfway_vector));
    
    float specular_intensity = std::pow(cosAlpha, record.material.phong_exponent);
    Vec3f color = multS(componentP(intensity_at_intersection, record.material.specular), specular_intensity);
    return color;

}

hitRecord closestHit(ray ray, const Scene& scene, std::vector<Vec3f>& triangle_normals, std::vector<Vec3f>& mesh_normals){

    /* std::vector<Mesh> meshes = scene.meshes;
    std::vector<Triangle> triangles = scene.triangles;
    std::vector<Sphere> spheres = scene.spheres; */ 
    hitRecord intersection;
    intersection.hit = false;
    intersection.time = std::numeric_limits<float>::infinity(); 
    int object[3] = {-1,-1,-1};

    for (int i = 0; i < scene.triangles.size(); i++) {

        Vec3f normalized_direction = ray.d;
        normalized_direction.normalize();
        float dotProduct = dotP(normalized_direction, triangle_normals[i]);
        if (std::abs(dotProduct) < 1e-4 || dotProduct > 0) 
            continue;

        Triangle tri = scene.triangles[i];
        Vec3f a = scene.vertex_data[tri.indices.v0_id - 1], b = scene.vertex_data[tri.indices.v1_id - 1], c = scene.vertex_data[tri.indices.v2_id - 1] ; 
        if(triangleIntersection(intersection, ray, a, b, c)){
            intersection.material = scene.materials[tri.material_id - 1];
            intersection.normal = triangle_normals[i];
            object[0] = 0;
            object[1] = i;
        }
        
    }

    for (int i = 0, ind = 0; i < scene.meshes.size(); i++) {
        
        int face_count = scene.meshes[i].faces.size(); 

        for(int j = 0; j < face_count; j++, ind++){

            Vec3f normalized_direction = ray.d;
            normalized_direction.normalize();
            float dotProduct = dotP(normalized_direction, mesh_normals[ind]);
            if (std::abs(dotProduct) < 1e-4 || dotProduct > 0) 
                continue;

            Face tri = scene.meshes[i].faces[j];
            Vec3f a = scene.vertex_data[tri.v0_id - 1], b = scene.vertex_data[tri.v1_id - 1], c = scene.vertex_data[tri.v2_id - 1]; 
            if(triangleIntersection(intersection, ray, a, b, c)){
                intersection.material = scene.materials[scene.meshes[i].material_id - 1];
                intersection.normal = mesh_normals[ind];
                object[0] = 1;
                object[1] = i;
                object[2] = j;
            }
            
        }
    }

    for (int i = 0; i < scene.spheres.size(); i++) {

        Sphere sphere = scene.spheres[i];
        if(sphereIntersection(intersection, ray, sphere, scene.vertex_data[sphere.center_vertex_id-1])) {

            intersection.material = scene.materials[sphere.material_id - 1];
            object[0] = 2;
            object[1] = i;
        }

    }


    return intersection;
}

bool sphereIntersection(hitRecord& intersection, ray ray, Sphere sphere, Vec3f center, bool shadow) {

    struct Vec3f oc = ray.o - center;
    float tmin = 1.0001, tmax = 50;
    ray.d.normalize();
    float a = 1;
    float b = 2*dotP(oc, ray.d);
    float c = dotP(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = b * b -  4 * a * c;
    if (std::abs(discriminant) < 1e-4) {

        float t = -b / 2*a;

        if(shadow){
            if(t > 1e-4)
                return true;
        }

        else if(t < intersection.time && t > tmin && t < tmax ) {
            
            intersection.hit = true;
            intersection.time = t;
            intersection.intersection = add(ray.o,multS(ray.d, t));
            intersection.normal = (intersection.intersection - center);
            intersection.normal.normalize();
            return true;
        }

    } 
    
    else if (discriminant > 1e-4) {

        float t1 = (-b + std::sqrt(discriminant)) /  2*a;
        float t2 = (-b - std::sqrt(discriminant)) /  2*a;
        float t;

        if(shadow){
            if(t1 > 1e-4 || t2 > 1e-4)
                return true;
        }

        else {
            
            if(t1 < tmin)
                t = t2;

            else if (t2 < tmin)
                t = t1;

            else
                t = t1 < t2 ? t1 : t2;

            if(t < intersection.time && t > tmin && t < tmax ){

                intersection.hit = true;
                intersection.time = t;
                intersection.intersection = add(ray.o,multS(ray.d, t));
                intersection.normal = (intersection.intersection - center);
                intersection.normal.normalize();
                return true;
            }
        }
    } 

    return false;
}


bool triangleIntersection(hitRecord& record, ray ray, Vec3f a, Vec3f b, Vec3f c, bool shadow){
    
    Vec3f o = ray.o, d = ray.d;
    float detA = (a.x-b.x)*((a.y-c.y)*d.z - (a.z-c.z)*d.y) + (a.x-c.x)*((a.z-b.z)*d.y - (a.y-b.y)*d.z) + d.x*((a.y-b.y)*(a.z-c.z) - (a.y-c.y)*(a.z-b.z));
    if(std::abs(detA) < 1e-7)
        return false;

    float B = ( (a.x-o.x)*((a.y-c.y)*d.z - (a.z-c.z)*d.y) + (a.x-c.x)*((a.z-o.z)*d.y - (a.y-o.y)*d.z) + d.x*((a.y-o.y)*(a.z-c.z) - (a.y-c.y)*(a.z-o.z)) ) / detA;
    if(B < 0 || B > 1)
        return false;

    float y = ( (a.x-b.x)*((a.y-o.y)*d.z - (a.z-o.z)*d.y) + (a.x-o.x)*((a.z-b.z)*d.y - (a.y-b.y)*d.z) + d.x*((a.y-b.y)*(a.z-o.z) - (a.y-o.y)*(a.z-b.z)) ) / detA;
    if(y< 0 || y > 1 || (B + y) > 1)
        return false;

    float t = ( (a.x-b.x)*((a.y-c.y)*(a.z-o.z) - (a.y-o.y)*(a.z-c.z)) + (a.x-c.x)*((a.y-o.y)*(a.z-b.z) - (a.y-b.y)*(a.z-o.z)) + (a.x-o.x)*((a.y-b.y)*(a.z-c.z) - (a.y-c.y)*(a.z-b.z)) ) / detA;

    float tmin = 1 + 1e-4, tmax = 50;

    
    if(shadow){
        if(t > 1e-4 && t < 1 + 1e-4)
            return true;
    }

    else if(t < record.time && t > tmin && t < tmax) {

        record.hit = true;
        record.time = t;
        record.intersection = add(ray.o,multS(ray.d,t));
        /* Vec3f alfaA = multS(a, 1-b-y), betaB = multS(b, b), gammaC = multS(c, y);
        record.intersection.x = alfaA.x + betaB.x + gammaC.x;
        record.intersection.y = alfaA.y + betaB.y + gammaC.y;
        record.intersection.z = alfaA.z + betaB.z + gammaC.z; */
        return true;
    }

    return false;

}

int main(int argc, char* argv[])
{
    // Sample usage for reading an XML scene file
    parser::Scene scene;
    scene.loadFromXml(argv[1]);
    unsigned char* image;
    Vec3f m, q, u, v, w, e ;
    float suCoeff, svCoeff;                     
    std::vector<Vec3f> triangle_normals, mesh_normals;
    for(const auto& triangle : scene.triangles){

        const Vec3f& v0 = scene.vertex_data[triangle.indices.v0_id - 1];
        const Vec3f& v1 = scene.vertex_data[triangle.indices.v1_id - 1];
        const Vec3f& v2 = scene.vertex_data[triangle.indices.v2_id - 1];
        Vec3f edge1 = v1 - v0;
        Vec3f edge2 = v2 - v0;
        Vec3f normal = crossP(edge1,edge2);
        normal.normalize();
        triangle_normals.push_back(normal);
    }

    for(const auto& mesh : scene.meshes){    
        for(const auto& face : mesh.faces){

            const Vec3f& v0 = scene.vertex_data[face.v0_id - 1];
            const Vec3f& v1 = scene.vertex_data[face.v1_id - 1];
            const Vec3f& v2 = scene.vertex_data[face.v2_id - 1];
            Vec3f edge1 = v1 - v0;
            Vec3f edge2 = v2 - v0;
            Vec3f normal = crossP(edge1,edge2);
            normal.normalize();
            mesh_normals.push_back(normal);
        }
    }

    for (const auto& camera : scene.cameras) {

        image = new unsigned char[3*camera.image_height*camera.image_width];
        w = multS(camera.gaze, -1); 
        v = camera.up;
        u = crossP(v, w);
        e = camera.position;
        m = add(e, multS(camera.gaze, camera.near_distance));
        q = add(m, add(multS(u, camera.near_plane.x), multS(v, camera.near_plane.w)));
        suCoeff = (camera.near_plane.y - camera.near_plane.x)/camera.image_width;
        svCoeff = (camera.near_plane.w - camera.near_plane.z)/camera.image_height;
        for(int i = 0, idx = 0; i < camera.image_height; i++){

            for(int j = 0; j < camera.image_width; j++){

                ray generatedRay = generateRay(i, j, w, v, u, e, m, q, suCoeff, svCoeff);
                generatedRay.depth = 0;
                Vec3f rayColor = computeColor(generatedRay, scene, triangle_normals, mesh_normals);
                image[idx++] = static_cast<unsigned char>(std::round(std::clamp(rayColor.x, 0.0f, 255.0f)));
                image[idx++] = static_cast<unsigned char>(std::round(std::clamp(rayColor.y, 0.0f, 255.0f)));
                image[idx++] = static_cast<unsigned char>(std::round(std::clamp(rayColor.z, 0.0f, 255.0f)));

            }
        }

        write_ppm(camera.image_name.c_str(), image, camera.image_width, camera.image_height);
        delete [] image;
    }

    /*

    const RGB BAR_COLOR[8] =
    {
        { 255, 255, 255 },  // 100% White
        { 255, 255,   0 },  // Yellow
        {   0, 255, 255 },  // Cyan
        {   0, 255,   0 },  // Green
        { 255,   0, 255 },  // Magenta
        { 255,   0,   0 },  // Red
        {   0,   0, 255 },  // Blue
        {   0,   0,   0 },  // Black
    };

    int width = 640, height = 480;
    int columnWidth = width / 8;

    unsigned char* image = new unsigned char [width * height * 3];

    int i = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int colIdx = x / columnWidth;
            image[i++] = BAR_COLOR[colIdx][0];
            image[i++] = BAR_COLOR[colIdx][1];
            image[i++] = BAR_COLOR[colIdx][2];
        }
    }

    write_ppm("test.ppm", image, width, height);

    */

}
