
#include "terrain.hpp"


using namespace cgp;

// Evaluate 3D position of the terrain for any (x,y)
float evaluate_terrain_height(float x, float y)
{
    /*vec2 p_0 = { 0, 0 };
    float h_0 = 2.0f;
    float sigma_0 = 3.0f;

    float d = norm(vec2(x, y) - p_0) / sigma_0;

    float z = h_0 * std::exp(-d * d);

    return z;*/

    vec2  p[4] = { { -10, -10 }, { 5,5 }, { -3,4 }, { 6,4 } };
    float h[4] = { 3.0f, -1.5f, 1.0f, 2.0f };
    float o[4] = { 10.0f, 3.0f, 4.0f, 4.0f };

    float z = 0.0f;
    float arg = 0.0f;
    for (int i = 0; i < 4; i++) {
         arg = norm(vec2(x, y) - p[i]) / o[i];
        z += h[i] * std::exp (-arg * arg);
    }
    return z;
}

mesh create_terrain_mesh(int N, float terrain_length)
{

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N*N);

    // Fill terrain geometry
    for(int ku=0; ku<N; ++ku)
    {
        for(int kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku/(N-1.0f);
            float v = kv/(N-1.0f);

            // Compute the real coordinates (x,y) of the terrain in [-terrain_length/2, +terrain_length/2]
            float x = (u - 0.5f) * terrain_length;
            float y = (v - 0.5f) * terrain_length;

            // Compute the surface height function at the given sampled coordinate
            float z = evaluate_terrain_height(x,y);

            // Store vertex coordinates
            terrain.position[kv+N*ku] = {x,y,z};
        }
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for(int ku=0; ku<N-1; ++ku)
    {
        for(int kv=0; kv<N-1; ++kv)
        {
            unsigned int idx = kv + N*ku; // current vertex offset

            uint3 triangle_1 = {idx, idx+1+N, idx+1};
            uint3 triangle_2 = {idx, idx+N, idx+1+N};

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    // need to call this function to fill the other buffer with default values (normal, color, etc)
	terrain.fill_empty_field(); 

    return terrain;
}

std::vector<cgp::vec3> generate_positions_on_terrain(int N, float terrain_length) {
    
    std::vector<cgp::vec3> positions;
    
    

    float sep = 10 * 0.1; // about two times the folliage radius and a half

    float range = (terrain_length - sep) / 2.0f; // no trees out of field bounds

    int i = 0;
    while( i < N) {
        float x = rand_interval(-range, range);
        float y = rand_interval(-range, range);
        bool flag = true;
        for (vec3 p : positions) {
            if (norm(vec2(x, y) - p.xy()) < sep) {
                flag = false;
                break;
            }
        }
        if (flag) {
            positions.push_back(vec3(x, y, evaluate_terrain_height(x, y)));
            i++;
        }
    }

    return positions;
}