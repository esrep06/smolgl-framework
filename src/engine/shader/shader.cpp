#include "shader.hpp"

namespace sm
{
	shader::shader(std::string vertex, std::string fragment)
		: m_vertex_path(vertex), m_fragment_path(fragment)
	{}

	shader::shader()
	{}

	uint8_t shader::load_and_compile()
	{
		std::getline(std::ifstream(m_vertex_path), m_vertex_string, '\0');
		std::getline(std::ifstream(m_fragment_path), m_fragment_string, '\0');

		utilz::logger::log("Compiling shader: " + m_vertex_path + " & " + m_fragment_path + "...\n");

		uint32_t vertexId, fragmentId;

		vertexId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexId, 1, (const char* const*)&m_vertex_string, NULL);
		glCompileShader(vertexId);

		int success;
		char log[512];
		glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertexId, 512, NULL, log);
			free();
			utilz::logger::log("Shader vertex compilation failed\n", utilz::logger::ERROR);
			fputs(log, stderr);
			return 0;
		}

		fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentId, 1,  (const char* const*)&m_fragment_string, NULL);
		glCompileShader(fragmentId);

		glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(fragmentId, 512, NULL, log);
			free();
			utilz::logger::log("Shader fragment compilation failed\n", utilz::logger::ERROR);
			fputs(log, stderr);
			return 0;
		}

		m_id = glCreateProgram();

		glAttachShader(m_id, vertexId);
		glAttachShader(m_id, fragmentId);

		glLinkProgram(m_id);

		glGetProgramiv(m_id, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(m_id, 512, NULL, log);
			utilz::logger::log("Shader program link failed\n", utilz::logger::ERROR);
			fputs(log, stderr);
			return 0;
		}

		m_was_initialized = 1;
		m_being_used = 0;

		glDeleteShader(vertexId);
		glDeleteShader(fragmentId);

		detach();

		utilz::logger::log("Shader compiled successfully!\n", utilz::logger::SUCCESS);

		return 1;
	}

	void shader::free()
	{
		if (!m_was_initialized) return;

		utilz::logger::log("Freeing shader '" + m_vertex_path + "' memory\n");

		glDeleteProgram(m_id);

		m_was_freed = 1;
	}

	void shader::detach()
	{
		if (!m_being_used) return;

		glUseProgram(0);
		m_being_used = 0;
	}

	void shader::use()
	{
		if (m_being_used) return;

		glUseProgram(m_id);
		m_being_used = 1;
	}

	void shader::send_float(float val, std::string name)
	{ 
		use();
		glUniform1f(glGetUniformLocation(m_id, name.c_str()), val);
		detach();
	}

	void shader::send_mat4(glm::mat4 mat, std::string name)
	{ 
		use();
		glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat)); 
		detach();
	}

	shader::~shader()
	{
		free();
	}
}

