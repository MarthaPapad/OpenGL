#include "GL/glew.h" // include GLEW and new version of GL on Windows
#include "GL/glfw3.h" // GLFW helper library

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include "../shader.hpp"
#include "../mesh.hpp"

#include <iostream>

float density = 0.07; //initial density
float dd = 0.02; //density change 

//callback to close window and increase/decrease
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
        
    if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) density += dd;
    
    if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) density -= dd;
    
    if (density<0) density = 0;
    if (density>0.45) density = 0.45;
}

int main(void) {
	
    //start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    } 
    
    //create window
    int width = 800; 
    int height = 800;
    GLFWwindow* window = glfwCreateWindow(width, height, "Textures", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    //start GLEW extension handler
    glewInit();
    
    //get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    
    //keyboard callback
    glfwSetKeyCallback(window, key_callback);
    
    //background color
    glClearColor(0.8, 0.8, 0.9,0.0);
    
    // Initialise and activate shaders
    // Using standard MVP vertex shader and color fragment shader
    shader shaderProgram("../shaders/vertfog.vert","../shaders/fragfog.frag");
    shaderProgram.use();
    
    //enable depth - testing
    glEnable(GL_DEPTH_TEST); 
    
    //load objects
    meshvft texsquare("../obj/planevft.obj" , "../textures/grass.jpg");
    meshvft texsun("../obj/spherevft.obj" , "../textures/sun.jpg");
    meshvft texearth("../obj/spherevft.obj","../textures/earth720x360.jpg");
    meshvft texasteroid("../obj/asteroidvft.obj","../textures/asteroid700x700.jpg");
    
    //view matrix 
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f,-8.0f,3.0f),glm::vec3(0.0f),glm::vec3(0.0f,0.0f,1.0f));
    shaderProgram.set_mat4_uniform("view",view);
    
    //perspective matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)width/height, 0.01f,100.0f);
    shaderProgram.set_mat4_uniform("projection",projection);
	    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //update fog density
        shaderProgram.set_float_uniform("density", density);
        
        //rotation matrix
        float t = glfwGetTime();
        glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::degrees(0.01f*t), glm::vec3(0.0f, 0.0f, 1.0f));
        shaderProgram.set_mat4_uniform("rotation",R);
    	
        //draw asteroid near camera
        glm::mat4 T = glm::translate(glm::mat4(1.0f),glm::vec3(1.0f, -4.0f, 0.0f));
        shaderProgram.set_mat4_uniform("translation",T);
        texasteroid.draw(GL_TRIANGLES);
        
        //draw earth
        T = glm::translate(glm::mat4(1.0f),glm::vec3(-1.0f, -1.0f, 1.0f));
        shaderProgram.set_mat4_uniform("translation",T);
        texearth.draw(GL_TRIANGLES);
        
        //draw sun away from camera
        T = glm::translate(glm::mat4(1.0f),glm::vec3(3.0f, 4.0f, 1.0f));
        shaderProgram.set_mat4_uniform("translation",T);
        texsun.draw(GL_TRIANGLES);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    } 
    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}
