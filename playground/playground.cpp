// Inclure les en-têtes standards
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>

int main(void)
{
	// Initialiser GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Échec de l'initialisation de GLFW\n");
		getchar();
		return -1;
	}


	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
	// Ouvrir une fenêtre et créer son contexte OpenGL
	window = glfwCreateWindow(1024, 768, "Tutoriel 0 - Clavier et Souris", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Échec de l'ouverture de la fenêtre GLFW. Si vous avez un GPU Intel, il n'est pas compatible 3.3. Essayez la version 2.1 des tutoriels.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialiser GLEW
	glewExperimental = true; 
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Échec de l'initialisation de GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}


	
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Placer la souris au centre de l'écran
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	//couleur fond
	glClearColor(0.0f, 0.0f, 0.6f, 0.0f);

	// test de profondeur
	glEnable(GL_DEPTH_TEST);
	// Accepter le fragment s'il est plus proche de la caméra que l'ancien
	glDepthFunc(GL_LESS);

	// Ignorer les triangles dont la normale ne fait pas face à la caméra
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// compiler programme GLSL à partir des shaders
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

	// identifiant pour notre uniforme "MVP"
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Charger la texture
	GLuint Texture = loadDDS("uvtemplate.DDS");

	//  identifiant pour notre uniforme "myTextureSampler"
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	
	// Un cube a 6 faces avec 2 triangles chacun, donc cela fait 6*2=12 triangles soit 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};

	// Deux coordonnées UV pour chaque vertex.
	static const GLfloat g_uv_buffer_data[] = {
		0.000059f, 0.000004f,
		0.000103f, 0.336048f,
		0.335973f, 0.335903f,
		1.000023f, 0.000013f,
		0.667979f, 0.335851f,
		0.999958f, 0.336064f,
		0.667979f, 0.335851f,
		0.336024f, 0.671877f,
		0.667969f, 0.671889f,
		1.000023f, 0.000013f,
		0.668104f, 0.000013f,
		0.667979f, 0.335851f,
		0.000059f, 0.000004f,
		0.335973f, 0.335903f,
		0.336098f, 0.000071f,
		0.667979f, 0.335851f,
		0.335973f, 0.335903f,
		0.336024f, 0.671877f,
		1.000004f, 0.671847f,
		0.999958f, 0.336064f,
		0.667979f, 0.335851f,
		0.668104f, 0.000013f,
		0.335973f, 0.335903f,
		0.667979f, 0.335851f,
		0.335973f, 0.335903f,
		0.668104f, 0.000013f,
		0.336098f, 0.000071f,
		0.000103f, 0.336048f,
		0.000004f, 0.671870f,
		0.336024f, 0.671877f,
		0.000103f, 0.336048f,
		0.336024f, 0.671877f,
		0.335973f, 0.335903f,
		0.667969f, 0.671889f,
		1.000004f, 0.671847f,
		0.667979f, 0.335851f
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	do {

		// Effacer l'écran
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Utiliser shader
		glUseProgram(programID);

		// Calculer la matrice MVP à partir des entrées clavier et souris
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Envoyer notre transformation au shader actuellement lié,
		// dans l'uniforme "MVP"
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Lier notre texture à l'unité de texture 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Définir notre sampler "myTextureSampler" pour utiliser l'unité de texture 0
		glUniform1i(TextureID, 0);

		// 1er attribut de buffer : les vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                 
			3,                  
			GL_FLOAT,           
			GL_FALSE,          
			0,                  
			(void*)0           
		);

		// 2ème attribut de buffer : les UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                               
			2,                               
			GL_FLOAT,                         
			GL_FALSE,                         
			0,                                
			(void*)0                          
		);

		// Dessiner le triangle 
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices commençant à 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Echanger les buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Vérifier si la touche ESC a été pressée ou si la fenêtre a été fermée
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Nettoyer les VBO et le shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Fermer la fenêtre OpenGL et terminer GLFW
	glfwTerminate();

	return 0;
}