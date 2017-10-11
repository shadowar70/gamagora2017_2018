#define GLM_ENABLE_EXPERIMENTAL
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include<cmath>
#include <math.h>

#include "include/glad/glad.h"

#include "glfw3.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#include "library.h"

// Store the global state of your program
struct
{
	GLuint program; // a shader
	GLuint tex; // a texture
	GLuint renderedTexture;
	GLuint fBuff;
	GLuint vao; // a vertex array object
	size_t nTris;
} gs;

const GLuint attribPosition = 0;

void init();

void init()
{
	// load the stl mesh
	auto tris = readStl("C:/Users/jothenin/Downloads/Batman/Batman.stl");
	gs.nTris = tris.size();
	std::cout << tris.size() << std::endl;

	// Build our program and an empty VAO
	gs.program = buildProgram("basic.vsl", "basic.fsl");

	glGenVertexArrays(1, &gs.vao);
	glBindVertexArray(gs.vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	//Build FrameBuffer
	glGenFramebuffers(1, &gs.fBuff);
	//glBindFramebuffer(GL_FRAMEBUFFER, fBuff);

	
	glGenTextures(1, &gs.renderedTexture);
	glBindTexture(GL_TEXTURE_2D, gs.renderedTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 200, 200);

	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 200, 200, GL_RGB, GL_FLOAT, nullptr);

	//Build texture
	glGenTextures(1, &gs.tex);
	glBindTexture(GL_TEXTURE_2D, gs.tex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 200, 200);

	std::vector<float> px(200 * 200 * 3.0);

	int abc = 0;
	for (int j = 0; j < 200 * 200 * 3; j = j + 3) {
		if (abc % 100 == 8) {
			px.at(j) = (float)133 / (float)255;
			px.at(j + 1) = (float)6 / (float)255;
			px.at(j + 2) = (float)6 / (float)255;
		}
		else {
			px.at(j) = (float)21 / (float)255;
			px.at(j + 1) = (float)96 / (float)255;
			px.at(j + 2) = (float)189 / (float)255;
		}
		abc++;
	}

	glBindTexture(GL_TEXTURE_2D, gs.tex);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 200, 200, GL_RGB, GL_FLOAT, px.data());


	// fill the buffer
	int size = sizeof(Triangle) * tris.size();
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Triangle) * tris.size(), tris.data());

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	// set the VAO
	/*
	3: 3 floats
	GL_FLOAT: c'est des floats
	GL_FALSE: on ne veut pas les normaliser
	3 * 4 * 2: c'est l'espace entre chaque nombre
	3 float
	3 sizeof(float)
	2 (il y a les normals à passer)
	*/
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 3 * 4 * 2, 0);
	glEnableVertexAttribArray(attribPosition);

	glBindVertexArray(0);
}

void render(const int width, const int height)
{
	glUseProgram(gs.program);
	double time = glfwGetTime();
	glProgramUniform3f(gs.program, 0, 0.5, 0.5, 0.7);
	float ratio = (float)width / (float)height;
	glm::mat4x4 matriceCam = glm::perspective((float)(M_PI/3), ratio, (float)0.5, (float)800);
	glm::mat4x4 camera = glm::lookAt(glm::vec3(cos(time)*150, sin(time)*150, 80), glm::vec3(0,0,50), glm::vec3(0,0,1));
	matriceCam = matriceCam*camera;

	glProgramUniformMatrix4fv(gs.program, 2, 1, false, &matriceCam[0][0]);

	if (width < height) {
		float scale = (float)height / (float)width;
		glProgramUniform2f(gs.program, 1, 1, 1/scale);
	}
	if(width > height) {
		float scale = (float)width / (float)height;
		glProgramUniform2f(gs.program, 1, 1/scale, 1);
	}

	//Off screen rendering
	glBindFramebuffer(GL_FRAMEBUFFER, gs.fBuff);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gs.renderedTexture, 0);
	glViewport(0, 0, 500, 500);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gs.tex);

	glBindVertexArray(gs.vao);
	glDrawArrays(GL_TRIANGLES, 0, gs.nTris * 3);
	//Screen rendering

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(gs.vao);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gs.renderedTexture);

	glDrawArrays(GL_TRIANGLES, 0, gs.nTris * 3);

	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
}

int main(void)
{
	runGL(init, render);
}