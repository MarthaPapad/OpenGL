#include "GL/glew.h" // include GLEW and new version of GL on Windows
#include "GL/glfw3.h" // GLFW helper library

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

//window size
unsigned int width = 800, height = 800;

//shaders
const char* vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aCol;\n"
                                "uniform mat4 projection;\n"
                                "uniform mat4 view;\n"
                                "uniform mat4 rotate;\n"
                                "uniform mat4 translate;\n"
                                "out vec3 vrtxCol;\n"
                                "void main()\n"
                                "{\n"
                                "  vrtxCol = aCol;\n"
                                "  gl_Position = projection*view*translate*rotate*vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
									"in vec3 vrtxCol;\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vec4(vrtxCol, 1.0);\n"
                                    "}\n\0";

//shader program
unsigned int shaderProgram;

void InitMyShaders()
{
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

//user's drawing world
float xmin = -4, xmax = 4, ymin = -4, ymax = 4, zmin = -4, zmax = 4;

//triangles
float posvert [] = {
		1.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, //top triangle
		-1.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 0.0f, //left triangle
		-1.0f, 0.0f, -1.0f,
		1.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 0.0f, // bottom triangle 
		1.0f, 0.0f, -1.0f, 
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f //right triangle	
};

float colvert [] = {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, // green
        1.0f, 0.0f, 0.0f,  
        1.0f, 0.0f, 0.0f,  
        1.0f, 0.0f, 0.0f, // red 
        1.0f,  1.0f, 0.0f, 
        1.0f,  1.0f, 0.0f, 
        1.0f,  1.0f, 0.0f, // yellow
		0.0f,  0.0f, 1.0f, 
        0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 1.0f // blue
};

//axes
float mylines[] = {
                    -3.5,10.0,0.0f,
                    -3.5,-6.5,0.0f, 
                    -1.75,10.0,0.0f,
                    -1.75,-6.5,0.0f,
                    0.0f,10.0,0.0f,
                    0.0f,-6.5,0.0f,
                     1.75,10.0,0.0f,
                    1.75,-6.5,0.0f,
                    3.5,10.0,0.0f,
                    3.5,-6.5,0.0f,
};

float colorlines[] = {
                       0.2f, 0.2f, 0.4f,
                       0.8f, 0.8f, 0.5f, 
                       0.2f, 0.2f, 0.4f,
                       0.8f, 0.8f, 0.5f, 
                       0.2f, 0.2f, 0.4f,
                       0.8f, 0.8f, 0.5f, 
                       0.2f, 0.2f, 0.4f,
                       0.8f, 0.8f, 0.5f, 
                       0.2f, 0.2f, 0.4f,
                       0.8f, 0.8f, 0.5f, 
};


//set up VAOs for triangles and lines
unsigned int VAO1, VAO2;
unsigned int VBO1, VBOc1, VBO2, VBOc2;

void SetupVerticesData()
{
	//generate the triangles VAO, 
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(posvert), posvert, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    //colors for triangles, 
    glGenBuffers(1, &VBOc1);
    glBindBuffer(GL_ARRAY_BUFFER, VBOc1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colvert), colvert, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    
    //generate the lines VAO
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mylines), mylines, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    //colors for cube, 
    glGenBuffers(1, &VBOc2);
    glBindBuffer(GL_ARRAY_BUFFER, VBOc2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorlines), colorlines, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
}

//seet up initial values (background color, projection/view matrix)
void myInit()
{
    glClearColor(0.2, 0.2, 0.4, 0.0);
    glUseProgram(shaderProgram);
    float windowaspectratio = 1.0f * width / height;
    xmin = ymin * windowaspectratio; xmax = ymax * windowaspectratio;
    
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f,-10.0f,1.2f),glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f,0.0f,1.0f));
    int transform_matrix_location = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(view));
    
    glm::mat4 myprojectionmatrix = glm::perspective(45.0f, windowaspectratio, 0.01f, 100.0f);
    transform_matrix_location = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(myprojectionmatrix));
}


//draw all objects
void mydisplay(float angle) 
{
	glm::mat4 myIdentitymatrix = glm::mat4(1.0f);
	
	//draw lines
	//rotate
	int transform_matrix_location = glGetUniformLocation(shaderProgram, "rotate");
	glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(myIdentitymatrix));
	
	//translate
	transform_matrix_location = glGetUniformLocation(shaderProgram, "translate");
	glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(myIdentitymatrix));
	
	//draw
	glLineWidth(4.0); 
    glBindVertexArray(VAO2);
    glDrawArrays(GL_LINES, 0, 10);
	
	
	//draw back rectangle
	//rotate
    transform_matrix_location = glGetUniformLocation(shaderProgram, "rotate");
    glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(myIdentitymatrix));
    
    //translate
    glm::mat4 tranmatrix = glm::translate(myIdentitymatrix, glm::vec3(-2.0f, 5.0f, 1.2f));
    transform_matrix_location = glGetUniformLocation(shaderProgram, "translate");
    glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(tranmatrix));
	
	//draw
	glBindVertexArray(VAO1);
    glDrawArrays(GL_TRIANGLES, 0, 12);
	
	
	//draw front rectangle
	//rotate
    glm::mat4 rotmatrix = glm::rotate(myIdentitymatrix, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    transform_matrix_location = glGetUniformLocation(shaderProgram, "rotate");
    glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(rotmatrix));
    
    //translate
    tranmatrix = myIdentitymatrix;
    transform_matrix_location = glGetUniformLocation(shaderProgram, "translate");
    glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(tranmatrix));
    
    //draw (VAO is already binded)
    glDrawArrays(GL_TRIANGLES, 0, 12);
}

int main(void) {
	
    //start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    } 
    
    //make window
    GLFWwindow* window = glfwCreateWindow(width, height, "My Exam", NULL, NULL);
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

    InitMyShaders();
    SetupVerticesData();
    myInit();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        
        float time = glfwGetTime();
        float angle = time*10;
        mydisplay(angle);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } 
    //close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}

