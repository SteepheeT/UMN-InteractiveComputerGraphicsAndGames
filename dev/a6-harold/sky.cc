/** CSci-4611 Assignment 6: Harold
 */

#include "sky.h"


Sky::Sky() {
    
}


Sky::~Sky() {
    
}

void Sky::Init(ShaderProgram *stroke3d_shaderprog) {
    stroke3d_shaderprog_ = stroke3d_shaderprog;
}


/// Projects a 2D normalized screen point (e.g., the mouse position in normalized
/// device coordinates) to a 3D point on the "sky", which is really a huge sphere
/// (radius = 1500) that the viewer is inside.  This function should always return
/// true since any screen point can successfully be projected onto the sphere.
/// sky_point is set to the resulting 3D point.  Note, this function only checks
/// to see if the ray passing through the screen point intersects the sphere; it
/// does not check to see if the ray hits the ground or anything else first.
bool Sky::ScreenPtHitsSky(const Matrix4 &view_matrix, const Matrix4 &proj_matrix,
                        const Point2 &normalized_screen_pt, Point3 *sky_point)
{
    // TODO: Stitch together your worksheet implementation of this method
    Matrix4 camera_matrix = view_matrix.Inverse();
    Point3 eye = camera_matrix.ColumnToPoint3(3);
    Point3 mouseIn3d = GfxMath::ScreenToNearPlane(view_matrix, proj_matrix, normalized_screen_pt);
    Ray eyeThroughMouse = Ray(eye, (mouseIn3d - eye).ToUnit());
    
    float insertionTime = 0;
    float radius = 1500;
    Point3 center = Point3::Origin();
    bool intersects = eyeThroughMouse.IntersectSphere(center, radius, &insertionTime, sky_point);
    return intersects;
}




/// Creates a new sky stroke mesh by projecting each vertex of the 2D mesh
/// onto the sky dome and saving the result as a new 3D mesh.
void Sky::AddSkyStroke(const Matrix4 &view_matrix, const Matrix4 &proj_matrix,
                       const Mesh &stroke2d_mesh, const Color &stroke_color)
{
    // TODO: Create a new SkyStroke and add it to the strokes_ array.
    SkyStroke sky_stroke;
    std::vector<Point3> skyVertices;
    
    Mesh mesh3d = stroke2d_mesh;
    int totalVerticies = stroke2d_mesh.num_vertices();
    for (int idx = 0; idx < totalVerticies; idx++) {
        Point2 vertexIn2d = Point2(mesh3d.read_vertex_data(idx)[0], mesh3d.read_vertex_data(idx)[1]);
        Point3 vertexIn3d;
        // ScreenPtHitsSky(view_matrix, proj_matrix, Point2(mesh3d.read_vertex_data(idx)[0], mesh3d.read_vertex_data(idx)[1]), &mesh3d.read_vertex_data(idx));
        ScreenPtHitsSky(view_matrix, proj_matrix, vertexIn2d, &vertexIn3d);
        skyVertices.push_back(vertexIn3d);
    }
    mesh3d.SetVertices(skyVertices);

    sky_stroke.mesh = mesh3d;
    sky_stroke.color = stroke_color;
    
    strokes_.push_back(sky_stroke);
}


/// Draws all of the sky strokes
void Sky::Draw(const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {

    // Precompute matrices needed in the shader
    Matrix4 model_matrix; // identity
    Matrix4 modelview_matrix = view_matrix * model_matrix;
    
    // Draw sky meshes
    stroke3d_shaderprog_->UseProgram();
    stroke3d_shaderprog_->SetUniform("modelViewMatrix", modelview_matrix);
    stroke3d_shaderprog_->SetUniform("projectionMatrix", proj_matrix);
    for (int i=0; i<strokes_.size(); i++) {
        stroke3d_shaderprog_->SetUniform("strokeColor", strokes_[i].color);
        strokes_[i].mesh.Draw();
    }
    stroke3d_shaderprog_->StopProgram();
}

