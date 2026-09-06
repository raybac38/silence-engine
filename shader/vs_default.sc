$input a_position, a_color0
$output v_color0
#include <bgfx_shader.sh>

uniform vec4 u_time;

void main()
{
    vec3 pos = a_position;

    // u_modelViewProj[0][3] change quand la caméra ou l'objet bouge !
    float pseudoTime = u_modelViewProj[0][3]; 
    float wave = sin(pos.x * 2.0 + pseudoTime) * 0.5;
    pos.y += wave;

    gl_Position = mul(u_modelViewProj, vec4(pos, 1.0));
    v_color0 = a_color0;
}