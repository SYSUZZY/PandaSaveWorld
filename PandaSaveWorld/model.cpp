#include "model.h"

Model::Model(string const &path, bool gamma) : gammaCorrection(gamma) {
	loadModel(path);
}


void Model::Draw(Shader shader) {
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}


void Model::loadModel(string const &path) {
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) { // if is Not Zero 
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
	processAnimation(scene);
}


void Model::processNode(aiNode *node, const aiScene *scene) {

	vector<Node> childs;

	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++) {

		Node tempChildNode;
		memcpy(tempChildNode.name, node->mChildren[i]->mName.C_Str(), node->mChildren[i]->mName.length + 1);
		aiMatrix4x4 tempMatrix = node->mChildren[i]->mTransformation;
		tempChildNode.transformation = glm::mat4(tempMatrix.a1, tempMatrix.b1, tempMatrix.c1, tempMatrix.d1,
			tempMatrix.a2, tempMatrix.b2, tempMatrix.c2, tempMatrix.d2,
			tempMatrix.a3, tempMatrix.b3, tempMatrix.c3, tempMatrix.d3,
			tempMatrix.a4, tempMatrix.b4, tempMatrix.c4, tempMatrix.d4);
		memcpy(tempChildNode.parentName, node->mName.C_Str(), node->mName.length + 1);

		childs.push_back(tempChildNode);

		processNode(node->mChildren[i], scene);
	}

	Node tempNode; 
	memcpy(tempNode.name, node->mName.C_Str(), node->mName.length + 1);
	aiMatrix4x4 tempMatrix = node->mTransformation;
	tempNode.transformation = glm::mat4(tempMatrix.a1, tempMatrix.b1, tempMatrix.c1, tempMatrix.d1,
		tempMatrix.a2, tempMatrix.b2, tempMatrix.c2, tempMatrix.d2,
		tempMatrix.a3, tempMatrix.b3, tempMatrix.c3, tempMatrix.d3,
		tempMatrix.a4, tempMatrix.b4, tempMatrix.c4, tempMatrix.d4);
	if (node->mParent)
		memcpy(tempNode.parentName, node->mParent->mName.C_Str(), node->mParent->mName.length + 1);
	else
		memcpy(tempNode.parentName, "", 1);

	nodes.push_back(tempNode);
	nodepairs[tempNode.name] = (make_pair(tempNode, childs));
}


Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
	// data to fill
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	vector<Bone> bones;

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
						  // positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) { // does the mesh contain texture coordinates?
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		// tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;
		// bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;
		// weight
		int currentbone = 0;
		for (unsigned int boneindex = 0; boneindex < mesh->mNumBones; boneindex++) {
			aiBone* bone = mesh->mBones[boneindex];
			for (unsigned int weightindex = 0; weightindex < bone->mNumWeights; weightindex++) {
				if (mesh->mBones[boneindex]->mWeights[weightindex].mVertexId == i) {
					Weight weight;
					weight.boneid = boneindex;
					weight.weight = mesh->mBones[boneindex]->mWeights[weightindex].mWeight;
					if (currentbone == VERTEX_MAX_BONE) {
						cout << "Error: " << "bone count > " << VERTEX_MAX_BONE << endl;
						getchar();
					}
					vertex.Weights[currentbone++] = weight;
				}
			}
		}

		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	//Process bones;  
	for (size_t boneindex = 0; boneindex < mesh->mNumBones; boneindex++) {
		Bone bone;
		aiBone* bonesrc = mesh->mBones[boneindex];
		memcpy(bone.name, bonesrc->mName.C_Str(), bonesrc->mName.length + 1);
		bone.offsetMatrix = glm::mat4(bonesrc->mOffsetMatrix.a1, bonesrc->mOffsetMatrix.b1, bonesrc->mOffsetMatrix.c1, bonesrc->mOffsetMatrix.d1,
			bonesrc->mOffsetMatrix.a2, bonesrc->mOffsetMatrix.b2, bonesrc->mOffsetMatrix.c2, bonesrc->mOffsetMatrix.d2,
			bonesrc->mOffsetMatrix.a3, bonesrc->mOffsetMatrix.b3, bonesrc->mOffsetMatrix.c3, bonesrc->mOffsetMatrix.d3,
			bonesrc->mOffsetMatrix.a4, bonesrc->mOffsetMatrix.b4, bonesrc->mOffsetMatrix.c4, bonesrc->mOffsetMatrix.d4);
		bones.push_back(bone);
	}

	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures, bones);
}

// 处理所有的Animation;  
void Model::processAnimation(const aiScene* scene) {
	for (size_t animationindex = 0; animationindex < scene->mNumAnimations; animationindex++) {
		Animation animation;
		aiAnimation* animationsrc = scene->mAnimations[animationindex];

		//Animation 名字;  
		memcpy(animation.name, animationsrc->mName.C_Str(), animationsrc->mName.length + 1);
		animation.duration = animationsrc->mDuration;
		animation.ticksPerSecond = animationsrc->mTicksPerSecond;
		animation.numChannels = animationsrc->mNumChannels;

		//处理这个Animation下的所有的Channel(一个joint的动画集合);  
		for (size_t channelindex = 0; channelindex < animationsrc->mNumChannels; channelindex++) {
			AnimationChannel animationChannel;
			aiNodeAnim* channel = animationsrc->mChannels[channelindex];
			memcpy(animationChannel.nodeName, channel->mNodeName.C_Str(), channel->mNodeName.length);

			//位移动画;  
			animationChannel.numPositionKeys = channel->mNumPositionKeys;
			for (size_t positionkeyindex = 0; positionkeyindex < channel->mNumPositionKeys; positionkeyindex++) {
				AnimationChannelKeyVec3 animationChannelKey;
				aiVectorKey vectorKey = channel->mPositionKeys[positionkeyindex];
				animationChannelKey.time = vectorKey.mTime;
				animationChannelKey.keyData.x = vectorKey.mValue.x;
				animationChannelKey.keyData.y = vectorKey.mValue.y;
				animationChannelKey.keyData.z = vectorKey.mValue.z;

				animationChannel.positionKeys.push_back(animationChannelKey);
			}

			//旋转动画;  
			animationChannel.numRotationKeys = channel->mNumRotationKeys;
			for (size_t rotationkeyindex = 0; rotationkeyindex < channel->mNumRotationKeys; rotationkeyindex++) {
				AnimationChannelKeyQuat animationChannelKey;
				aiQuatKey quatKey = channel->mRotationKeys[rotationkeyindex];

				animationChannelKey.time = quatKey.mTime;
				animationChannelKey.keyData.x = quatKey.mValue.x;
				animationChannelKey.keyData.y = quatKey.mValue.y;
				animationChannelKey.keyData.z = quatKey.mValue.z;
				animationChannelKey.keyData.w = quatKey.mValue.w;

				animationChannel.rotationKeys.push_back(animationChannelKey);
			}

			//缩放动画;  
			animationChannel.numScalingKeys = channel->mNumScalingKeys;
			for (size_t scalingindex = 0; scalingindex < channel->mNumScalingKeys; scalingindex++) {
				AnimationChannelKeyVec3 animationChannelKey;
				aiVectorKey vectorKey = channel->mScalingKeys[scalingindex];

				animationChannelKey.time = vectorKey.mTime;
				animationChannelKey.keyData.x = vectorKey.mValue.x;
				animationChannelKey.keyData.y = vectorKey.mValue.y;
				animationChannelKey.keyData.z = vectorKey.mValue.z;

				animationChannel.scalingKeys.push_back(animationChannelKey);
			}
			animation.channels.push_back(animationChannel);
		}
		animations.push_back(animation);
	}
}

//从RootNode开始;
void Model::TransformNode(const char* nodename, int framecount, glm::mat4& parenttransform) {
	glm::mat4 GlobalTransformation;
	AnimationChannel animationChannel;
	Bone bone;
	memset(bone.name, NULL, sizeof(bone.name));
	std::pair<Node, std::vector<Node>> tempNode = nodepairs[nodename];
	glm::mat4 nodeTransformation(tempNode.first.transformation);

	//找到和node同名的AnimationChannel;
	for (int animationchannelindex = 0; animationchannelindex < animations[0].numChannels; animationchannelindex++) {
		if (strcmp(animations[0].channels[animationchannelindex].nodeName, nodename) == 0) {
			animationChannel = animations[0].channels[animationchannelindex];

			//对AnimationChannel中的 Rotation Scaling Translate 进行插值(暂时还没有插值……);
			//先直接用当前帧获取到对应的数据 用着。正确的应该是判断游戏时间和Animation时间的。
			int rotationkeyindex = fmod(framecount, animationChannel.numRotationKeys);
			AnimationChannelKeyQuat rotationkey = animationChannel.rotationKeys[rotationkeyindex];

			int scalingkeyindex = fmod(framecount, animationChannel.numScalingKeys);
			AnimationChannelKeyVec3 scalingkey = animationChannel.scalingKeys[scalingkeyindex];

			int positionkeyindex = fmod(framecount, animationChannel.numPositionKeys);
			AnimationChannelKeyVec3 positionKey = animationChannel.positionKeys[positionkeyindex];

			glm::mat4 rotationM = glm::mat4_cast(rotationkey.keyData);
			glm::mat4 scalingM;
			glm::scale(scalingM, scalingkey.keyData);
			glm::mat4 translateM;
			glm::translate(translateM, positionKey.keyData);

			nodeTransformation = translateM*rotationM*scalingM;

			break;
		}
	}
	GlobalTransformation = parenttransform*nodeTransformation;

	//找到同名的Bone;
	for (size_t meshindex = 0; meshindex < this->meshes.size(); meshindex++) {
		for (size_t boneindex = 0; boneindex < this->meshes[meshindex].bones.size(); boneindex++) {
			bone = this->meshes[meshindex].bones[boneindex];
			if (strcmp(bone.name, nodename) == 0) {
				bone.finalMatrix = globalInverseTransform*GlobalTransformation* bone.offsetMatrix;
				this->meshes[meshindex].bones[boneindex] = bone;
				break;
			}
		}
	}


	//更新child;
	for (size_t childindex = 0; childindex < tempNode.second.size(); childindex++) {
		if (strcmp(bone.name, "") == 0) {
			glm::mat4 Identity;
			TransformNode(tempNode.second[childindex].name, framecount, Identity);
		}
		else {
			TransformNode(tempNode.second[childindex].name, framecount, GlobalTransformation);
		}
	}
}


void Model::OnDraw() {
	framecount++;

	Node rootNode;
	for (size_t nodeindex = 0; nodeindex < nodes.size(); nodeindex++) {
		Node node = nodes[nodeindex];
		if (strcmp(node.parentName, "") == 0) {
			rootNode = node;
			break;
		}
	}

	globalInverseTransform = rootNode.transformation;
	globalInverseTransform = glm::inverse(globalInverseTransform);

	glm::mat4 identity;
	glm::mat4 rootnodetransform;
	TransformNode(rootNode.name, framecount, identity * rootnodetransform);

	//更新Vertex Position;
	for (size_t meshindex = 0; meshindex < this->meshes.size(); meshindex++) {
		for (size_t vertexindex = 0; vertexindex < meshes[meshindex].vertices.size(); vertexindex++) {
			Vertex vertex = meshes[meshindex].vertices[vertexindex];

			glm::mat4 boneTransform;

			//计算权重;
			for (int weightindex = 0; weightindex < VERTEX_MAX_BONE; weightindex++) {
				Weight weight = vertex.Weights[weightindex];
				Bone bone = this->meshes[meshindex].bones[weight.boneid];
				boneTransform += bone.finalMatrix * weight.weight;
			}

			glm::vec4 animPosition(vertex.Position, 1.0f);
			animPosition = boneTransform * animPosition;

			vertex.animPosition = glm::vec3(animPosition);
			meshes[meshindex].vertices[vertexindex] = vertex;
		}
	}
	
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip) {   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}


unsigned int TextureFromFile(const char *path, const string &directory, bool gamma) {
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}