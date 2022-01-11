/** CSci-4611 Assignment 3:  Earthquake
 */

#include "earth.h"
#include "config.h"

#include <vector>

// for M_PI constant
#define _USE_MATH_DEFINES
#include <math.h>


Earth::Earth() {
}

Earth::~Earth() {
}

void Earth::Init(const std::vector<std::string> &search_path) {
    // init shader program
    shader_.Init();
    
    // init texture: you can change to a lower-res texture here if needed
    earth_tex_.InitFromFile(Platform::FindFile("earth-1k.png", search_path));

    // init geometry
    const int nslices = 32;
    const int nstacks = 16;

    // TODO: This is where you need to set the vertices and indiceds for earth_mesh_.

    // int nstacks = 16, nslices = 32;
    float x = GfxMath::PI, y = GfxMath::PI / 2;
    float x_tex_plane = 0, y_tex_plane = 512;
    float x_plane, y_plane, x_sphere, y_sphere;

    // lerp(x,y,a) = x + a(y - x)
    for (int i = 0; i < nslices + 1; i++) {
        for (int j = 0; j < nstacks + 1; j++) {
            x_plane = GfxMath::Lerp(-x, x, i * 1.0 / nslices);
            y_plane = GfxMath::Lerp(-y, y, j * 1.0 / nstacks);
            x_sphere = GfxMath::Lerp(-x, x, i * 1.0 / nslices);
            y_sphere = GfxMath::Lerp(-y, y, j * 1.0 / nstacks);

            Point3 Point_Plane = Point3(x_plane, y_plane, 0);
            Point3 Point_Sphere = LatLongToSphere(GfxMath::ToDegrees(y_sphere), GfxMath::ToDegrees(x_sphere));
            Point2 tex_Plane = Point2(x_tex_plane / 1024.0, y_tex_plane / 512.0);

            vertices.push_back(Point_Plane);
            normals.push_back(Vector3(0, 0, 1));

            sphere_vertices.push_back(Point_Sphere);
            sphere_normals.push_back(Point_Sphere - Point3(0, 0, 0));

            tex_coords.push_back(tex_Plane);

            if (i < nslices && j < nstacks) {
                // lower tri
                indices.push_back(i * (nstacks + 1) + j);
                indices.push_back((i + 1) * (nstacks + 1) + j);
                indices.push_back(i * (nstacks + 1) + j + 1);
                // upper tri
                indices.push_back(i * (nstacks + 1) + j + 1);
                indices.push_back((i + 1) * (nstacks + 1) + j);
                indices.push_back((i + 1) * (nstacks + 1) + j + 1);
            }
            y_tex_plane -= 512 / nstacks;
        }
        x_tex_plane += 1024 / nslices;
        y_tex_plane = 512;
    }



    earth_mesh_.SetVertices(vertices);
    earth_mesh_.SetNormals(normals);
    earth_mesh_.SetTexCoords(0, tex_coords);
    earth_mesh_.SetIndices(indices);
    earth_mesh_.UpdateGPUMemory();

    
    
}



void Earth::Draw(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // Define a really bright white light.  Lighting is a property of the "shader"
    DefaultShader::LightProperties light;
    light.position = Point3(10,10,10);
    light.ambient_intensity = Color(1,1,1);
    light.diffuse_intensity = Color(1,1,1);
    light.specular_intensity = Color(1,1,1);
    shader_.SetLight(0, light);

    // Adust the material properties, material is a property of the thing
    // (e.g., a mesh) that we draw with the shader.  The reflectance properties
    // affect the lighting.  The surface texture is the key for getting the
    // image of the earth to show up.
    DefaultShader::MaterialProperties mat;
    mat.ambient_reflectance = Color(0.5, 0.5, 0.5);
    mat.diffuse_reflectance = Color(0.75, 0.75, 0.75);
    mat.specular_reflectance = Color(0.75, 0.75, 0.75);
    mat.surface_texture = earth_tex_;

    // Draw the earth mesh using these settings
    if (earth_mesh_.num_triangles() > 0) {
        shader_.Draw(model_matrix, view_matrix, proj_matrix, &earth_mesh_, mat);
    }
}


Point3 Earth::LatLongToSphere(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> sphere calculations in one place.
    float x_sphere = cos(GfxMath::ToRadians(latitude)) * sin(GfxMath::ToRadians(longitude));
    float y_sphere = sin(GfxMath::ToRadians(latitude));
    float z_sphere = cos(GfxMath::ToRadians(latitude)) * cos(GfxMath::ToRadians(longitude));
    return Point3(x_sphere, y_sphere, z_sphere);
}

Point3 Earth::LatLongToPlane(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> plane calculations in one place.
    
    float y_plane = - GfxMath::PI + (2.0 * GfxMath::PI) * (latitude + 180.0) / (180.0 + 180.0);
    float x_plane = - GfxMath::PI / 2 + GfxMath::PI * (longitude + 90) / (90.0 + 90.0);
 
    return Point3(x_plane, y_plane,0);
}



void Earth::DrawDebugInfo(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // This draws a cylinder for each line segment on each edge of each triangle in your mesh.
    // So it will be very slow if you have a large mesh, but it's quite useful when you are
    // debugging your mesh code, especially if you start with a small mesh.
    for (int t=0; t<earth_mesh_.num_triangles(); t++) {
        std::vector<unsigned int> indices = earth_mesh_.read_triangle_indices_data(t);
        std::vector<Point3> loop;
        loop.push_back(earth_mesh_.read_vertex_data(indices[0]));
        loop.push_back(earth_mesh_.read_vertex_data(indices[1]));
        loop.push_back(earth_mesh_.read_vertex_data(indices[2]));
        quick_shapes_.DrawLines(model_matrix, view_matrix, proj_matrix,
            Color(1,1,0), loop, QuickShapes::LinesType::LINE_LOOP, 0.005f);
    }
}


bool Earth::UpdateEarthMesh(Matrix4 matrix, bool global_transition, float alpha) {
    // plane status
    if (alpha == 0) {
        earth_mesh_.SetVertices(vertices);
        earth_mesh_.SetIndices(indices);
        earth_mesh_.SetTexCoords(0, tex_coords);
        earth_mesh_.SetNormals(normals);
        earth_mesh_.UpdateGPUMemory();
        global_transition = false;
    }
    // sphere status
    else if (alpha == 1) {
        earth_mesh_.SetVertices(sphere_vertices);
        earth_mesh_.SetNormals(sphere_normals);
        earth_mesh_.SetTexCoords(0, tex_coords);
        earth_mesh_.SetIndices(indices);
        earth_mesh_.UpdateGPUMemory();
        global_transition = false;
    }
    // transition status
    else {
        std::vector<Point3> vertices_transition;
        std::vector<Vector3> normals_transition;
        for (int i = 0; i < vertices.size(); i++) {
            vertices_transition.push_back(Point3::Lerp(vertices.at(i), matrix * (sphere_vertices.at(i)), alpha));
            normals_transition.push_back(Vector3::Lerp(matrix * (normals.at(i)), sphere_normals.at(i), alpha));
        }
        earth_mesh_.SetVertices(vertices_transition);
        earth_mesh_.SetIndices(indices);
        earth_mesh_.SetTexCoords(0, tex_coords);
        earth_mesh_.SetNormals(normals_transition);
        earth_mesh_.UpdateGPUMemory();
    }
    
    return global_transition;
}
