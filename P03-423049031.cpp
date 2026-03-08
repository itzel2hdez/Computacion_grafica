// Práctica 3: Modelado Geométrico y Cámara Sintética.
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtc\random.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "Sphere.h"
#include "Window.h"
#include "Camera.h"

using std::vector;

const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0f;

Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader> shaderList;

static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";

Sphere sp = Sphere(1.0f, 20, 20);


void CrearCubo()
{
    unsigned int cubo_indices[] = {
        0, 1, 2,  2, 3, 0,
        1, 5, 6,  6, 2, 1,
        7, 6, 5,  5, 4, 7,
        4, 0, 3,  3, 7, 4,
        4, 5, 1,  1, 0, 4,
        3, 2, 6,  6, 7, 3
    };

    GLfloat cubo_vertices[] = {
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f
    };

    Mesh* cubo = new Mesh();
    cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
    meshList.push_back(cubo);
}

void CrearPiramideTriangular()
{
    unsigned int indices_piramide_triangular[] = {
        0, 1, 2,
        1, 3, 2,
        3, 0, 2,
        1, 0, 3
    };

    GLfloat vertices_piramide_triangular[] = {
        -0.5f, -0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
         0.0f,  0.5f, -0.25f,
         0.0f, -0.5f, -0.5f
    };

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
    meshList.push_back(obj1);
}

void CrearCilindro(int res, float R)
{
    int n, i;
    GLfloat dt = 2.0f * PI / res;
    GLfloat x, z, y = -0.5f;

    vector<GLfloat> vertices;
    vector<unsigned int> indices;

    for (n = 0; n <= res; n++) {
        if (n != res) {
            x = R * cos(n * dt);
            z = R * sin(n * dt);
        }
        else {
            x = R * cos(0 * dt);
            z = R * sin(0 * dt);
        }

        for (i = 0; i < 6; i++) {
            switch (i) {
            case 0: vertices.push_back(x);     break;
            case 1: vertices.push_back(y);     break;
            case 2: vertices.push_back(z);     break;
            case 3: vertices.push_back(x);     break;
            case 4: vertices.push_back(0.5f);  break;
            case 5: vertices.push_back(z);     break;
            }
        }
    }

    for (n = 0; n <= res; n++) {
        x = R * cos(n * dt);
        z = R * sin(n * dt);

        for (i = 0; i < 3; i++) {
            switch (i) {
            case 0: vertices.push_back(x);      break;
            case 1: vertices.push_back(-0.5f);  break;
            case 2: vertices.push_back(z);      break;
            }
        }
    }

    for (n = 0; n <= res; n++) {
        x = R * cos(n * dt);
        z = R * sin(n * dt);

        for (i = 0; i < 3; i++) {
            switch (i) {
            case 0: vertices.push_back(x);     break;
            case 1: vertices.push_back(0.5f);  break;
            case 2: vertices.push_back(z);     break;
            }
        }
    }

    for (i = 0; i < (int)vertices.size(); i++) {
        indices.push_back(i);
    }

    Mesh* cilindro = new Mesh();
    cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
    meshList.push_back(cilindro);
}

void CrearCono(int res, float R)
{
    int n, i;
    GLfloat dt = 2.0f * PI / res;
    GLfloat x, z, y = -0.5f;

    vector<GLfloat> vertices;
    vector<unsigned int> indices;

    vertices.push_back(0.0f);
    vertices.push_back(0.5f);
    vertices.push_back(0.0f);

    for (n = 0; n <= res; n++) {
        x = R * cos(n * dt);
        z = R * sin(n * dt);

        for (i = 0; i < 3; i++) {
            switch (i) {
            case 0: vertices.push_back(x); break;
            case 1: vertices.push_back(y); break;
            case 2: vertices.push_back(z); break;
            }
        }
    }

    vertices.push_back(R * cos(0) * dt);
    vertices.push_back(-0.5f);
    vertices.push_back(R * sin(0) * dt);

    for (i = 0; i < res + 2; i++) {
        indices.push_back(i);
    }

    Mesh* cono = new Mesh();
    cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
    meshList.push_back(cono);
}

void CrearPiramideCuadrangular()
{
    vector<unsigned int> piramidecuadrangular_indices = {
        0, 3, 4,
        3, 2, 4,
        2, 1, 4,
        1, 0, 4,
        0, 1, 2,
        0, 2, 3
    };

    vector<GLfloat> piramidecuadrangular_vertices = {
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.0f,  0.5f,  0.0f
    };

    Mesh* piramide = new Mesh();
    piramide->CreateMeshGeometry(
        piramidecuadrangular_vertices,
        piramidecuadrangular_indices,
        piramidecuadrangular_vertices.size(),
        piramidecuadrangular_indices.size()
    );
    meshList.push_back(piramide);
}

// -------------------------
// Pyraminx
// -------------------------

Mesh* pyraminxBase = nullptr;
vector<Mesh*> pyraminxStickers;
vector<glm::vec3> pyraminxStickerColors;

void CrearStickerPyraminx(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 colorCara)
{
    glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));
    glm::vec3 centro = (a + b + c) / 3.0f;
    float encoger = 0.82f;
    float relieve = 0.02f;

    a = centro + (a - centro) * encoger + normal * relieve;
    b = centro + (b - centro) * encoger + normal * relieve;
    c = centro + (c - centro) * encoger + normal * relieve;

    vector<GLfloat> vertices = {
        a.x, a.y, a.z,
        b.x, b.y, b.z,
        c.x, c.y, c.z
    };

    vector<unsigned int> indices = { 0, 1, 2 };

    Mesh* sticker = new Mesh();
    sticker->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
    pyraminxStickers.push_back(sticker);
    pyraminxStickerColors.push_back(colorCara);
}

void CrearCaraPyraminxSimple(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 colorCara)
{
    const int niveles = 3;
    vector<vector<glm::vec3>> puntos(niveles + 1);

    for (int fila = 0; fila <= niveles; fila++) {
        float t = (float)fila / (float)niveles;
        glm::vec3 inicio = A + (B - A) * t;
        glm::vec3 fin = A + (C - A) * t;

        for (int columna = 0; columna <= fila; columna++) {
            float s = (fila == 0) ? 0.0f : (float)columna / (float)fila;
            glm::vec3 punto = inicio + (fin - inicio) * s;
            puntos[fila].push_back(punto);
        }
    }

    for (int fila = 0; fila < niveles; fila++) {
        for (int columna = 0; columna <= fila; columna++) {
            CrearStickerPyraminx(
                puntos[fila][columna],
                puntos[fila + 1][columna],
                puntos[fila + 1][columna + 1],
                colorCara
            );

            if (columna < fila) {
                CrearStickerPyraminx(
                    puntos[fila][columna],
                    puntos[fila + 1][columna + 1],
                    puntos[fila][columna + 1],
                    colorCara
                );
            }
        }
    }
}

void CrearBasePyraminx(glm::vec3 punta, glm::vec3 frente, glm::vec3 izquierda, glm::vec3 derecha)
{
    vector<GLfloat> vertices = {
        punta.x, punta.y, punta.z,
        frente.x, frente.y, frente.z,
        izquierda.x, izquierda.y, izquierda.z,
        derecha.x, derecha.y, derecha.z
    };

    vector<unsigned int> indices = {
        0, 2, 1,
        0, 1, 3,
        0, 3, 2,
        1, 2, 3
    };

    pyraminxBase = new Mesh();
    pyraminxBase->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
}

void CrearPyraminx()
{
    glm::vec3 punta(0.0f, 1.84f, 0.0f);
    glm::vec3 frente(0.0f, -0.61f, 1.73f);
    glm::vec3 izquierda(-1.5f, -0.61f, -0.86f);
    glm::vec3 derecha(1.5f, -0.61f, -0.86f);

    CrearBasePyraminx(punta, frente, izquierda, derecha);

    CrearCaraPyraminxSimple(punta, izquierda, frente, glm::vec3(1.0f, 0.0f, 0.0f));
    CrearCaraPyraminxSimple(punta, frente, derecha, glm::vec3(0.0f, 1.0f, 0.0f));
    CrearCaraPyraminxSimple(punta, derecha, izquierda, glm::vec3(0.0f, 0.4f, 1.0f));
    CrearCaraPyraminxSimple(frente, izquierda, derecha, glm::vec3(1.0f, 1.0f, 0.0f));
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);

    Shader* shader2 = new Shader();
    shader2->CreateFromFiles(vShaderColor, fShader);
    shaderList.push_back(*shader2);
}

int main()
{
    mainWindow = Window(800, 800);
    mainWindow.Initialise();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    CrearCubo();
    CrearPiramideTriangular();
    CrearCilindro(15, 1.0f);
    CrearCono(25, 2.0f);
    CrearPiramideCuadrangular();
    CrearPyraminx();
    CreateShaders();

    camera = Camera(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -60.0f,
        0.0f,
        0.3f,
        0.3f
    );

    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformColor = 0;

    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f),
        (float)mainWindow.getBufferWidth() / (float)mainWindow.getBufferHeight(),
        0.1f,
        100.0f
    );

    sp.init();
    sp.load();

    glm::mat4 model(1.0f);
    glm::vec3 color(0.0f, 0.0f, 0.0f);

    while (!mainWindow.getShouldClose()) {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        deltaTime += (now - lastTime) / limitFPS;
        lastTime = now;

        glfwPollEvents();
        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].useShader();
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectLocation();
        uniformView = shaderList[0].getViewLocation();
        uniformColor = shaderList[0].getColorLocation();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
        model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

        color = glm::vec3(1.0f, 0.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));

        // Dibujar Pyraminx
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, -7.0f));
        model = glm::rotate(model, glm::radians(-18.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));

        model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f);

        color = glm::vec3(0.0f, 0.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));

        if (pyraminxBase != nullptr) {
            pyraminxBase->RenderMeshGeometry();
        }

        for (size_t i = 0; i < pyraminxStickers.size(); ++i) {
            glUniform3fv(uniformColor, 1, glm::value_ptr(pyraminxStickerColors[i]));
            pyraminxStickers[i]->RenderMeshGeometry();
        }

        glDisable(GL_POLYGON_OFFSET_FILL);

        glUseProgram(0);
        mainWindow.swapBuffers();
    }
    return 0;
    }