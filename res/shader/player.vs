#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec3 aAnimPosition;
layout (location = 6) in ivec4 aBoneIDs;
layout (location = 7) in vec4 aWeights;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;

uniform mat4 gBones[MAX_BONES];

void main()
{
    TexCoords = aTexCoords;

    mat4 BoneTransform;
    BoneTransform = gBones[aBoneIDs[0]]*aWeights[0];
    BoneTransform += gBones[aBoneIDs[1]]*aWeights[1];
    BoneTransform += gBones[aBoneIDs[2]]*aWeights[2];
    BoneTransform += gBones[aBoneIDs[3]];
    
    vec4 PosL = BoneTransform * vec4(aPos, 1.0);
    gl_Position = projection * view * model * PosL;
}