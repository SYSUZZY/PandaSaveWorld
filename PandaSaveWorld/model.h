#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "stb_image.h"
#include "shader.h"
#include "Animation.h"
#include "Node.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model {
public:
	/*  Node  */
	vector<Node> nodes;

	/*  Model Data */
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh> meshes;

	/* Animation Data */
	vector<Animation> animations;

	string directory;
	bool gammaCorrection;
	int framecount = 0;

	/*  Functions   */
	// constructor, expects a filepath to a 3D model.
	Model(string const &path, bool gamma = false);

	// draws the model, and thus all its meshes
	void Draw(Shader shader);

	void OnDraw();

private:
	/*  Functions   */
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(string const &path);

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode *node, const aiScene *scene);

	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

	// processAnimation
	void processAnimation(const aiScene* scene);

	

	void TransformNode(const char* nodename, int framecount, glm::mat4& parenttransform);

	/*  nodepairs node's child  */
	std::vector<std::pair<Node, std::vector<Node>>> nodepairs;

	glm::mat4 globalInverseTransform;

	vector<glm::mat4> transforms;
};
#endif