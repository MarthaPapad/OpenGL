#include "GL/glew.h" // include GLEW and new version of GL on Windows
#include "GL/glfw3.h" // GLFW helper library
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include "../shader.hpp"
#include "../mesh.hpp"

#include <iostream>

//callback to close window
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(void) {
	
    //start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    } 
    
    //width and height of window
    int width = 800; 
    int height = 800;
    //space objects are in
	float xmin = -4.0f, xmax = 4.0f, ymin = -4.0f, ymax = 4.0f, zmin=-4.0f, zmax=4.0f;
	
	//create window
    GLFWwindow* window = glfwCreateWindow(width, height, "Textures", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    //start GLEW extension handler
    glewInit();
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    
    //keyboard callback
    glfwSetKeyCallback(window, key_callback);
    
    //background color
    glClearColor(0.2, 0.2, 0.4, 0.0);
    
    // Initialise and activate shaders
    // Using standard MVP vertex shader and color fragment shader
    shader shaderProgram("../shaders/opencubev.vert","../shaders/opencubef.frag");
    shaderProgram.use();
    
    //enable depth - testing to correctly render 3D stuff and face culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    //load the cubes
    meshvft texcubefront("../obj/opencubeftfront.obj" , "../textures/front1.jpg");
    meshvft texcubeleft("../obj/opencubeftleft.obj" , "../textures/side2.jpg");
    meshvft texcuberight("../obj/opencubeftright.obj" , "../textures/side2.jpg");
    meshvft texcubeback("../obj/opencubeftback.obj" , "../textures/back1.jpg");
    meshvft texcubein("../obj/opencubeft.obj" , "../textures/coins.jpg");
    
    //fix aspect ratio and set projection
	float windowaspectratio = 1.0f * width / height;
    xmin = ymin * windowaspectratio; xmax = ymax * windowaspectratio; 
    glm::mat4 myprojection = glm::ortho(xmin, xmax, ymin, ymax, zmin, zmax);
    shaderProgram.set_mat4_uniform("projection", myprojection);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //get time and calculate angle
        float t = glfwGetTime();
        float angle = t*10;
        
        //set rotation
        glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));
        shaderProgram.set_mat4_uniform("model",R);
        
        //draw front sides
        glCullFace(GL_BACK);
        glPolygonMode(GL_FRONT, GL_FILL);
        texcubefront.draw(GL_TRIANGLES);
        texcubeleft.draw(GL_TRIANGLES);
        texcuberight.draw(GL_TRIANGLES);
        texcubeback.draw(GL_TRIANGLES);
        
        //draw back sides
        glCullFace(GL_FRONT);
        glPolygonMode(GL_BACK, GL_FILL);
        texcubein.draw(GL_TRIANGLES);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    } 
    
    //close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}
