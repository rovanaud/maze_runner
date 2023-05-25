#version 330 core // Header for OpenGL 3.3


// Inputs coming from the vertex shader
in struct fragment_data
{
    vec3 position; // position in the world space
    vec3 normal;   // normal in the world space
    vec3 color;    // current color on the fragment
    vec2 uv;       // current uv-texture on the fragment

} fragment;

// Output of the fragment shader - output color
layout(location=0) out vec4 FragColor;

// View matrix
uniform mat4 view;

struct material_structure
{
	vec3 color;  // Uniform color of the object
};
uniform material_structure material;

// Ambiant uniform controled from the GUI
uniform float ambiant;
uniform vec3 light_color;
uniform vec3 light_position;

uniform float diffuse;
uniform float specular;
uniform float specular_exp;

uniform float couleur_brume; 

void main()
{
    vec3 current_color;
  
  vec3 L = normalize(light_position - fragment.position);
  vec3 N = normalize(fragment.normal);

  float diffuse_value = max(dot(N, L), 0.0);
  float diffuse_magnitude = diffuse * diffuse_value;

   // Compute the position of the center of the camera
  mat3 O = transpose(mat3(view));                   // get the orientation matrix
  vec3 last_col = vec3(view * vec4(0.0, 0.0, 0.0, 1.0)); // get the last column
  vec3 camera_position = -O * last_col;
  d = length(camera_position);
  d_max = 15;  

 // Specular coefficient
  float specular_magnitude = 0.0;
  vec3 u_r = reflect(-L, N); // reflection of light vector relative to the normal.
  vec3 u_v = normalize(camera_position - fragment.position);
  specular_magnitude = specular * pow(max(dot(u_r, u_v), 0.0), specular_exp);

  current_color = (ambiant + diffuse_magnitude) * material.color * light_color + specular_magnitude * light_color;

  a_f = min(d/d_max, 1); 
  brume = (1-a_f)*current_color + a_f*couleur_brume;  
  FragColor = vec4(brume, 1.0);
}