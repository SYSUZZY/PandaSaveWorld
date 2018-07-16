#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "Bone.h"
#include "Weight.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	glm::vec3 animPosition;
	unsigned int boneIDs[VERTEX_MAX_BONE];
	float weights[VERTEX_MAX_BONE];
	//Weight Weights[VERTEX_MAX_BONE];  //限定每个顶点受 VERTEX_MAX_BONE 个骨骼影响;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	vector<Bone> bones;
	unsigned int VAO;

	vector<glm::mat4> transform;

	/*  Functions  */
	// constructor
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, vector<Bone> bones);

	// render the mesh
	void Draw(Shader shader);
	void DrawScene(Shader shader, unsigned int id);
	void shadowDebug(Shader shader, unsigned int id);

	void updateMesh();

private:
	/*  Render data  */
	unsigned int VBO, EBO;

	/*  Functions    */
	// initializes all the buffer objects/arrays
	void setupMesh();
};
#endif#pragma once
