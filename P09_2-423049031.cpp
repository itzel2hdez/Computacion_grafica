/*
Animación:
- Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
-Compleja: Por medio de funciones y algoritmos. 
-Textura Animada
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

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
float timerNumero = 0.0f;
bool fuegoPrendido = false;
float anguloAeolipile = 0.0f;
float toffsetFuegoV = 0.0f;
float timerEnfriamiento = 0.0f; 

// variables para animación del humo
int estadoHumo = 0; // 0: Apagado, 1: Calentando, 2: Creciendo, 3: Máximo, 4: Desvaneciendo
float escalaHumo = 0.0f;
float timerCalentamiento = 0.0f;
float toffsetHumoV = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;
Texture FuegoTexture;
Texture HumoTexture;



Model Kitt_M;
Model Llanta_M;
Model Pista_M;
Model Nave_M;
Model Ala_M;
Model Aeolipile_base_M;
Model Aeolipile_M;

Model carro;
Model cofre;
Model llanta_df; // derecha frente
Model llanta_if; // izquierda frente
Model llanta_da; // derecha atras
Model llanta_ia; // izquierda atras

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



//cálculo del promedio de las normales para sombreado de Phong
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
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	float rot_cofre = 0.0f;
	float pos_carro = 0.0f;
	float rot_llantas = 0.0f;

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

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
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA(); 
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();
	FuegoTexture = Texture("Textures/fuego1.png");
	FuegoTexture.LoadTextureA();
	HumoTexture = Texture("Textures/humito.png"); 
	HumoTexture.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Pista_M = Model();
	Pista_M.LoadModel("Models/pista.obj");
	Nave_M = Model();
	Nave_M.LoadModel("Models/nave.obj");
	Ala_M = Model();
	Ala_M.LoadModel("Models/ala.obj");
	Aeolipile_base_M = Model();
	Aeolipile_base_M.LoadModel("Models/Aeolipile_base.obj");
	Aeolipile_M = Model();
	Aeolipile_M.LoadModel("Models/Aeolipile.obj");
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
		0.5f, 0.5f,
		0.0f, -1.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
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
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(1.0f, 0.0f, 0.0f, // Color inicial (rojo)
		0.6f, 2.0f,                             // ambiente, difusa
		0.0f, 0.0f, 0.0f,                       // posición inicial
		-1.0f, 0.0f, 0.0f,                      // dirección inicial
		1.0f, 0.7f, 0.01f,                      // atenuación (constante, lineal, exponente)
		10.0f);                                 // ángulo (edge)
	spotLightCount++;

	// faro frontal
	spotLights[3] = SpotLight(0.7f, 0.0f, 1.0f,
		0.0f, 3.0f,
		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, 0.08f, 0.01f,
		20.0f);
	spotLightCount++;

	// faro trasero
	spotLights[4] = SpotLight(1.0f, 1.0f, 0.6f,
		0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 0.08f, 0.01f,
		20.0f);
	spotLightCount++;




	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	
	struct PuntoRuta {
		float x, y, z;   
		float giroY;        
		float giroZ;         
	};

	PuntoRuta rutaCoche[] = {
		// recta principal
		{    0.0f,      -0.9f,       3.0f,        0.0f,         0.0f }, // salida
		{  -40.0f,      -0.9f,       3.0f,        0.0f,         0.0f }, // recta

		// curva 1
		{  -45.0f,      -0.9f,       9.0f,       35.0f,         0.0f }, // curva izquierda
		{  -55.0f,       1.1f,       9.0f,        0.0f,         0.0f }, // se endereza

		// curva 2
		{  -74.0f,       2.5f,      -5.0f,      -65.0f,         0.0f }, // curva derecha
		{  -76.0f,       5.5f,      -5.0f,        0.0f,        -10.0f }, // frente a la rampa

		// rampa
		{  -100.0f,      14.5f,      -4.0f,      -15.0f,        -20.0f }, //sube rampa
		{  -110.0f,      23.0f,      -3.0f,        0.0f,        -20.0f }, // media rampa
		{  -125.0f,      24.0f,      -7.0f,      -15.0f,         0.0f }, // cima
		{  -176.0f,      26.0f,     -14.0f,      -18.0f,         0.0f }  // meta
	};

	int numPuntosPista = 10; 
	float indiceRuta = 0.0f;
	float velRuta = 0.007f;
	bool animandoCoche = true;



	// ruta de la nave
	PuntoRuta rutaNave[] = {
		//   x(avance)  y(altura)  z(lado)  giroY(volante)  giroZ(cabeceo)
		{ -180.0f,      35.0f,     -15.0f,     160.0f,        0.0f }, 
		{ -125.0f,      30.0f,      -4.0f,     180.0f,       -5.0f }, 
		{  -70.0f,      20.0f,      -6.0f,     180.0f,      -10.0f }, 
		{  -30.0f,      10.0f,      -8.0f,     180.0f,      -15.0f }, 
		{    0.0f,       0.5f,     -10.0f,     180.0f,        0.0f }  
	};

	int numPuntosNave = 5;
	float indiceRutaNave = 0.0f;
	float velRutaNave = 0.005f; 
	bool animacionNaveActiva = false;
	bool naveYaVolo = false; 

	// Variables para faro y animación de rueda
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;


	glm::vec3 lowerLight(0.0f,0.0f,0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

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

	
	lastTime = glfwGetTime();
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f*deltaTime;
		//¿Cómo haces para que el carro no se salga del piso


		// coche
		if (mainWindow.getsKeys()[GLFW_KEY_R]) {
			indiceRuta = 0.0f;
			animandoCoche = true;
		}

		glm::vec3 posCocheCalculada(0.0f);
		float yawCoche = 0.0f;
		float pitchCoche = 0.0f;

		if (animandoCoche) {
			indiceRuta += velRuta * deltaTime;
			if (indiceRuta >= numPuntosPista - 1) {
				indiceRuta = numPuntosPista - 1.001f; 
				animandoCoche = false;
			}
			else {
				rotllanta -= rotllantaOffset * deltaTime;
			}
		}

		// interpolación lineal
		int pActual = (int)indiceRuta;
		int pSiguiente = pActual + 1;
		float t = indiceRuta - pActual;

		posCocheCalculada.x = rutaCoche[pActual].x + t * (rutaCoche[pSiguiente].x - rutaCoche[pActual].x);
		posCocheCalculada.y = rutaCoche[pActual].y + t * (rutaCoche[pSiguiente].y - rutaCoche[pActual].y);
		posCocheCalculada.z = rutaCoche[pActual].z + t * (rutaCoche[pSiguiente].z - rutaCoche[pActual].z);
		yawCoche = rutaCoche[pActual].giroY + t * (rutaCoche[pSiguiente].giroY - rutaCoche[pActual].giroY);
		pitchCoche = rutaCoche[pActual].giroZ + t * (rutaCoche[pSiguiente].giroZ - rutaCoche[pActual].giroZ);

		// E3 Tecla C 
		if (mainWindow.getsKeys()[GLFW_KEY_C]) {
			if (!keyC_Pressed) {
				colorIndex++;
				if (colorIndex > 5) colorIndex = 0;
				keyC_Pressed = true;
			}
		}
		else {
			keyC_Pressed = false;
		}

		GLfloat fr = 1.0f, fg = 1.0f, fb = 1.0f;
		switch (colorIndex) {
		case 0: fr = 1.0f; fg = 0.0f; fb = 0.0f; break; // Rojo
		case 1: fr = 1.0f; fg = 1.0f; fb = 0.0f; break; // Amarillo
		case 2: fr = 0.0f; fg = 1.0f; fb = 0.0f; break; // Verde
		case 3: fr = 0.0f; fg = 1.0f; fb = 1.0f; break; // Cian
		case 4: fr = 0.0f; fg = 0.0f; fb = 1.0f; break; // Azul
		case 5: fr = 1.0f; fg = 0.0f; fb = 1.0f; break; // Magenta
		}

		spotLights[2].SetColor(fr, fg, fb);



		// control de la nave ( tecla v)
		if (mainWindow.getsKeys()[GLFW_KEY_V]) {
			if (!naveYaVolo) {
				animacionNaveActiva = true; 
				naveYaVolo = true;         
			}
		}

		glm::vec3 posNaveCalculada(0.0f);
		float yawNave = 0.0f, pitchNave = 0.0f;
		static float rotAla = 0.0f;
		static float rotHelice = 0.0f;
		static bool bajaAla = true;

		if (animacionNaveActiva) {
			indiceRutaNave += velRutaNave * deltaTime;

			if (indiceRutaNave >= numPuntosNave - 1) {
				indiceRutaNave = numPuntosNave - 1.001f;
				animacionNaveActiva = false;
			}
			else {
				rotHelice += 100.0f * deltaTime; 
				if (bajaAla) {
					rotAla -= 50.0f * deltaTime;
					if (rotAla <= -45.0f) bajaAla = false;
				}
				else {
					rotAla += 50.0f * deltaTime;
					if (rotAla >= 45.0f) bajaAla = true;
				}
			}
		}

		// interpolacion nave
		int pActNave = (int)indiceRutaNave;
		int pSigNave = pActNave + 1;
		float tNave = indiceRutaNave - pActNave;

		posNaveCalculada.x = rutaNave[pActNave].x + tNave * (rutaNave[pSigNave].x - rutaNave[pActNave].x);
		posNaveCalculada.y = rutaNave[pActNave].y + tNave * (rutaNave[pSigNave].y - rutaNave[pActNave].y);
		posNaveCalculada.z = rutaNave[pActNave].z + tNave * (rutaNave[pSigNave].z - rutaNave[pActNave].z);
		yawNave = rutaNave[pActNave].giroY + tNave * (rutaNave[pSigNave].giroY - rutaNave[pActNave].giroY);
		pitchNave = rutaNave[pActNave].giroZ + tNave * (rutaNave[pSigNave].giroZ - rutaNave[pActNave].giroZ);
	
		



		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Prender y apagar con teclado (F = Fuego, G = apagar)
		if (mainWindow.getsKeys()[GLFW_KEY_F]) {
			fuegoPrendido = true;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_G]) {
			fuegoPrendido = false;
		}

		//logica de secuencia
		if (fuegoPrendido) {
			toffsetFuegoV += 0.005f * deltaTime; 
			if (toffsetFuegoV > 1.0f) toffsetFuegoV = 0.0f;

			if (estadoHumo == 0) {
				estadoHumo = 1;
				timerCalentamiento = 0.0f;
			}
			else if (estadoHumo == 1) {
				timerCalentamiento += deltaTime;
				if (timerCalentamiento >= 180.0f) { //le puse 3 segundos porque con 2 no se apreciaba bien
					estadoHumo = 2; 
				}
			}
			else if (estadoHumo == 2) {
				anguloAeolipile += 0.8f * deltaTime; 
				if (escalaHumo < 2.5f) escalaHumo += 0.03f * deltaTime;
			}
		}
		else { //con G se apaga
			if (estadoHumo != 0) {
				// fuego apagado, pero sigue girando y hay humo
				if (timerEnfriamiento < 180.0f) {
					timerEnfriamiento += deltaTime;
					anguloAeolipile += 0.8f * deltaTime; // sigue girando 3 seg
				}
				// deja de girar, pero el humo tarda otros 3 seg en irse
				else {
					if (escalaHumo > 0.0f) {
						escalaHumo -= 0.02f * deltaTime; // el humo se desvanece
					}
					else {
						estadoHumo = 0; // apagado total
						timerEnfriamiento = 0.0f;
						timerCalentamiento = 0.0f;
					}
				}
			}
		}

		// movimiento de la textura del humo 
		if (estadoHumo > 1 || escalaHumo > 0.0f) {
			toffsetHumoV += 0.008f * deltaTime;
			if (toffsetHumoV > 1.0f) toffsetHumoV = 0.0f;
		}

		// E3 Tecla C 
		if (mainWindow.getsKeys()[GLFW_KEY_C]) {
			if (!keyC_Pressed) {
				colorIndex = (colorIndex + 1) % 6;
				keyC_Pressed = true;
			}
		}
		else {
			keyC_Pressed = false;
		}

		

			shaderList[0].SetSpotLights(spotLights, spotLightCount);

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
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());


		//spotLights[1].SetPos(poscoche + glm::vec(x, y, cofre));
		glm::vec3 c = cycleColors[colorIndex];
		spotLights[2].SetColor(c.r, c.g, c.b);

		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		float movHeli = mainWindow.getmueveHelicoptero(); //movimiento del helicoptero


		//Reinicializando variables cada ciclo de reloj
		model= glm::mat4(1.0);
		modelaux= glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//Pista
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.1f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pista_M.RenderModel();

		

		//aqui empieza el carro
		glm::mat4 modelCocheBase = glm::mat4(1.0);
		// Usamos las variables interpoladas
		modelCocheBase = glm::translate(modelCocheBase, posCocheCalculada);
		modelCocheBase = glm::rotate(modelCocheBase, yawCoche * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelCocheBase = glm::rotate(modelCocheBase, pitchCoche * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		// faro
		glm::vec4 posFaroLocal = glm::vec4(-2.1f, 0.2f, 1.0f, 1.0f);
		glm::vec4 dirFaroLocal = glm::vec4(-1.0f, -0.3f, 0.0f, 0.0f);
		spotLights[2].SetFlash(glm::vec3(modelCocheBase * posFaroLocal), glm::vec3(modelCocheBase * dirFaroLocal));

		// carro
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCocheBase));
		carro.RenderModel();

		// llantas
		// llanta delantera derecha
		glm::mat4 modelLlanta = modelCocheBase;
		modelLlanta = glm::translate(modelLlanta, glm::vec3(0.69f, -0.48f, 0.53f));
		modelLlanta = glm::rotate(modelLlanta, rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlanta));
		llanta_df.RenderModel();

		// Llanta delantera izquierda
		modelLlanta = modelCocheBase;
		modelLlanta = glm::translate(modelLlanta, glm::vec3(-1.15f, -0.48f, 0.53f));
		modelLlanta = glm::rotate(modelLlanta, rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlanta));
		llanta_if.RenderModel();

		// llanta trasera derecha
		modelLlanta = modelCocheBase;
		modelLlanta = glm::translate(modelLlanta, glm::vec3(0.7f, -0.48f, -0.5f));
		modelLlanta = glm::rotate(modelLlanta, rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlanta));
		llanta_da.RenderModel();

		// llanta trasera izquierda
		//color = glm::vec3(1.0f, 1.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		modelLlanta = modelCocheBase;
		modelLlanta = glm::translate(modelLlanta, glm::vec3(-1.13f, -0.48f, -0.5f));
		modelLlanta = glm::rotate(modelLlanta, rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlanta));
		llanta_ia.RenderModel();


		// Cofre
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		modelaux = modelCocheBase;
		modelaux = glm::translate(modelaux, glm::vec3(-0.8f, 0.06f, -0.026f));
		modelaux = glm::rotate(modelaux, -rot_cofre * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		cofre.RenderModel();



		// nave
		glm::mat4 modelNaveBase = glm::mat4(1.0);
		modelNaveBase = glm::translate(modelNaveBase, posNaveCalculada);
		modelNaveBase = glm::rotate(modelNaveBase, yawNave * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelNaveBase = glm::rotate(modelNaveBase, pitchNave * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelNaveBase = glm::scale(modelNaveBase, glm::vec3(5.0f, 5.0f, 5.0f));

		GLfloat luzNavePrendida = animacionNaveActiva ? 3.0f : 0.0f;
		spotLights[3] = SpotLight(0.0f, 1.0f, 1.0f, 0.0f, luzNavePrendida, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.05f, 0.012f, 30.0f);

		glm::vec4 posFaroNaveLocal = glm::vec4(0.0f, -0.5f, 0.0f, 1.0f);
		glm::vec4 dirFaroNaveLocal = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
		spotLights[3].SetFlash(glm::vec3(modelNaveBase* posFaroNaveLocal), glm::vec3(modelNaveBase* dirFaroNaveLocal));
		
		//cuerpo
		model = modelNaveBase;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Nave_M.RenderModel();

		// ala izq
		glm::mat4 modelAlaIzq = modelNaveBase;
		modelAlaIzq = glm::translate(modelAlaIzq, glm::vec3(0.0f, 0.0f, -0.3f));
		modelAlaIzq = glm::rotate(modelAlaIzq, rotAla * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = modelAlaIzq;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala_M.RenderModel();

		// ala derecha
		glm::mat4 modelAlaDer = modelNaveBase;
		modelAlaDer = glm::translate(modelAlaDer, glm::vec3(0.0f, 0.0f, 0.3f));
		modelAlaDer = glm::rotate(modelAlaDer, -rotAla * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelAlaDer = glm::scale(modelAlaDer, glm::vec3(1.0f, 1.0f, -1.0f)); 
		model = modelAlaDer;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala_M.RenderModel();

		



		//aeolopile
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, -0.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Aeolipile_base_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 5.2f, 0.0f));
		model = glm::rotate(model, anguloAeolipile * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Aeolipile_M.RenderModel();

		

		//fuego y humo
		glEnable(GL_BLEND); 
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

			if (fuegoPrendido) {
				toffset = glm::vec2(0.0f, -toffsetFuegoV);
				glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(19.8f, 0.8f, 1.5f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 

				FuegoTexture.UseTexture(); 
				meshList[5]->RenderMesh();
			}

			if (estadoHumo > 1 || escalaHumo > 0.0f) {
				toffset = glm::vec2(0.0f, -toffsetHumoV);
				glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset)); 

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(20.0f, 4.5f + (escalaHumo * 0.5f), 1.5f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(escalaHumo, escalaHumo, escalaHumo));

				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
				HumoTexture.UseTexture(); 
				meshList[5]->RenderMesh();
			}

		glDisable(GL_BLEND); 


		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetflechau += 0.001;
		toffsetflechav = 0.000;
		//para que no se desborde la variable
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//plano con todos los números
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//número 1
		//toffsetnumerou = 0.0;
		//toffsetnumerov = 0.0;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		for (int i = 1; i < 4; i++)
		{
			//números 2-4
			toffsetnumerou += 0.25;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();

		}

		for (int j = 1; j < 5; j++)
		{
			//números 5-8
			toffsetnumerou += 0.25;
			toffsetnumerov = -0.33;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();
		}


		//número cambiante 
		/*
		¿Cómo hacer para que sea a una velocidad visible?
		*/
		timerNumero += deltaTime;
		if (timerNumero >= 45.0f) { 
			toffsetnumerocambiau += 0.25f;
			if (toffsetnumerocambiau >= 1.0f)
				toffsetnumerocambiau = 0.0f;
			timerNumero = 0.0f; 
		}
		toffsetnumerov = 0.0; 

	

		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		//cambiar automáticamente entre textura número 1 y número 2
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Numero1Texture.UseTexture();
		//if
		//Numero1Texture.UseTexture();
		//Numero2Texture.UseTexture();

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
