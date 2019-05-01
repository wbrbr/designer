#version 430

layout (local_size_x = 16, local_size_y = 16) in;

layout (r32f, binding = 0) uniform image2D img_input;
layout (rgba32f, binding = 1) uniform image2D img_output;

uniform vec3 tint;

void main() {
  ivec2 coords = ivec2(gl_GlobalInvocationID);

  float val = imageLoad(img_input, coords).r;

  vec4 pixel = vec4(val * tint, 1.0);
  imageStore(img_output, coords, pixel);
}
