$input v_normal, v_texcoord0

#include <bgfx_shader.sh>

void main()
{
    // Directionnel basique pour visualiser la forme 3D
    vec3 lightDir = normalize(vec3(0.5, 1.0, -0.5));
    float diff = max(dot(normalize(v_normal), lightDir), 0.2);
    
    gl_FragColor = vec4(vec3_splat(diff), 1.0); // Gris éclairé
}