#version 430

layout (local_size_x = 16, local_size_y = 16) in;

layout (rgba32f, binding = 0) uniform image2D img_input;
layout (r32f, binding = 1) uniform image2D img_output;

void main() {
  ivec2 coords = ivec2(gl_GlobalInvocationID);

  vec4 col = imageLoad(img_input, coords);
  float val = (col.r + col.g + col.b) / 3.0;
  // Pour mettre en evidence. Les groupes de travail locaux on dessine un damier.
  vec4 pixel = vec4(val, 0.0, 0.0, 1.0);
  imageStore(img_output, coords, pixel);
}
