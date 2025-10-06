#include <iostream>
#include <vector>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "Camera.h"

using namespace std;
using namespace glm;

// Rotasi (radian)
float gRotX = 0.0f;
float gRotY = 0.0f;
float gRotZ = 0.0f;

// Skala global
float gScale = 1.0f;

// Translasi global (world-space)
vec3 gOffset(0.0f);

static vector<GLfloat> makeOctahedronFlat_PN()
{

    // Titik-titik utama
    vec3 T(0, 1, 0), B(0, -1, 0);
    vec3 R(1, 0, 0), L(-1, 0, 0);
    vec3 F(0, 0, 1), K(0, 0, -1);

    struct Tri { vec3 a,b,c; };
    vector<Tri> faces = {
        // Bagian atas (+Y)
        {T, R, F}, {T, F, L}, {T, L, K}, {T, K, R},
        // Bagian bawah (-Y)
        {B, F, R}, {B, L, F}, {B, K, L}, {B, R, K}
    };

    vector<GLfloat> data; data.reserve(faces.size()*3*6);

    for (const auto& f : faces) {
        vec3 n = normalize(cross(f.b - f.a, f.c - f.a)); // normal per-face (flat)
        vec3 tri[3] = { f.a, f.b, f.c };
        for (int i=0;i<3;i++) {
            // POS
            data.push_back((GLfloat)tri[i].x);
            data.push_back((GLfloat)tri[i].y);
            data.push_back((GLfloat)tri[i].z);
            // NORMAL (flat)
            data.push_back((GLfloat)n.x);
            data.push_back((GLfloat)n.y);
            data.push_back((GLfloat)n.z);
        }
    }
    return data;
}

// Instansiasi susunan crystal (5 ring × 6 per ring)
struct Instance { mat4 M; vec3 color; };

static vector<Instance> makeCrystal(float unitScale)
{
    using namespace glm;
    vector<Instance> inst; inst.reserve(30);

    const int rings = 5, perRing = 6;
    const float R = 2.0f;          // jari-jari ring
    const float spacingY = 1.35f;  // jarak antar ring
    const float baseY = -spacingY*2;
    const float tilt = radians(12.0f);

    vec3 palette[rings] = {
        {0.95f, 0.30f, 0.35f}, // red
        {0.95f, 0.65f, 0.25f}, // yellow
        {0.30f, 0.80f, 0.60f}, // green
        {0.30f, 0.55f, 0.95f}, // blue
        {0.70f, 0.40f, 0.95f}  // purple
    };

    for (int r = 0; r < rings; ++r) {
        float y = baseY + r * spacingY;
        float phase = (r % 2 == 0) ? 0.0f : radians(30.0f);

        // radius dinamis
        float Rmax = 2.0f, Rmin = 1.0f;
        float R = mix(Rmax, Rmin, (float)r / (rings - 1));  // mengecil ke atas

        for (int i = 0; i < perRing; ++i) {
            float ang = phase + i * radians(360.0f / perRing);
            float x = R * cos(ang);
            float z = R * sin(ang);

            mat4 M(1.0f);
            M = translate(M, vec3(x, y, z));
            M = rotate(M, -ang, vec3(0, 1, 0));
            M = scale(M, vec3(unitScale));

            inst.push_back({M, palette[r]});
        }
    }

    return inst;
}

int main()
{
    const int WIDTH = 1280, HEIGHT = 720;
    if (!glfwInit()) { 
        cout << "GLFW init failed\n"; return -1; 
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Crystal", nullptr, nullptr);
    if (!window) {
        cout << "Window create failed\n"; glfwTerminate(); return -1; 
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGL()) { 
        cout << "Failed to init GLAD\n"; return -1; 
    }

    glViewport(0,0,WIDTH,HEIGHT);
    glEnable(GL_DEPTH_TEST); // test depth 3D
    glClearColor(0.04f, 0.05f, 0.07f, 1.0f); // background

    // Kamera sesuai class kamu
    Camera camera(WIDTH, HEIGHT, vec3(0.0f, -10.0f, 0.0f));

    // Shader (nama uniform mengikuti kebiasaanmu: model + camMatrix)
    Shader shader("shaders/crystal.vert", "shaders/crystal.frag");

    vector<GLfloat> vertices = makeOctahedronFlat_PN();
    const GLsizei stride = 6 * sizeof(GLfloat); // pos3 + normal3

    VAO vao; vao.Bind();
    VBO vbo(vertices.data(), (GLsizeiptr)(vertices.size()*sizeof(GLfloat)));
    // aPos -> location 0
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, stride, (void*)0);
    // aNormal -> location 1 (sesuai shader Crystal.vert)
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, stride, (void*)(3*sizeof(GLfloat)));
    vao.Unbind(); vbo.Unbind();

    // Buat semua instance
    auto instances = makeCrystal(0.5f);

    // Pencahayaan directional dari depan-atas
    vec3 lightDir   = normalize(vec3(-0.2f, -0.6f, -0.5f));
    vec3 lightColor = vec3(1.0f);
    float ambientK = 0.18f;

    // Lokasi uniform (sekali ambil)
    GLint u_model    = glGetUniformLocation(shader.ID, "model");
    GLint u_camMat   = glGetUniformLocation(shader.ID, "camMatrix");
    GLint u_normalM  = glGetUniformLocation(shader.ID, "normalMat");
    GLint u_camPos   = glGetUniformLocation(shader.ID, "camPos");
    GLint u_lightDir = glGetUniformLocation(shader.ID, "lightDir");
    GLint u_lightCol = glGetUniformLocation(shader.ID, "lightColor");
    GLint u_baseCol  = glGetUniformLocation(shader.ID, "baseColor");
    GLint u_ambientK = glGetUniformLocation(shader.ID, "ambientK");

    double last = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double now = glfwGetTime();
        float dt = float(now - last);
        last = now;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Inputs(window, dt);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // ===== INPUT GLOBAL TRANSFORM =====
        const float rotSpeedDeg = 90.0f;                 // derajat per detik
        const float rotSpeed    = radians(rotSpeedDeg);
        const float moveSpeed   = 2.5f;                  // unit per detik
        const float scaleSpeed  = 0.8f;                  // skala per detik

        const bool shiftHeld = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)  == GLFW_PRESS) ||
                               (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
        const float sgn = shiftHeld ? -1.0f : 1.0f;

        // Rotasi: X / Y / Z (Shift = arah minus)
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) gRotX += sgn * rotSpeed * dt;
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) gRotY += sgn * rotSpeed * dt;
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) gRotZ += sgn * rotSpeed * dt;

        // (opsional) Reset rotasi
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) { gRotX = gRotY = gRotZ = 0.0f; }

        // Scaling: - (down), + / = (up) ; keypad juga didukung
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
        {
            gScale -= scaleSpeed * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
        {
            gScale += scaleSpeed * dt;
        }
        gScale = clamp(gScale, 0.05f, 5.0f);

        // Translasi global (pan) dengan arrow keys pada bidang XZ
        if (glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS) gOffset.x += moveSpeed * dt;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) gOffset.x -= moveSpeed * dt;
        if (glfwGetKey(window, GLFW_KEY_UP)    == GLFW_PRESS) gOffset.z -= moveSpeed * dt;
        if (glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS) gOffset.z += moveSpeed * dt;

        // Reset offset & scale
        if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) { gOffset = vec3(0.0f); gScale = 1.0f; }

        // ===== SHADER GLOBAL UNIFORMS =====
        shader.Activate();
        camera.Matrix(shader, "camMatrix");

        glUniform3fv(u_camPos,   1, value_ptr(camera.Position));
        glUniform3fv(u_lightDir, 1, value_ptr(lightDir));
        glUniform3fv(u_lightCol, 1, value_ptr(lightColor));
        glUniform1f (u_ambientK, ambientK);

        float autoSpin = (float)now * radians(30.0f); // selalu berputar 30° per detik pada sumbu Y
        mat4 global(1.0f);
        // Urutan transform: Translate → RotateX → RotateY (manual + auto) → RotateZ → Scale
        global = translate(global, gOffset);
        global = rotate(global, gRotX, vec3(1,0,0));
        global = rotate(global, gRotY + autoSpin, vec3(0,1,0)); 
        global = rotate(global, gRotZ, vec3(0,0,1));
        global = scale(global, vec3(gScale));

        vao.Bind();
        // jumlah vertex = vertices.size()/6 (karena 6 float per vertex)
        GLsizei vertexCount = (GLsizei)(vertices.size() / 6);

        for (const auto& inst : instances) {
            mat4 M = global * inst.M;
            mat3 N = mat3(transpose(inverse(M)));

            glUniformMatrix4fv(u_model,   1, GL_FALSE, value_ptr(M));
            glUniformMatrix3fv(u_normalM, 1, GL_FALSE, value_ptr(N));
            glUniform3fv(u_baseCol,       1, value_ptr(inst.color));

            glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        }
        vao.Unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vao.Delete();
    vbo.Delete();
    shader.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
