#pragma once
#include "horse.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"


void Horse::makeUV() {
	int cubeNumVertex = 36;
	int numCubes = 17;
	int numVertex = cubeNumVertex * numCubes;

	frontLegPos[0] = glm::vec3(0.20, 0.1, -0.20); // rear right
	frontLegPos[1] = glm::vec3(0.20, -0.1, -0.20); // rear left
	backLegPos[0] = glm::vec3(-0.20, 0.1, -0.20); // front right
	backLegPos[1] = glm::vec3(-0.20, -0.1, -0.20); // front left

	ear[0] = glm::vec3(0.35, -0.025, 0.3);
	ear[1] = glm::vec3(0.35, 0.025, 0.3);

	cube();
}

void Horse::move(int time) {

	//clear vertices, normals
	verts.clear();
	vector<glm::vec4>().swap(verts);
	normals.clear();
	vector<glm::vec4>().swap(normals);
	texCoords.clear();
	vector<glm::vec2>().swap(texCoords);

	glm::mat4 m4(1.0f); //identity matrix
	calculateAngles(time);
	glm::mat4 modelMat;

	//body
	modelMat = glm::scale(glm::vec3(0.5, 0.2, 0.2));
	pushTransformedModel(modelMat);

	int dir = 1;	//leg move direction

	const float fLength1 = 0.24;
	const float fLength2 = 0.08;
	const float fLength3 = 0.08;

	//front leg
	for (int i = 0; i < 2; i++) {

		//translate each bone to position
		frontLegMat_1[i] = glm::translate(m4, frontLegPos[i]);
		frontLegMat_2[i] = glm::translate(m4, frontLegPos[i]);
		frontLegMat_3[i] = glm::translate(m4, frontLegPos[i]);

		frontLegMat_2[i] = glm::translate(frontLegMat_2[i], glm::vec3(0, 0, -(fLength1 + fLength2) / 2));
		frontLegMat_3[i] = glm::translate(frontLegMat_3[i], glm::vec3(0, 0, -((fLength1 + fLength3) / 2 + fLength2)));

		//rotate first joint
		frontLegMat_1[i] = glm::rotate(frontLegMat_1[i], legAngle * dir, glm::vec3(0, 1, 0));
		frontLegMat_2[i] = glm::rotate(frontLegMat_2[i], legAngle * dir, glm::vec3(0, 1, 0));
		frontLegMat_3[i] = glm::rotate(frontLegMat_3[i], legAngle * dir, glm::vec3(0, 1, 0));

		//translate after rotate
		//다리 회전에 따른 위치 보정 
		frontLegMat_1[i] = glm::translate(frontLegMat_1[i], glm::vec3(-fLength1 * glm::sin(legAngle) / 2 * dir, 0, 0));
		frontLegMat_2[i] = glm::translate(frontLegMat_2[i], glm::vec3(-(fLength2 / 2 + fLength1) * glm::sin(legAngle) * dir, 0, 0));
		frontLegMat_3[i] = glm::translate(frontLegMat_3[i], glm::vec3(-(fLength3 / 2 + fLength1 + fLength2) * glm::sin(legAngle) * dir, 0, 0));

		//joint 2
		frontLegMat_2[i] = glm::rotate(frontLegMat_2[i], frontJointAngle1[i], glm::vec3(0, 1, 0));
		frontLegMat_3[i] = glm::rotate(frontLegMat_3[i], frontJointAngle1[i], glm::vec3(0, 1, 0));

		frontLegMat_2[i] = glm::translate(frontLegMat_2[i], glm::vec3(-(fLength2 / 2) * glm::sin(frontJointAngle1[i]), 0, 0));
		frontLegMat_3[i] = glm::translate(frontLegMat_3[i], glm::vec3(-(fLength3 / 2 + fLength2) * glm::sin(frontJointAngle1[i]), 0, 0));

		//joint 3
		frontLegMat_3[i] = glm::rotate(frontLegMat_3[i], frontJointAngle2[i], glm::vec3(0, 1, 0));
		frontLegMat_3[i] = glm::translate(frontLegMat_3[i], glm::vec3(-(fLength3 / 2) * glm::sin(frontJointAngle2[i]), 0, 0));

		//scale each bone
		frontLegMat_1[i] = glm::scale(frontLegMat_1[i], glm::vec3(0.1, 0.05, fLength1));
		frontLegMat_2[i] = glm::scale(frontLegMat_2[i], glm::vec3(0.1, 0.05, fLength2));
		frontLegMat_3[i] = glm::scale(frontLegMat_3[i], glm::vec3(0.1, 0.05, fLength3));

		pushTransformedModel(frontLegMat_1[i]);
		pushTransformedModel(frontLegMat_2[i]);
		pushTransformedModel(frontLegMat_3[i]);

		dir *= -1;
	}
	//back leg
	const float bLength1 = 0.28;
	const float bLength2 = 0.12;
	const float bLength3 = 0.06;
	for (int i = 0; i < 2; i++) {

		//translate each bone to position
		backLegMat_1[i] = glm::translate(m4, backLegPos[i]);
		backLegMat_2[i] = glm::translate(m4, backLegPos[i]);
		backLegMat_3[i] = glm::translate(m4, backLegPos[i]);
		backLegMat_2[i] = glm::translate(backLegMat_2[i], glm::vec3(0, 0, -(bLength1 + bLength2) / 2));
		backLegMat_3[i] = glm::translate(backLegMat_3[i], glm::vec3(0, 0, -((bLength1 + bLength3) / 2 + bLength2)));

		//rotate first joint
		backLegMat_1[i] = glm::rotate(backLegMat_1[i], backJointAngle[i], glm::vec3(0, 1, 0));
		backLegMat_2[i] = glm::rotate(backLegMat_2[i], backJointAngle[i], glm::vec3(0, 1, 0));
		backLegMat_3[i] = glm::rotate(backLegMat_3[i], backJointAngle[i], glm::vec3(0, 1, 0));

		//translate after rotate
		//다리 회전에 따른 위치 보정 
		backLegMat_1[i] = glm::translate(backLegMat_1[i], glm::vec3(-bLength1 / 2 * glm::sin(backJointAngle[i]), 0, 0));
		backLegMat_2[i] = glm::translate(backLegMat_2[i], glm::vec3(-(bLength2 / 2 + bLength1) * glm::sin(backJointAngle[i]), 0, 0));
		backLegMat_3[i] = glm::translate(backLegMat_3[i], glm::vec3(-(bLength3 / 2 + bLength1 + bLength2) * glm::sin(backJointAngle[i]), 0, 0));

		//joint 2
		backLegMat_2[i] = glm::rotate(backLegMat_2[i], backJointAngle1[i], glm::vec3(0, 1, 0));
		backLegMat_3[i] = glm::rotate(backLegMat_3[i], backJointAngle1[i], glm::vec3(0, 1, 0));
		backLegMat_2[i] = glm::translate(backLegMat_2[i], glm::vec3(-(bLength2 / 2) * glm::sin(backJointAngle1[i]), 0, 0));
		backLegMat_3[i] = glm::translate(backLegMat_3[i], glm::vec3(-(bLength3 / 2 + bLength2) * glm::sin(backJointAngle1[i]), 0, 0));

		//joint 3
		backLegMat_3[i] = glm::rotate(backLegMat_3[i], backJointAngle2[i], glm::vec3(0, 1, 0));
		backLegMat_3[i] = glm::translate(backLegMat_3[i], glm::vec3(-(bLength3 / 2) * glm::sin(backJointAngle2[i]), 0, 0));

		//scale each bone
		backLegMat_1[i] = glm::scale(backLegMat_1[i], glm::vec3(0.1, 0.05, bLength1));
		backLegMat_2[i] = glm::scale(backLegMat_2[i], glm::vec3(0.1, 0.05, bLength2));
		backLegMat_3[i] = glm::scale(backLegMat_3[i], glm::vec3(0.1, 0.05, bLength3));

		pushTransformedModel(backLegMat_1[i]);
		pushTransformedModel(backLegMat_2[i]);
		pushTransformedModel(backLegMat_3[i]);

		dir *= -1;
	}

	//neck
	modelMat = glm::rotate(modelMat, neckAngle, glm::vec3(0, 1, 0));
	modelMat = glm::translate(m4, glm::vec3(0.25, 0, 0.1));
	modelMat = glm::translate(modelMat, glm::vec3((0.125 * sin(neckAngle)), 0, -(0.125 * (1 - cos(neckAngle)))));
	modelMat = glm::rotate(modelMat, -glm::radians(45.0f), glm::vec3(0, 1, 0));
	modelMat = glm::scale(modelMat, glm::vec3(0.3, 0.09, 0.18));
	pushTransformedModel(modelMat);

	//head
	modelMat = glm::translate(m4, glm::vec3(0.39, 0, 0.15));
	modelMat = glm::rotate(modelMat, glm::radians(45.0f), glm::vec3(0, 1, 0));
	modelMat = glm::rotate(modelMat, neckAngle, glm::vec3(0, 1, 0));
	modelMat = glm::translate(modelMat, glm::vec3((0.2 * sin(neckAngle)), 0, -(0.2 * (1 - cos(neckAngle)))));
	modelMat = glm::scale(modelMat, glm::vec3(0.3, 0.10, 0.15));
	pushTransformedModel(modelMat);


	//ear
	for (int i = 0; i < 2; i++) {
		modelMat = glm::translate(m4, ear[i]);
		modelMat = glm::translate(modelMat, glm::vec3((0.25 * sin(neckAngle)), 0, -(0.35 * (1 - cos(neckAngle)))));
		modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(0, 1, 0));
		modelMat = glm::rotate(modelMat, neckAngle, glm::vec3(0, 1, 0));
		modelMat = glm::scale(modelMat, glm::vec3(0.08, 0.04, 0.03));
		pushTransformedModel(modelMat);
	}

	//tail
	modelMat = glm::translate(m4, glm::vec3(-0.3, 0, -0.025));
	modelMat = glm::translate(modelMat, glm::vec3(0, 0.5 * 0.3 * glm::sin(legAngle), 0)); // 움직임에 따른 위치보정
	modelMat = glm::rotate(modelMat, glm::radians(45.0f), glm::vec3(0, 1, 0));
	modelMat = glm::rotate(modelMat, legAngle, glm::vec3(1, 0, 0));
	modelMat = glm::scale(modelMat, glm::vec3(0.05, 0.05, 0.3));
	pushTransformedModel(modelMat);
}

void Horse::pushTransformedModel(glm::mat4 transMatrix) {

	for (int i = 0; i < 36; i++) {
		verts.push_back(transMatrix * baseModel[i]);
		normals.push_back(transMatrix * baseNormal[i]);
		texCoords.push_back(baseTexCoord[i]);
	}
}

void Horse::quad(int a, int b, int c, int d) {

	//add normal vector setting
	glm::vec4 vec_ab = vertices[b] - vertices[a];
	glm::vec4 vec_ac = vertices[c] - vertices[a];
	glm::vec3 normal_vec3 = glm::cross(glm::vec3(vec_ab), glm::vec3(vec_ac));
	glm::vec4 normal_vec4 = glm::vec4(normal_vec3, 0.0f);
	normal_vec4 = glm::normalize(normal_vec4);

	baseModel[Index] = vertices[a]; baseNormal[Index] = normal_vec4; baseTexCoord[Index][0] = 0.0f; baseTexCoord[Index][1] = 0.0f; Index++;
	baseModel[Index] = vertices[b]; baseNormal[Index] = normal_vec4; baseTexCoord[Index][0] = 1.0f; baseTexCoord[Index][1] = 0.0f; Index++;
	baseModel[Index] = vertices[c]; baseNormal[Index] = normal_vec4; baseTexCoord[Index][0] = 1.0f; baseTexCoord[Index][1] = 1.0f; Index++;
	baseModel[Index] = vertices[a]; baseNormal[Index] = normal_vec4; baseTexCoord[Index][0] = 0.0f; baseTexCoord[Index][1] = 0.0f; Index++;
	baseModel[Index] = vertices[c]; baseNormal[Index] = normal_vec4; baseTexCoord[Index][0] = 1.0f; baseTexCoord[Index][1] = 1.0f; Index++;
	baseModel[Index] = vertices[d]; baseNormal[Index] = normal_vec4; baseTexCoord[Index][0] = 0.0f; baseTexCoord[Index][1] = 1.0f; Index++;
}

void Horse::cube() {
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}

void Horse::calculateAngles(int time) {

	cycleTime = time;
	oppositeCycle = (cycleTime + (int)(moveCycle / 2)) % moveCycle;
	backCycleTime = (cycleTime + (int)(moveCycle / 8)) % moveCycle;
	int backOppositeCycle = (backCycleTime + (int)(moveCycle / 2)) % moveCycle;
	float maxLegRotation = glm::radians(15.0f);

	//neck angle
	if (cycleTime > 0 && cycleTime <= 2000) {
		neckAngle = glm::radians((cycleTime) * 7.5f / 1000.0f);
	}
	else if (cycleTime > 2000 && cycleTime <= 4000) {
		neckAngle = glm::radians(15 - (cycleTime - 2000) * 7.5f / 1000.0f);
	}

	//front leg joint 1
	if (cycleTime > 0 && cycleTime <= 1000) {
		frontJointAngle1[1] = glm::radians(45.0f - cycleTime * 45.0f / 1000.0f);
	}
	else if (cycleTime > 3000 && cycleTime <= 4000) {
		frontJointAngle1[1] = glm::radians((cycleTime - 3000) * 45.0f / 1000.0f);
	}
	if (oppositeCycle > 0 && oppositeCycle <= 1000) {
		frontJointAngle1[0] = glm::radians(45.0f - oppositeCycle * 45.0f / 1000.0f);
	}
	else if (oppositeCycle > 3000 && oppositeCycle <= 4000) {
		frontJointAngle1[0] = glm::radians((oppositeCycle - 3000) * 45.0f / 1000.0f);
	}

	//front leg joint 2
	if (cycleTime > 1000 && cycleTime <= 2000) {
		frontJointAngle2[1] = -glm::radians((cycleTime - 1000) * 45.0f / 1000.0f);
	}
	else if (cycleTime > 3000 && cycleTime <= 4000) {
		frontJointAngle2[1] = -glm::radians(45 - (cycleTime - 3000) * 45.0f / 1000.0f);
	}
	if (oppositeCycle > 1000 && oppositeCycle <= 2000) {
		frontJointAngle2[0] = -glm::radians((oppositeCycle - 1000) * 45.0f / 1000.0f);
	}
	else if (oppositeCycle > 3000 && oppositeCycle <= 4000) {
		frontJointAngle2[0] = -glm::radians(45 - (oppositeCycle - 3000) * 45.0f / 1000.0f);
	}

	//back leg joint 1
	if (backCycleTime > 0 && backCycleTime <= 1000) {
		backJointAngle1[1] = -glm::radians(45 - (backCycleTime) * 45.0f / 1000.0f);
	}
	else if (backCycleTime > 1000 && backCycleTime <= 1500) {
		backJointAngle1[1] = -glm::radians((backCycleTime - 1000) * 90.0f / 1000.0f);
	}
	if (backOppositeCycle > 0 && backOppositeCycle <= 1000) {
		backJointAngle1[0] = -glm::radians(45 - (backOppositeCycle) * 45.0f / 1000.0f);
	}
	else if (backOppositeCycle > 1000 && backOppositeCycle <= 1500) {
		backJointAngle1[0] = -glm::radians((backOppositeCycle - 1000) * 90.0f / 1000.0f);
	}

	//back leg joint 2
	if (backCycleTime > 500 && backCycleTime <= 1000) {
		backJointAngle2[1] = glm::radians(30 - (backCycleTime - 500) * 60.0f / 1000.0f);
	}
	else if (backCycleTime > 1000 && backCycleTime <= 1500) {
		backJointAngle2[1] = glm::radians((backCycleTime - 1000) * 60.0f / 1000.0f);
	}
	else if (backCycleTime > 3000 && backCycleTime <= 3500) {
		backJointAngle2[1] = glm::radians((backCycleTime - 3000) * 30.0f / 1000.0f);
	}
	if (backOppositeCycle > 500 && backOppositeCycle <= 1000) {
		backJointAngle2[0] = glm::radians(30 - (backOppositeCycle - 500) * 60.0f / 1000.0f);
	}
	else if (backOppositeCycle > 1000 && backOppositeCycle <= 3000) {
		backJointAngle2[0] = glm::radians((backOppositeCycle - 1000) * 15.0f / 1000.0f);
	}

	//back joint 0 
	if (backCycleTime > 0 && backCycleTime <= 1000) {
		backJointAngle[0] = glm::radians((backCycleTime) * 20.0f / 1000.0f);
		backJointAngle[1] = -backJointAngle[0];
	}
	else if (backCycleTime > 1000 && backCycleTime <= 3000) {
		backJointAngle[0] = glm::radians(20 - (backCycleTime - 1000) * 20.0f / 1000.0f);
		backJointAngle[1] = -backJointAngle[0];
	}
	else if (backCycleTime > 3000 && backCycleTime <= 4000) {
		backJointAngle[0] = glm::radians(-20 + (backCycleTime - 3000) * 20.0f / 1000.0f);
		backJointAngle[1] = -backJointAngle[0];
	}

	//front joint 0
	if (cycleTime > 0 && cycleTime <= 1000) {
		legAngle = glm::radians((cycleTime) * 15.0f / 1000.0f);
	}
	else if (cycleTime > 1000 && cycleTime <= 3000) {
		legAngle = glm::radians(15 - (cycleTime - 1000) * 15.0f / 1000.0f);
	}
	else if (cycleTime > 3000 && cycleTime <= 4000) {
		legAngle = glm::radians(-15 + (cycleTime - 3000) * 15.0f / 1000.0f);
	}
}