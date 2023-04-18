#include "tree.hpp"

using namespace cgp;


mesh create_cylinder_mesh(float radius, float height)
{
    mesh m; 
    // To do: fill this mesh ...
    // ...
    // To add a position: 
    //   m.position.push_back(vec3{x,y,z})
    // Or in pre-allocating the buffer:
    //   m.position.resize(maximalSize);
    //   m.position[index] = vec3{x,y,z}; (with 0<= index < maximalSize)
    // 
    // Similar with the triangle connectivity:
    //  m.connectivity.push_back(uint3{index_1, index_2, index_3});


    // Need to call fill_empty_field() before returning the mesh 
    //  this function fill all empty buffer with default values (ex. normals, colors, etc).
    
    int n = 16; // double de la taille
    float alpha = 0.0f;
    float increment = 4 * Pi / n;

    for (int i = 0; i < n; i += 2) {
        vec3 base = vec3(radius * cos(alpha), radius * sin(alpha), 0);
        vec3 top = base + vec3(0.0f, 0.0f, base[2] + height);

        alpha += increment;

        m.position.push_back(base);
        m.position.push_back(top);

        /*m.connectivity.push_back(uint3{ i, i + 1, i + 2 * (i % 2)});
        m.connectivity.push_back(uint3{ (n + i + 2) % (2 * n), (n + i + 1) % (2 * n), n + i });*/

        m.connectivity.push_back(uint3{ (i + 2) % n, i + 1, i});
        m.connectivity.push_back(uint3{ (i + 2) % n, (i + 3) % (n), i + 1 });
        
    }


    m.fill_empty_field();

    return m;
}

mesh create_cone_mesh(float radius, float height, float z_offset)
{
    mesh m; 
    // To do: fill this mesh ...

    int n = 8; //nombre de points
    float alpha = 0.0f;
    float increment = 2 * Pi / n;

    //add vertice index 0
    m.position.push_back(vec3(0.0f, 0.0f, z_offset + height));

    for (int i = 1; i <= n; i++) {
        m.position.push_back(vec3(radius * cos(alpha), radius * sin(alpha), z_offset));
        alpha += increment;
        m.connectivity.push_back(uint3{ 0, i, i + 1 });
        m.connectivity.push_back(uint3{ n + 1, i, i + 1 });
    }

    //add the last triangles
    m.connectivity.push_back(uint3{ n + 1, n, 1 });
    m.connectivity.push_back(uint3{ 0    , n, 1 });

    //add base center at index n + 1
    m.position.push_back(vec3(0.0f, 0.0f, z_offset));

    m.fill_empty_field();

    return m;
}

mesh create_tree()
{
    float h = 0.7f; // trunk height
    float r = 0.1f; // trunk radius

    // Create a brown trunk
    mesh trunk = create_cylinder_mesh(r, h);
    trunk.color.fill({ 0.4f, 0.3f, 0.3f });


    // Create a green foliage from 3 cones
    mesh foliage = create_cone_mesh(4 * r, 6 * r, 0.0f);      // base-cone
    foliage.push_back(create_cone_mesh(4 * r, 6 * r, 2 * r));   // middle-cone
    foliage.push_back(create_cone_mesh(4 * r, 6 * r, 4 * r));   // top-cone
    foliage.apply_translation_to_position({ 0,0,h });       // place foliage at the top of the trunk
    foliage.color.fill({ 0.4f, 0.6f, 0.3f });

    // The tree is composed of the trunk and the foliage
    mesh tree = trunk;
    tree.push_back(foliage);

    return tree;
}

mesh create_mushroom()
{
    float h = 0.07f; // trunk height
    float r = 0.03f; // trunk radius

    // Create a brown trunk
    mesh trunk = create_cylinder_mesh(r, h);
    trunk.color.fill({ 0.4f, 0.3f, 0.3f });


    // Create a green foliage from 3 cones
    mesh foliage = create_cone_mesh(4 * r, 3 * r, 0.0f);      // cone
    foliage.apply_translation_to_position({ 0,0,h });       // place foliage at the top of the trunk
    foliage.color.fill({ 0.8f, 0.2f, 0.2f });

    // The tree is composed of the trunk and the foliage
    mesh mushroom = trunk;
    mushroom.push_back(foliage);

    return mushroom;
}

