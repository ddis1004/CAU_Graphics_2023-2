//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include "cube.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

glm::mat4 projectMat;
glm::mat4 viewMat;

GLuint pvmMatrixID;

int isDrawingCar = 0;
int isRotate = 0;
int	rotationDirection = 0;
int isMove = 1;

float moveAngle = 0.0f;
float legAngle = 0.0f;
float rotAngles[3] = { 0.0f, 0.0f, 0.0f };
float backJointAngle[2];
float neckAngle;
float frontJointAngle1[2];
float frontJointAngle2[2];
float backJointAngle1[2];
float backJointAngle2[2];

typedef glm::vec4  color4;
typedef glm::vec4  point4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
color4 colors[NumVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
	point4(-0.5, -0.5, 0.5, 1.0),
	point4(-0.5, 0.5, 0.5, 1.0),
	point4(0.5, 0.5, 0.5, 1.0),
	point4(0.5, -0.5, 0.5, 1.0),
	point4(-0.5, -0.5, -0.5, 1.0),
	point4(-0.5, 0.5, -0.5, 1.0),
	point4(0.5, 0.5, -0.5, 1.0),
	point4(0.5, -0.5, -0.5, 1.0)
};

// RGBA colors
color4 vertex_colors[8] = {
	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(0.0, 1.0, 1.0, 1.0),   // cyan
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(1.0, 1.0, 1.0, 1.0),  // white
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(0.0, 0.0, 1.0, 1.0)  // blue
	
};
//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices
int Index = 0;
void
quad(int a, int b, int c, int d)
{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a];  Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b];  Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c];  Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a];  Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c];  Index++;
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d];  Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
	colorcube();

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("src/vshader.glsl", "src/fshader.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	pvmMatrixID = glGetUniformLocation(program, "mPVM");

	projectMat = glm::perspective(glm::radians(65.0f), 1.0f, 0.1f, 100.0f);
	viewMat = glm::lookAt(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

//----------------------------------------------------------------------------

void drawCar(glm::mat4 carMat)
{
	glm::mat4 modelMat, pvmMat;
	glm::vec3 wheelPos[4];

	wheelPos[0] = glm::vec3(0.3, 0.24, -0.1); // rear right
	wheelPos[1] = glm::vec3(0.3, -0.24, -0.1); // rear left
	wheelPos[2] = glm::vec3(-0.3, 0.24, -0.1); // front right
	wheelPos[3] = glm::vec3(-0.3, -0.24, -0.1); // front left

	// car body
	modelMat = glm::scale(carMat, glm::vec3(1, 0.6, 0.2));
	pvmMat = projectMat * viewMat * modelMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// car top
	modelMat = glm::translate(carMat, glm::vec3(0, 0, 0.2));  //P*V*C*T*S*v
	modelMat = glm::scale(modelMat, glm::vec3(0.5, 0.6, 0.2));
	pvmMat = projectMat * viewMat * modelMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// car wheel
	for (int i = 0; i < 4; i++)
	{
		modelMat = glm::translate(carMat, wheelPos[i]);  //P*V*C*T*S*v
		modelMat = glm::scale(modelMat, glm::vec3(0.2, 0.1, 0.2));
		modelMat = glm::rotate(modelMat, -moveAngle*50.0f, glm::vec3(0, 1, 0));
		pvmMat = projectMat * viewMat * modelMat;
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}
}

void drawAnimal(glm::mat4 animalMat) {
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

	frontLegPos[0] = glm::vec3(0.20, 0.1, -0.20); // rear right
	frontLegPos[1] = glm::vec3(0.20, -0.1, -0.20); // rear left
	backLegPos[0] = glm::vec3(-0.20, 0.1, -0.20); // front right
	backLegPos[1] = glm::vec3(-0.20, -0.1, -0.20); // front left

	ear[0] = glm::vec3(0.35, -0.025, 0.3);
	ear[1] = glm::vec3(0.35, 0.025, 0.3);

	//body
	modelMat = glm::scale(animalMat, glm::vec3(0.5, 0.2, 0.2));
	pvmMat = projectMat * viewMat * modelMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	int legDirection = 1; //양 쪽 다리가 교차해서 움직이도록 하기위한 변수

	int dir = 1;	//leg move direction

	const float fLength1 = 0.24;
	const float fLength2 = 0.08;
	const float fLength3 = 0.08;

	//front leg
	for (int i = 0; i < 2; i++) {
		//translate each bone to position
		frontLegMat_1[i] = glm::translate(animalMat, frontLegPos[i]);
		frontLegMat_2[i] = glm::translate(animalMat, frontLegPos[i]);
		frontLegMat_3[i] = glm::translate(animalMat, frontLegPos[i]);
		frontLegMat_2[i] = glm::translate(frontLegMat_2[i], glm::vec3(0, 0, -(fLength1 + fLength2) / 2));
		frontLegMat_3[i] = glm::translate(frontLegMat_3[i], glm::vec3(0, 0, -((fLength1 + fLength3) / 2  + fLength2)));

		//rotate first joint
		frontLegMat_1[i] = glm::rotate(frontLegMat_1[i], legAngle*dir, glm::vec3(0, 1, 0));
		frontLegMat_2[i] = glm::rotate(frontLegMat_2[i], legAngle*dir, glm::vec3(0, 1, 0));
		frontLegMat_3[i] = glm::rotate(frontLegMat_3[i], legAngle*dir, glm::vec3(0, 1, 0));

		//translate after rotate
		//다리 회전에 따른 위치 보정 
		frontLegMat_1[i] = glm::translate(frontLegMat_1[i], glm::vec3(-fLength1 * glm::sin(legAngle)/2 * dir, 0, 0));	
		frontLegMat_2[i] = glm::translate(frontLegMat_2[i], glm::vec3(-(fLength2/2 + fLength1) * glm::sin(legAngle) * dir, 0, 0));
		frontLegMat_3[i] = glm::translate(frontLegMat_3[i], glm::vec3(-(fLength3/2 + fLength1 + fLength2) * glm::sin(legAngle) * dir, 0, 0));

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


		pvmMat = projectMat * viewMat * frontLegMat_1[i];
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);

		pvmMat = projectMat * viewMat * frontLegMat_2[i];
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);

		pvmMat = projectMat * viewMat * frontLegMat_3[i];
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
		
		dir *= -1;
	}

	//back leg
	const float bLength1 = 0.28;
	const float bLength2 = 0.12;
	const float bLength3 = 0.06;
	for (int i = 0; i < 2; i++) {
		
		//translate each bone to position
		backLegMat_1[i] = glm::translate(animalMat, backLegPos[i]);
		backLegMat_2[i] = glm::translate(animalMat, backLegPos[i]);
		backLegMat_3[i] = glm::translate(animalMat, backLegPos[i]);
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


		pvmMat = projectMat * viewMat * backLegMat_1[i];
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);

		pvmMat = projectMat * viewMat * backLegMat_2[i];
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);

		pvmMat = projectMat * viewMat * backLegMat_3[i];
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);

		dir *= -1;
	}

	//neck
	modelMat = glm::translate(animalMat, glm::vec3(0.25, 0, 0.1));
	modelMat = glm::rotate(modelMat, -glm::radians(45.0f), glm::vec3(0, 1, 0));
	modelMat = glm::rotate(modelMat, neckAngle, glm::vec3(0, 1, 0));
	modelMat = glm::translate(modelMat, glm::vec3((0.125 * sin(neckAngle)), 0, -(0.125 * (1 - cos(neckAngle)))));
	modelMat = glm::scale(modelMat, glm::vec3(0.3, 0.09, 0.18));
	pvmMat = projectMat * viewMat * modelMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//head
	modelMat = glm::translate(animalMat, glm::vec3(0.39, 0, 0.15));
	modelMat = glm::rotate(modelMat, glm::radians(45.0f), glm::vec3(0, 1, 0));
	modelMat = glm::rotate(modelMat, neckAngle, glm::vec3(0, 1, 0));
	modelMat = glm::translate(modelMat, glm::vec3((0.2 * sin(neckAngle)), 0, -(0.2 * (1 - cos(neckAngle)))));
	modelMat = glm::scale(modelMat, glm::vec3(0.3, 0.10, 0.15));
	pvmMat = projectMat * viewMat * modelMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);


	//ear
	for (int i = 0; i < 2; i++) {
		modelMat = glm::translate(animalMat, ear[i]);
		modelMat = glm::translate(modelMat, glm::vec3((0.25 * sin(neckAngle)), 0, -(0.35 * (1 - cos(neckAngle)))));
		modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(0, 1, 0));
		modelMat = glm::rotate(modelMat, neckAngle, glm::vec3(0, 1, 0));
		modelMat = glm::scale(modelMat, glm::vec3(0.08, 0.04, 0.03));
		pvmMat = projectMat * viewMat * modelMat;
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}

	//tail
	modelMat = glm::translate(animalMat, glm::vec3(-0.3, 0, -0.025));
	modelMat = glm::translate(modelMat, glm::vec3(0, 0.5 * 0.3 * glm::sin(legAngle), 0)); // 움직임에 따른 위치보정
	modelMat = glm::rotate(modelMat, glm::radians(45.0f), glm::vec3(0, 1, 0));
	modelMat = glm::rotate(modelMat, legAngle, glm::vec3(1, 0, 0));
	modelMat = glm::scale(modelMat, glm::vec3(0.05, 0.05, 0.3));
	pvmMat = projectMat * viewMat * modelMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices); 
}


void display(void)
{
	glm::mat4 pvmMat;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 worldMat = glm::mat4(1.0f);

	glm::vec3 rotationAxis[3];
	rotationAxis[0] = glm::vec3(0.0f, 1.0f, 0.0f);
	rotationAxis[1] = glm::vec3(1.0f, 0.0f, 0.0f);
	rotationAxis[2] = glm::vec3(0.0f, 0.0f, 1.0f);
	for (int i = 0; i < 3; i++) {
		worldMat = glm::rotate(worldMat, rotAngles[i], rotationAxis[i]);
	}

	if (isDrawingCar == 1){
		drawCar(worldMat);
	}
	else if (isDrawingCar == 2) {
		drawAnimal(worldMat);
	}
	else
	{
		pvmMat = projectMat * viewMat * worldMat;
		glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}

	glutSwapBuffers();
}

//----------------------------------------------------------------------------
int legMoveDirection = 1;
float maxLegRotation = glm::radians(15.0f);
static int moveCycle = 4000;
int cycleTime = 0;
int backCycleTime = 500;

void idle() {
	static int prevTime = glutGet(GLUT_ELAPSED_TIME);
	int currTime = glutGet(GLUT_ELAPSED_TIME);

	if (abs(currTime - prevTime) >= 20)
	{
		float t = abs(currTime - prevTime);

		if (isRotate) {
			rotAngles[rotationDirection] += glm::radians(t * 360.0f / 10000.0f);
		}
		if (isMove) {
			moveAngle += glm::radians(t * 720.0f / 10000.0f);

			cycleTime += abs(currTime - prevTime);
			backCycleTime += abs(currTime - prevTime);
			if (cycleTime > moveCycle)
				cycleTime = 0;
			if (backCycleTime > moveCycle)
				backCycleTime = 0;

			int oppositeCycle = (cycleTime + moveCycle / 2) % moveCycle;
			int backOppositeCycle = (backCycleTime + moveCycle / 2) % moveCycle;


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

			legAngle += legMoveDirection * glm::radians(t * 15.0f / 1000.0f);
			if (legAngle > maxLegRotation || -1 * legAngle > maxLegRotation)
				legMoveDirection *= -1;
		}
		prevTime = currTime;
		glutPostRedisplay();
	}
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'c': case 'C':
		++isDrawingCar;
		if (isDrawingCar > 2) {
			isDrawingCar = 0;
		}
		break;
	case 033:  // Escape key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 'r': case 'R':
		isRotate = !isRotate;
		break;
	case 't': case 'T':
		++rotationDirection;
		if (rotationDirection > 2) {
			rotationDirection = 0;
		}
		break;

	case 'm': case 'M':
		isMove = !isMove;
	}
}

//----------------------------------------------------------------------------

void resize(int w, int h)
{
	float ratio = (float)w / (float)h;
	glViewport(0, 0, w, h);

	projectMat = glm::perspective(glm::radians(65.0f), ratio, 0.1f, 100.0f);

	glutPostRedisplay();
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Color Car");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
