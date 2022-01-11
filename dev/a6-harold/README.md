#CSci-4611 Assignment 6: Harold
This is a discription file for assignment 6 - Harold, created by Junyue Tao.

In short, all required features are achieved.

###Drawing in the sky:
Projects the mouse position in normalized device coordinates to a 3D point on the "sky", which is really a huge sphere (radius = 1500) that the viewer is inside. This function should always return true since any screen point can successfully be projected onto the sphere. sky_point is set to the resulting 3D point.

bool Sky::ScreenPtHitsSky(const Matrix4 &view_matrix, const Matrix4 &proj_matrix,
const Point2 &normalized_screen_pt, Point3 *sky_point)
```
    Matrix4 camera_matrix = view_matrix.Inverse();
    Point3 eye = camera_matrix.ColumnToPoint3(3);
    Point3 mouseIn3d = GfxMath::ScreenToNearPlane(view_matrix, proj_matrix, normalized_screen_pt);
    Ray eyeThroughMouse = Ray(eye, (mouseIn3d - eye).ToUnit());
        
    float insertionTime = 0;
    float radius = 1500;
    Point3 center = Point3::Origin();
    bool intersects = eyeThroughMouse.IntersectSphere(center, radius, &insertionTime, sky_point);
```

Creates a new sky stroke mesh by projecting each vertex of the 2D mesh onto the sky dome and saving the result as a new 3D mesh.

void Sky::AddSkyStroke(const Matrix4 &view_matrix, const Matrix4 &proj_matrix,
const Mesh &stroke2d_mesh, const Color &stroke_color)
```
    SkyStroke sky_stroke;
    std::vector<Point3> skyVertices;
        
    Mesh mesh3d = stroke2d_mesh;
    int totalVerticies = stroke2d_mesh.num_vertices();
    for (int idx = 0; idx < totalVerticies; idx++) {
        Point2 vertexIn2d = Point2(mesh3d.read_vertex_data(idx)[0], mesh3d.read_vertex_data(idx)[1]);
        Point3 vertexIn3d;
        ScreenPtHitsSky(view_matrix, proj_matrix, vertexIn2d, &vertexIn3d);
        skyVertices.push_back(vertexIn3d);
    }
    mesh3d.SetVertices(skyVertices);
    
    sky_stroke.mesh = mesh3d;
    sky_stroke.color = stroke_color;
        
    strokes_.push_back(sky_stroke);

```
###Editing the ground:

There are 3 major steps to the algorithm, outlined here:

Step 1. Define a plane to project the stroke onto. The first and last points of the stroke are projected onto the ground plane. The plane passes through these two points on the ground. The plane has a normal vector that points toward the camera and is parallel to the ground plane. In order to get the normal vector of correct direction, the plane vector is determined according to the relationship of start and end points of the stroke.

```
    Point2 StartVertexIn2d = stroke2d[0];
    Point2 EndVertexIn2d = stroke2d[stroke2d.size() - 1];
    Point3 StartVertexIn3d = Point3(0, 0, 0);
    Point3 EndVertexIn3d = Point3(0, 0, 0);

    ScreenPtHitsGround(view_matrix, proj_matrix, StartVertexIn2d, &StartVertexIn3d);
    ScreenPtHitsGround(view_matrix, proj_matrix, EndVertexIn2d, &EndVertexIn3d);

    Vector3 upVector = Vector3(0, 1, 0);
    Vector3 proj_plane_dir_vec;

    if (StartVertexIn3d.x() < EndVertexIn3d.x()) {
        proj_plane_dir_vec = EndVertexIn3d - StartVertexIn3d;
    }
    else {
        proj_plane_dir_vec = StartVertexIn3d - EndVertexIn3d;
    }

    Vector3 proj_plane_normal_vec = proj_plane_dir_vec.Cross(upVector).ToUnit();
```

Step 2. Project the 2D stroke into 3D so that it lies on the "projection plane" defined in step 1.

```
    std::vector<Point3> planeVertices;
    
    for (int i = 0; i < stroke2d.size(); i++) {
        Point3 pt3d = GfxMath::ScreenToNearPlane(view_matrix, proj_matrix, stroke2d[i]);
        Ray ray(eye, (pt3d - eye).ToUnit());
        float insertionTime;
        Point3 insertionPoint;

        ray.IntersectPlane(StartVertexIn3d, proj_plane_normal_vec, &insertionTime, &insertionPoint);

        planeVertices.push_back(insertionPoint);
    }
```

Step 3. Loop through all of the vertices of the ground mesh, and adjust the height of each based on the equations in section 4.5 of the paper.

```
    std::vector<Point3> new_verts;
    for (int i=0; i<ground_mesh_.num_vertices(); i++) {
        Point3 P = ground_mesh_.read_vertex_data(i); // original vertex

        // adjust P according to equations...
        Point3 p_on_plane = P.ClosestPointOnPlane(StartVertexIn3d, proj_plane_normal_vec);
        float height = hfunc(proj_plane_normal_vec, planeVertices, p_on_plane);
        float distance = P.DistanceToPlane(StartVertexIn3d, proj_plane_normal_vec);

        float P_y;

        if (height != 0) {
            float wd = 1 - pow((distance / 5), 2);
            if (wd < 0) {
                wd = 0;
            }

            P_y = (1 - wd) * P.y() + wd * height;
        }
        else {
            P_y = P.y();
        }

        P = Point3(P.x(), P_y, P.z());

        new_verts.push_back(P);
    }
```