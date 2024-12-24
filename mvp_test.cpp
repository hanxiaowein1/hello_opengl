#include "unit_test.h"
#include "opengl_chaos.h"
#include "chaos_shader.h"
#include "chaos_shower.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TEST(GlobalTest, mvp_triangle)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (window == NULL)
	{
        std::string err_msg = "Failed to create GLFW window";
		std::cout << err_msg << std::endl;
		glfwTerminate();
		// return -1;
        throw std::exception(err_msg.c_str());
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
        std::string err_msg = "Failed to initialize GLAD";
		std::cout << err_msg << std::endl;
		throw std::exception(err_msg.c_str());
	}
	glEnable(GL_DEPTH_TEST);

	Shader self_shader(
        "D:\\Projects\\hello_opengl\\shader\\mvp_test_shader_vs.glsl",
        "D:\\Projects\\hello_opengl\\shader\\mvp_test_shader_fs.glsl"
    );
    std::vector<float> vertices1{
		0, 0, 0,
		0.5f, 0, 0,
		0, 0.5f, 0,
    };
    std::vector<unsigned int> indices1{
        0, 1, 2,
    };
    ChaosShower shower1(vertices1, indices1, self_shader);

    Shader self_shader2(
        "D:\\Projects\\hello_opengl\\shader\\mvp_test_shader_vs.glsl",
        "D:\\Projects\\hello_opengl\\shader\\mvp_test_shader_fs_2.glsl"
    );
    std::vector<float> vertices2{
		0.5f, 0, 0,
		0, 0.5f, 0,
		//0.7f, 0.7f, 0.3f
		//0.7f, 0.7f, -300.0f
		0.7f, 0.7f, 0.1f
    };
    std::vector<unsigned int> indices2{
        0, 1, 2,
    };
    ChaosShower shower2(vertices2, indices2, self_shader2);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //self_shader.use();

		glm::mat4 view = glm::mat4(1.0f);
		// note that we're translating the scene in the reverse direction
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
		//view = glm::translate(view, glm::vec3(0.1f, 0.3f, -2.0f));
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.00001f, 1000.0f);
        glm::mat4 model = glm::mat4(1.0f);

        shower1.shader.set_mat4("view", view);
        shower1.shader.set_mat4("projection", projection);
        shower1.shader.set_mat4("model", model);
		shower1.show();
        // glDrawArrays(GL_TRIANGLES, 0, 36);
		shower2.shader.set_mat4("view", view);
		shower2.shader.set_mat4("projection", projection);
		shower2.shader.set_mat4("model", model);
		shower2.show();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

}