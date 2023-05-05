#include "../../include/main.h"


unsigned char	mount_shadows(t_env *env)
{
    glGenFramebuffers(1, &env->model.fbo);

	glBindFramebuffer(GL_FRAMEBUFFER, env->model.fbo);

	// we link the depthmap to the framebuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, env->model.depthmap, 0);
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE
		? ERR_NONE : ERR_FRAMEBUFFER_ERROR);
}
