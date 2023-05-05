#ifndef ERROR_H
# define ERROR_H


# include <stdbool.h>

enum	e_error_id
{
	ERR_NONE,
	ERR_INVALID_ARGC,
	ERR_INVALID_SEED,
	ERR_OPENING_FILE,
	ERR_READING_FILE,
	ERR_MALLOC_FAILED,
	ERR_OUT_OF_BOUNDS_VALUE,
	ERR_INVALID_SETTINGS_SYNTAX,
	ERR_INVALID_SETTING_KEY,
	ERR_SETTING_KEY_NOT_FOUND,
	ERR_INCOMPATIBLE_TEXTURE_FILE,
	ERR_COULD_NOT_LOAD_BMP_FILE,
	ERR_UNRECOGNIZED_KEY_ID,
	ERR_GLFW_INIT,
	ERR_FAILED_WIN,
	ERR_FAILED_GLAD,
	ERR_FAILED_TO_COMPILE_SHADER,
	ERR_UNCOMPATIBLE_OPENGL_VERSION,
	ERR_OPENING_SHADER_FILE,
	ERR_CHDIR_FAILED,
	ERR_FAILED_TO_LINK_SHADER_PROGRAM,
	ERR_PRIMARY_MONITOR_NOT_FOUND,
	ERR_INVALID_FOV_VALUE,
	ERR_INVALID_GAMMA_VALUE,
	ERR_TEXTURE_LOADING_FAILED,
	ERR_CHUNK_NOT_FOUND,
	ERR_FRAMEBUFFER_ERROR,
	ERR_PTHREAD_CREATE_FAILED,
	ERR_MAX
};

typedef	struct	s_error
{
	char	msg[64]; // Error message
	bool	usage; // Does usage needs to be displayed
}				t_error;

static const t_error	errors[ERR_MAX] = {
	[ERR_INVALID_ARGC] = {"Invalid number of arguments.", true},
	[ERR_INVALID_SEED] = {"Invalid seed argument.", true},
	[ERR_OPENING_FILE] = {"Can not open file.", true},
	[ERR_OPENING_SHADER_FILE] = {"Can not open shader source file.", false},
	[ERR_READING_FILE] = {"Can not read file.", false},
	[ERR_MALLOC_FAILED] = {"malloc() call failed.", false},
	[ERR_INVALID_SETTINGS_SYNTAX] = {"Invalid syntax in Settings.toml.", false},
	[ERR_OUT_OF_BOUNDS_VALUE] = {"Out of bounds value in Settings.toml.", false},
	[ERR_INVALID_SETTING_KEY] = {"Unrecognized setting key.", false},
	[ERR_SETTING_KEY_NOT_FOUND] = {"Missing setting.", false},
	[ERR_COULD_NOT_LOAD_BMP_FILE] = {"Could not load bmp texture file.", false},
	[ERR_UNRECOGNIZED_KEY_ID] = {"Unrecognized key id in Settings.toml", false},
	[ERR_GLFW_INIT] = {"GLFW initialization failed.", false},
	[ERR_FAILED_WIN] = {"Failed to initialize window.", false},
	[ERR_FAILED_GLAD] = {"Failed to initialize GLAD.", false},
	[ERR_FAILED_TO_COMPILE_SHADER] = {"OpenGL : Failed to compile shader.", false},
	[ERR_UNCOMPATIBLE_OPENGL_VERSION] = {"Incompatible version of OpenGL.", false},
	[ERR_CHDIR_FAILED] = {"Chdir failed.", false},
	[ERR_FAILED_TO_LINK_SHADER_PROGRAM] = {"OpenGL : Failed to link shader program.", false},
	[ERR_PRIMARY_MONITOR_NOT_FOUND] = {"OpenGL : Failed to find primary monitor", false},
	[ERR_INVALID_FOV_VALUE] = {"Invalid fov value", false},
	[ERR_INVALID_GAMMA_VALUE] = {"Invalid gamma value", false},
	[ERR_TEXTURE_LOADING_FAILED] = {"lodepng : Failed to load texture file", false},
	[ERR_CHUNK_NOT_FOUND] = {"Could not delete unused chunk", false},
	[ERR_FRAMEBUFFER_ERROR] = {"GL :: couldn't create framebuffer", false},
	[ERR_PTHREAD_CREATE_FAILED] = {"pthread_create() failed", false}
	//[] = {"", false}
	//[] = {"", false}
	//[] = {"", false}
};


#endif
