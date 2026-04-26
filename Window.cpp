#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
	mueveHelicoptero= 0.0f;
	mueveNave = 0.0f;
	posPezX = 22.0f;
	posPezY = 1.5f;
	posPezZ = 10.0f;
	spotlightDirX = 1.0f;
	spotlightDirY = 0.0f;
	spotlightDirZ = 0.0f;
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "PracticaXX:Nombre de la practica", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	//carro
	if (key == GLFW_KEY_Y)
	{
		theWindow-> muevex += 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow-> muevex -= 1.0;
	}
	//helicoptero
	if (key == GLFW_KEY_I) 
	{
		theWindow->mueveHelicoptero += 0.4f;
	}
	if (key == GLFW_KEY_K) 
	{
		theWindow->mueveHelicoptero -= 0.4f;
	}

	//nave
	if (key == GLFW_KEY_J) 
	{
		theWindow->mueveNave -= 0.4f;
	}
	if (key == GLFW_KEY_L) 
	{
		theWindow->mueveNave += 0.4f;
	}

	//pez
	if (key == GLFW_KEY_T)
	{
		if (theWindow->posPezX < 24.5f && theWindow->posPezY < 4.0f)
		{
			theWindow->posPezX += 0.1f;
			theWindow->posPezY += 0.1f;
		}
	}

	if (key == GLFW_KEY_G)
	{
		if (theWindow->posPezX > 19.5f && theWindow->posPezY > 0.5f)
		{
			theWindow->posPezX -= 0.1f;
			theWindow->posPezY -= 0.1f;
		}
	}

	//luces en ejes
	// Eje X
	if (key == GLFW_KEY_7) theWindow->spotlightDirX += 0.1f;
	if (key == GLFW_KEY_4) theWindow->spotlightDirX -= 0.1f;

	// Eje Y
	if (key == GLFW_KEY_8) theWindow->spotlightDirY += 0.1f;
	if (key == GLFW_KEY_5) theWindow->spotlightDirY -= 0.1f;

	// Eje Z
	if (key == GLFW_KEY_9) theWindow->spotlightDirZ += 0.1f;
	if (key == GLFW_KEY_6) theWindow->spotlightDirZ -= 0.1f;


	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
