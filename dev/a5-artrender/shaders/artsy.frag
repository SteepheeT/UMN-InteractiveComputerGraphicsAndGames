#version 330

// CSci-4611 Assignment 5:  Art Render

// TODO: You need to calculate per-fragment shading here using a toon shading model

in vec3 position_in_eye_space;
in vec3 normal_in_eye_space;

out vec4 color;

uniform vec3 light_in_eye_space;
uniform vec4 Ia, Id, Is;

uniform vec4 ka, kd, ks;
uniform float s;

uniform sampler2D diffuse_ramp;
uniform sampler2D specular_ramp;


void main() {
    vec3 v = position_in_eye_space;
    vec3 l = normalize(light_in_eye_space - v);
    vec3 e = normalize(vec3(0, 0, 0) - v);
    vec3 n = normalize(normal_in_eye_space);
    vec3 h = normalize(l + e);

    float diffuse_intensity = (dot(n, l) + 1) / 2;
    if(diffuse_intensity > 0.7){
        diffuse_intensity = 1;
    } else if (diffuse_intensity > 0.5){
        diffuse_intensity = 0.6;
    } else {
        diffuse_intensity = 0;
    }
    vec2 diffuse_texture_coord = vec2(diffuse_intensity, 1 - diffuse_intensity);
    vec4 ambient = ka * Ia;
    vec4 diffuse = kd * Id * texture(diffuse_ramp, diffuse_texture_coord);

    float specular_intensity = pow(max(dot(n, h), 0), s);
    vec2 specular_texture_coord = vec2(specular_intensity, 1 - specular_intensity);
    vec4 specular = texture(specular_ramp, specular_texture_coord);

    color = ambient + diffuse + specular;
}
