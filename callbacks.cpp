//Graphics OpenGL
//Papadopoulou Martha

#include "GL/glew.h" // include GLEW and new version of GL on Windows
#include "GL/glfw3.h" // GLFW helper library

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include <iostream>

//width and height of window, and initial conditions of the object to be drawn
int Wwidth = 800, Wheight = 800;
float xPos = 0, yPos = 0;
int index = 0;

//shaders
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "uniform mat4 projection;\n"
                                 "uniform float xloc;\n"
                                 "uniform float yloc;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = projection*vec4(aPos.x+xloc, aPos.y+yloc, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "uniform vec4 mycolor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor =  mycolor;\n"
                                   "}\n\0";

//shader program                                   
unsigned int shaderProgram;

void InitMyShaders()
{
    //vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    //fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    //link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    //delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

//objects in space (xmin,xmax,ymin,ymax)
float xmin = -10, xmax = 10, ymin = -10, ymax = 10;

//points of rectangle in space
float rectangles[] = {
        -2.0f, -2.0f, 0.0f, // left-bottom  
         -2.0f, 2.0f, 0.0f, // left-top 
         2.0f,  2.0f, 0.0f, // right-top
         2.0f,  -2.0f, 0.0f // right-bottom
};

//points of triangle in space
float triangles[] = {
        -2.0f, -2.0f, 0.0f, // left-bottom  
         0.0f, 2.0f, 0.0f, // top
         2.0f,  -2.0f, 0.0f // right-bottom
};

//vertex array objects for rectangle(r) and triangle(t)
unsigned int VAOr, VAOt, VBOr, VBOt;

void SetupVerticesData()
{
	//generate and bind VAO for rectangle
    glGenVertexArrays(1, &VAOr);
    glBindVertexArray(VAOr);
    
    //generate and bind and set vertex buffer, then configure vertex attributes(s)
    glGenBuffers(1, &VBOr);
    glBindBuffer(GL_ARRAY_BUFFER, VBOr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangles), rectangles, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    
    //generate and bind VAO for triangle
    glGenVertexArrays(1, &VAOt);
    glBindVertexArray(VAOt);
    
    //generate and bind and set vertex buffer, then configure vertex attributes(s)
    glGenBuffers(1, &VBOt);
    glBindBuffer(GL_ARRAY_BUFFER, VBOt);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
}

//initialize background colour, shader program use and projection 
void myInit()
{
	//background color
    glClearColor(0.29f, 0.34f, 0.349f, 0.0f);
    
    //use shader program
    glUseProgram(shaderProgram);
    
    //initial projection in the created window 
    glm::mat4 myprojectionmatrix = glm::ortho(xmin, xmax, ymin, ymax);
    //inform GLSL
    int transform_matrix_location = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(myprojectionmatrix));
}

//draws the objects
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	//updates x and y displacement in vertex shader
	int object_location_var = glGetUniformLocation(shaderProgram, "xloc"); 
    glUniform1f(object_location_var, xPos);
    object_location_var = glGetUniformLocation(shaderProgram, "yloc");
    glUniform1f(object_location_var, yPos);
    
    //gets location for color in vertex shader
    int color_location_var = glGetUniformLocation(shaderProgram, "mycolor");
    //initialize color vector
    glm::vec4 color; 
	
	if (index == 1){
		//when the user presses right click it draws a pink trangle
		color = glm::vec4(0.93f, 0.69f, 0.72f, 1.0f);
		glUniform4fv(color_location_var, 1, glm::value_ptr(color));
		glBindVertexArray(VAOt);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}else if(index == 2){
		//when the user presses right click it draws a green renctangle
		color = glm::vec4(0.69f, 0.77f, 0.69f, 1.0f);
		glUniform4fv(color_location_var, 1, glm::value_ptr(color));
		glBindVertexArray(VAOr);
		glDrawArrays(GL_QUADS, 0, 4);
	}
	
}

//mouse callback for right and left click
void my_mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	//gets cursor's coordinates 
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
    	index = 1;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
    	index = 2;
	}
	
	//only when the user clicks, the coordinates are calculated within the space
	if(index == 1 || index == 2){
		xPos = xmin + (xmax - xmin) * (float(xpos) / Wwidth);
		yPos = ymax - (ymax - ymin) * (float(ypos) / Wheight);
	}
}

int main(void) {
	
    //start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    } 
    
    GLFWwindow* window = glfwCreateWindow(Wwidth, Wheight, "My Callback", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    
    glfwMakeContextCurrent(window);
    glewInit();
    
    //get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    glfwSetMouseButtonCallback(window, my_mouse_callback);
    InitMyShaders();
    SetupVerticesData();
    myInit();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        myDisplay();
        glfwSwapBuffers(window);
        glfwPollEvents();
    } 
    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}
