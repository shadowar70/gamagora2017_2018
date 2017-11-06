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
#include "glm\gtc\type_ptr.hpp"
#include "glm/gtx/transform.hpp"

#include "library.h"

// Store the global state of your program
struct
{
	GLuint program; // a shader
	GLuint tex; // a texture
	GLuint texReflet; // a texture
	GLuint renderedTexture;
	GLuint fBuff;
	GLuint vao; // a vertex array object
	size_t nTris;
} gs;

const GLuint attribPosition = 0;
const GLuint taille = 200;

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
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, taille, taille);
	//Build texture
	glGenTextures(1, &gs.texReflet);
	glBindTexture(GL_TEXTURE_2D, gs.texReflet);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, taille, taille);

	std::vector<float> px(taille * taille * 3.0);

	int abc = 0;
	for (int j = 0; j < taille * taille * 3; j = j + 3) {
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
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, taille, taille, GL_RGB, GL_FLOAT, px.data());

	abc = 0;
	for (int j = 0; j < taille * taille * 3; j = j + 3) {
		if (abc % 100 == 8) {
			px.at(j) = (float)100 / (float)255;
			px.at(j + 1) = (float)2 / (float)255;
			px.at(j + 2) = (float)2 / (float)255;
		}
		else {
			px.at(j) = (float)10 / (float)255;
			px.at(j + 1) = (float)56 / (float)255;
			px.at(j + 2) = (float)159 / (float)255;
		}

		abc++;
	}


	glBindTexture(GL_TEXTURE_2D, gs.texReflet);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, taille, taille, GL_RGB, GL_FLOAT, px.data());


	// fill the buffer
	/*int size = sizeof(Triangle) * tris.size();
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);*/
	/*glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Triangle) * tris.size(), tris.data());*/
	GLfloat planMiroir[] = {
		//CUBE
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		//PLAN MIROIR
		- 1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(planMiroir), planMiroir, GL_STATIC_DRAW);
	

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
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 8*4, 0);
	glEnableVertexAttribArray(attribPosition);

	glBindVertexArray(0);
}

void render(const int width, const int height)
{
	glUseProgram(gs.program);
	double time = glfwGetTime();
	glProgramUniform3f(gs.program, 0, 0.5, 0.5, 0.0);
	float ratio = (float)width / (float)height;
	glm::mat4x4 matriceCam = glm::perspective((float)(M_PI/3), ratio, (float)0.5, (float)800);
	
	// Set up projection
	glm::mat4 camera = glm::lookAt(
		glm::vec3(cos(time)*2.5f, sin(time)*2.5f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	// Set up projection
	glm::mat4 cameraCube = glm::lookAt(
		glm::vec3(cos(time)*2.5f, 2.5f, -2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f)
	);

	cameraCube = glm::translate(camera, glm::vec3(0, 0, -1));


	glm::mat4x4 matriceCamCube = glm::perspective((float)(M_PI / 3), ratio, (float)0.5, (float)800);
	matriceCamCube = matriceCamCube*cameraCube;

	matriceCam = matriceCam*camera;

	if (width < height) {
		float scale = (float)height / (float)width;
		glProgramUniform2f(gs.program, 1, 1, 1/scale);
	}
	if(width > height) {
		float scale = (float)width / (float)height;
		glProgramUniform2f(gs.program, 1, 1/scale, 1);
	}




	//=======DEBUG TEST CAMERA CUBE==============//

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, width, height);
	//glClearColor(0, 1, 0, 1); //Couleur Fond
	//glClear(GL_COLOR_BUFFER_BIT);
	//glClear(GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	//glBindVertexArray(gs.vao);

	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, gs.tex);


	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//glClearColor(0.0f, 0, 1, 1.0f);

	//==================LE TRUC QU'ON VEUT QUI MARCHE==================//

	////OFF Screen rendering
	//glBindFramebuffer(GL_FRAMEBUFFER, gs.fBuff);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gs.renderedTexture, 0);
	//glViewport(0, 0, 200, 200);
	//glClearColor(1, 0, 0, 1); //Couleur Fond
	//glClear(GL_COLOR_BUFFER_BIT);
	//glClear(GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	//glBindVertexArray(gs.vao);

	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, gs.tex);
	//

	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//glClearColor(0.0f, 0, 1, 1.0f);

	//glProgramUniformMatrix4fv(gs.program, 2, 1, false, &matriceCam[0][0]);


	////Screen rendering
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, width, height);
	//glClearColor(0, 1, 1, 1); //Couleur Fond
	//glClear(GL_COLOR_BUFFER_BIT);
	//glClear(GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	//glBindVertexArray(gs.vao);

	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, gs.renderedTexture);

	//glDrawArrays(GL_TRIANGLES, 36, 6);
	//glClearColor(0.0f, 0, 1, 1.0f);


	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, gs.tex);
	//glDrawArrays(GL_TRIANGLES, 0, 36);

	//===============================FIN=========================//

	glProgramUniformMatrix4fv(gs.program, 2, 1, false, &matriceCam[0][0]);

	//Screen rendering
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClearColor(0, 1, 1, 1); //Couleur Fond
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(gs.vao);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gs.renderedTexture);
	glDrawArrays(GL_TRIANGLES, 36, 6);
	glClearColor(0.0f, 0, 1, 1.0f);

	//===STENCIL===
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		// draw stencil pattern
		glStencilMask(0xFF);
		glClear(GL_STENCIL_BUFFER_BIT);  // needs mask=0xFF
		glDrawArrays(GL_TRIANGLES, 36, 6);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glStencilMask(0x00);
		// draw where stencil's value is 0
		glStencilFunc(GL_EQUAL, 0, 0xFF);
		/* (nothing to draw) */
		// draw only where stencil's value is 1
		glStencilFunc(GL_EQUAL, 1, 0xFF);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gs.texReflet);
	
		glProgramUniformMatrix4fv(gs.program, 2, 1, false, &matriceCamCube[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisable(GL_STENCIL_TEST);
	
	glProgramUniformMatrix4fv(gs.program, 2, 1, false, &matriceCam[0][0]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gs.tex);
	glDrawArrays(GL_TRIANGLES, 0, 36);	

	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
}

int main(void)
{
	runGL(init, render);
}