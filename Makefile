NAME=ft_vox

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	FLAGS_OS = -lGL -ldl -lm
else
	FLAGS_OS = -framework OpenGL
endif

CC=clang
# FLAGS = -Wall -Werror -Wextra
FLAGS = -Wuninitialized
FLAGS += -Wno-documentation
FLAGS += -Wno-documentation-unknown-command
FLAGS += -Wno-reserved-id-macro
FLAGS += -Wno-incompatible-pointer-types-discards-qualifiers
FLAGS += -Wno-unused-variable
FLAGS += -Wno-implicit-float-conversion
FLAGS += -Wno-double-promotion
#FLAGS += -Wno-poison-system-directories

# Flags variables
DEBUG ?= 0
ifeq ($(DEBUG), 1)
	FLAGS += -g3 -fsanitize=address
endif

VALGRIND_CHECK ?= 0
ifeq ($(VALGRIND_CHECK), 1)
	FLAGS += -g
endif

OPTI ?= 0
ifeq ($(OPTI), 1)
	FLAGS += -Ofast -ffast-math -march=native
endif

SRC_PATH=src/
SRC_NAME=main.c\
	error.c\
	free.c\
	render.c\
	singletons.c\
	actions/events.c\
	actions/live.c\
	actions/toggles.c\
	generation/generation.c\
	generation/height_map.c\
	generation/noise.c\
	generation/caves.c\
	generation/cave_vertexs.c\
	generation/vertexs.c\
	generation/borders.c\
	generation/update_world.c\
	generation/water.c\
	init/camera.c\
	init/init.c\
	init/init_opengl.c\
	init/init_utils.c\
	init/init_world.c\
	init/light.c\
	init/meshs.c\
	init/model.c\
	init/shaders.c\
	init/shadows.c\
	init/settings.c\
	init/textures.c\
	init/uniforms.c\
	utils/fps.c\
	utils/viewport.c

SRC=$(addprefix $(SRC_PATH), $(SRC_NAME))

INC_PATH=include/
INC_NAME=main.h\
	camera.h\
	error.h\
	keys.h\
	light.h\
	model.h\
	shaders.h\
	gen.h

INC=$(addprefix $(INC_PATH), $(INC_NAME))

OBJS=$(SRC:.c=.o)

########################## Librarys ######################
LIBS_PATH=libs

LIB_PATH=$(LIBS_PATH)/libft
LIB=$(LIB_PATH)/libft.a

LIB_MAT_VEC_PATH=$(LIBS_PATH)/lib_mat_vec
LIB_MAT_VEC=$(LIB_MAT_VEC_PATH)/lib_mat_vec.a

LODEPNG_PATH=$(LIBS_PATH)/lodepng
LODEPNG=$(LODEPNG_PATH)/liblodepng.a

LIB_GLAD_PATH = $(LIBS_PATH)/glad
GLAD_PATH = $(LIB_GLAD_PATH)/include/glad/
LIB_GLAD = $(LIB_GLAD_PATH)/libglad.a

##########################################################

all: $(NAME)

$(NAME): $(LIB) $(LIB_MAT_VEC) $(LODEPNG) $(LIB_GLAD) $(OBJS)
	$(CC) $(FLAGS) -o $(NAME) $(OBJS) $(LIB) $(LIB_MAT_VEC) $(LODEPNG) $(LIB_GLAD) -lpthread -lglfw $(FLAGS_OS)

$(SRC_PATH)%.o: $(SRC_PATH)%.c $(INC)
	@tput civis
	@printf " Compiling $<"
	@printf "									   \\r"
	@tput cnorm
	@$(CC) $(FLAGS) -I$(INC_PATH) -I$(LIB_PATH) -I$(LIB_MAT_VEC_PATH) -I$(GLAD_PATH) -I$(LODEPNG_PATH) -o $@ -c $<

########################## Library rules ##########################

$(LIB): $(LIB_PATH)
	@echo "Making Libft..."
	@make -C $(LIB_PATH) -j4

$(LIB_MAT_VEC): $(LIB_MAT_MAT_VEC_PATH)
	@echo "Making lib_mat_vec..."
	@make -C $(LIB_MAT_VEC_PATH) -j4

$(LODEPNG): $(LODEPNG_PATH)
	@echo "Making lodepng..."
	@make -C $(LODEPNG_PATH) -j4

$(LIB_GLAD): $(LIB_GLAD_PATH)
	@echo "Making libglad..."
	@make -C $(LIB_GLAD_PATH) -j4


###################################################################

clean:
	@rm -rf $(OBJS)
	@make -C $(LIB_PATH) clean
	@make -C $(LIB_MAT_VEC_PATH) clean
	@make -C $(LIB_GLAD_PATH) clean

fclean: clean
	@rm -rf $(NAME)
	@make -C $(LIB_PATH) fclean
	@make -C $(LIB_MAT_VEC_PATH) fclean
	@make -C $(LIB_GLAD_PATH) fclean

re: fclean all
