#include "../../include/main.h"


static unsigned char	compile_shader(unsigned int shader_id, const GLchar *source, size_t size)
{
	(void)size;

	char			info_log[4096];
	int				success;

	// Give shader source code to OpenGL
	glShaderSource(shader_id, 1, &source, NULL);
	// Compile the loaded shader source
	glCompileShader(shader_id);

	// Check for more informations about compilation
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	// If shader's compilation failed
	if (success == GL_FALSE)
	{ // Then display error log message before to exit
		memset(info_log, 0, sizeof(info_log));
		glGetShaderInfoLog(shader_id, sizeof(info_log), NULL, info_log);
		ft_putstr_fd(info_log, 2);
		return (ERR_FAILED_TO_COMPILE_SHADER);
	}
	return (ERR_NONE);
}

static unsigned char	build_shader(GLuint *shader_ptr, GLenum shader_type, t_shaders shader)
{
	unsigned char	code;
	unsigned int	shader_id;

	// Create new shader object
	shader_id = glCreateShader(shader_type);
	// Compile shader with its source code
	if ((code = compile_shader(shader_id, shader.source, shader.size)) != ERR_NONE)
		return (code);
	*shader_ptr = shader_id;
	return (ERR_NONE);
}

static unsigned char	link_program(GLuint *program, GLuint *v, GLuint *f)
{
	char	info_log[4096];
	int		success;

	*program = glCreateProgram(); // Create new program object
	glAttachShader(*program, *v); // Attach vertex shader to the program
	glAttachShader(*program, *f); // Attach fragment shader to the program
	glLinkProgram(*program); // Link the final program

	glDeleteShader(*v);
    glDeleteShader(*f);

	// Checks for more informations about compilation.
	glGetProgramiv(*program, GL_LINK_STATUS, &success);
	// If compilation failed
	if (!success)
	{ // Then display the error log message before to exit
		glGetProgramInfoLog(*program, 4096, NULL, info_log);
		ft_putendl_fd(info_log, 2);
		return (ERR_FAILED_TO_LINK_SHADER_PROGRAM);
	}
	return (ERR_NONE);
}

unsigned char		mount_shaders(GLuint *program, t_shaders vertex, t_shaders fragment)
{
	unsigned char	code;
	GLuint			v, f;

	// il faut identifier la mesh skybox,

	if ((code = build_shader(&v, GL_VERTEX_SHADER, vertex)) != ERR_NONE
			|| (code = build_shader(&f, GL_FRAGMENT_SHADER, fragment)) != ERR_NONE
			|| (code = link_program(program, &v, &f)) != ERR_NONE)
		return (code);

	return (ERR_NONE);
}

// =======================================================================

static unsigned char	load_shader_source(const char *path, const GLchar **ptr, size_t *file_size)
{
	int		fd;

	// Open file descriptor on shader file
	if ((fd = open(path, O_RDONLY)) == -1)
	{
		// errno message
		ft_putstr_fd(path, 2);
		ft_putstr_fd(" : ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (ERR_OPENING_SHADER_FILE);
	}
	// Read shader source file content
	if (!(*ptr = read_file(fd, file_size)))
	{
		close(fd);
		return (ERR_READING_FILE);
	}
	close(fd);
	return (ERR_NONE);
}

unsigned char		load_shaders(t_env *env)
{
	t_shaders		*shader;
	unsigned char	code;
	int				i;

	i = -1;
	while (++i < SHADER_MAX) {
		shader = &env->shaders[i];
		shader->id = i;
		// Map shader source file content in memory
		if ((code = load_shader_source(shaders_path[i], &shader->source, &shader->size)))
			return (code);
	}
	// penser a free les shaders
	return (ERR_NONE);
}
