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


float orbitSpeed = 0.5f;
float orbitRadius = 5.0f;
glm::vec3 sunPosition = glm::vec3(0.0f, 9.0f, 0.0f); 
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

    sunShader.use();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f);
    sunShader.setMat4("uP", projection);

    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    sunShader.setMat4("uV", camera.GetViewMatrix());

    unifiedShader.use();
    unifiedShader.setMat4("uV", camera.GetViewMatrix());

    unifiedShader.setVec3("uLightPos", 0, 1, 3);
    unifiedShader.setVec3("uViewPos",camera.Position);
    unifiedShader.setVec3("uLightColor", 1, 1, 1);
    unifiedShader.setMat4("uP", projection);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));

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

    float groundVertices[] = {
         -25.0, -0.1,  20.0, 0.0, 1.0, 0.0,
         25.0, -0.1,  20.0, 0.0, 1.0, 0.0,
         -25.0,  0.0,  -20.0, 0.0, 1.0,  0.0,
         25.0,  0.0,  -20.0, 0.0, 1.0,  0.0,
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
           // door red
           -0.1, -0.0,  1.0f, 1.0f, 0.0f, 0.0,
           0.1, -0.0,   1.0f, 1.0f, 0.0f, 0.0,
           -0.1,  -0.1,   1.0f, 1.0f, 0.0f,  0.0,
           0.1,  -0.1,   1.0f, 1.0f, 0.0f,  0.0,
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

    };
    unsigned int stride = 6 * sizeof(float); //Korak pri kretanju po podacima o tjemenima = Koliko mjesta u memoriji izmedju istih komponenti susjednih tjemena
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
    // omogucavanje dubine
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


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

        houseShader.use();
        houseShader.setMat4("uV", camera.GetViewMatrix());
        glUseProgram(0);
        groundShader.use();
        groundShader.setMat4("uV", camera.GetViewMatrix());
        glUseProgram(0);
        unifiedShader.use();
        unifiedShader.setMat4("uV", camera.GetViewMatrix());
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
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 8);
        glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

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

        glm::vec3 manPosition = glm::vec3(3.0f, 0.0f, -3.0f);
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
        glm::vec3 treePosition = glm::vec3(0.5f, 0.0f, 0.0f);  // Set the actual position of the tree
        float dogX = treePosition.x + glm::cos(orbitAngle) * orbitRadius;
        float dogZ = treePosition.z + glm::sin(orbitAngle) * orbitRadius;

        glm::vec3 dogScale = glm::vec3(0.03f);

        // Update the model matrix for the dog's position and scale
        glm::mat4 dogModel = glm::mat4(1.0f);
        dogModel = glm::translate(dogModel, glm::vec3(dogX, 0.0f, dogZ));
        dogModel = glm::scale(dogModel, dogScale);
        dogModel = glm::rotate(dogModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

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


