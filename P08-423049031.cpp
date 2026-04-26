/*
Práctica 7: Iluminación 1
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dado8Texture;



Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

Model carro;
Model cofre;
Model llanta_df; // derecha frente
Model llanta_if; // izquierda frente
Model llanta_da; // derecha atras
Model llanta_ia; // izquierda atras

Model lampara;
Model Nave;
Model Pecera;
Model foco_pez;
Model antena_pez;
Model cuerpo_pez;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};	GLfloat cubo_vertices[] = {
			// front
			//x		y		z		S		T			NX		NY		NZ
			-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
			0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
			0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	-1.0f,	//2
			-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	-1.0f,	//3
			// right
			//x		y		z		S		T
			0.5f, -0.5f,  0.5f,	    0.0f,  0.0f,		-1.0f,	0.0f,	0.0f,
			0.5f, -0.5f,  -0.5f,	1.0f,	0.0f,		-1.0f,	0.0f,	0.0f,
			0.5f,  0.5f,  -0.5f,	1.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
			0.5f,  0.5f,  0.5f,	    0.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
			// back
			-0.5f, -0.5f, -0.5f,	0.0f,  0.0f,		0.0f,	0.0f,	1.0f,
			0.5f, -0.5f, -0.5f,		1.0f,	0.0f,		0.0f,	0.0f,	1.0f,
			0.5f,  0.5f, -0.5f,		1.0f,	1.0f,		0.0f,	0.0f,	1.0f,
			-0.5f,  0.5f, -0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	1.0f,

			// left
			//x		y		z		S		T
			-0.5f, -0.5f,  -0.5f,	0.0f,  0.0f,		1.0f,	0.0f,	0.0f,
			-0.5f, -0.5f,  0.5f,	1.0f,	0.0f,		1.0f,	0.0f,	0.0f,
			-0.5f,  0.5f,  0.5f,	1.0f,	1.0f,		1.0f,	0.0f,	0.0f,
			-0.5f,  0.5f,  -0.5f,	0.0f,	1.0f,		1.0f,	0.0f,	0.0f,

			// bottom
			//x		y		z		S		T
			-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	1.0f,	0.0f,
			0.5f,  -0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	1.0f,	0.0f,
			 0.5f,  -0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,
			-0.5f, -0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	1.0f,	0.0f,

			//UP
			 //x		y		z		S		T
			 -0.5f, 0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	-1.0f,	-1.0f,
			 0.5f,  0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	-1.0f,	-1.0f,
			  0.5f, 0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	-1.0f,	-1.0f,
			 -0.5f, 0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	-1.0f,	-1.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}

void CrearOctaedro()
{
	unsigned int indices[] = {
		0, 1, 2,   3, 4, 5,   6, 7, 8,   9, 10, 11,
		12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23
	};

	GLfloat vertices[] = {
		// x      y      z      S     T      nx    ny    nz
		// Cara 1 (1)
		0.0f,  1.0f,  0.0f,   0.5f, 0.73f,   -1.0f, -1.0f, -1.0f,
		0.0f,  0.0f,  1.0f,   0.25f, 0.51f,   -1.0f, -1.0f, -1.0f,
		1.0f,  0.0f,  0.0f,   0.75f, 0.5f,   -1.0f, -1.0f, -1.0f,
		// Cara 2 (7)
		0.0f,  1.0f,  0.0f,   0.5f, 0.75f,   -1.0f, -1.0f, 1.0f,
		1.0f,  0.0f,  0.0f,   0.75f, 0.51f,   -1.0f, -1.0f, 1.0f,
		0.0f,  0.0f, -1.0f,   0.99f, 0.76f,   -1.0f, -1.0f, 1.0f,
		// Cara 3 (5)
		0.0f,  1.0f,  0.0f,   0.5f, 0.76f,   1.0f, -1.0f, 1.0f,
		0.0f,  0.0f, -1.0f,   1.0f, 0.76f,   1.0f, -1.0f, 1.0f,
		-1.0f, 0.0f,  0.0f,   0.75f, 1.0f,   1.0f, -1.0f, 1.0f,
		// Cara  (3)
		0.0f,  1.0f,  0.0f,   0.49f, 0.75f,   1.0f, -1.0f, -1.0f,
		-1.0f, 0.0f,  0.0f,   0.0f, 0.75f,   1.0f, -1.0f, -1.0f,
		0.0f,  0.0f,  1.0f,   0.25f, 0.51f,   1.0f, -1.0f, -1.0f,
		// Cara 5 (4)
		0.0f, -1.0f,  0.0f,   0.5f, 0.25f,   -1.0f, 1.0f, -1.0f,
		1.0f,  0.0f,  0.0f,   0.75f, 0.5f,   -1.0f, 1.0f, -1.0f,
		0.0f,  0.0f,  1.0f,   0.25f, 0.5f,   -1.0f, 1.0f, -1.0f,
		// Cara 6 (6)
		0.0f, -1.0f,  0.0f,   0.5f, 0.25f,   -1.0f, 1.0f, 1.0f,
		0.0f,  0.0f, -1.0f,   1.0f, 0.25f,   -1.0f, 1.0f, 1.0f,
		1.0f,  0.0f,  0.0f,   0.75, 0.5f,   -1.0f, 1.0f, 1.0f,
		// Cara 7 (8)
		0.0f, -1.0f,  0.0f,   0.5f, 0.25f,   1.0f, 1.0f, 1.0f,
		-1.0f, 0.0f,  0.0f,   1.0f, 0.25f,   1.0f, 1.0f, 1.0f,
		0.0f,  0.0f, -1.0f,   0.75f, 0.0f,   1.0f, 1.0f, 1.0f,
		// Cara 8 (2)
		0.0f, -1.0f,  0.0f,   0.5f, 0.25f,   1.0f, 1.0f, -1.0f,
		0.0f,  0.0f,  1.0f,   0.25f, 0.5f,   1.0f, 1.0f, -1.0f,
		-1.0f, 0.0f,  0.0f,   0.0f, 0.25f,   1.0f, 1.0f, -1.0f
	};

	Mesh* octaedro = new Mesh();
	octaedro->CreateMesh(vertices, indices, 192, 24);
	meshList.push_back(octaedro);
}





int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	float rot_cofre = 0.0f;
	float pos_carro = 0.0f;
	float rot_llantas = 0.0f;

	CreateObjects();
	CrearDado();
	CreateShaders();
	CrearOctaedro();


	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	dado8Texture = Texture("Textures/dado8.jpg");
	dado8Texture.LoadTextureA();
	 

	/*Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");*/
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	carro = Model();
	carro.LoadModel("Models/carro_texturi.obj");
	cofre = Model();
	cofre.LoadModel("Models/cofre_t.obj");
	llanta_df = Model();
	llanta_df.LoadModel("Models/llanta_t.obj");
	llanta_if = Model();
	llanta_if.LoadModel("Models/llanta_t.obj");
	llanta_da = Model();
	llanta_da.LoadModel("Models/llanta_t_atras.obj");
	llanta_ia = Model();
	llanta_ia.LoadModel("Models/llanta_t_atras.obj");
	lampara = Model();
	lampara.LoadModel("Models/lampara_buena.obj");
	Nave = Model();
	Nave.LoadModel("Models/nave.obj");
	Pecera = Model();
	Pecera.LoadModel("Models/pecera.obj");
	foco_pez = Model();
	foco_pez.LoadModel("Models/foco_pez.obj");
	antena_pez = Model();
	antena_pez.LoadModel("Models/antena_pez.obj");
	cuerpo_pez = Model();
	cuerpo_pez.LoadModel("Models/cuerpo_pez.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f, //el primero es radiacion que hace que los colores se pierdan(componente ambiental), el dos es el sol
		0.0f, 0.0f, -1.0f); //si es negativo es de arriba hacia abajo
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.3f, 1.0f, //que tan intensa es la luz roja (intensidad de color (el segundo valor)), el primer valor es la radiacion ()
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// Luz de la lampara
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		0.3f, 2.0f,
		12.0f, 2.0f, 4.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//para el pez
	pointLights[2] = PointLight(0.0f, 0.0f, 1.0f, 
		0.2f, 9.0f,                               
		0.0f, 0.0f, 0.0f,                        
		1.0f, 4.5f, 9.0f);                   
	pointLightCount++;



	unsigned int spotLightCount = 0;

	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f, //alcance
		15.0f); //ilumina mas o menos zona
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 
	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f,
		0.6f, 2.0f,
		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.7f, 0.01f,
		10.0f);
	spotLightCount++;

	// Luz para el helicóptero 
	spotLights[3] = SpotLight(1.0f, 1.0f, 0.0f,
		0.5f, 3.0f,
		0.0f, 5.0f, 6.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	// nave adelante
	spotLights[4] = SpotLight(0.7f, 0.0f, 1.0f,
		0.0f, 3.0f,
		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, 0.08f, 0.01f,
		20.0f);
	spotLightCount++;

	// nave atras
	spotLights[5] = SpotLight(1.0f, 1.0f, 0.6f,
		0.0f, 0.0f,                        
		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 0.08f, 0.01f,
		20.0f);
	spotLightCount++;

	//luz de los ejes
	spotLights[6] = SpotLight(1.0f, 0.0f, 1.0f,  
		0.0f, 3.0f,                              
		0.0f, 0.0f, 0.0f,                        
		0.0f, -1.0f, 0.0f,                       
		1.0f, 0.05f, 0.012f,                    
		15.0f);                                  
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	// ejercicio 2
	bool lampOn = true;
	bool keyO_Pressed = false;
	//ejercicio3 pero de la practica
	bool fishLightOn = true;
	bool keyP_Pressed = false;

	// ejercicio 3
	int colorIndex = 0;
	bool keyC_Pressed = false;
	glm::vec3 cycleColors[] = {
		glm::vec3(1.0f, 0.0f, 0.0f), 
		glm::vec3(1.0f, 1.0f, 0.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 1.0f), 
		glm::vec3(0.0f, 0.0f, 1.0f), 
		glm::vec3(1.0f, 0.0f, 1.0f) 
	};

	


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		float actualX = mainWindow.getposPezX();
		float actualY = mainWindow.getposPezY();
		float actualZ = mainWindow.getposPezZ();

		// E2 tecla O (lampara)
		if (mainWindow.getsKeys()[GLFW_KEY_O]) {
			if (!keyO_Pressed) {
				lampOn = !lampOn;
				keyO_Pressed = true;
			}
		}
		else {
			keyO_Pressed = false;
		}

		//E3 pero de la practica
		if (mainWindow.getsKeys()[GLFW_KEY_P]) {
			if (!keyP_Pressed) {
				fishLightOn = !fishLightOn;
				keyP_Pressed = true;
			}
		}
		else {
			keyP_Pressed = false;
		}


		// E3 Tecla C (luces del carro)
		if (mainWindow.getsKeys()[GLFW_KEY_C]) {
			if (!keyC_Pressed) {
				colorIndex = (colorIndex + 1) % 6; 
				keyC_Pressed = true;
			}
		}
		else {
			keyC_Pressed = false;
		}


		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());



		//spotLights[1].SetPos(poscoche + glm::vec(x, y, cofre));
		glm::vec3 c = cycleColors[colorIndex];
		spotLights[2].SetColor(c.r, c.g, c.b);

		glm::vec3 posFaro = glm::vec3(mainWindow.getmuevex() - 1.4f, 0.3f, -3.0f);
		glm::vec3 dirFaro = glm::vec3(-1.0f, -0.4f, 0.0f);
		spotLights[2].SetFlash(posFaro, dirFaro);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		float movHeli = mainWindow.getmueveHelicoptero(); //movimiento del helicoptero
		float movNave = mainWindow.getmueveNave(); //movimiento de la nave


		static float xPreviaNave = 0.0f; 
		static bool movHaciaFrente = true; 

		if (movNave < xPreviaNave) {
			movHaciaFrente = true;  
		}
		else if (movNave > xPreviaNave) {
			movHaciaFrente = false; 
		}
		xPreviaNave = movNave; 



		spotLights[4].SetDiffuseIntensity(movHaciaFrente ? 3.0f : 0.0f);
		spotLights[5].SetDiffuseIntensity(!movHaciaFrente ? 3.0f : 0.0f);


		glm::vec3 dirSpot = glm::vec3(
			mainWindow.getSpotlightDirX(),
			mainWindow.getSpotlightDirY(),
			mainWindow.getSpotlightDirZ()
		);

		if (glm::length(dirSpot) > 0.1f) {
			dirSpot = glm::normalize(dirSpot);
		}

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();



		// EJ1 del dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.5f, 2.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado8Texture.UseTexture();
		meshList[5]->RenderMesh();


		// carro
		color = glm::vec3(0.7f, 0.7f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		carro.RenderModel();



		// llantas
		// llanta delantera derecha
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		modelaux = model;
		modelaux = glm::translate(modelaux, glm::vec3(0.69f, -0.48f, 0.53f));
		modelaux = glm::rotate(modelaux, rot_llantas * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		llanta_df.RenderModel();

		// Llanta delantera izquierda
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		modelaux = model;
		modelaux = glm::translate(modelaux, glm::vec3(-1.15f, -0.48f, 0.53f));
		modelaux = glm::rotate(modelaux, rot_llantas * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		llanta_if.RenderModel();

		// llanta trasera derecha
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		modelaux = model;
		modelaux = glm::translate(modelaux, glm::vec3(0.7f, -0.48f, -0.5f));
		modelaux = glm::rotate(modelaux, rot_llantas * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		spotLights[3].SetFlash(glm::vec3(0.0f + movHeli, 5.0f, 6.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		llanta_da.RenderModel();

		// llanta trasera izquierda
		//color = glm::vec3(1.0f, 1.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		modelaux = model;
		modelaux = glm::translate(modelaux, glm::vec3(-1.13f, -0.48f, -0.5f));
		modelaux = glm::rotate(modelaux, rot_llantas * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		llanta_ia.RenderModel();

		// Cofre
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		modelaux = model;
		modelaux = glm::translate(modelaux, glm::vec3(-0.8f, 0.06f, -0.026f));
		modelaux = glm::rotate(modelaux, -rot_cofre * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		cofre.RenderModel();



		//helicoptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + movHeli, 5.0f, 8.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec4 posLuzHelicopRel = glm::vec4(0.0f, -0.5f, 0.0f, 1.0f);
		glm::vec4 posLuzHelicopA = model * posLuzHelicopRel;
		spotLights[3].SetFlash(glm::vec3(posLuzHelicopA), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 5.0f, 6.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[4]->RenderMesh();
		
		//nave
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f + movNave, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glm::vec4 posAdelanteLocal = glm::vec4(-2.0f, 0.0f, 0.0f, 1.0f); 
		glm::vec4 posAtrasLocal = glm::vec4(2.0f, 0.0f, 0.0f, 1.0f); 
		spotLights[4].SetFlash(glm::vec3(model* posAdelanteLocal), glm::vec3(-1.0f, -1.0f, 0.0f));
		spotLights[5].SetFlash(glm::vec3(model* posAtrasLocal), glm::vec3(1.0f, -1.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Nave.RenderModel();


		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));


		// lampara
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(12.0f, -1.0f, 4.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glm::vec4 posLocalFoco = glm::vec4(0.0f, 150.0f, 0.0f, 1.0f);
		glm::vec3 posMundoFoco = glm::vec3(model * posLocalFoco);
		pointLights[1].SetPos(posMundoFoco);
		pointLights[1].SetDiffuseIntensity(lampOn ? 2.0f : 0.0f); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		
		//cuerpo
		glm::mat4 modelPezBase = glm::mat4(1.0f);
		modelPezBase = glm::translate(modelPezBase, glm::vec3(actualX, actualY, actualZ));
		modelPezBase = glm::rotate(modelPezBase, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelPezBase = glm::scale(modelPezBase, glm::vec3(0.1f, 0.1f, 0.1f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPezBase));
		cuerpo_pez.RenderModel();

		// antena
		glm::mat4 modelAux = modelPezBase;
		modelAux = glm::translate(modelAux, glm::vec3(1.5f, 6.55f, -2.25f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));
		antena_pez.RenderModel();

		// bulbo
		modelAux = modelPezBase;
		modelAux = glm::translate(modelAux, glm::vec3(1.3f, 15.6f, 4.2f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAux));

		//luz
		glm::vec3 posLuzPez = glm::vec3(modelAux[3]);
		posLuzPez.y += 0.2f;
		posLuzPez.z += 0.6f;

		pointLights[2].SetPos(posLuzPez);
		pointLights[2].SetDiffuseIntensity(fishLightOn ? 5.0f : 0.0f);
		spotLights[6].SetFlash(posLuzPez, dirSpot);
		foco_pez.RenderModel();

		//pecera
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE); 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(22.0f, 1.0f, 10.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.13f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform4f(uniformColor, 1.0f, 1.0f, 1.0f, 1.0f);
		Pecera.RenderModel();
		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);


		//agave
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}