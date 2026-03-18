#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



// camera
Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float followDistance = 3.0f;
float followHeight = 1.5f;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
glm::vec3 catPos = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 coinPos = glm::vec3(2.0f, 1.0f, 0.0f);

float catSpeed = 5.0f;
glm::vec3 moveDir(0.0f);
int score = 0;
float catRadius = 0.6f;
float coinRadius = 0.3f;


void respawnCoin()
{
    coinPos.x = (rand() % 20 - 10) * 0.5f;
    coinPos.z = (rand() % 20 - 10) * 0.5f;
    coinPos.y = 1.0f;
}
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
    stbi_set_flip_vertically_on_load(false);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

    // load models
    // -----------

    Model FLoor(FileSystem::getPath("resources/objects/gameobj/woodflormodel.obj"));
    Model Cat(FileSystem::getPath("resources/objects/gameobj/CatModel.obj"));
    Model Coin(FileSystem::getPath("resources/objects/gameobj/coinModel.obj"));


    std::cout << "loaded model" << std::endl;
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        camera.Position = catPos - camera.Front * followDistance + glm::vec3(0.0f, followHeight, 0.0f);
        float currentFrame = static_cast<float>(glfwGetTime());
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

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        //renderFloor
        glm::mat4 FloorModel = glm::mat4(1.0f);
        FloorModel = glm::translate(FloorModel, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        FloorModel = glm::scale(FloorModel, glm::vec3(0.25f, 0.25f, 0.25f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", FloorModel);
        glBindTexture(GL_TEXTURE_2D, 0);
        FLoor.Draw(ourShader);

        glm::mat4 CatModel = glm::mat4(1.0f);
        CatModel = glm::translate(CatModel, catPos);
        CatModel = glm::scale(CatModel, glm::vec3(0.02f));

        ourShader.setMat4("model", CatModel);
        Cat.Draw(ourShader);

        glm::mat4 CoinModel = glm::mat4(1.0f);
        CoinModel = glm::translate(CoinModel, coinPos);
        CoinModel = glm::scale(CoinModel, glm::vec3(0.01f));
        ourShader.setMat4("model", CoinModel);
        Coin.Draw(ourShader);


        float distance = glm::length(catPos - coinPos);

        if (distance < (catRadius + coinRadius))
        {
            score++;
            std::cout << "Score: " << score << std::endl;
            respawnCoin();
        }


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
    float velocity = catSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        catPos += glm::vec3(0, 0, -1) * velocity;

    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        catPos += glm::vec3(0, 0, 1) * velocity;

    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        catPos += glm::vec3(-1, 0, 0) * velocity;

    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        catPos += glm::vec3(1, 0, 0) * velocity;

    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
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
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
