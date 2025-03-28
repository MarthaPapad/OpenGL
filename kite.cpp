//Papadopoulou Martha
//Graphics OpenGL

#include "GL/glew.h" // include GLEW and new version of GL on Windows
#include "GL/glfw3.h" // GLFW helper library

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtx/string_cast.hpp"

#include <vector>
#include <iostream>

int Wwidth, Wheight;

//Shaders
const char* vertexShaderSource = "#version 330 core\n"
								 "layout (location = 0) in vec2 aPos;\n"
								 "layout (location = 1) in vec3 aCol;\n"
								 "uniform mat4 translate;\n"
								 "uniform mat4 projection;\n"
								 "out vec3 vrtxCol;\n"
								 "void main()\n"
								 "{\n"
								 "	 vrtxCol = aCol;\n"
                                 "   gl_Position = projection*translate*vec4(aPos.x, aPos.y,0.0f, 1.0f);\n"
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

//Space
float xmin = -10, xmax = 10.0, ymin=-10.0, ymax=10.0;

float vertices[] = {  //TRIANGLES
         -5.0f, 9.0f, 
         -2.5f, 4.5f,
         -6.5f,  3.0f, //first
         -5.0f, 9.0f,
         1.4f, 6.5f, 
         -2.5f, 4.5f, //second
         1.4f, 6.5f,
         1.0f, -1.5f, 
         -2.5f, 4.5f, //third
         -2.5f, 4.5f,
         1.0f, -1.5f,
         -6.5f,  3.0f //fourth
};

float colors[] = {
		0.95f, 0.95f, 0.4f,
		0.95f, 0.95f, 0.4f,
		0.95f, 0.95f, 0.4f, //yellow
		1.0f, 0.6f, 0.3f,
		1.0f, 0.6f, 0.3f,
		1.0f, 0.6f, 0.3f, //orange
		0.8f, 0.0f, 0.0f,
		0.8f, 0.0f, 0.0f,
		0.8f, 0.0f, 0.0f, //red
		0.3f, 0.7f, 1.0f,
		0.3f, 0.7f, 1.0f,
		0.3f, 0.7f, 1.0f //blue
};

//set up VAO position and color for triangles
unsigned int VAO, VBO, VBOc;

void SetupVerticesData()
{
	//generate and bind the Vertex Array Object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
	//generate and bind and set vertex buffer(s), and then configure vertex attributes(s)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    
	//generate and bind the VertexCOLOR Array Object
    glGenBuffers(1, &VBOc);
    glBindBuffer(GL_ARRAY_BUFFER, VBOc);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    
}

//set up VAO position and color for strip
unsigned int VAOL, VBOL, VBOcL, NSTRIP;
unsigned int NPOINTS = 100;

void SetupStripData()
{
    std::vector<float> points;
    std::vector<float> color;
    
    float startX = -3.3f;
    float endX = 2.6f;
    
    float stepX = (endX - startX)/(NPOINTS -1);

    for(int i= 0 ; i < NPOINTS; i++) 
    { 
        float x = startX + i*stepX; 
        float y = 2.0f*sin(x)-1.5f*x; 
        points.push_back(x);
        points.push_back(y);
        points.push_back(x+0.15f);
        points.push_back(y+0.3f);
        
        color.push_back(0.8f); // Red component
        color.push_back(0.0f); // Green component
        color.push_back(0.0f); // Blue component
        color.push_back(0.8f); // Red component
        color.push_back(0.0f); // Green component
        color.push_back(0.0f); // Blue component
    }
    
    NSTRIP = points.size()/2;
    
    //generate and bind the Vertex Array Object 
    glGenVertexArrays(1, &VAOL);
    glBindVertexArray(VAOL);
    
    //generate and bind and set vertex buffer(s), and then configure vertex attributes(s)
    glGenBuffers(1, &VBOL);
    glBindBuffer(GL_ARRAY_BUFFER, VBOL);
    glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(float), &points[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
	
	//generate and bind the VertexCOLOR Array Object
	glGenBuffers(1, &VBOcL);
    glBindBuffer(GL_ARRAY_BUFFER, VBOcL);
    glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(float), &color[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
}

//initialize background color, use shader program, window ratio and projection
void myInit()
{
	//background color
    glClearColor(0.75f, 0.95f, 1.0f, 1.0f);
    
    //use shader program
    glUseProgram(shaderProgram);
    
    //correct window aspect ratio
    float windowaspectratio = 1.0f * Wwidth / Wheight;
    xmin = ymin * windowaspectratio; xmax = ymax * windowaspectratio; 
    
    //initial projection in the created window 
    glm::mat4 myprojectionmatrix = glm::ortho(xmin, xmax, ymin, ymax);
    //inform GLSL
    int transform_matrix_location = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(myprojectionmatrix));
    
}

void mydisplay()
{
	//draw strip
    
    //find diff between initial starting point x and final starting point x
    float initialX = -3.3f;
    float diffX = 1.0f - initialX - 0.2f; // -0.2f is a correction of the strip's width
    
    //find diff between initial starting point y and final starting point y
    float initialY = 2.0f * sin(initialX) - 1.5f * initialX;
    float diffY = -1.5f - initialY + 0.3f; // -0.3f is a correction of the strip's height
    
    //update the translation of the strip
    glm::mat4 myIdentitymatrix = glm::mat4(1.0f);
    int transform_matrix_location = glGetUniformLocation(shaderProgram, "translate");
    glm::mat4 mymodelmatrix = glm::translate(myIdentitymatrix, glm::vec3(diffX, diffY, 0.0f));
    glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(mymodelmatrix));
	
	//bind VAOL and draw the strip
	glBindVertexArray(VAOL);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glDrawArrays(GL_TRIANGLE_STRIP,0,NSTRIP);
	
	
	//draw triangles
	
	//update the translation of the triangles (identity matrix - no translation)
	transform_matrix_location = glGetUniformLocation(shaderProgram, "translate");
    glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(myIdentitymatrix));
    
    //bind VAOL and draw the triangles
	glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 12);
}

void mydisplay2()
{
	 //draw strip
    
    //find diff between initial starting point x and final starting point x
    float initialX = -3.3f;
    float diffX = 1.0f - initialX - 0.2f; // -0.2f is a correction of the strip's width
    
    //find diff between initial starting point y and final starting point y
    float initialY = 2.0f * sin(initialX) - 1.5f * initialX;
    float diffY = -1.5f - initialY + 0.3f; // -0.3f is a correction of the strip's height
    
    //update the translation of the strip
    glm::mat4 myIdentitymatrix = glm::mat4(1.0f);
    int transform_matrix_location = glGetUniformLocation(shaderProgram, "translate");
    glm::mat4 mymodelmatrix = glm::translate(myIdentitymatrix, glm::vec3(diffX, diffY, 0.0f));
    glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(mymodelmatrix));
	
	//bind VAOL and draw the strip
	glBindVertexArray(VAOL);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    
    //correct viewport to depict two strips right and left of its original placement 
    int pixr = Wwidth-Wwidth/4-Wwidth/2;
    int pixl = -pixr;
    glViewport(pixr, 0, Wwidth, Wheight);
    glDrawArrays(GL_TRIANGLE_STRIP,0,NSTRIP);
    glViewport(pixl, 0, Wwidth, Wheight);
    glDrawArrays(GL_TRIANGLE_STRIP,0,NSTRIP);

	
	//draw triangles
	
	//update the translation of the triangles
	transform_matrix_location = glGetUniformLocation(shaderProgram, "translate");
    glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(myIdentitymatrix));
    
    //bind VAO and draw the triangles
	glBindVertexArray(VAO);
	
	//correct viewport to depict two kites right and left of its original placement 
	glViewport(pixr, 0, Wwidth, Wheight);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glViewport(pixl, 0, Wwidth, Wheight);
    glDrawArrays(GL_TRIANGLES, 0, 12);
}

int main(void){
	
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    } 
    
    GLFWwindow* window = glfwCreateWindow(1040, 585, "My Kites", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    // start GLEW extension handler
    glewInit();
    
    InitMyShaders();
    SetupVerticesData();
    SetupStripData();
     
    //retrive window's width and height
    glfwGetWindowSize(window, &Wwidth, &Wheight);
    
    myInit();
     
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        mydisplay(); //one kite
    	//mydisplay2(); //two kites
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    } 
    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}

