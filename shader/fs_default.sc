$input v_texcoord0, v_normal

#include <bgfx_shader.sh>

void main()
{
    vec3 lightDir = normalize(vec3(0.5, 1.0, -0.5));
    float diff = max(dot(normalize(v_normal), lightDir), 0.2);

    gl_FragColor = vec4(vec3_splat(diff), 1.0);
}