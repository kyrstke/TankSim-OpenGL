#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>
#include <camera.h>
#include <model.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
glm::vec3 cameraPosition = glm::vec3(-0.3096f, 2.0068f, -5.1461f);
//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(-0.0066, -0.3338, 0.9426);

Camera camera(cameraPosition);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 tankPosition = glm::vec3(0.f);
float tankAngle = 0.f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("vertexShader.glsl", "fragmentShader.glsl");

    // load models
    // -----------
    struct Czolg {
        Model hull;
        Model turret;
        glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        //const glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

        void Draw(Shader& ourShader) {
            //turret
            glm::mat4 model = glm::mat4(1.0f);        
            

            //model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.3f));
            //model = glm::rotate(model, (float)glfwGetTime()*float(0.5), glm::vec3(0.0f, 1.0f, 0.0f));
            tankPosition.y = 0.f;
            model = glm::translate(model, tankPosition);
            model = glm::rotate(model, glm::radians(90.f + -1*camera.getYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
            
            //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.3f));
            ourShader.setMat4("model", model);
            turret.Draw(ourShader);
            //hull
            model = glm::mat4(1.0f);
            model = glm::translate(model, tankPosition);
            model = glm::rotate(model, glm::radians(tankAngle), glm::vec3(0.f, 1.f, 0.f));
            //model = glm::translate(model, position); // translate it down so it's at the center of the scene
            ourShader.setMat4("model", model);
            hull.Draw(ourShader);
        };
    };

    Czolg tank = { Model("czoug/du.obj"), Model("czoug/gura.obj") };

    Model brick = Model("brick/testcube.obj");

    //Model tank("czoug/maus.obj");
    /*Model turret("tankModel/files/turret2.obj"); */
    //Model tank("brick/testcube.obj");

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        float radius = -7.f;
        float height = -0.5f;
        glm::vec3 newVector = glm::vec3(camera.getFront().x, height, camera.getFront().z);
        camera.setPosition(radius * newVector + tankPosition);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        
        //cube drawing
        glm::mat4 cube = glm::mat4(1.0f);
        glm::vec3 cube_pos = glm::vec3(7.f, 0.f, 7.f);
        cube = glm::translate(cube, cube_pos);
        ourShader.setMat4("model", cube);
        brick.Draw(ourShader);
        

        //printf("%f %f %f\n", camera.getFront().x, camera.getFront().y, camera.getFront().z);

        //glm::mat4 model = glm::mat4(1.0f);
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        //ourShader.setMat4("model", model);

        //float enemyYaw = glm::radians(180.0f) + atan2((ENEMY_POSITION.z - camera.GetPosition().z), (ENEMY_POSITION.x - camera.GetPosition().x));

        // render the loaded model
        
        //glm::mat4 model = glm::mat4(1.0f);
        //glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        //model = glm::translate(model, position); // translate it down so it's at the center of the scene
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //ourShader.setMat4("model", model);
        tank.Draw(ourShader);

        ///tank.Draw(ourShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    float MovementSpeed = 4.f;
    float rotateSpeed = 50.f;
    float velocity = MovementSpeed * deltaTime;
    glm::vec3 tankVector = glm::vec3(sin(glm::radians(tankAngle)), 0.f, cos(glm::radians(tankAngle)));

    

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        tankPosition += tankVector * velocity;
        printf("%f, %f %f %f\n", tankAngle, tankVector.x, tankVector.y, tankVector.z);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        tankPosition -= tankVector * velocity;
        printf("%f, %f %f %f\n", tankAngle, tankVector.x, tankVector.y, tankVector.z);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        tankAngle += rotateSpeed * deltaTime;
        printf("%f, %f %f %f\n", tankAngle, tankVector.x, tankVector.y, tankVector.z);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        tankAngle -= rotateSpeed * deltaTime;
        printf("%f, %f %f %f\n", tankAngle, tankVector.x, tankVector.y, tankVector.z);
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
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    //printf("%.4f, %.4f, %.4f\n", camera.getFront().x, camera.getFront().y, camera.getFront().z);

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}