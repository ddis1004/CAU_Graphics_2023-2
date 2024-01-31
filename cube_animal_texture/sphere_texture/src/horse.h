#pragma once
#pragma once

#include <vector>
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

using namespace std;

class Horse {

public:
	vector<glm::vec4> verts;
	vector<glm::vec4> normals;
	vector<glm::vec2> texCoords;

	Horse() {
		makeUV(); // sets base model (cube)
		move(0); // pushes moved vertexes
	}

	int moveCycle = 4000;
	void move(int time);

private:
	int cycleTime;
	int backCycleTime;
	int oppositeCycle;

	float moveAngle = 0.0f;
	float legAngle = 0.0f;
	float legMoveDirection = 1;
	float backJointAngle[2];
	float neckAngle;
	float frontJointAngle1[2];
	float frontJointAngle2[2];
	float backJointAngle1[2];
	float backJointAngle2[2];

	glm::vec4 baseModel[36]; //1 x 1 x 1 cube 
	glm::vec4 baseNormal[36];
	glm::vec2 baseTexCoord[36];

	void calculateAngles(int time);
	void makeUV();
	void cube();
	void quad(int a, int b, int c, int d);
	void pushTransformedModel(glm::mat4 transMatrix);

	glm::mat4 modelMat, pvmMat;

	glm::vec3 frontLegPos[2];
	glm::vec3 backLegPos[2];

	glm::mat4 frontLegMat_1[2];
	glm::mat4 frontLegMat_2[2];
	glm::mat4 frontLegMat_3[2];

	glm::mat4 backLegMat_1[2];
	glm::mat4 backLegMat_2[2];
	glm::mat4 backLegMat_3[2];

	glm::vec3 ear[2];

	glm::vec4 vertices[8] = {
	glm::vec4(-0.5, -0.5, 0.5, 1.0),
	glm::vec4(-0.5, 0.5, 0.5, 1.0),
	glm::vec4(0.5, 0.5, 0.5, 1.0),
	glm::vec4(0.5, -0.5, 0.5, 1.0),
	glm::vec4(-0.5, -0.5, -0.5, 1.0),
	glm::vec4(-0.5, 0.5, -0.5, 1.0),
	glm::vec4(0.5, 0.5, -0.5, 1.0),
	glm::vec4(0.5, -0.5, -0.5, 1.0)
	};
	int Index = 0;
};