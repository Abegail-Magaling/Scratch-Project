#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


//Modifier for the model's x Position
float x_mod = 0.0f;
float z_mod = -1.0f;


void Key_Callback(GLFWwindow* window // Pointer to window
    , int key// keycode of press
    , int scancode // physical postion of the press
    , int action // either press / release
    , int mods) // which modifier keys are held down
{
    if (key == GLFW_KEY_D /*&& action == GLFW_PRESS*/)
    {
        //Move
        x_mod += 0.1f;
    }
    if (key == GLFW_KEY_A /*&& action == GLFW_PRESS*/)
    {
        //Move
        x_mod -= 0.1f;
    }
    if (key == GLFW_KEY_W /*&& action == GLFW_PRESS*/)
    {
        z_mod -= -0.1f;
    }
}

int main(void)
{
    //load the vert shader file into a string stream
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;

    //Add the file stream to the string stream
    vertBuff << vertSrc.rdbuf();

    //Convert the stream to a character array
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    //load the frag shader file into a string stream
    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;

    //Add the file stream to the string stream
    fragBuff << fragSrc.rdbuf();

    //Convert the stream to a character array
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLFWwindow* window;

    float window_width = 600;
    float window_height = 600;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(600, 600, "Abegail Laureen R. Magaling", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    //Create a Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //Assign the source to the vertex shader
    glShaderSource(vertexShader, 1, &v, NULL);

    //Compile the vertex shader
    glCompileShader(vertexShader);

    //Create a Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    //Assign the source to the fragment shader
    glShaderSource(fragmentShader, 1, &f, NULL);

    //Compile the fragment shader
    glCompileShader(fragmentShader);


    //Create shader program
    GLuint shaderProg = glCreateProgram();


    //Attach the compiled Vertex shader
    glAttachShader(shaderProg, vertexShader);
    //Attach the compiled Vertex shader
    glAttachShader(shaderProg, fragmentShader);

    //Finalize the compilation process
    glLinkProgram(shaderProg);

    //Set the callback function to the window
    glfwSetKeyCallback(window, Key_Callback);

    /*glViewport(0, // Min x
        0,//Min y
        600,//Width
        600); // Height
    */

    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    GLfloat vertices[]
    {
        //x  //y   //z
        0.f, 0.5f, 0.f, //0
        -0.5f, -0.5f, 0.f, //1
        0.5f, -0.5f, 0.f //2

    };

    GLuint indices[]
    {
        0,1,2
    };

    //Create VAO,VBO,EBO Variables
    GLuint VAO, VBO, EBO;

    //Initialize VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Currently Editing VAO = null
    glBindVertexArray(VAO);
    //Currently Editing VAO = VAO
    // 
    //Currently Editing VBO = null
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Currently Editing VBO = VBO
    //VAO <-VBO

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * attributes.vertices.size() /*Size of buffer in bytes*/,
        attributes.vertices.data() /*Array*/, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3 /*x y z*/, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //Currently Editing VBO = VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //Currently Editing VBO = EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indices.size(), mesh_indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glm::mat4 identity_martix = glm::mat4(1.0f);

    float x = 0.0;
    float y = 0.0;
    float z = -5.0;

    float scale_x = 3.0;
    float scale_y = 3.0;
    float scale_z = 3.0;

    float axis_x = 0.0;
    float axis_y = 0.0;
    float axis_z = 1.0;

    float theta = 0.0;

    /*//Create projection matrix
    glm::mat4 projectionMatrix = glm::ortho(-2.f, //L
        2.f,//R
        -2.f,//B
        2.f,//T
        -1.f,//Znear
        1.f);//Zfar
    */
    glm::mat4 identity_matrix = glm::mat4(1.0f);

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.f), // FOV
        window_height / window_width, // Aspect Ratio
        0.1f, // ZNear > 0
        100.f); // ZFar
    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProg);

        float radius = 2.0f;

        float x_mod = radius * cos(glm::radians(theta));
        float y_mod = radius * sin(glm::radians(theta));

        float x_mod1 = radius * cos(glm::radians(theta + 120));
        float y_mod1 = radius * sin(glm::radians(theta + 120));

        float x_mod2 = radius * cos(glm::radians(theta + 240));
        float y_mod2 = radius * sin(glm::radians(theta + 240));

        theta += 0.07f;

        glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(x_mod, y_mod, z));
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(scale_x, scale_y, scale_z));
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta), glm::vec3(axis_x, axis_x, axis_z));

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        glm::mat4 transformation_matrix1 = glm::translate(identity_matrix, glm::vec3(x_mod1, y_mod1, -6.0f));
        transformation_matrix1 = glm::scale(transformation_matrix1, glm::vec3(scale_x, scale_y, scale_z));
        transformation_matrix1 = glm::rotate(transformation_matrix1, glm::radians(theta), glm::vec3(axis_x, axis_x, axis_z));

        unsigned int transformLoc2 = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(transformation_matrix1));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        glm::mat4 transformation_matrix2 = glm::translate(identity_matrix, glm::vec3(x_mod2, y_mod2, -6.0f));
        transformation_matrix2 = glm::scale(transformation_matrix2, glm::vec3(scale_x, scale_y, scale_z));
        transformation_matrix2 = glm::rotate(transformation_matrix2, glm::radians(theta), glm::vec3(axis_x, axis_x, axis_z));

        unsigned int transformLoc3 = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(transformation_matrix2));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    //Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}