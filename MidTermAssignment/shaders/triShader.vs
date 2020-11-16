#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float scale_factor;

out vec3 LightIntensity;

vec4 LightPosition = vec4 (10.0, 10.0, 4.0, 1.0); // Light position in world coords.
vec3 Kd = vec3 (0.0, 0.8, 0.0); // green diffuse surface reflectance
vec3 Ld = vec3 (1.0, 1.0, 1.0); // Light source intensity

void main()
{
    gl_Position = projection * view * model * vec4(aPos, scale_factor);
    mat4 ModelViewMatrix = view * model;
    mat3 NormalMatrix =  mat3(ModelViewMatrix);
    // // Convert normal and position to eye coords
    // // Normal in view space
    vec3 tnorm = normalize( NormalMatrix * aNormal);
    // // Position in view space
    vec4 eyeCoords = ModelViewMatrix * vec4(aPos,1.0);
    // //normalised vector towards the light source
    vec3 s = normalize(vec3(LightPosition - eyeCoords));
    
    // // The diffuse shading equation, dot product gives us the cosine of angle between the vectors
    // LightIntensity = Ld * Kd * max( s, 0.0 );
    LightIntensity = Ld * Kd * max( dot( s, tnorm ), 0.0 );
}
