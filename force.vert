#version 150

uniform samplerBuffer table;
out vec4 position;
const float l = 0.1;
const float k = 0.1;

void main(void)
{
  vec4 p0 = texelFetch(table, gl_VertexID);

  if (p0.w > 0.0)
  {
    vec2 p1 = texelFetch(table, gl_VertexID - 1).xy;
    vec2 p2 = texelFetch(table, gl_VertexID + 1).xy;
    vec2 v1 = p1 - p0.xy;
    vec2 v2 = p2 - p0.xy;
    vec2 f1 = (length(v1) - l) * k * normalize(v1);
    vec2 f2 = (length(v2) - l) * k * normalize(v2);
    vec2 f = f1 + f2;

    p0 += vec4(f, 0.0, 0.0);
  }
  position = p0;
}
