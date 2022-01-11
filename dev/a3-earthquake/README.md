# CSci-4611 Assignment 3:  Earthquake
This is a discription file for assignment 3 - Earthquake, created by Junyue Tao.

## Verticies, indicies and mesh coordinates
### Adding / Setting
Using a rectangle subdivided into multiple triangles to represent a flat map of the earth.
The flat map mesh lies on the xy plane, with x values ranging from −π to π nad y values 
ranging from −π/2 to π/2. It is divided into nslices and nstacks divisions vertically.
The 3D coordiantes in the plane view, 3D coordinates in the global view, texture
coordinates and normals in both views of all vertices are calculated in the for loops.
Using the following pseudocode(partially) to achieve that.
```
for (int i = 0; i < nslices + 1; i++) {
        for (int j = 0; j < nstacks + 1; j++) {
            plane_x/y = Lerp(-x, x, i * 1.0 / nslices);
            sphere_x/y = Lerp(-x, x, i * 1.0 / nslices);

            calculate Point_Plane
            calculate Point_Sphere
            calculate tex_Plane

            vertices.push_back(Point_Plane);
            normals.push_back(Vector3(0, 0, 1));

            if (i < nslices && j < nstacks) {
                push upper triange indices follow counterclock order within single rect

                push lower triange indices follow counterclock order within single rect
            }
            ...
        }
        ...
    }
```

The 3D Cartesian coordinates of the corresponding point on the plane can be calculated 
using LatLongToPlane(latitude_, longitude), while the 3D Cartesian coordinates of the 
corresponding point on the sphere can be calculated using LatLongToSphere(latitude_, 
longitude).

```
Point3 Earth::LatLongToSphere(double latitude, double longitude) const {
    float x_sphere = GfxMath::cos(GfxMath::ToRadians(latitude)) * GfxMath::sin(GfxMath::ToRadians(longitude));
    float y_sphere = GfxMath::sin(GfxMath::ToRadians(latitude));
    float z_sphere = GfxMath::cos(GfxMath::ToRadians(latitude)) * GfxMath::cos(GfxMath::ToRadians(longitude));
    return Point3(x_sphere, y_sphere, z_sphere);
}

Point3 Earth::LatLongToPlane(double latitude, double longitude) const {
    float y_plane = - GfxMath::PI + (2.0 * GfxMath::PI) * (latitude + 180.0) / (180.0 + 180.0);
    float x_plane = - GfxMath::PI / 2 + GfxMath::PI * (longitude + 90) / (90.0 + 90.0);
    return Point3(x_plane, y_plane,0);
```

### Updateing 
There are two symbol bool parameters that indicates the tansform status between plane state
and sphere date. If these parameters are detected, an update function will be called and 
set different vertices array. 

```
bool Earth::UpdateEarthMesh(Matrix4 matrix, bool global_transition, float alpha) {
    if (alpha == 0) {
        set up plane status
    }
    else if (alpha == 1) {
        set up sphere status
    }
    else {
        set up transition status
    }
}
```

## Earthquakes
### Drawing earthquakes
To find the earthquakes that have happened in the past one year of the current visualization 
time, the program would firstly find the correct time one year ago by subtracting 
PLAYBACK_WINDOW(Number of seconds in 1 year (approx.)) from the current_time. Then I can 
find the start index (included) of the earthquakes in the past one year by finding the most 
recent quake after the one_year_ago_time_ and find the end index (excluded) by finding the 
most recent quake after current_time_.

Under each situations: plane, sphere, and transition. General drawing principles inplemented
as below(pseudocode):

```
for (int i = earthquake_start_idx; i < earthquake_end_idx; i++) {
    get latitude/longitute
    get scaled earthquake magnitude
    calculate earthquake balls' position and scale size
    combine transform matrix together
    quick_shapes_.DrawSphere();
}
```

### Transform between plane state and sphere state
Once the global button is pressed. The program will call a function called updateMesh repeatedly.
Each time the parameter alpha will be different in order to smoothly change the look of the mesh.
After the transition complete, the UpdateMesh function will no longer be called.

```
if (global_transition) {
        if (global_switch) {
            alpha += 0.01;
        }
        else {
            alpha -= 0.01;
        }
        alpha = GfxMath::Clamp(alpha, 0, 1);
        updateMesh
    }
    else {
        if (global_switch) {
            calcuate rotation angle
            calcuate global sphere matrix
        }
    }
}
```