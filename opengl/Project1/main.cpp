#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <learnopengl/shader_m.h>

#include <iostream>
#include <vector>
#include "PlyLoader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;


float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;
bool mouseLeftButtonPress = false;
bool mouseRightButtonPress = false;
glm::mat4 g_viewMatrix(1.0);
struct CamPostion {
    glm::mat4 view;
    glm::vec3 postion;
};


glm::mat4 g_modelMat(1.0);
int g_renderMode = 0;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw初始化
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "plyLoad", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad初始化
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);


    PlyLoader pPlyModel("test1.ply");


    // 加载模型obj文件
    //pModel = new Model("bunny-small.obj");

    


    // 申请vao vbo ebo等
    unsigned int VBO, VAO, NBO, CBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &NBO);
    glGenBuffers(1, &CBO);

    // 绑定vao
    glBindVertexArray(VAO);
 
    // 顶点
    std::vector<float> vertices;
    for (int i = 0; i < pPlyModel.vertices.size(); ++i) {
        auto vertex = pPlyModel.vertices[i];
        vertices.push_back(vertex.x);
        vertices.push_back(vertex.y);
        vertices.push_back(vertex.z);
    }
    // 绑定vbo
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // 法向量
    std::vector<float> normals;
    for (int i = 0; i < pPlyModel.normals.size(); ++i) {
        auto normal = pPlyModel.normals[i];
        normals.push_back(normal.x);
        normals.push_back(normal.y);
        normals.push_back(normal.z);
    }
    // 绑定nbo
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

    // 颜色
    std::vector<float> colors;
    for (int i = 0; i < pPlyModel.colors.size(); ++i) {
        auto color = pPlyModel.colors[i];
        colors.push_back(color.x / 255.0);
        colors.push_back(color.y / 255.0);
        colors.push_back(color.z / 255.0);
    }
    // 绑定cbo
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);




    // position attribute
    // 顶点
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 法向量
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    // 颜色
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);


    // shader初始化
    Shader ourShader("./Shader/model.vs", "./Shader/model.fs");

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window); // 键盘输入


        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // 实体模式或者线框
        if (g_renderMode == 0) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else if (g_renderMode == 1) {

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }


        ourShader.use();
        // 相机矩阵设置
        glm::mat4 model = glm::mat4(1.0f);
        model = model * g_modelMat;
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = g_viewMatrix;


        glBindVertexArray(VAO);
        ourShader.setMat4("projection", projection);    
        ourShader.setMat4("view", view);
        ourShader.setMat4("model", model);


        // 绘制图像
        glDrawArrays(GL_POINTS, 0, pPlyModel.vertices.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{
    static std::vector<int> last_key(128, GLFW_RELEASE);
    int curr_key = 0;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float span = deltaTime * 5;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        g_viewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, span)) * g_viewMatrix;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        g_viewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -span)) * g_viewMatrix;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        g_viewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(span, 0, 0)) * g_viewMatrix;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        g_viewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-span, 0, 0)) * g_viewMatrix;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        g_viewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0, -span, 0)) * g_viewMatrix;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        g_viewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0, span, 0)) * g_viewMatrix;
    }
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}



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


    float intensity = 0.08;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && mouseLeftButtonPress) {
        g_viewMatrix = g_viewMatrix * glm::rotate(glm::mat4(1.0), (float)(deltaTime * intensity * xoffset), glm::vec3(0, 1, 0));
        glm::vec3 v = glm::vec3(glm::inverse(g_viewMatrix) * glm::vec4(1, 0, 0, 0));
        g_viewMatrix = g_viewMatrix * glm::rotate(glm::mat4(1.0), (float)(-deltaTime * intensity * yoffset), v);
    }

    if (mouseRightButtonPress) {
        if (yoffset != 0) {
            g_viewMatrix = glm::rotate(glm::mat4(1.0), float(deltaTime * intensity * yoffset), glm::vec3(1, 0, 0)) * g_viewMatrix;
        }
        if (xoffset != 0) {
            g_viewMatrix = glm::rotate(glm::mat4(1.0), float(deltaTime * intensity * xoffset), glm::vec3(g_viewMatrix * glm::vec4(0, 1, 0, 0))) * g_viewMatrix;
        }
    }

    //camera.ProcessMouseMovement(xoffset, yoffset);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            mouseLeftButtonPress = true;
            mouseRightButtonPress = false;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            mouseRightButtonPress = true;
            mouseLeftButtonPress = false;
            break;
        default:
            break;
        }
    }
    else {
        mouseLeftButtonPress = false;
        mouseRightButtonPress = false;
    }
    return;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    g_viewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 30 * deltaTime * yoffset)) * g_viewMatrix;
}
