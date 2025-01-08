#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec4 vCol;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

// NEW: for sprite animation
uniform float currentFrame;
uniform float numFrames;

// NEW: Flip sprite horizontally
uniform bool flipHorizontally;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	
	// Calculate how wide each frame is (1 / numFrames)
    float uWidth = 1.0f / numFrames;
    // Starting U offset for the current frame
    float uOffset = currentFrame * uWidth;

    // Adjust tex.x based on flipHorizontally
    float texX = flipHorizontally ? (1.0 - tex.x) : tex.x;

    // Original UV is [tex.x, tex.y]. We scale and shift .x
    TexCoord = vec2(texX * uWidth + uOffset, tex.y);
}