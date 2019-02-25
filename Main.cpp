#include <Angel_commons/Angel.h> //You can use GLFW3 and GLAD
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float theta = 0;
float speedOfRotate = 0.1;

// camera
vec3 cameraPos = vec3(0.0f, 2.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);



bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = -27.0f;
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;
float fov = 15.0f;



// timing
float passedTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// build and compile our shader program
	// ------------------------------------
	GLuint ourShader = InitShader("vShader.glsl", "fShader.glsl");

	// set up vertex data (and buffer(s)) and configure vertex attributes for Dragon Object
	// ------------------------------------------------------------------
	
	
		char value1[255], value2[255], value3[255],  read[255], str[255];

		//Reading File 
		FILE* inputFile;
		inputFile = fopen("dragon.obj", "r");
		vector<vec4> vboArray;
		vector<GLuint> eboArray;

		while (fscanf(inputFile, "%s", read) == 1) {
			if (strcmp(read, "v") == 0) {
				fscanf(inputFile, "%s", value1);
				fscanf(inputFile, "%s", value2);
				fscanf(inputFile, "%s", value3);
				vec4 myvec;
				myvec.x = (float) atof(value1);
				myvec.y = (float) atof(value2);
				myvec.z = (float) atof(value3);
				myvec.w = 1.0f;
				vboArray.push_back(myvec);
			}
			else if (strcmp(read, "f") == 0) {
				fscanf(inputFile, "%s", value1);
				fscanf(inputFile, "%s", value2);
				fscanf(inputFile, "%s", value3);
				eboArray.push_back((GLuint)atoi(value1) - 1);
				eboArray.push_back((GLuint)atoi(value2) - 1);
				eboArray.push_back((GLuint)atoi(value3) - 1);
			}
			else if (feof(inputFile)) {
				break;
			}

		}
	
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vboArray.size() * sizeof(vec4), &vboArray[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboArray.size() * sizeof(GLuint), &eboArray[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//----------------------------------------------------------------------------------------------
	// set up vertex data (and buffer(s)) and configure vertex attributes for surface
	// ------------------------------------------------------------------
	float planeVertices[] = {
		// positions            // normals         
		 20.0f, -1.0f,  20.0f,  0.0f, 1.0f, 0.0f,
		-20.0f, -1.0f,  20.0f,  0.0f, 1.0f, 0.0f, 
		-20.0f, -1.0f, -20.0f,  0.0f, 1.0f, 0.0f,

		 20.0f, -1.0f,  20.0f,  0.0f, 1.0f, 0.0f,
		-20.0f, -1.0f, -20.0f,  0.0f, 1.0f, 0.0f,
		 20.0f, -1.0f, -20.0f,  0.0f, 1.0f, 0.0f,
	};
	// plane VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));




	//Settting Light Position
	vec3 lightPos = vec3(0.0f, 4.0f, 2.0f);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		passedTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//This setting for beginnig camera angle
		vec3 front;
		front.x = cos(DegreesToRadians * (yaw)) * cos(DegreesToRadians * (pitch));
		front.y = sin(DegreesToRadians * (pitch));
		front.z = sin(DegreesToRadians * (yaw)) * cos(DegreesToRadians * (pitch));
		cameraFront = normalize(front);
		//------------------------------


		//Angel perspective function
		mat4 projection = Perspective(fov, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//Angel use transpose so we have to transpose again
		projection = transpose(projection);

		// camera/view transformation
		mat4 view = LookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		//Angel use transpose so we have to transpose again
		view = transpose(view);

		//Send these matrix to vertex shader
		glUniformMatrix4fv(glGetUniformLocation(ourShader, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(ourShader, "projection"), 1, GL_FALSE, &projection[0][0]);


		// set light uniforms
		glUniform3f(glGetUniformLocation(ourShader, "viewPos"), 0.0f, -5.0f, 0.0f);
		glUniform3f(glGetUniformLocation(ourShader, "lightPos"), 0.0f, 4.0f, 0.0f);


		// Define Model Matrix
		mat4 scale;
		mat4 rotate;
		mat4 translate;


		//Setting surface matrix and color
		scale = Scale(1.0f); //No scale so identity matrix
		rotate = RotateY(0.0f);	// No Rotate 
		translate = Translate(0.0f, 1.211f, 0.0f); // No translate
		translate = transpose(translate);
		glUniform1i(glGetUniformLocation(ourShader, "objectColor"), 0);

		glUniformMatrix4fv(glGetUniformLocation(ourShader, "translate"), 1, GL_FALSE, &translate[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(ourShader, "rotate"), 1, GL_FALSE, &rotate[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(ourShader, "scale"), 1, GL_FALSE, &scale[0][0]);

		// Draw Surface
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		scale = Scale(4.0f, 4.0f, 4.0f);	// it's a bit too small for our scene, so scale it up
		rotate = RotateY(theta);	// Rotate 
		translate = Translate(0.0f, 0.0f, 0.0f); // translate it down so it's at the center of the scene

		//This decide to fragment shader use which color
		glUniform1i(glGetUniformLocation(ourShader, "objectColor"), 1);
		glUniformMatrix4fv(glGetUniformLocation(ourShader, "translate"), 1, GL_FALSE, &translate[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(ourShader, "rotate"), 1, GL_FALSE, &rotate[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(ourShader, "scale"), 1, GL_FALSE, &scale[0][0]);
		
		
		// draw our Dragon
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawElements(GL_TRIANGLES, eboArray.size(), GL_UNSIGNED_INT, 0);


		//Rotate angle of dragon
		theta -= speedOfRotate;

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	vec3 front;
	front.x = cos(DegreesToRadians * (yaw)) * cos(DegreesToRadians * (pitch));
	front.y = sin(DegreesToRadians * (pitch));
	front.z = sin(DegreesToRadians * (yaw)) * cos(DegreesToRadians * (pitch));

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	//It's too slow for camera movement so I multiplyied with 3
	float cameraSpeed = 3 * passedTime;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	}		
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		speedOfRotate += 0.005;
	}
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		speedOfRotate -= 0.005;
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraUp;
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraUp;
	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		//This positions start to middle of screen and calculate from center
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // Mouse sensitivity
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	vec3 front;
	front.x = cos(DegreesToRadians * (yaw)) * cos(DegreesToRadians * (pitch));
	front.y = sin(DegreesToRadians * (pitch));
	front.z = sin(DegreesToRadians * (yaw)) * cos(DegreesToRadians * (pitch));
	cameraFront = normalize(front);
}

