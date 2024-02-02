#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define PI 3.14159265359

float x_mod = 0;
float y_mod = 0;
float theta = 90.0;

void Key_Callback(GLFWwindow* window,
    int key, //keycode of press
    int scancode, //physical position of the press
    int action, //either press / release
    int mods) //modifier for which key is pressed / held down
{
    if (key == GLFW_KEY_D) {
        theta += 1;
    }

    if (key == GLFW_KEY_A) {
        theta -= 1;
    }

    if (key == GLFW_KEY_W) {
        y_mod += 0.01f;
    }

    if (key == GLFW_KEY_S) {
        y_mod -= 0.01f;
    }

}

int main(void)
{
    GLFWwindow* window;


    //load shader
    std::fstream vertSrc("Shaders/sample.vert");
    std::fstream fragSrc("Shaders/sample.frag");
    
    std::stringstream vertBuff;
    std::stringstream fragBuff;

    //Add the file strean to the string strean
    vertBuff << vertSrc.rdbuf();
    fragBuff << fragSrc.rdbuf();

    //convert the stream to a character array
    std::string vertS = vertBuff.str();
    std::string fragS = fragBuff.str();

    const char* v = vertS.c_str();
    const char* f = fragS.c_str();

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1000, 800, "Abegail Laureen Magaling", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();
    
    glfwSetKeyCallback(window, Key_Callback);

    //create a vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    //Assign the source to the vertex shader
    glShaderSource(vertexShader, 1, &v, NULL);
    glShaderSource(fragShader, 1, &f, NULL);

    //compile the vertex shader
    glCompileShader(vertexShader);
    glCompileShader(fragShader);

    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragShader);

    glLinkProgram(shaderProg);

    std::string path = "3D/bunny.obj"; //path
    std::vector<tinyobj::shape_t> shapes; //defintion of model
    std::vector<tinyobj::material_t> material; //texture of model
    std::string warning, error;  //errors

    tinyobj::attrib_t attributes; //model itself

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLuint> mesh_indices;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    GLfloat vertices[]{
        0.f, 0.5f, 0.f, //0
        -0.5f, -0.5f, 0.f, //1
        0.5f, -0.5f, 0.f //2
    };

    GLuint indices[] = {
       0,2,1, //triangle a
    };

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Currently editing VAO = null
    glBindVertexArray(VAO);
    //after previous line Currently editing VAO = VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //VAO <- VBO

    glBufferData(GL_ARRAY_BUFFER,//data type of buffer
        sizeof(GL_FLOAT) * attributes.vertices.size(), //size in bytes
        &attributes.vertices[0],// or attributes.vertices.data() //array
        GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        3, //number of components X, Y, Z
        GL_FLOAT, //type of array to expect
        GL_FALSE,
        3 * sizeof(float), //size of vertex data
        (void*)0
    );

    //CURRENTLY EDITING VBO = VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //CURRENTLY EDITNG VBO = EBO

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        &mesh_indices[0],
        GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    //currently editing VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // VBO -> null (used for clean up)
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glm::mat4 identity_matrix = glm::mat4(1.0f);

    float x = 0.0, y = 0.0, z = 0.0;
    float scale_x = 5.0, scale_y = 5.0, scale_z = 5.0;
    //float theta = 90.0;
    float axis_x = 0.0, axis_y = 1.0, axis_z = 0.0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        theta += 0.2;
        //y += -0.0001;

        glm::mat4 transformation_matrix = glm::translate(identity_matrix,
                                          glm::vec3(x, y, z));

        transformation_matrix = glm::scale(transformation_matrix, 
                                glm::vec3(scale_x, scale_y, scale_z));

        transformation_matrix = glm::rotate(transformation_matrix, 
                                glm::radians(theta), 
                                glm::vec3(axis_x, axis_y, axis_z));

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");

        glUniformMatrix4fv(transformLoc,
                            1, 
                            GL_FALSE, 
                            glm::value_ptr(transformation_matrix));


        glUseProgram(shaderProg);
        glBindVertexArray(VAO);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 
            mesh_indices.size(), 
            GL_UNSIGNED_INT, 
            0);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}