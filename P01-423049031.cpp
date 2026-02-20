#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <stdlib.h>   
#include <time.h>
//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader;

//LENGUAJE DE SHADER (SOMBRAS) GLSL lenguaje de proogramacion que procesa el gpu
//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = " \n\
#version 330 \n\
layout (location =0) in vec3 pos; \n\
void main() \n\
{ \n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); \n\
}";

//Fragment Shader
//recibir Vcolor y dar de salida color
static const char* fShader = " \n\
#version 330 \n\
out vec4 color; \n\
void main() \n\
{ \n\
color = vec4(1.0f,1.0f,1.0f,1.0f); \n\
}";



void CrearTriangulo()
{
	GLfloat vertices[] = {
		//Primero comenzamos con la letra I
		//Parte superior
		 -0.9f,  -0.2f, 0.0f, //1 
		 -0.9f,  -0.3f, 0.0f, //2
		 -0.7f,  -0.2f, 0.0f, //3

		 -0.7f,  -0.2f, 0.0f, //3
		 -0.5f,  -0.2f, 0.0f, 
		 -0.5f,  -0.3f, 0.0f, 

		 -0.7f,  -0.2f, 0.0f,
		 -0.9f,  -0.3f, 0.0f,
		 -0.5f,  -0.3f, 0.0f,

		 //Parte de enmedio 
		 -0.75f,  -0.25f, 0.0f,
		 -0.75f,  -0.65f, 0.0f,
		 -0.65f,  -0.45f, 0.0f,

		 -0.75f,  -0.25f, 0.0f,
		 -0.65f,  -0.25f, 0.0f,
		 -0.65f,  -0.45f, 0.0f,

		 -0.75f,  -0.65f, 0.0f,
		 -0.65f,  -0.65f, 0.0f,
		 -0.65f,  -0.45f, 0.0f,

		 //Parte inferior
		 -0.9f,  -0.65f, 0.0f,
		 -0.9f,  -0.75f, 0.0f,
		 -0.7f,  -0.65f, 0.0f,

		 -0.7f,  -0.65f, 0.0f,
		 -0.5f,  -0.65f, 0.0f,
		 -0.5f,  -0.75f, 0.0f,
		 
		 -0.7f,  -0.65f, 0.0f,
		 -0.9f,  -0.75f, 0.0f,
		 -0.5f,  -0.75f, 0.0f, //acaba la i

		 //Ahora hacemos la H
		// Parte izquierda
		 -0.3f,  0.4f, 0.0f,
		 -0.2f,  0.4f, 0.0f,
		 -0.3f,  0.15f, 0.0f,

		 -0.2f,  0.4f, 0.0f,
		 -0.3f,  0.15f, 0.0f,
		 -0.2f,  0.15f, 0.0f,

		 -0.3f,  0.15f, 0.0f,
		 -0.2f,  0.15f, 0.0f,
		 -0.3f,  -0.2f, 0.0f,
		 
		 -0.2f,  0.15f, 0.0f,
		 -0.3f,  -0.2f, 0.0f,
		 -0.2f,  -0.2f, 0.0f,

		 //Parte de en medio
		 -0.2f,  0.2f, 0.0f,
		 -0.2f,  0.1f, 0.0f,
		 0.0f,  0.2f, 0.0f,

		 0.0f,  0.2f, 0.0f,
		 -0.2f,  0.1f, 0.0f,
		 0.0f,  0.1f, 0.0f,

		 //Parte derecha
		 0.0f,  0.4f, 0.0f,
		 0.1f,  0.4f, 0.0f,
		 0.0f,  0.1f, 0.0f,

		 0.1f,  0.4f, 0.0f,
		 0.1f,  0.1f, 0.0f,
		 0.0f,  0.1f, 0.0f,

		 0.0f,  0.1f, 0.0f,
		 0.1f,  0.1f, 0.0f,
		 0.0f,  -0.2f, 0.0f,

		 0.1f,  0.1f, 0.0f,
		 0.1f,  -0.2f, 0.0f,
		 0.0f,  -0.2f, 0.0f, //se acaba h

		 //Segunda letra H
		//Parte izquierda
		 0.4f,  0.9f, 0.0f,
		 0.5f,  0.9f, 0.0f,
		 0.4f,  0.6f, 0.0f,

		 0.5f,  0.9f, 0.0f,
		 0.4f,  0.6f, 0.0f,
		 0.5f,  0.6f, 0.0f,

		 0.4f,  0.6f, 0.0f,
		 0.5f,  0.6f, 0.0f,
		 0.4f,  0.3f, 0.0f,

		 0.5f,  0.6f, 0.0f,
		 0.4f,  0.3f, 0.0f,
		 0.5f,  0.3f, 0.0f,

		 //Parte de en medio
		 0.7f,  0.9f, 0.0f,
		 0.8f,  0.9f, 0.0f,
		 0.7f,  0.6f, 0.0f,

		 0.8f,  0.9f, 0.0f,
		 0.7f,  0.6f, 0.0f,
		 0.8f,  0.6f, 0.0f,

		 //Parte derecha
		 0.7f,  0.6f, 0.0f,
		 0.8f,  0.6f, 0.0f,
		 0.7f,  0.3f, 0.0f,

		 0.8f,  0.6f, 0.0f,
		 0.7f,  0.3f, 0.0f,
		 0.8f,  0.3f, 0.0f,

		 0.5f, 0.65f, 0.0f,
		 0.7f, 0.65f, 0.0f,
		 0.5f, 0.55f, 0.0f,

		 0.7f, 0.65f, 0.0f,
		 0.5f, 0.55f, 0.0f,
		 0.7f, 0.55f, 0.0f,

	};
	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)

//the Program recibe los datos de theShader

{
	GLuint theShader = glCreateShader(shaderType);
	const GLchar* theCode[1];
	theCode[0] = shaderCode;
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);
}

void CompileShaders() {
	shader = glCreateProgram();
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}
int main()
{	//se usa la hora del sistema para generar los numeros aleatorios
	srand(time(NULL));
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	//****  LAS SIGUIENTES 4 LÍNEAS SE COMENTAN EN DADO CASO DE QUE AL USUARIO NO LE FUNCIONE LA VENTANA Y PUEDA CONOCER LA VERSIÓN DE OPENGL QUE TIENE ****/

	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

	//Llamada a las funciones creadas antes del main
	CrearTriangulo();
	CompileShaders();

	//Contador de ciclos para controlar el tiempo de espera
	int contador = 0;
	//Limite de ciclos para alcanzar aprox los 2 segundos de espera
	int limite = 1700;
	//Se genera el primer color aleatorio para que la ventana no inicie
	//siempre en color negro y dividimos entre 100.0f para obtener un valor decimal entre
	//0 y 1
	float r = (rand() % 101) / 100.0f;
	float g = (rand() % 101) / 100.0f;
	float b = (rand() % 101) / 100.0f;

	//Loop 
	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();

		//Incrementamos el contador en cada vuelta de bucle
		contador++;
		//Si el contador llega al límite, se cambia el color del fondo
		if (contador >= limite) {
			r = (rand() % 101) / 100.0f;
			g = (rand() % 101) / 100.0f; //valores aleatorios
			b = (rand() % 101) / 100.0f;

			contador = 0; //reiniciamos el valor del contador para
			//esperar otros dos segundos
		}

		//se aplica el color generado
		glClearColor(r, g, b, 1.0f);

		
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 93);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);

		//NO ESCRIBIR NINGUNA LÍNEA DESPUÉS DE glfwSwapBuffers(mainWindow);
	}


	return 0;
}