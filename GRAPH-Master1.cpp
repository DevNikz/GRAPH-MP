#include "GRAPH-Master1.h"
#include <random>
using namespace std;

/*
BOLTER MODEL
https://sketchfab.com/3d-models/bolter-low-poly-93fe17c586024779955dd0c9f2b4b832
*/

enum CameraType {
    THIRDPERSON,
    FIRSTPERSON,
    TOPDOWN
};

CameraType cameraType = THIRDPERSON;

#ifndef CAMERA_H
#define CAMERA_H

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 25.0f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::mat4 Projection;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        MouseSensitivity = 0.1f;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        MouseSensitivity = 0.1f;
        updateCameraVectors();
    }

    glm::mat4 GetPerspectiveViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 GetPerspectiveViewMatrix(glm::vec3 target)
    {
        return glm::lookAt(Position, target, glm::vec3(0.f, 1.f, 0.f));
    }

    glm::mat4 GetOrthoViewMatrix()
    {
        return glm::lookAt(Position, Position + glm::vec3(0, -1, 0), glm::vec3(0,0,-1));
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
        updateCameraVectors();
    }
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;

        //if (cameraType == PERSPECTIVE)
        //{
        //    //if (OrbitRadius < 1.0f) OrbitRadius = 1.0f;
        //    //updateCameraVectors();
        //}   
    }

public:
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
      
        /*
        Position.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Position.y = sin(glm::radians(Pitch));
        Position.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

        Position.x = objectPosRef.x + OrbitRadius * cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Position.y = objectPosRef.y + OrbitRadius * sin(glm::radians(Pitch));
        Position.z = objectPosRef.z + OrbitRadius * sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));


        Front = glm::normalize(Position);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
        */
    }
};
#endif

#ifndef PERSPECTIVE_CAM_H

class PerspectiveCamera : public Camera 
{
public:
    PerspectiveCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH)
        : Camera(position, up, yaw, pitch)
    {
        updateCameraVectors();
    }
};

#endif

#ifndef ORTHOGRAPHIC_CAM_H

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH)
        : Camera(position, up, yaw, pitch)
    {
        updateCameraVectors();
    }
};

#endif

float windowWidth = 1600;
float windowHeight = 900;

std::mt19937 rng(42); // seed for reproducibility
std::uniform_real_distribution<float> dist(-25.f, 25.f);

PerspectiveCamera thirdPerson(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
PerspectiveCamera firstPerson(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
OrthographicCamera topDown(glm::vec3(0.f, 0.0f, 1.0f));

float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool firstMouse = true;

//Global variables
float deltaTime = 0.f;
float lastFrame = 0.f;
bool allowLightControl = false;
bool perspectiveSwitch = false;
bool cameraSwitch = false;
bool tankMoved = false;

float orbitYawOffset = 180.f;
float orbitPitch = 25.f;     
const float sensitivity = 0.1f;

// Third-person shoulder camera state
glm::vec3 modelPosition = glm::vec3(0.f, 0.f, 10.f);
float modelYaw = 0.f;
float firstPersonYaw = 0.f;
float firstPersonPitch = 0.f;
float firstPersonFOV = 45.f;

// Shoulder camera offsets (tweak these to taste)
const float CAM_DIST = 8.f;   
const float CAM_HEIGHT = 2.5f;   
const float CAM_SIDE = 0.0f; 

glm::vec3 topDownPosition = glm::vec3(0.f, 40.f, 0.f);

std::vector<glm::vec3> treePosList = { 
    glm::vec3(5.f, -2.0f, 0.f),  
    glm::vec3(5.f, -2.0f, 5.f),
    glm::vec3(5.f, -2.0f, 10.f),
    glm::vec3(5.f, -2.0f, 15.f),
    glm::vec3(5.f, -2.0f, 20.f),
    glm::vec3(5.f, -2.0f, 25.f),
};

#ifndef SHADER_H
#define SHADER_H
class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use()
    {
        glUseProgram(ID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setVec3(const std::string& name, int i, glm::vec3 value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), i, glm::value_ptr(value));
    }
    void setMat4(const std::string& name, int i, glm::mat4 value) {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), i, GL_FALSE, glm::value_ptr(value));
    }
    void LoadTexture(GLuint diffuse) {
        glActiveTexture(GL_TEXTURE0);
        GLuint tex0 = glGetUniformLocation(ID, "diffuseMap");
        glUniform1i(tex0, 0);
        glBindTexture(GL_TEXTURE_2D, diffuse);
    }

    void LoadOverlay(GLuint overlay) {
        glActiveTexture(GL_TEXTURE2);
        GLuint tex2 = glGetUniformLocation(ID, "overlayMap");
        glUniform1i(tex2, 2);
        glBindTexture(GL_TEXTURE_2D, overlay);
    }

    void LoadNormal(GLuint normal) {
        glActiveTexture(GL_TEXTURE1);
        GLuint tex1 = glGetUniformLocation(ID, "normalMap");
        glUniform1i(tex1, 1);
        glBindTexture(GL_TEXTURE_2D, normal);
        
    }

    void PassSkybox(Camera cam) {
        setMat4("projection", 1, cam.Projection);
        setMat4("view", 1, glm::mat4(glm::mat3(cam.GetPerspectiveViewMatrix())));
    }

    void PassOrthoSkybox(Camera cam) {
        setMat4("projection", 1, cam.Projection);
        setMat4("view", 1, glm::mat4(glm::mat3(cam.GetOrthoViewMatrix())));
    }

    void PassSkybox(Camera cam, glm::vec3 target) {
        setMat4("projection", 1, cam.Projection);
        setMat4("view", 1, glm::mat4(glm::mat3(cam.GetPerspectiveViewMatrix(target))));
    }

    void passPerspectiveCamera(Camera cam) {
        setMat4("projection", 1, cam.Projection);
        setMat4("view", 1, cam.GetPerspectiveViewMatrix());
        setVec3("cameraPos", 1, cam.Position);
    }

    void passPerspectiveCamera(Camera cam, glm::vec3 target) {
        setMat4("projection", 1, cam.Projection);
        setMat4("view", 1, cam.GetPerspectiveViewMatrix(target));
        setVec3("cameraPos", 1, cam.Position);
    }

    void passOrthoCamera(Camera cam) {
        setMat4("projection", 1, cam.Projection);
        setMat4("view", 1, cam.GetOrthoViewMatrix());
        setVec3("cameraPos", 1, cam.Position);
    }

    void passPointLight(const std::string& name, glm::vec3 position, glm::vec3 color,
                        float strength, float ambientStr, glm::vec3 ambientColor,
                        float specularStr, float specularPhong, float constant,
                        float linear, float quadratic, float range) {
        setVec3(name + ".position", 1, position);
        setVec3(name + ".color", 1, color);
        setFloat(name + ".strength", strength);
        setFloat(name + ".ambientStr", ambientStr);
        setVec3(name + ".ambientColor", 1, ambientColor);
        setFloat(name + ".specularStr", specularStr);
        setFloat(name + ".specularPhong", specularPhong);
        setFloat(name + ".constant", constant);
        setFloat(name + ".linear", linear);
        setFloat(name + ".quadratic", quadratic);
        setFloat(name + ".range", range);


    }

    void passDirLight(const std::string& name, glm::vec3 direction, glm::vec3 color,
                      float strength, float ambientStr, glm::vec3 ambientColor,
                      float specularStr, float specularPhong) {
        setVec3(name + ".direction", 1, glm::normalize(direction));
        setVec3(name + ".color", 1, color);
        setFloat(name + ".strength", strength);
        setFloat(name + ".ambientStr", ambientStr);
        setVec3(name + ".ambientColor", 1, ambientColor);
        setFloat(name + ".specularStr", specularStr);
        setFloat(name + ".specularPhong", specularPhong);


    }


private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
#endif

class PointLight
{
public:
    glm::vec3 position;
    glm::vec3 color;
    float strength;

    float ambientStr;
    glm::vec3 ambientColor;

    float specularStr;
    float specularPhong;

    float constant;
    float linear;
    float quadratic;
    float range;

    PointLight(glm::vec3 position = glm::vec3(0.f), glm::vec3 color = glm::vec3(1.f),
        float strength = 1.f, float ambientStr = 0.1f, glm::vec3 ambientColor = glm::vec3(1.f),
        float specularStr = 0.5f, float specularPhong = 32.f, float constant = 1.f,
        float linear = 0.09f, float quadratic = 0.032f, float range = 2.f)
        : position(position), color(color), strength(strength),
        ambientStr(ambientStr), ambientColor(ambientColor),
        specularStr(specularStr), specularPhong(specularPhong),
        constant(constant), linear(linear), quadratic(quadratic), range(range)
    {
    }

    void Apply(Shader& shader, const std::string& name = "pointLight")
    {
        shader.passPointLight(name, position, color, strength,
                              ambientStr, ambientColor, specularStr, 
                              specularPhong, constant, linear, quadratic,
                              range);
    }
};

class DirectionalLight
{
public:
    glm::vec3 direction;
    glm::vec3 color;
    float strength;

    float ambientStr;
    glm::vec3 ambientColor;

    float specularStr;
    float specularPhong;

    DirectionalLight(glm::vec3 direction = glm::vec3(0.f, -1.f, 0.f), glm::vec3 color = glm::vec3(1.f),
        float strength = 1.f, float ambientStr = 0.1f, glm::vec3 ambientColor = glm::vec3(1.f),
        float specularStr = 0.5f, float specularPhong = 32.f)
        : direction(direction), color(color), strength(strength),
        ambientStr(ambientStr), ambientColor(ambientColor),
        specularStr(specularStr), specularPhong(specularPhong)
    {
    }

    void Apply(Shader& shader, const std::string& name = "dirLight")
    {
        shader.passDirLight(name, direction, color, strength,
                              ambientStr, ambientColor, specularStr, specularPhong);
    }
};

PointLight pointLight(
    glm::vec3(0.f, 3.f, 5.f), //Pos
    glm::vec3(1.0f, 1.0f, 1.0f),
    2.5f, //strength
    0.1f, //ambientstr
    glm::vec3(1.f, 1.f, 1.f), //ambientColor
    0.3f, //specularStr
    32.0f, //specularPhong
    1.0f, //constant
    0.007f, //linear
    0.0002f, //quadratic
    15.0f //range
);

DirectionalLight dirLight(
    glm::vec3(-0.2f, -1.0f, -0.6f), // direction: high angle, slightly angled like a moon
    glm::vec3(0.6f, 0.7f, 1.0f),    // color: cool blue-white moonlight
    0.15f,                            // strength: dim, moon is much weaker than sun
    0.08f,                            // ambientStr: dark night ambient
    glm::vec3(0.05f, 0.05f, 0.15f), // ambientColor: deep blue night tint
    0.1f,                             // specularStr: subtle glint
    64.f                              // specularPhong
);

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    static bool onePress = false;
    static bool twoPress = false;
    static bool fPress = false;
    static int lightIntensity = 0;
    static CameraType prevMode = THIRDPERSON;

    const float lightIntensities[] = { 2.5f, 5.f, 7.5f };

    const float moveSpeed = 20.f;
    const float turnSpeed = 90.f;

    const float fpTurnSpeed = 60.f;
    const float fpMoveSpeed = 10.f;

    const float panSpeed = 20.f;

    //Exit Game
    if (glfwGetKey(window,  GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //ThirdPerson or FirstPerson
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        if (!onePress && cameraType != TOPDOWN)
        {
            cameraType = (cameraType == THIRDPERSON) ? FIRSTPERSON : THIRDPERSON;
            prevMode = cameraType;
        }
        onePress = true;
    }
    else onePress = false;

    //Perspective or Ortho
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        if (!twoPress)
        {
            if (cameraType != TOPDOWN)
            {
                prevMode = cameraType; // save current before switching
                cameraType = TOPDOWN;
                topDownPosition = modelPosition + glm::vec3(0.f, 40.f, 0.f);
            }
            else
            {
                cameraType = prevMode; // return to previous
            }
        }
        twoPress = true;
    }
    else twoPress = false;

    //Point light
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        if (!fPress)
        {
            lightIntensity = (lightIntensity + 1) % 3;
            pointLight.strength = lightIntensities[lightIntensity];
        }
        fPress = true;
    }
    else fPress = false;

    
    //Move
    if (cameraType == THIRDPERSON) 
    {
        // --- Rotate tank in place (A = turn left, D = turn right) -----------
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            modelYaw += turnSpeed * deltaTime;
            tankMoved = true;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            modelYaw -= turnSpeed * deltaTime;
            tankMoved = true;
        }

        // --- Compute tank's local forward vector from its current yaw -------
        glm::vec3 tankForward = glm::normalize(glm::vec3(
            cos(glm::radians(-modelYaw)),
            0.f,
            sin(glm::radians(-modelYaw))
        ));

        // --- Drive forward / backward (W / S) --------------------------------
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            modelPosition += tankForward * moveSpeed * deltaTime;
            tankMoved = true;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            modelPosition -= tankForward * moveSpeed * deltaTime;
            tankMoved = true;
        }

        if (tankMoved)
        {
            firstPersonYaw = -modelYaw;
            firstPersonPitch = 0.f;
        }
    }
    else if (cameraType == FIRSTPERSON) 
    {
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            firstPersonYaw -= fpTurnSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            firstPersonYaw += fpTurnSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            firstPersonPitch += fpMoveSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            firstPersonPitch -= fpMoveSpeed * deltaTime;

        // Clamp pitch
        if (firstPersonPitch > 89.f) firstPersonPitch = 89.f;
        if (firstPersonPitch < -89.f) firstPersonPitch = -89.f;

        // Q/E - zoom in/out
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            firstPersonFOV -= fpMoveSpeed * deltaTime * 10.f;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            firstPersonFOV += fpMoveSpeed * deltaTime * 10.f;

        if (firstPersonFOV < 10.f) firstPersonFOV = 10.f;
        if (firstPersonFOV > 120.f) firstPersonFOV = 120.f;
        
    }

    else {
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            topDownPosition.x -= panSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            topDownPosition.x += panSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            topDownPosition.z -= panSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            topDownPosition.z += panSpeed * deltaTime;
    }
    
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
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
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    switch (cameraType) {
        case THIRDPERSON:
            
            orbitYawOffset += xoffset * sensitivity;
            orbitPitch += yoffset * sensitivity;

            if (orbitPitch < 5.f)  orbitPitch = 5.f;
            if (orbitPitch > 80.f) orbitPitch = 80.f;

            thirdPerson.ProcessMouseMovement(orbitYawOffset, orbitPitch);
            break;
    }

    //switch (cameraType)
    //{
    //case PERSPECTIVE:
    //    //perspective.ProcessMouseMovement(xoffset, yoffset);
    //    break;
    //case ORTHO:
    //    //ortho.ProcessMouseMovement(xoffset, yoffset);
    //    break;
    //}
    
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    /*switch (cameraType)
    {
    case PERSPECTIVE:
        perspective.ProcessMouseScroll(static_cast<float>(yoffset));
        break;

    case ORTHO:
        ortho.ProcessMouseScroll(static_cast<float>(yoffset));
        break;
    }*/
}

//MODEL CLASS
#ifndef MODEL_H
class Model {
private:
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string error;
    tinyobj::attrib_t attributes;
    std::vector<GLuint> mesh_indices;
    GLuint vao, vbo, ebo, vbo_uv;
    std::vector<GLfloat> fullVertexData;
    GLuint diffuseTexture;
    GLuint normalTexture;
    GLuint overlayTexture;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

public:
    string modelName;
    string texName;
    string normName;
    string overlayName;
    //glm::vec3 position;

    Model(string _name, string _tex, string _file, string _norm = "", string _normFile = "", string _over = "", string _overFile = "");
    void InitModel();
    void InitCube();
    void DrawCube();
    void DrawModel();
    void DeleteBuffers();
    void InitTextures();
    void InitOverlayMap();
    void InitNormals();
    GLuint GetDiffuse();
    GLuint GetNormal();
    GLuint GetOverlay();
};

GLuint Model::GetDiffuse() {
    return this->diffuseTexture;
}

GLuint Model::GetNormal() {
    return this->normalTexture;
}

GLuint Model::GetOverlay() {
    return this->overlayTexture;
}

Model::Model(string _name, string _tex, string _file, string _norm, string _normFile, string _over, string _overFile) {
    modelName = "3D/" + _name + ".obj";
    texName = "3D/" + _tex + _file;
    normName = "3D/" + _norm + _normFile;
    overlayName = "3D/" + _over + _overFile;
}

void Model::InitOverlayMap() {
    stbi_set_flip_vertically_on_load(true);
    int img_width3, img_height3, colorChannels3;
    GLenum format3{};
    unsigned char* overlay_bytes = stbi_load(overlayName.c_str(), &img_width3, &img_height3, &colorChannels3, 0);

    if (overlay_bytes) {
        if (colorChannels3 == 1) format3 = GL_RED;
        else if (colorChannels3 == 3) format3 = GL_RGB;
        else if (colorChannels3 == 4) format3 = GL_RGBA;

        //(TEXTURE) Load / bind after shaders
        glGenTextures(1, &overlayTexture);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, overlayTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D,
            0, //Texture 0
            format3, //Target color format
            img_width3,
            img_height3,
            0,
            format3,
            GL_UNSIGNED_BYTE,
            overlay_bytes //loaded texture in bytes
        );
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(overlay_bytes);
    }

    else {
        stbi_image_free(overlay_bytes);
    }
}

void Model::InitTextures() {
    stbi_set_flip_vertically_on_load(true);
    int img_width, img_height, colorChannels;
    GLenum format{};
    unsigned char* tex_bytes = stbi_load(texName.c_str(), &img_width, &img_height, &colorChannels, 0);

    if (tex_bytes) {
        GLenum format = (colorChannels == 4) ? GL_RGBA : GL_RGB;

        //(TEXTURE) Load / bind after shaders
        glGenTextures(1, &diffuseTexture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(
            GL_TEXTURE_2D,
            0, //Texture 0
            format, //Target color format
            img_width,
            img_height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            tex_bytes //loaded texture in bytes
        );
        glGenerateMipmap(GL_TEXTURE_2D);


        stbi_image_free(tex_bytes);
    }

    else {
        stbi_image_free(tex_bytes);
    }
}

void Model::InitNormals() {
    int img_width2, img_height2, colorChannels2;
    unsigned char* normal_bytes = stbi_load(normName.c_str(), &img_width2, &img_height2, &colorChannels2, 0);

    if (normal_bytes) {
        GLenum format = (colorChannels2 == 4) ? GL_RGBA : GL_RGB;
        //(TEXTURE) Load / bind after shaders
        glGenTextures(1, &normalTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalTexture);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format, //Target color format
            img_width2,
            img_height2,
            0,
            format,
            GL_UNSIGNED_BYTE,
            normal_bytes //loaded texture in bytes
        );
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(normal_bytes);
    }
    else {
        stbi_image_free(normal_bytes);
    }
}

void Model::DrawCube() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
}

void Model::DrawModel() {
    glBindVertexArray(vao);
    //Vertex Data Method
    glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);
}

void Model::DeleteBuffers() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Model::InitCube() {
    //Load Object. If success, it loads
    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &materials,
        &error,
        modelName.c_str()
    );

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    //(SHADERS) Generate vertices and buffers
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &vbo_uv);
    glGenBuffers(1, &ebo);

    //(POSITIONS) VBO
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * attributes.vertices.size(),
        &attributes.vertices[0],
        GL_DYNAMIC_DRAW
    );

    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_DYNAMIC_DRAW
    );
    glVertexAttribPointer(
        //0 = Position
        0, // Index / buffer index
        3, // x y z
        GL_FLOAT, // array of floats
        GL_FALSE, // if its normalized
        3 * sizeof(float), // size of data per vertex
        (void*)0
    );
    glEnableVertexAttribArray(0);

    //UV
    glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(float) * (sizeof(UV) / sizeof(UV[0])),
        &UV[0],
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void Model::InitModel() {
    //Load Object. If success, it loads
    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &materials,
        &error,
        modelName.c_str()
    );

    

    for (int i = 0; i < shapes.size(); i++) {
        for (int j = 0; j < shapes[i].mesh.indices.size(); j+=3) {
            tinyobj::index_t vData1 = shapes[i].mesh.indices[j];
            tinyobj::index_t vData2 = shapes[i].mesh.indices[j+1];
            tinyobj::index_t vData3 = shapes[i].mesh.indices[j+2];

            //Pos
            glm::vec3 v1 = glm::vec3(
                attributes.vertices[vData1.vertex_index * 3],
                attributes.vertices[vData1.vertex_index * 3 + 1],
                attributes.vertices[vData1.vertex_index * 3 + 2]
            );

            glm::vec3 v2 = glm::vec3(
                attributes.vertices[vData2.vertex_index * 3],
                attributes.vertices[vData2.vertex_index * 3 + 1],
                attributes.vertices[vData2.vertex_index * 3 + 2]
            );

            glm::vec3 v3 = glm::vec3(
                attributes.vertices[vData3.vertex_index * 3],
                attributes.vertices[vData3.vertex_index * 3 + 1],
                attributes.vertices[vData3.vertex_index * 3 + 2]
            );

            //UV
            glm::vec2 uv1 = glm::vec2(
                attributes.texcoords[(vData1.texcoord_index * 2)],
                attributes.texcoords[(vData1.texcoord_index * 2) + 1]
            );

            glm::vec2 uv2 = glm::vec2(
                attributes.texcoords[(vData2.texcoord_index * 2)],
                attributes.texcoords[(vData2.texcoord_index * 2) + 1]
            );

            glm::vec2 uv3 = glm::vec2(
                attributes.texcoords[(vData3.texcoord_index * 2)],
                attributes.texcoords[(vData3.texcoord_index * 2) + 1]
            );

            glm::vec3 deltaPos1 = v2 - v1;
            glm::vec3 deltaPos2 = v3 - v1;

            glm::vec2 deltaUV1 = uv2 - uv1;
            glm::vec2 deltaUV2 = uv3 - uv1;

            float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));
            glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
            glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

            tangents.push_back(tangent);
            tangents.push_back(tangent);
            tangents.push_back(tangent);

            bitangents.push_back(bitangent);
            bitangents.push_back(bitangent);
            bitangents.push_back(bitangent);
        }
    }
    
    for (int i = 0; i < shapes.size(); i++) {
        for (int j = 0; j < shapes[i].mesh.indices.size(); j++) {
            tinyobj::index_t vData = shapes[i].mesh.indices[j];
            fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3)]); //X
            fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 1]); //Y
            fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 2]); //Z
            fullVertexData.push_back(attributes.normals[(vData.normal_index * 3)]);
            fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 1]);
            fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 2]);
            fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2)]); //U
            fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2) + 1]); //V

            fullVertexData.push_back(tangents[i].x);
            fullVertexData.push_back(tangents[i].y);
            fullVertexData.push_back(tangents[i].z);
            fullVertexData.push_back(bitangents[i].x);
            fullVertexData.push_back(bitangents[i].y);
            fullVertexData.push_back(bitangents[i].z);
        }
    }
    

    GLintptr normalPtr = 3 * sizeof(float);
    GLintptr uvPtr = 6 * sizeof(float);
    GLintptr tangentPtr = 8 * sizeof(float);
    GLintptr bitangentPtr = 11 * sizeof(float);

    //(SHADERS) Generate vertices and buffers
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    //(POSITIONS) VBO
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //Using fullvertexdata
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * fullVertexData.size(),
        fullVertexData.data(),
        GL_DYNAMIC_DRAW
    );
    glVertexAttribPointer(
        0, // Index / buffer index
        3, // x y z
        GL_FLOAT, // array of floats
        GL_FALSE, // if its normalized
        14 * sizeof(GLfloat), // size of data per vertex
        (void*)0
    );
    glEnableVertexAttribArray(0);

    //Normal
    glVertexAttribPointer(
        1,
        3, // x y z
        GL_FLOAT, // array of floats
        GL_FALSE, // if its normalized
        14 * sizeof(GLfloat), // size of data per vertex
        (void*)normalPtr
    );
    glEnableVertexAttribArray(1);
    
    //UV
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GLfloat),
        (void*)uvPtr
    );
    glEnableVertexAttribArray(2);

    //Tangent
    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GLfloat),
        (void*)tangentPtr
    );
    glEnableVertexAttribArray(3);

    //Bitangent
    glVertexAttribPointer(
        4,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GLfloat),
        (void*)bitangentPtr
    );
    glEnableVertexAttribArray(4);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
#endif

//MAIN
int main(void)
{
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit()) return -1;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_SAMPLES, 8);
    
    window = glfwCreateWindow(windowWidth, windowHeight, "Niks :>", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //Can be used for split screen games
    //glViewport(0,0,1280,720);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress); // if using CMAKE
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    Shader defaultShader("Shaders/classShader.vert", "Shaders/classShader.frag");
    Shader skyboxShader("Shaders/skybox.vert", "Shaders/skybox.frag");
    Shader nvShader("Shaders/classShader.vert", "Shaders/nvShader.frag");

    //Load Vertices
    GLfloat vertices[]{
        0.f, 0.5f, 0.f,     //0
        -1.f, -0.f, 0.f,    //1
        0.5f, -0.5f, 0.f    //2
    };
    GLuint indices[]{ 0,1,2 };

    float skyboxVertices[]{
    -1.f, -1.f, 1.f, //0
    1.f, -1.f, 1.f,  //1
    1.f, -1.f, -1.f, //2
    -1.f, -1.f, -1.f,//3
    -1.f, 1.f, 1.f,  //4
    1.f, 1.f, 1.f,   //5
    1.f, 1.f, -1.f,  //6
    -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6, //R
        6,5,1,

        0,4,7, //L
        7,3,0,

        4,5,6, //T
        6,7,4,

        0,3,2, //B
        2,1,0,

        0,1,5, //Front
        5,4,0,

        3,7,6, //Back
        6,2,3
    };

    //SKY BOX (DO NOT TOUCH)
    unsigned int skyVAO, skyVBO, skyEBO;
    glGenVertexArrays(1, &skyVAO);
    glGenBuffers(1, &skyVBO);
    glGenBuffers(1, &skyEBO);

    glBindVertexArray(skyVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(skyboxVertices),
        &skyboxVertices,
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyEBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(int) * 36,
        &skyboxIndices,
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    //SKY BOX Tex
    
    std::string faceSky[]{
        "Skybox/cubemap1_right.png",
        "Skybox/cubemap1_left.png",
        "Skybox/cubemap1_top.png",
        "Skybox/cubemap1_bottom.png",
        "Skybox/cubemap1_front.png",
        "Skybox/cubemap1_back.png"
    };
    

    unsigned int skyboxTex;
    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //if 3d this, else only s and t
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyCh;
        stbi_set_flip_vertically_on_load(false);

        unsigned char* skybytes = stbi_load(faceSky[i].c_str(), &w, &h, &skyCh, 0);

        if (skybytes) {
            GLenum format = (skyCh == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, skybytes);
            stbi_image_free(skybytes);
        }
        else {
            std::cout << "Failed to load skybox face: " << faceSky[i] << std::endl;
            stbi_image_free(skybytes);
        }
        
    }

    //Model newModel = Model("BOLTER_low", "BOLTER_Albedo", ".jpg");
    Model newModel = Model("sm_altaytank_1", "T_Altay_1_BaseMap", ".png", "T_Altay_1_Normal", ".png");
    Model floorModel = Model("plane", "patchy-meadow1_albedo", ".png", "patchy-meadow1_normal-ogl", ".png");
    Model treeStem = Model("MapleTreeStem", "maple_bark", ".png", "maple_bark_normal", ".png");
    Model treeLeaves = Model("MapleTreeLeaves", "maple_leaf", ".png", "", "");
       
    //Create new model plane then initialize texture, overlay map, and normals
    newModel.InitModel();
    newModel.InitTextures();
    newModel.InitNormals();

    floorModel.InitModel();
    floorModel.InitTextures();
    floorModel.InitNormals();

    treeStem.InitModel();
    treeStem.InitTextures();
    treeStem.InitNormals();

    treeLeaves.InitModel();
    treeLeaves.InitTextures();
    treeLeaves.InitNormals();


    //Camera instances
    topDown.Projection = glm::ortho(
        -60.f, //L
        60.f, //R
        -60.f, //Bottom
        60.f, //Top
        -100.0f, //Near
        10000.f //Far
    );

    float offsetX[10][10], offsetZ[10][10];
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            offsetX[i][j] = dist(rng);
            offsetZ[i][j] = dist(rng);
        }
    }
    
    //Enable anti-aliasing and blend
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        float pointLightFront = 8.f;
        float pointLightY = 1.f;

        //Cam
        glm::vec3 tankFront = glm::normalize(glm::vec3(
            cos(glm::radians(-modelYaw)),
            0.f,
            sin(glm::radians(-modelYaw))
        ));
        glm::vec3 tankRight = glm::normalize(glm::cross(tankFront, glm::vec3(0.f, 1.f, 0.f)));
        glm::vec3 lookTarget = glm::vec3(1.f);
        float totalYaw;

        switch (cameraType)
        {
        case THIRDPERSON:
            thirdPerson.Projection = glm::perspective(glm::radians(60.f), windowWidth / windowHeight, 0.1f, 75.f);
            totalYaw = -modelYaw + orbitYawOffset;

            thirdPerson.Position = modelPosition + glm::vec3(
                CAM_DIST * cos(glm::radians(orbitPitch)) * cos(glm::radians(totalYaw)),
                CAM_DIST * sin(glm::radians(orbitPitch)),
                CAM_DIST * cos(glm::radians(orbitPitch)) * sin(glm::radians(totalYaw))
            );

            lookTarget = modelPosition + glm::vec3(0.f, 1.5f, 0.f);

            pointLight.position = modelPosition + tankFront * pointLightFront + glm::vec3(0.f, pointLightY, 0.f);
            break;

        case FIRSTPERSON:
            // Position locked inside the tank
            firstPerson.Position = modelPosition + tankFront * 1.5f + glm::vec3(0.f, CAM_HEIGHT, 0.f);

            // Look direction from pitch/yaw controls
            glm::vec3 fpFront;
            fpFront.x = cos(glm::radians(firstPersonPitch)) * cos(glm::radians(firstPersonYaw));
            fpFront.y = sin(glm::radians(firstPersonPitch));
            fpFront.z = cos(glm::radians(firstPersonPitch)) * sin(glm::radians(firstPersonYaw));
            fpFront = glm::normalize(fpFront);

            lookTarget = firstPerson.Position + fpFront * 20.f + glm::vec3(0.f, CAM_HEIGHT, 0.f);

            // Apply per-mode FOV
            firstPerson.Projection = glm::perspective(glm::radians(firstPersonFOV), windowWidth / windowHeight, 0.1f, 1000.f);
            
            break;

        case TOPDOWN:
            // Directly above the tank, looking straight down
            topDown.Position = topDownPosition;
            lookTarget = glm::vec3(topDownPosition.x, 0.f, topDownPosition.z);
            break;
        }
        
        processInput(window);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //Skybox
        skyboxShader.use();
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        //glm::mat4 skyView = glm::mat4(1.f);
        skyboxShader.setInt("skybox", 0);
        if (cameraType == THIRDPERSON) skyboxShader.PassSkybox(thirdPerson, lookTarget);
        else if (cameraType == FIRSTPERSON) skyboxShader.PassSkybox(firstPerson, lookTarget);
        else skyboxShader.PassOrthoSkybox(topDown);

        glBindVertexArray(skyVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        //EOF Skybox

        //Default Shader
        if (cameraType == THIRDPERSON || cameraType == TOPDOWN) {
            defaultShader.use();
            if (cameraType == THIRDPERSON) defaultShader.passPerspectiveCamera(thirdPerson, lookTarget);
            else defaultShader.passOrthoCamera(topDown);

            //Pass lights to shader
            pointLight.Apply(defaultShader, "pointLight");
            dirLight.Apply(defaultShader, "dirLight");

            //Tank
            glm::mat4 objectMatrix = glm::mat4(1.0f);
            objectMatrix = glm::translate(objectMatrix, modelPosition);
            objectMatrix = glm::scale(objectMatrix, glm::vec3(1.0f));
            objectMatrix = glm::rotate(objectMatrix, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));
            objectMatrix = glm::rotate(objectMatrix, glm::radians(modelYaw), glm::vec3(0.f, 1.f, 0.f));
            defaultShader.setFloat("tiling", 1.f);
            defaultShader.setMat4("transform", 1, objectMatrix);
            defaultShader.setBool("useAlphaClip", false);
            defaultShader.LoadTexture(newModel.GetDiffuse());
            defaultShader.LoadNormal(newModel.GetNormal());
            newModel.DrawModel();

            //Floor
            glm::mat4 floor = glm::mat4(1.0f);
            floor = glm::translate(floor, glm::vec3(0.f, 0.f, 0.f));
            floor = glm::scale(floor, glm::vec3(1000.0f));
            //floor = glm::rotate(floor, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
            defaultShader.setFloat("tiling", 50.f);
            defaultShader.setMat4("transform", 1, floor);
            defaultShader.setBool("useAlphaClip", false);
            defaultShader.LoadTexture(floorModel.GetDiffuse());
            defaultShader.LoadNormal(floorModel.GetNormal());
            floorModel.DrawModel();

            glm::mat4 trunk, leaves;
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    glm::vec3 pos = glm::vec3(5.f * i + offsetX[i][j], -2.f, 5.f * j + offsetZ[i][j]);

                    trunk = glm::mat4(1.0f);
                    trunk = glm::translate(trunk, pos);
                    trunk = glm::scale(trunk, glm::vec3(0.5f));
                    defaultShader.setFloat("tiling", 1.f);
                    defaultShader.setMat4("transform", 1, trunk);
                    defaultShader.setBool("useAlphaClip", false);
                    defaultShader.LoadTexture(treeStem.GetDiffuse());
                    defaultShader.LoadNormal(treeStem.GetNormal());
                    treeStem.DrawModel();

                    leaves = glm::mat4(1.0f);
                    leaves = glm::translate(leaves, pos);
                    leaves = glm::scale(leaves, glm::vec3(0.5f));
                    defaultShader.setFloat("tiling", 1.f);
                    defaultShader.setMat4("transform", 1, leaves);
                    defaultShader.setBool("useAlphaClip", true);
                    defaultShader.LoadTexture(treeLeaves.GetDiffuse());
                    defaultShader.LoadNormal(treeLeaves.GetNormal());
                    treeLeaves.DrawModel();

                }
            }
        }

        else {
            nvShader.use();
            nvShader.passPerspectiveCamera(firstPerson, lookTarget);

            //Pass lights to shader
            pointLight.Apply(nvShader, "pointLight");
            dirLight.Apply(nvShader, "dirLight");

            //Tank
            glm::mat4 objectMatrix = glm::mat4(1.0f);
            objectMatrix = glm::translate(objectMatrix, modelPosition);
            objectMatrix = glm::scale(objectMatrix, glm::vec3(1.0f));
            objectMatrix = glm::rotate(objectMatrix, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));
            objectMatrix = glm::rotate(objectMatrix, glm::radians(modelYaw), glm::vec3(0.f, 1.f, 0.f));
            nvShader.setFloat("tiling", 1.f);
            nvShader.setMat4("transform", 1, objectMatrix);
            nvShader.setBool("useAlphaClip", false);
            nvShader.LoadTexture(newModel.GetDiffuse());
            nvShader.LoadNormal(newModel.GetNormal());
            newModel.DrawModel();

            //Floor
            glm::mat4 floor = glm::mat4(1.0f);
            floor = glm::translate(floor, glm::vec3(0.f, 0.f, 0.f));
            floor = glm::scale(floor, glm::vec3(1000.0f));
            //floor = glm::rotate(floor, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
            nvShader.setFloat("tiling", 50.f);
            nvShader.setMat4("transform", 1, floor);
            nvShader.setBool("useAlphaClip", false);
            nvShader.LoadTexture(floorModel.GetDiffuse());
            nvShader.LoadNormal(floorModel.GetNormal());
            floorModel.DrawModel();

            glm::mat4 trunk, leaves;
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    glm::vec3 pos = glm::vec3(5.f * i + offsetX[i][j], -2.f, 5.f * j + offsetZ[i][j]);

                    trunk = glm::mat4(1.0f);
                    trunk = glm::translate(trunk, pos);
                    trunk = glm::scale(trunk, glm::vec3(0.5f));
                    nvShader.setFloat("tiling", 1.f);
                    nvShader.setMat4("transform", 1, trunk);
                    nvShader.setBool("useAlphaClip", false);
                    nvShader.LoadTexture(treeStem.GetDiffuse());
                    nvShader.LoadNormal(treeStem.GetNormal());
                    treeStem.DrawModel();

                    leaves = glm::mat4(1.0f);
                    leaves = glm::translate(leaves, pos);
                    leaves = glm::scale(leaves, glm::vec3(0.5f));
                    nvShader.setFloat("tiling", 1.f);
                    nvShader.setMat4("transform", 1, leaves);
                    nvShader.setBool("useAlphaClip", true);
                    nvShader.LoadTexture(treeLeaves.GetDiffuse());
                    nvShader.LoadNormal(treeLeaves.GetNormal());
                    treeLeaves.DrawModel();

                }
            }
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    //lightModel.DeleteBuffers();
    newModel.DeleteBuffers();
    floorModel.DeleteBuffers();

    //Terminate gl
    glfwTerminate();
    return 0;
}

