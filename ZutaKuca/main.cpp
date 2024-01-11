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
float orbitRadius = 8.6f;
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
        -0.98, -0.7, 0.0, 1.0,
        -0.98, -1.0, 0.0, 0.0,
        -0.5, -0.7, 1.0, 1.0,
        -0.5, -1.0, 1.0, 0.0
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

    Model tree("models/Tree/Tree.obj");
    Model dog("models/Dog/13466_Canaan_Dog_v1_L3.obj");
    Model man("models/Man/man.obj");
    Model sun("models/Sun/sun.obj");

    Shader unifiedShader("basic.vert", "basic.frag");
    Shader sunShader("basic.vert", "cube.frag");
    Shader houseShader("basic.vert", "house.frag");
    Shader simpleShader("simple.vert", "simple.frag");
    Shader groundShader("basic.vert", "ground.frag");
    Shader chimneyShader("basic.vert", "chimney.frag");
    Shader doorShader("basic.vert", "door.frag");
    Shader smokeShader("basic.vert", "smoke.frag");



    sunShader.use();

    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f);
    sunShader.setMat4("uP", projection);

    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    sunShader.setMat4("uV", camera.GetViewMatrix());

    smokeShader.use();
    smokeShader.setMat4("uP", projection);
    smokeShader.setMat4("uV", camera.GetViewMatrix());
    doorShader.use();
    doorShader.setMat4("uP", projection);
    doorShader.setMat4("uV", camera.GetViewMatrix());
    chimneyShader.use();
    chimneyShader.setMat4("uP", projection);
    chimneyShader.setMat4("uV", camera.GetViewMatrix());

    unifiedShader.use();
    unifiedShader.setMat4("uV", camera.GetViewMatrix());

    unifiedShader.setVec3("uLightPos", 0, 1, 3);
    unifiedShader.setVec3("uViewPos",camera.Position);
    unifiedShader.setVec3("uLightColor", 1, 1, 1);
    unifiedShader.setMat4("uP", projection);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.5f, 0.0f, 0.0f));

    houseShader.use();
    houseShader.setMat4("uV", camera.GetViewMatrix());
    houseShader.setMat4("uP", projection);

    groundShader.use();
    groundShader.setMat4("uV", camera.GetViewMatrix());
    groundShader.setMat4("uP", projection);
    unsigned name = loadImageToTexture("res/imebelo.png");
    glBindTexture(GL_TEXTURE_2D, name);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);


    unsigned grassTexture = loadImageToTexture("res/trava.png");
    glBindTexture(GL_TEXTURE_2D, grassTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
#pragma region Vertices



    float groundVertices[] = {
         -25.0, -0.1,  20.0, 0.0, 1.0, 0.0,
         25.0, -0.1,  20.0, 0.0, 1.0, 0.0,
         -25.0,  0.0,  -20.0, 0.0, 1.0,  0.0,
         25.0,  0.0,  -20.0, 0.0, 1.0,  0.0,
    };
    float doorVertices[] = {
           -0.4, -0.0,  1.01f, 1.0f, 0.0f, 0.0,
           0.4, -0.0,   1.01f, 1.0f, 0.0f, 0.0,
           -0.4,  3.0,   1.01f, 1.0f, 0.0f,  0.0,
           0.4,  3.0,   1.01f, 1.0f, 0.0f,  0.0,
    };
    float houseVertices[] =
    {
        // First Floor (Yellow)
        -5.0, -0.0,  1.0, 1.0, 1.0, 0.0,
         5.3, -0.0,  1.0, 1.0, 1.0, 0.0,
         -5.0,  4.5,  1.0, 1.0, 1.0,  0.0,
         5.3,  4.5,  1.0, 1.0, 1.0,  0.0,

         // Second Floor (Yellow)
         -4.2,  4.5,  1.0, 1.0, 1.0,  0.0,
          4.2,  4.5,  1.0, 1.0, 1.0,  0.0,
         -4.2,  8.2,  1.0, 1.0, 1.0,  0.0,
          4.2,  8.2,  1.0, 1.0, 1.0,  0.0,
           
        // first floor
        -5.0, -0.0,  -4.0, 1.0, 1.0, 0.0,
         5.3, -0.0,  -4.0, 1.0, 1.0, 0.0,
         -5.0,  4.5,  -4.0, 1.0, 1.0,  0.0,
         5.3,  4.5,  -4.0, 1.0, 1.0,  0.0,

         // Second Floor (Yellow)
        -4.2,  4.5,  -4.0, 1.0, 1.0,  0.0,
         4.2,  4.5,  -4.0, 1.0, 1.0,  0.0,
        -4.2,  8.2,  -4.0, 1.0, 1.0,  0.0,
         4.2,  8.2,  -4.0, 1.0, 1.0,  0.0,

         // flat roof
           -4.4,  8.2,  -4.2, 1.0, 1.0,  0.0,
          4.4,  8.2,  -4.2, 1.0, 1.0,  0.0,
         -4.4,  8.15,  1.2, 1.0, 1.0,  0.0,

          4.4,  8.15,  1.2, 1.0, 1.0,  0.0,

          // first floor parket
          -5.0,  0.0,  -4.0, 1.0, 1.0,  0.0,
         5.3,  0.0,  -4.0, 1.0, 1.0,  0.0,
        -5.0,  0.01,  1.0, 1.0, 1.0,  0.0,
         5.3,  0.01,  1.0, 1.0, 1.0,  0.0,

          // left side of house top
        -4.20,  4.5,  -4.0, 1.0, 1.0,  0.0,
         -4.21,  8.2,  -4.0, 1.0, 1.0,  0.0,
        -4.20,  4.5,  1.0, 1.0, 1.0,  0.0,
         -4.21,  8.2,  1.0, 1.0, 1.0,  0.0,
         // right side
         4.2,  4.5,  -4.0, 1.0, 1.0,  0.0,
         4.21,  8.2,  -4.0, 1.0, 1.0,  0.0,
        4.2,  4.5,  1.0, 1.0, 1.0,  0.0,
         4.21,  8.2,  1.0, 1.0, 1.0,  0.0,

         // left side of house botton
      -4.99,  0.0,  -4.0, 1.0, 1.0,  0.0,
       -5.0,  4.5,  -4.0, 1.0, 1.0,  0.0,
      -4.99,  0.0,  1.0, 1.0, 1.0,  0.0,
       -5.0,  4.5,  1.0, 1.0, 1.0,  0.0,
       // right side
       5.3,  0.0,  -4.0, 1.0, 1.0,  0.0,
       5.29,  4.5,  -4.0, 1.0, 1.0,  0.0,
      5.3,  0.0,  1.0, 1.0, 1.0,  0.0,
       5.29,  4.5,  1.0, 1.0, 1.0,  0.0,

       // second floor parket
         -5.0,  4.5,  -4.0, 1.0, 1.0,  0.0,
        5.3,  4.49,  -4.0, 1.0, 1.0,  0.0,
       -5.0,  4.5,  1.0, 1.0, 1.0,  0.0,
        5.3,  4.49,  1.0, 1.0, 1.0,  0.0,
    };

    float chimneyVertices[] =
    {
         // Second Floor (Yellow)
         3.7,  8.2,  -1.0, 1.0, 1.0,  0.0,
          4.2,  8.2,  -1.0, 1.0, 1.0,  0.0,
         3.7,  12.4,  -1.0, 1.0, 1.0,  0.0,
          4.2,  12.4,  -1.0, 1.0, 1.0,  0.0,

          3.7,  8.2,  -2.0, 1.0, 1.0,  0.0,
           4.2,  8.2,  -2.0, 1.0, 1.0,  0.0,
          3.7,  12.4,  -2.0, 1.0, 1.0,  0.0,
           4.2,  12.4,  -2.0, 1.0, 1.0,  0.0,

           // left side of house top
          3.7,  8.2,  -2.0, 1.0, 1.0,  0.0,
          3.71,  12.4,  -2.0, 1.0, 1.0,  0.0,
          3.7,  8.2,  -1.0, 1.0, 1.0,  0.0,
          3.71,  12.4,  -1.0, 1.0, 1.0,  0.0,
          // right side
          4.2,  8.2,  -2.0, 1.0, 1.0,  0.0,
          4.21,  12.4,  -2.0, 1.0, 1.0,  0.0,
         4.2,  8.2,  -1.0, 1.0, 1.0,  0.0,
          4.21,  12.4,  -1.0, 1.0, 1.0,  0.0,

    };
    float smokeVertices[] =
    {
        3.9,  12.27,  -1.3, 1.0, 1.0,  0.0,
         4.1,  12.27,  -1.3, 1.0, 1.0,  0.0,
        3.9,  12.7,  -1.3, 1.0, 1.0,  0.0,
         4.1,  12.7,  -1.3, 1.0, 1.0,  0.0,

         3.9,  12.27,  -1.6, 1.0, 1.0,  0.0,
          4.1,  12.27,  -1.6, 1.0, 1.0,  0.0,
         3.9,  12.7,  -1.6, 1.0, 1.0,  0.0,
          4.1,  12.7,  -1.6, 1.0, 1.0,  0.0,

         3.9,  12.27,  -1.6, 1.0, 1.0,  0.0,
         3.91,  12.7,  -1.6, 1.0, 1.0,  0.0,
         3.9,  12.27,  -1.3, 1.0, 1.0,  0.0,
         3.91,  12.7,  -1.3, 1.0, 1.0,  0.0,

         4.1,  12.27,  -1.6, 1.0, 1.0,  0.0,
         4.11,  12.7,  -1.6, 1.0, 1.0,  0.0,
        4.1,  12.27,  -1.3, 1.0, 1.0,  0.0,
         4.11,  12.7,  -1.3, 1.0, 1.0,  0.0,

          3.9,  12.27,  -1.6, 1.0, 1.0,  0.0,
          4.1,  12.271,  -1.3, 1.0, 1.0,  0.0,
         3.9,  12.31,  -1.6, 1.0, 1.0,  0.0,
          4.1,  12.31,  -1.3, 1.0, 1.0,  0.0,

         3.9,  12.7,  -1.6, 1.0, 1.0,  0.0,
          4.1,  12.7,  -1.3, 1.0, 1.0,  0.0,
         3.9,  12.71,  -1.6, 1.0, 1.0,  0.0,
          4.1,  12.71,  -1.3, 1.0, 1.0,  0.0,

    };
#pragma endregion
#pragma region Buffing


    unsigned int stride = 6 * sizeof(float); //Korak pri kretanju po podacima o tjemenima = Koliko mjesta u memoriji izmedju istih komponenti susjednih tjemena
    unsigned int smokeVao, smokeVbo;
    glGenVertexArrays(1, &smokeVao);
    glBindVertexArray(smokeVao);

    glGenBuffers(1, &smokeVbo);
    glBindBuffer(GL_ARRAY_BUFFER, smokeVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(smokeVertices), smokeVertices, GL_STATIC_DRAW);

    // Set up attribute pointers for the house
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind buffers for the house
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    unsigned int doorVao, doorVbo;
    glGenVertexArrays(1, &doorVao);
    glBindVertexArray(doorVao);

    glGenBuffers(1, &doorVbo);
    glBindBuffer(GL_ARRAY_BUFFER, doorVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(doorVertices), doorVertices, GL_STATIC_DRAW);

    // Set up attribute pointers for the house
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind buffers for the house
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    unsigned int houseVAO, houseVBO;
    glGenVertexArrays(1, &houseVAO);
    glBindVertexArray(houseVAO);

    glGenBuffers(1, &houseVBO);
    glBindBuffer(GL_ARRAY_BUFFER, houseVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(houseVertices), houseVertices, GL_STATIC_DRAW);

    // Set up attribute pointers for the house
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind buffers for the house
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int chimneyVao, chimneyVbo;
    glGenVertexArrays(1, &chimneyVao);
    glBindVertexArray(chimneyVao);

    glGenBuffers(1, &chimneyVbo);
    glBindBuffer(GL_ARRAY_BUFFER, chimneyVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(chimneyVertices), chimneyVertices, GL_STATIC_DRAW);

    // Set up attribute pointers for the house
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind buffers for the house
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int groundVao, groundVbo;
    glGenVertexArrays(1, &groundVao);
    glBindVertexArray(groundVao);

    glGenBuffers(1, &groundVbo);
    glBindBuffer(GL_ARRAY_BUFFER, groundVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    // Set up attribute pointers for the house
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind buffers for the house
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
#pragma endregion

    // omogucavanje dubine
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    float smokeTranslate = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
       
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, 0.02f);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, 0.02f);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, 0.02f);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, 0.02f);

        glClearColor(0.529f, 0.804f, 0.922f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        smokeShader.use();
        smokeShader.setMat4("uV", camera.GetViewMatrix());
        glUseProgram(0);

        chimneyShader.use();
        chimneyShader.setMat4("uV", camera.GetViewMatrix());
        chimneyShader.setVec4("chColooor", glm::vec4(0.6, 0.4, 0.2, 1.0));
        glUseProgram(0);
        houseShader.use();
        houseShader.setMat4("uV", camera.GetViewMatrix());
        glUseProgram(0);
        groundShader.use();
        groundShader.setMat4("uV", camera.GetViewMatrix());
        glUseProgram(0);
        unifiedShader.use();
        unifiedShader.setMat4("uV", camera.GetViewMatrix());
        glUseProgram(0);

        doorShader.use();
        doorShader.setMat4("uV", camera.GetViewMatrix());
        glUseProgram(0);

        groundShader.use();
        glm::mat4 groundModel = glm::mat4(1.0f);
        groundModel = glm::translate(groundModel, glm::vec3(2.0f, 0.0f, -5.0f));
        groundShader.setMat4("uM", groundModel);
        glBindVertexArray(groundVao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glUseProgram(0);

        glm::mat4 houseModel = glm::mat4(1.0f);
        houseModel = glm::translate(houseModel, glm::vec3(2.0f, 0.0f, -5.0f));
       
        glDisable(GL_CULL_FACE);
        houseShader.use();

        houseShader.setMat4("uM", houseModel);
        glBindVertexArray(houseVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 24, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 28, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 32, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 36, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 40, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 44, 4);

        glUseProgram(0);
        
        float smokeTime = glfwGetTime();
        float timeSinceLastMove = std::fmod(smokeTime, 1.0f);

        if (timeSinceLastMove < 0.03f) { 
            smokeTranslate += 0.5f; 
        }
        if (smokeTranslate > 3.0f) {
            smokeTranslate = 0.0f;
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glm::mat4 smokeModel = glm::mat4(1.0f);
        smokeModel = glm::translate(smokeModel, glm::vec3(2.0f, smokeTranslate, -5.0f));
        smokeShader.use();

        smokeShader.setMat4("uM", smokeModel);
        glBindVertexArray(smokeVao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 8);
        glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
        glDisable(GL_BLEND);

        glUseProgram(0);

        glm::mat4 doorModel = glm::mat4(1.0f);
        doorModel = glm::translate(doorModel, glm::vec3(2.0f, 0.0f, -5.0f));
        doorShader.use();
        doorShader.setMat4("uM", doorModel);
        glBindVertexArray(doorVao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glUseProgram(0);

        glm::mat4 chimneyModel = glm::mat4(1.0f);
        chimneyModel = glm::translate(chimneyModel, glm::vec3(2.0f, 0.0f, -5.0f));
        chimneyShader.use();

        chimneyShader.setMat4("uM", chimneyModel);
        glBindVertexArray(chimneyVao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 16);

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
        unifiedShader.use();
        unifiedShader.setBool("displayWhite", true);
        unifiedShader.setVec3("uViewPos", camera.Position);

        unifiedShader.setMat4("uM", sunModel);
        sun.Draw(unifiedShader);
        unifiedShader.setBool("displayWhite", false);
        glUseProgram(0);


        unifiedShader.use();
        unifiedShader.setMat4("uM", model);

        tree.Draw(unifiedShader);
        glUseProgram(0);

        unifiedShader.use();

        glm::vec3 manPosition = glm::vec3(3.0f, 0.1f, -4.3f);
        glm::mat4 manModel = glm::mat4(1.0f);
        manModel = glm::translate(manModel, manPosition);
        glm::vec3 manScale = glm::vec3(0.4f);
        manModel = glm::scale(manModel, manScale);
        unifiedShader.setMat4("uM", manModel);
        man.Draw(unifiedShader);
        glUseProgram(0);

        unifiedShader.use();
       
        currentTime = glfwGetTime();
        float orbitAngle = currentTime * orbitSpeed;
        float dogX = 1.0f +  glm::cos(orbitAngle) * orbitRadius;
        float dogZ = -4.0f + glm::sin(orbitAngle) * orbitRadius;

        glm::vec3 dogScale = glm::vec3(0.03f);

        // Update the model matrix for the dog's position and scale
        float orientationAngle = -glm::atan(dogZ, dogX);  // Negate the angle

        // Update the model matrix for the dog's position, scale, and rotation
        glm::mat4 dogModel = glm::mat4(1.0f);
        dogModel = glm::translate(dogModel, glm::vec3(dogX, 0.0f, dogZ));
        dogModel = glm::rotate(dogModel, orientationAngle, glm::vec3(0.0f, 1.0f, 0.0f));  // Rotate around the y-axis
        dogModel = glm::rotate(dogModel, glm::radians(-124.0f), glm::vec3(1.0f, 1.0f, 1.0f));  // Additional rotation
        dogModel = glm::scale(dogModel, dogScale);
        // Set the model matrix in the shader
        unifiedShader.setMat4("uM", dogModel);

        // Draw the dog model
        dog.Draw(unifiedShader);
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


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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


