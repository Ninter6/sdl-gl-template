#include <iostream>
#include <fstream>

#include "glad/glad.h"

#include "window.hpp"
#include "shader.hpp"
#include "mesh.hpp"

#include "mathpls.h"

using namespace mathpls;

constexpr int WIDTH = 1200, HEIGHT = 900;

struct Camera {

    float fov = 1, asp = (float)WIDTH/HEIGHT, near = .1f, far = 100.f;

    vec3 pos;
    vec3 forward;

    bool dirty = false;

    [[nodiscard]] mat4 proj() const {
        return perspective(fov, asp, near, far);
    }
    [[nodiscard]] mat4 view() const {
        return lookAt(pos, pos + forward, vec3(0, 1, 0));
    }
};

struct CamUBO {
    mat4 proj;
    mat4 view;
};

int main() {
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    Window win{WIDTH, HEIGHT, "SDL GL Template"};

    // Load OpenGL functions using glad
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    // Print OpenGL version
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    auto mesh_sh = Shader::from_file(FILE_ROOT"shader/mesh.vert", FILE_ROOT"shader/mesh.frag");
    auto sky_sh = Shader::from_file(FILE_ROOT"shader/sky.vert", FILE_ROOT"shader/sky.frag");
    auto quad_mesh_sh = Shader::from_file(FILE_ROOT"shader/quad_mesh.vert", FILE_ROOT"shader/quad_mesh.frag");

    VAO vao, quad_vao;
    vao.bind();

    CubeVBO vbo;
    vbo.bind_attrib();

    quad_vao.bind();
    QuadVBO quad_vbo;
    quad_vbo.bind_attrib();

    Camera cam;
    cam.pos = vec3(0, 0, -2);
    cam.forward = vec3(0, 0, 1);
    cam.dirty = true;
    float len = 2.f;

    Buffer cam_ubo{GL_UNIFORM_BUFFER, true};
    cam_ubo.buffer(nullptr, 0, sizeof(CamUBO));
    ((CamUBO*)cam_ubo.map())->proj = cam.proj();
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, cam_ubo.id);

    glEnable(GL_DEPTH_TEST);

    // Main loop
    SDL_Event event;
    while (win.running()) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (cam.dirty) {
            ((CamUBO*)cam_ubo.mem_map)->view = cam.view();
            cam.dirty = false;
        }

        // sky pass
        glDepthMask(false);
        sky_sh->use();
        quad_vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // mesh quad pass
        glDepthMask(true);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        quad_mesh_sh->use();
        glUniform4f(glGetUniformLocation(quad_mesh_sh->ID(), "u_MeshColor"), .1f, .1f, .1f, 1.f);
        quad_vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // mesh pass
        auto model = translate(vec3{-.5f, 0,-.5f});
        mesh_sh->use();
        glUniformMatrix4fv(glGetUniformLocation(mesh_sh->ID(), "m"), 1, 0, model.value_ptr());
        glUniform3f(glGetUniformLocation(mesh_sh->ID(), "light_dir"),-.577f,-.577f,-.577f);
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        win.next_frame(0);
        while (SDL_PollEvent(&event)) switch (event.type) {
            case SDL_EVENT_QUIT:
                win.close();
            break;
            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE)
                    win.close();
                if (event.key.key == SDLK_UP) {
                    cam.pos.y += 0.1f;
                    cam.pos.normalize() *= len;
                    cam.forward =-cam.pos;
                    cam.dirty = true;
                }
                if (event.key.key == SDLK_DOWN) {
                    cam.pos.y -= 0.1f;
                    cam.pos.normalize() *= len;
                    cam.forward =-cam.pos;
                    cam.dirty = true;
                }
                if (event.key.key == SDLK_RIGHT) {
                    cam.pos = quat{{0, 1, 0}, 0.1} * cam.pos;
                    cam.forward =-cam.pos;
                    cam.dirty = true;
                }
                if (event.key.key == SDLK_LEFT) {
                    cam.pos = quat{{0, 1, 0},-0.1} * cam.pos;
                    cam.forward =-cam.pos;
                    cam.dirty = true;
                }
                if (event.key.key == SDLK_E && len > 1) {
                    len -= .02f;
                    cam.pos = cam.pos.normalized() * len;
                    cam.forward =-cam.pos;
                    cam.dirty = true;
                }
                if (event.key.key == SDLK_Q) {
                    len += .1f;
                    cam.pos = cam.pos.normalized() * len;
                    cam.forward =-cam.pos;
                    cam.dirty = true;
                }
            break;

            case SDL_EVENT_WINDOW_RESIZED:
                // Update viewport when window is resized
                glViewport(0, 0, event.window.data1, event.window.data2);
                cam.asp = (float)event.window.data1 / (float)event.window.data2;
                ((CamUBO*)cam_ubo.mem_map)->proj = cam.proj();
            break;
        }
    }

    SDL_Quit();
    return 0;
}