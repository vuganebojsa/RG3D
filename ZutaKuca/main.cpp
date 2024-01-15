//Opis: Primjer ucitavanja modela upotrebom ASSIMP biblioteke
//Preuzeto sa learnOpenGL

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "model.hpp"
#include "camera.cpp"

const unsigned int wWidth = 1000;
const unsigned int wHeight = 800;
static unsigned loadImageToTexture(const char* filePath);


float orbitSpeed = 0.8f;
float orbitRadius = 9.9f;
glm::vec3 sunPosition = glm::vec3(0.0f, 20.0f, 0.0f); 
float sunRotationSpeed = 40.0f;  
float sunPulseSpeed = 2.5f;     
float sunMinScale = 0.6f;       
float sunMaxScale = 1.0f;  

float lastX = wWidth / 2.0f;
float lastY = wHeight / 2.0f;

bool firstMouse = true;
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

int main()
{
#pragma region Loads
    if(!glfwInit())
    {
        std::cout << "GLFW fail!\n" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(wWidth, wHeight, "Na kraj sela zuta kuca", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Window fail!\n" << std::endl;
        glfwTerminate();
        return -2;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() !=GLEW_OK)
    {
        std::cout << "GLEW fail! :(\n" << std::endl;
        return -3;
    }
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#pragma endregion
#pragma region name
    Shader nameShader("name.vert", "name.frag");
    float nameVertices[] =
    {
        -0.99, -0.7, 0.0, 1.0,
        -0.99, -1.0, 0.0, 0.0,
        -0.4, -0.7, 1.0, 1.0,
        -0.4, -1.0, 1.0, 0.0
    };

    unsigned int nameIndices[] =
    {
        0, 1, 3,
        3, 2, 0
    };
    unsigned int nameVAO;
    glGenVertexArrays(1, &nameVAO);
    glBindVertexArray(nameVAO);

    unsigned int nameVBO;
    glGenBuffers(1, &nameVBO);
    glBindBuffer(GL_ARRAY_BUFFER, nameVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(nameVertices), nameVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int nameInd;
    glGenBuffers(1, &nameInd);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nameInd);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(nameIndices), nameIndices, GL_STATIC_DRAW);



#pragma endregion
#pragma region Models


    Model tree("models/Tree/Tree.obj");
    Model dog("models/Dog/13466_Canaan_Dog_v1_L3.obj");
    Model woman("models/Female/11554_pilgrim_female_V2_L2.obj");
    Model male("models/Male/ManCasual3.obj");
    Model sun("models/Sun/sun.obj");
    Model fence("models/Ograda/fence.obj");
    Model door("models/Door/door.obj");
    Model houseDoor("models/HouseDoor/10057_wooden_door_v3_iterations-2.obj");
#pragma endregion

#pragma region Shaders

    Shader lightingShader("basic.vert", "basic.frag");
    Shader lightCubeShader("simple.vert", "simple.frag");

    glm::vec3 pointLightPositions[] = {
       glm::vec3(1.0f,  3.8f,  -5.5f),
       glm::vec3(1.0f, 7.8f, -5.5f)
      
    };
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f);

    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setVec3("dirLight.direction", 0.0f, -1.0f, 0.0f);
    lightingShader.setVec3("dirLight.ambient", 0.6f, 0.6f, 0.6f);
    lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    lightingShader.setVec3("dirLight.specular", 0.8f, 0.8f, 0.8f);
    lightingShader.setVec3("dirLight.color", 1.0f, 1.0f, 1.0f);

    lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
    lightingShader.setVec3("pointLights[0].ambient", 0.35f, 0.35f, 0.35f);
    lightingShader.setVec3("pointLights[0].diffuse", 0.2f, 0.2f, 0.2f);
    lightingShader.setVec3("pointLights[0].specular", 0.2f, 0.2f, 0.2f);
    lightingShader.setFloat("pointLights[0].constant", 1.0f);
    lightingShader.setFloat("pointLights[0].linear", 0.09f);
    lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
    lightingShader.setVec3("pointLights[0].color", glm::vec3(1.0, 1.0, 1.0));
    lightingShader.setVec3("pointLights[0].intensity", glm::vec3(1.0, 1.0, 1.0));

    lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
    lightingShader.setVec3("pointLights[1].ambient", 0.15f, 0.15f, 0.15f);
    lightingShader.setVec3("pointLights[1].diffuse", 0.2f, 0.2f, 0.2f);
    lightingShader.setVec3("pointLights[1].specular", 0.2f, 0.2f,  0.2f);
    lightingShader.setFloat("pointLights[1].constant", 1.0f);
    lightingShader.setFloat("pointLights[1].linear", 0.09f);
    lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
    lightingShader.setVec3("pointLights[1].color", glm::vec3(1.0, 1.0, 1.0));
    lightingShader.setVec3("pointLights[1].intensity", glm::vec3(1.0, 1.0, 1.0));

    lightingShader.setVec3("pointLights[2].position", pointLightPositions[1]);
    lightingShader.setVec3("pointLights[2].ambient", 0.1f, 0.1f, 0.1f);
    lightingShader.setVec3("pointLights[2].diffuse", 0.5f, 0.5f, 0.5f);
    lightingShader.setVec3("pointLights[2].specular", 0.5f,0.5f, 0.5f);
    lightingShader.setFloat("pointLights[2].constant", 1.0f);
    lightingShader.setFloat("pointLights[2].linear", 0.09f);
    lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
    lightingShader.setVec3("pointLights[2].color", glm::vec3(1.0, 1.0, 1.0));
    lightingShader.setVec3("pointLights[2].intensity", glm::vec3(1.0, 1.0, 1.0));

    lightingShader.setVec3("spotLight.position", glm::vec3(2.0f, 4.32f, -2.3f));
    lightingShader.setVec3("spotLight.direction", glm::vec3(0.0f, -1.0f, 0.0));
    lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(15.0f)));
    lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(40.0f)));
    lightingShader.setVec3("spotLight.color", glm::vec3(0.0,1.0, 1.0));
    lightingShader.setVec3("spotLight.ambient", 1.0f, 1.0f, 1.0f);
    lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setFloat("spotLight.constant", 1.0f);
    lightingShader.setFloat("spotLight.linear", 0.09f);
    lightingShader.setFloat("spotLight.quadratic", 0.032f);

#pragma endregion

#pragma region Textures

    unsigned wall = loadImageToTexture("res/wall.jpg");
    glBindTexture(GL_TEXTURE_2D, wall);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
 
    unsigned name = loadImageToTexture("res/name.png");
    glBindTexture(GL_TEXTURE_2D, name);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    unsigned grassTexture = loadImageToTexture("res/trava.jpg");
    unsigned roofTexture = loadImageToTexture("res/roof.jpg");
    unsigned floorTexture = loadImageToTexture("res/floor.jpg");
    unsigned neonTexture = loadImageToTexture("res/neon.jpg");
    unsigned reflectorTexture = loadImageToTexture("res/chimney.jpg");
    unsigned smokeTexture = loadImageToTexture("res/smoke.png");
    unsigned chimneyTexture = loadImageToTexture("res/chimney.jpg");
    unsigned grassSpecularTexture = loadImageToTexture("res/grassspecular.png");
    unsigned tapestry4Texture = loadImageToTexture("res/tapestry4.jpg");
    unsigned tapestry3Texture = loadImageToTexture("res/tapestry3.jpg");
    unsigned tapestry5Texture = loadImageToTexture("res/tapestry5.jpg");
    unsigned tapestry6Texture = loadImageToTexture("res/tapestry6.jpg");
    unsigned tapestry1Texture = loadImageToTexture("res/tapestry1.jpg");
    unsigned tapestry2Texture = loadImageToTexture("res/tapestry2.jpg");
    unsigned tapestry7Texture = loadImageToTexture("res/tapestry7.jpg");
    unsigned windowTexture = loadImageToTexture("res/window.jpg");
    glBindTexture(GL_TEXTURE_2D, roofTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, windowTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, tapestry7Texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, tapestry4Texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, tapestry3Texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, tapestry2Texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, tapestry1Texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, tapestry5Texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, tapestry6Texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, reflectorTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, neonTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, smokeTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, grassSpecularTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, grassTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, chimneyTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
#pragma endregion

#pragma region Vertices

    float vertices[] = {
        // positions          // normals           // texture coords
        //X      Y       Z     NX     NY     NZ  
        // front   
        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f, 1.0f,  0.0f,  1.0f,
         0.5f, -0.5f, 0.5f,  0.0f,  0.0f, 1.0f,  1.0f,  1.0f,
       -0.5f,  0.5f, 0.5f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, 0.5f,  0.0f,  0.0f, 1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, 0.5f,  0.0f,  0.0f, 1.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, 0.5f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,

        //bottom
        -0.5f, -0.5f, -0.5f,  0.0f,  -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  -1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  -1.0f,  0.0f,  1.0f,  0.0f,

        //left
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,   0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,   0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,   0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

        //right
         0.5f,  -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f,   0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f,   0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,   0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

         //top
        -0.5f, 0.5f, 0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  0.0f,
         0.5f, 0.5f, 0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, 0.5f,-0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, 0.5f, 0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, 0.5f,-0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, 0.5f,-0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,

        //back
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,  0.0f,  0.0f
    };


    float smokeVertices[] =
    {
        3.85,  12.27,  -1.2, 1.0, 1.0,  0.0,
         4.15,  12.27,  -1.2, 1.0, 1.0,  0.0,
        3.85,  12.7,  -1.2, 1.0, 1.0,  0.0,
         4.15,  12.7,  -1.2, 1.0, 1.0,  0.0,

         3.85,  12.27,  -1.6, 1.0, 1.0,  0.0,
          4.15,  12.27,  -1.6, 1.0, 1.0,  0.0,
         3.85,  12.7,  -1.6, 1.0, 1.0,  0.0,
          4.15,  12.7,  -1.6, 1.0, 1.0,  0.0,

         3.85,  12.27,  -1.6, 1.0, 1.0,  0.0,
         3.96,  12.7,  -1.6, 1.0, 1.0,  0.0,
         3.85,  12.27,  -1.2, 1.0, 1.0,  0.0,
         3.96,  12.7,  -1.2, 1.0, 1.0,  0.0,

         4.15,  12.27,  -1.6, 1.0, 1.0,  0.0,
         4.16,  12.7,  -1.6, 1.0, 1.0,  0.0,
        4.15,  12.27,  -1.2, 1.0, 1.0,  0.0,
         4.16,  12.7,  -1.2, 1.0, 1.0,  0.0,

          3.85,  12.27,  -1.6, 1.0, 1.0,  0.0,
          4.15,  12.271,  -1.2, 1.0, 1.0,  0.0,
         3.85,  12.31,  -1.6, 1.0, 1.0,  0.0,
          4.15,  12.31,  -1.2, 1.0, 1.0,  0.0,

         3.85,  12.7,  -1.6, 1.0, 1.0,  0.0,
          4.15,  12.7,  -1.3, 1.0, 1.0,  0.0,
         3.85,  12.71,  -1.6, 1.0, 1.0,  0.0,
          4.15,  12.71,  -1.3, 1.0, 1.0,  0.0,

    };
#pragma endregion
#pragma region Buffing


    unsigned int stride = 6 * sizeof(float); //Korak pri kretanju po podacima o tjemenima = Koliko mjesta u memoriji izmedju istih komponenti susjednih tjemena
    unsigned int VBO, cubeVao;
    glGenVertexArrays(1, &cubeVao);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(cubeVao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    unsigned int smokeVao, smokeVbo;
    glGenVertexArrays(1, &smokeVao);
    glBindVertexArray(smokeVao);
    glGenBuffers(1, &smokeVbo);
    glBindBuffer(GL_ARRAY_BUFFER, smokeVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(smokeVertices), smokeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    
#pragma endregion

    // depth and adequate culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    float smokeTranslate = 0.0f;
    float sunColor = 1.0f;
    float doorRotationAngle = 0.0f;
    float doorFlag = 1;
    float randomX = 0;
    float randomZ = 0;
    std::vector<float> smokeTranslates = { -1.0f, 0.0f, 1.2f };
    bool isTransparent = false;


    bool light1Status = true;
    bool light2Status = true;
    double lastPressTime1 = 0.0;
    double lastPressTime2 = 0.0;
    double debounceDelay = 0.5;
    while (!glfwWindowShouldClose(window))
    {
#pragma region Keys
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            if (glfwGetTime() - lastPressTime1 > debounceDelay)
            {
                light1Status = !light1Status;
                lastPressTime1 = glfwGetTime(); 
            }
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            if (glfwGetTime() - lastPressTime2 > debounceDelay)
            {
                light2Status = !light2Status;
                lastPressTime2 = glfwGetTime(); 
            }
        }
        camera.MovementSpeed = 8.0f;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera.MovementSpeed = 15.0f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, 0.02f);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, 0.02f);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, 0.02f);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, 0.02f);
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        {
            isTransparent = true;
        }
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
            isTransparent = false;
        }
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        {
            
            doorRotationAngle += 20.0f * 0.03;
           

        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        {
               doorRotationAngle -= 20.0f * 0.03;
             
        }
       
        lightingShader.use();
        lightingShader.setBool("light1Status", light1Status);
        lightingShader.setBool("light2Status", light2Status);
#pragma endregion

        if (doorRotationAngle > 60) {
            doorRotationAngle = 60;
        }
        if (doorRotationAngle < 8) {
            doorRotationAngle = 8;
        }
        
        glClearColor(0.529f, 0.804f, 0.922f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // camera setup
        lightingShader.use();
        lightingShader.setMat4("view", camera.GetViewMatrix());
        lightingShader.setMat4("projection", projection);
        lightingShader.setVec3("viewPos", camera.Position);
        glUseProgram(0);
        lightCubeShader.use();
        lightCubeShader.setMat4("view", camera.GetViewMatrix());
        lightCubeShader.setMat4("projection", projection);
        glUseProgram(0);

       // man model setup
        lightingShader.use();
        glm::vec3 manPosition = glm::vec3(3.0f, 0.1f, -5.6f);
        glm::mat4 manModel = glm::mat4(1.0f);
        manModel = glm::translate(manModel, manPosition);
        glm::vec3 manScale = glm::vec3(1.0f);
        manModel = glm::scale(manModel, manScale);
        lightingShader.setMat4("model", manModel);
        male.Draw(lightingShader);
        glUseProgram(0);

        lightingShader.use();
        glm::vec3 femalePos = glm::vec3(0.0f, 4.5f, -5.4f);
        glm::mat4 femaleModel = glm::mat4(1.0f);
        femaleModel = glm::translate(femaleModel, femalePos);
        femaleModel = glm::rotate(femaleModel,glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        glm::vec3 femaleScale = glm::vec3(0.01f);
        femaleModel = glm::scale(femaleModel, femaleScale);
        lightingShader.setMat4("model", femaleModel);
        woman.Draw(lightingShader);
        glUseProgram(0);
    
        // window setup
        lightingShader.use();
        glDisable(GL_CULL_FACE);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        if (isTransparent) {
            lightingShader.setBool("isWindowTransparent", true);

        }
        else {
            lightingShader.setBool("isWindowTransparent", false);


        }
        lightingShader.setBool("isWindowDraw", true);

        glBindVertexArray(cubeVao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, windowTexture);
        glm::mat4 windowModel = glm::mat4(1.0f);
        windowModel = glm::translate(windowModel, glm::vec3(-1.0f, 2.0f, -3.5f));
        windowModel = glm::rotate(windowModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        windowModel = glm::scale(windowModel, glm::vec3(0.3f, 1.6f, 1.6f));
        lightingShader.setMat4("model", windowModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        windowModel = glm::mat4(1.0f);
        windowModel = glm::translate(windowModel, glm::vec3(5.0f, 2.0f, -3.5f));
        windowModel = glm::rotate(windowModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        windowModel = glm::scale(windowModel, glm::vec3(0.3f, 1.6f, 1.6f));
        lightingShader.setMat4("model", windowModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        windowModel = glm::mat4(1.0f);
        windowModel = glm::translate(windowModel, glm::vec3(-1.0f, 6.5f, -3.5f));
        windowModel = glm::rotate(windowModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        windowModel = glm::scale(windowModel, glm::vec3(0.3f, 1.6f, 1.6f));
        lightingShader.setMat4("model", windowModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        windowModel = glm::mat4(1.0f);
        windowModel = glm::translate(windowModel, glm::vec3(5.0f, 6.5f, -3.5f));
        windowModel = glm::rotate(windowModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        windowModel = glm::scale(windowModel, glm::vec3(0.3f, 1.6f, 1.6f));
        lightingShader.setMat4("model", windowModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        lightingShader.setBool("isWindowDraw", false);
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glUseProgram(0);

        // grass setup
        lightingShader.use();
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, grassSpecularTexture);
        glm::mat4 groundModel = glm::mat4(1.0f);
        groundModel = glm::translate(groundModel, glm::vec3(2.0f, -0.6f, 0.0f));
        groundModel = glm::scale(groundModel, glm::vec3(49.0, 1.0, 35.0));
        lightingShader.setMat4("model", groundModel);
        glBindVertexArray(cubeVao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 36);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);

#pragma region House
        // house setup
        lightingShader.use();
        glBindVertexArray(cubeVao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wall);
        glm::mat4 houseModel = glm::mat4(1.0f);
        houseModel = glm::translate(houseModel, glm::vec3(-4.0f, 2.0f, -6.0f));
        houseModel = glm::scale(houseModel, glm::vec3(0.2f, 4.6f, 5.0f));
        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        houseModel = glm::mat4(1.0f);
        houseModel = glm::translate(houseModel, glm::vec3(8.0f, 2.0f, -6.0f));
        houseModel = glm::scale(houseModel, glm::vec3(0.2f, 4.6f, 5.0f));

        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        houseModel = glm::mat4(1.0f);
        houseModel = glm::translate(houseModel, glm::vec3(7.0f, 6.3f, -6.0f));
        houseModel = glm::scale(houseModel, glm::vec3(0.2f, 3.9f, 5.0f));

        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        houseModel = glm::mat4(1.0f);
        houseModel = glm::translate(houseModel, glm::vec3(-3.0f, 6.3f, -6.0f));
        houseModel = glm::scale(houseModel, glm::vec3(0.2f, 3.9f, 5.0f));

        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        houseModel = glm::mat4(1.0f);
        houseModel = glm::rotate(houseModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        houseModel = glm::translate(houseModel, glm::vec3(8.4f, 2.1f, 2.0f));
        houseModel = glm::scale(houseModel, glm::vec3(0.2f, 4.2f, 12.2f));

        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        houseModel = glm::mat4(1.0f);
        houseModel = glm::translate(houseModel, glm::vec3(2.0f, 0.0f, -6.0f));
        houseModel = glm::scale(houseModel, glm::vec3(11.8f, 0.2f,5.0f));
        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        houseModel = glm::mat4(1.0f);
        houseModel = glm::translate(houseModel, glm::vec3(2.0f, 4.3f, -6.0f));
        houseModel = glm::scale(houseModel, glm::vec3(11.8f, 0.2f, 5.1f));
        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        houseModel = glm::mat4(1.0f);
        houseModel = glm::rotate(houseModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        houseModel = glm::translate(houseModel, glm::vec3(8.4f, 6.3f, 2.0f));
        houseModel = glm::scale(houseModel, glm::vec3(0.2f, 3.9f, 10.2f));

        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);
       

        houseModel = glm::mat4(1.0f);
        houseModel = glm::rotate(houseModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        houseModel = glm::translate(houseModel, glm::vec3(3.5f, 2.1f, 2.0f));
        houseModel = glm::scale(houseModel, glm::vec3(0.1f, 4.2f, 12.2f));

        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        houseModel = glm::mat4(1.0f);
        houseModel = glm::rotate(houseModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        houseModel = glm::translate(houseModel, glm::vec3(3.5f, 6.3f, 2.0f));
        houseModel = glm::scale(houseModel, glm::vec3(0.1f, 3.9f, 10.2f));

        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        houseModel = glm::mat4(1.0f);
        houseModel = glm::translate(houseModel, glm::vec3(2.0f, 0.07f, -6.0f));
        houseModel = glm::scale(houseModel, glm::vec3(11.7f, 0.1f, 4.9f));
        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        houseModel = glm::mat4(1.0f);
        houseModel = glm::translate(houseModel, glm::vec3(2.0f, 4.5f, -6.0f));
        houseModel = glm::scale(houseModel, glm::vec3(9.8f, 0.1f, 4.9f));
        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, roofTexture);
        houseModel = glm::mat4(1.0f);
        houseModel = glm::translate(houseModel, glm::vec3(2.0f, 8.35f, -6.0f));
        houseModel = glm::scale(houseModel, glm::vec3(11.8f, 0.2f, 5.1f));
        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_2D, 0);

#pragma endregion

#pragma region Tapestry

        // tapestry setup
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tapestry1Texture);
        houseModel = glm::mat4(1.0f);
        houseModel = glm::translate(houseModel, glm::vec3(-3.9f, 2.15f, -5.9f));
        houseModel = glm::scale(houseModel, glm::vec3(0.1f, 4.1f, 4.76f));
        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tapestry3Texture);
        houseModel = glm::mat4(1.0f);
        houseModel = glm::translate(houseModel, glm::vec3(7.9f, 2.15f, -5.9f));
        houseModel = glm::scale(houseModel, glm::vec3(0.1f, 4.1f, 4.76f));
        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tapestry2Texture);
        houseModel = glm::mat4(1.0f);
        houseModel = glm::rotate(houseModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        houseModel = glm::translate(houseModel, glm::vec3(8.3f, 2.15f, 2.0f));
        houseModel = glm::scale(houseModel, glm::vec3(0.1f, 4.1f, 11.9f));
        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tapestry7Texture);
        houseModel = glm::mat4(1.0f);
        houseModel = glm::rotate(houseModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        houseModel = glm::translate(houseModel, glm::vec3(3.56f, 2.15f, 2.0f));
        houseModel = glm::scale(houseModel, glm::vec3(0.01f, 4.1f, 11.9f));
        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tapestry7Texture);
        houseModel = glm::mat4(1.0f);
        houseModel = glm::rotate(houseModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        houseModel = glm::translate(houseModel, glm::vec3(3.56f, 6.2f, 2.0f));
        houseModel = glm::scale(houseModel, glm::vec3(0.01f, 4.2f, 9.9f));
        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tapestry4Texture);
        houseModel = glm::mat4(1.0f);
        houseModel = glm::translate(houseModel, glm::vec3(-2.9f, 6.4f, -5.9f));
        houseModel = glm::scale(houseModel, glm::vec3(0.1f, 3.9f, 4.76f));
        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tapestry5Texture);
        houseModel = glm::mat4(1.0f);
        houseModel = glm::translate(houseModel, glm::vec3(6.9f, 6.4f, -5.9f));
        houseModel = glm::scale(houseModel, glm::vec3(0.1f, 3.9f, 4.76f));
        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tapestry6Texture);
        houseModel = glm::mat4(1.0f);
        houseModel = glm::rotate(houseModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        houseModel = glm::translate(houseModel, glm::vec3(8.3f, 6.4f, 2.0f));
        houseModel = glm::scale(houseModel, glm::vec3(0.1f, 3.9f, 9.9f));
        lightingShader.setMat4("model", houseModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_2D, 0);
#pragma endregion

        // reflector light source setup
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, reflectorTexture);
        glm::mat4 reflector = glm::mat4(1.0f);
        reflector = glm::translate(reflector, glm::vec3(2.0f, 3.3f, -3.3f));
        reflector = glm::scale(reflector, glm::vec3(2.0f, 0.2f, 0.2f));
        lightingShader.setMat4("model", reflector);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, neonTexture);
        reflector = glm::mat4(1.0f);
        reflector = glm::translate(reflector, glm::vec3(2.0f, 3.15f, -3.3f));
        reflector = glm::scale(reflector, glm::vec3(2.0f, 0.1f, 0.2f));
        lightingShader.setMat4("model", reflector);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindTexture(GL_TEXTURE_2D, 0);

        // house door setup
        lightingShader.use();
        glm::vec3 houseDoorPosition = glm::vec3(2.0f, 3.0f, -3.43f);
        glm::mat4 houseDoorModelF = glm::mat4(1.0f);
        houseDoorModelF = glm::translate(houseDoorModelF, houseDoorPosition);

        glm::vec3 hdoorScale = glm::vec3(0.02f, 0.015f, 0.02f);
        houseDoorModelF = glm::scale(houseDoorModelF, hdoorScale);
        houseDoorModelF = glm::rotate(houseDoorModelF, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        lightingShader.setMat4("model", houseDoorModelF);

        houseDoor.Draw(lightingShader);

        houseDoorPosition = glm::vec3(2.0f, 3.0f, -3.56f);
         houseDoorModelF = glm::mat4(1.0f);
        houseDoorModelF = glm::translate(houseDoorModelF, houseDoorPosition);

         hdoorScale = glm::vec3(0.02f, 0.015f, 0.03f);
        houseDoorModelF = glm::scale(houseDoorModelF, hdoorScale);
        houseDoorModelF = glm::rotate(houseDoorModelF, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        lightingShader.setMat4("model", houseDoorModelF);

        houseDoor.Draw(lightingShader);

        glUseProgram(0);
        glBindVertexArray(0);

       
        // smoke setup
        float smokeTime = glfwGetTime();
        float timeSinceLastMove = std::fmod(smokeTime, 1.0f);

       
        for (int i = 0; i < smokeTranslates.size(); ++i) {
            if (timeSinceLastMove < 0.03f) {
                smokeTranslates[i] += 0.7f;
                randomX = (static_cast<float>(rand()) / RAND_MAX) * 0.2f - 0.1f;
                randomZ = (static_cast<float>(rand()) / RAND_MAX) * 0.2f - 0.1f;
            }
            if (smokeTranslates[0] > 3.0f) {
                smokeTranslates = { -1.0f, 0.0f, 1.2f };
            }
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glm::mat4 smokeModel = glm::mat4(1.0f);

        for (int i = 0; i < smokeTranslates.size(); ++i) {
            smokeModel = glm::mat4(1.0f);
            smokeModel = glm::translate(smokeModel, glm::vec3(1.95f + randomX, smokeTranslates[i], -5.0f + randomZ));

            lightingShader.use();
            lightingShader.setMat4("model", smokeModel);
            lightingShader.setBool("displaySmoke", true);
            glBindVertexArray(smokeVao);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
            glDrawArrays(GL_TRIANGLE_STRIP, 8, 8);
            glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
            glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
            lightingShader.setBool("displaySmoke", false);

            glUseProgram(0);
        }

        glDisable(GL_BLEND);

        //chimney block setup
        lightingShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, chimneyTexture);

        glm::mat4 chimneyModel = glm::mat4(1.0f);
        chimneyModel = glm::translate(chimneyModel, glm::vec3(6.0f, 9.7f, -5.85f));
        chimneyModel = glm::scale(chimneyModel, glm::vec3(1.0f, 2.5f, 0.1f));

        lightingShader.setMat4("model", chimneyModel);
        glBindVertexArray(cubeVao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 36);

        chimneyModel = glm::mat4(1.0f);
        chimneyModel = glm::translate(chimneyModel, glm::vec3(6.0f, 9.7f, -6.95f));
        chimneyModel = glm::scale(chimneyModel, glm::vec3(1.0f, 2.5f, 0.1f));

        lightingShader.setMat4("model", chimneyModel);
        glBindVertexArray(cubeVao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 36);

        chimneyModel = glm::mat4(1.0f);
        chimneyModel = glm::translate(chimneyModel, glm::vec3(5.5f, 9.7f, -6.4f));
        chimneyModel = glm::rotate(chimneyModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        chimneyModel = glm::scale(chimneyModel, glm::vec3(1.0f, 2.5f, 0.1f));

        lightingShader.setMat4("model", chimneyModel);
        glBindVertexArray(cubeVao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 36);
        chimneyModel = glm::mat4(1.0f);
        chimneyModel = glm::translate(chimneyModel, glm::vec3(6.5f, 9.7f, -6.4f));
        chimneyModel = glm::rotate(chimneyModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        chimneyModel = glm::scale(chimneyModel, glm::vec3(1.0f, 2.5f, 0.1f));

        lightingShader.setMat4("model", chimneyModel);
        glBindVertexArray(cubeVao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 36);

        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
        glEnable(GL_CULL_FACE);

        float currentTime = glfwGetTime();

        // Update sun
        float sunRotation = currentTime * sunRotationSpeed;
        float sunPulse = 0.5f * (sin(currentTime * sunPulseSpeed) + 1.0f);
        float sunScale = glm::mix(sunMinScale, sunMaxScale, sunPulse);
        glm::mat4 sunModel = glm::mat4(1.0f);

        sunModel = glm::translate(sunModel, sunPosition);
        sunModel = glm::rotate(sunModel, glm::radians(sunRotation), glm::vec3(0.0f, 1.0f, 0.0f));
        sunModel = glm::scale(sunModel, glm::vec3(sunScale));
        lightingShader.use();
        lightingShader.setBool("displayWhite", true);
        sunColor = 1.0f - 0.7f * sin(glfwGetTime());

        lightingShader.setVec4("uSunColor", glm::vec4(1.0f, sunColor, 0.0f, 1.0f));
        lightingShader.setVec3("uViewPos", camera.Position);

        lightingShader.setMat4("model", sunModel);
        sun.Draw(lightingShader);
        lightingShader.setBool("displayWhite", false);
        //lightingShader.setVec3("dirLight.color", glm::vec4(1.0f, sunColor, 0.0f, 1.0f));

        glUseProgram(0);


        lightingShader.use();
        glm::mat4 model = glm::mat4(1.0f);

        lightingShader.setMat4("model", model);

        tree.Draw(lightingShader);
        glUseProgram(0);
        lightingShader.use();

        glm::vec3 doorPosition = glm::vec3(2.8f, 0.88f, 7.8f);
        glm::mat4 doorModelF = glm::mat4(1.0f);
        doorModelF = glm::translate(doorModelF, doorPosition);
    
        glm::vec3 doorScale = glm::vec3(0.011f, 0.008f, 0.02f);
        doorModelF = glm::scale(doorModelF, doorScale);
        doorModelF = glm::rotate(doorModelF, -glm::radians(doorRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));

        lightingShader.setMat4("model", doorModelF);
        lightingShader.setBool("isDoorDraw", true);

        door.Draw(lightingShader);
        lightingShader.setBool("isDoorDraw", false);

        glUseProgram(0);

        lightingShader.use();
        glm::vec3 fencePosition = glm::vec3(-23.0f, 0.0f, 8.0f);
        glm::mat4 fenceModel = glm::mat4(1.0f);
        fenceModel = glm::translate(fenceModel, fencePosition);
        float fenceRotationAngle = glm::radians(10.0f);
        fenceModel = glm::rotate(fenceModel, fenceRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 fenceScale = glm::vec3(15.0f, 1.0f, 1.0f);
        fenceModel = glm::scale(fenceModel, fenceScale);
        lightingShader.setMat4("model", fenceModel);
        fence.Draw(lightingShader);
        glUseProgram(0);
        lightingShader.use();
        fencePosition = glm::vec3(4.0f, 0.0f, 8.0f);
        fenceModel = glm::mat4(1.0f);
        fenceModel = glm::translate(fenceModel, fencePosition);
        fenceRotationAngle = glm::radians(10.0f);
        fenceModel = glm::rotate(fenceModel, fenceRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        fenceScale = glm::vec3(12.8f, 1.0f, 1.0f);
        fenceModel = glm::scale(fenceModel, fenceScale);
        lightingShader.setMat4("model", fenceModel);
        fence.Draw(lightingShader);
        glUseProgram(0);


       
        currentTime = glfwGetTime();
        float orbitAngle = currentTime * orbitSpeed;
        float dogX = 1.0f +  glm::cos(orbitAngle) * orbitRadius;
        float dogZ = -4.0f + glm::sin(orbitAngle) * orbitRadius;

        // Update the model matrix for the dog's position, scale, and rotation

        glm::vec3 dogScale = glm::vec3(0.03f);

        float orientationAngle = -glm::atan(dogZ, dogX) + 0.5;  

        glm::mat4 dogModel = glm::mat4(1.0f);
        dogModel = glm::translate(dogModel, glm::vec3(dogX, 0.0f, dogZ));
        dogModel = glm::rotate(dogModel, orientationAngle, glm::vec3(0.0f, 1.0f, 0.0f));  
        dogModel = glm::rotate(dogModel, glm::radians(-124.0f), glm::vec3(1.0f, 1.0f, 1.0f)); 
        dogModel = glm::scale(dogModel, dogScale);
        lightingShader.use();

        lightingShader.setBool("isDogDraw", true);

        lightingShader.setVec3("pointLights[2].position", glm::vec3(dogX, 1.0f, dogZ));
        lightingShader.setVec3("pointLights[2].color", glm::vec3(1.0, 0.0, 1.0));
        float pulsation = 4.0f; 

        float intensity = 3.0f + 3.0f * sin(currentTime * pulsation);

        lightingShader.setVec3("pointLights[2].intensity", glm::vec3(intensity, intensity, intensity));        
 
        lightingShader.setMat4("model", dogModel);
        dog.Draw(lightingShader);
        lightingShader.setBool("isDogDraw", false);

        glUseProgram(0);


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        nameShader.use();

        glBindVertexArray(nameVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, name);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, 0);

        glUseProgram(0);
        glBindVertexArray(0);


        lightCubeShader.use();

        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 2; i++)
        {
            glm::mat4 cubeModel = glm::mat4(1.0f);
            cubeModel = glm::translate(cubeModel, pointLightPositions[i]);
            cubeModel = glm::scale(cubeModel, glm::vec3(0.2f)); 
            lightCubeShader.setMat4("model", cubeModel);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        glBindVertexArray(cubeVao);
        lightingShader.use();

        glm::mat4 cubeModel = glm::mat4(1.0f);
        cubeModel = glm::translate(cubeModel, pointLightPositions[0]);
        cubeModel = glm::translate(cubeModel, glm::vec3(0.0f, 0.3f, 0.0f));
        cubeModel = glm::scale(cubeModel, glm::vec3(0.1f, 0.35f, 0.1f));
        lightingShader.setMat4("model", cubeModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);
         cubeModel = glm::mat4(1.0f);
        cubeModel = glm::translate(cubeModel, pointLightPositions[1]);
        cubeModel = glm::translate(cubeModel, glm::vec3(0.0f, 0.3f, 0.0f));
        cubeModel = glm::scale(cubeModel, glm::vec3(0.1f, 0.35f, 0.1f));
        lightingShader.setMat4("model", cubeModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
#pragma region Cleanup
    glDeleteBuffers(1, &cubeVao);
    glDeleteBuffers(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &VBO);
    glDeleteBuffers(1, &smokeVao);
    glDeleteVertexArrays(1, &smokeVbo);
    glDeleteProgram(nameShader.ID);
    glDeleteProgram(lightingShader.ID);
    glDeleteProgram(lightCubeShader.ID);
   
    glDeleteTextures(1, &grassTexture);
    glDeleteTextures(1, &wall);
    glDeleteTextures(1, &name);
    glDeleteTextures(1, &neonTexture);
    glDeleteTextures(1, &reflectorTexture);
    glDeleteTextures(1, &smokeTexture);
    glDeleteTextures(1, &chimneyTexture);
    glDeleteTextures(1, &grassSpecularTexture);
    glDeleteTextures(1, &windowTexture);
    glDeleteTextures(1, &tapestry1Texture);
    glDeleteTextures(1, &tapestry2Texture);
    glDeleteTextures(1, &tapestry3Texture);
    glDeleteTextures(1, &tapestry4Texture);
    glDeleteTextures(1, &tapestry5Texture);
    glDeleteTextures(1, &tapestry6Texture);
    glDeleteTextures(1, &tapestry7Texture);
    glDeleteTextures(1, &floorTexture);
    glDeleteTextures(1, &roofTexture);
#pragma endregion

    glfwTerminate();
    return 0;
}

static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);

        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}


