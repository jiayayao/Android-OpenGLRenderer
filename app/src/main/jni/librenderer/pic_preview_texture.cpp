#include "./pic_preview_texture.h"

#define LOG_TAG "PicPreviewTexture"

PicPreviewTexture::PicPreviewTexture() {
}

PicPreviewTexture::~PicPreviewTexture() {
}

bool PicPreviewTexture::createTexture() {
	LOGI("enter PicPreviewTexture::createTexture");
	texture = 0;
	int ret = initTexture();
	if (ret < 0) {
		LOGI("init texture failed...");
		this->dealloc();
		return false;
	}
	return true;
}

void PicPreviewTexture::updateTexImage(byte* pixels, int frameWidth, int frameHeight) {
	if (pixels) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		if (checkGlError("glBindTexture")) {
			return;
		}
		// 将pixels中的RGBA数据上传至显卡texture代表的纹理对象中
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frameWidth, frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}
}

bool PicPreviewTexture::bindTexture(GLint uniformSampler) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (checkGlError("glBindTexture")) {
		return false;
	}
	// 设置sampler采样器的位置值，或者说纹理单元GL_TEXTURE0
	glUniform1i(uniformSampler, 0);
	return true;
}

bool PicPreviewTexture::bindTexture2(GLint uniformSampler) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (checkGlError("glBindTexture")) {
		return false;
	}
	// 设置sampler采样器的位置值，或者说纹理单元GL_TEXTURE0
	glUniform1i(uniformSampler, 0);
	return true;
}

int PicPreviewTexture::initTexture() {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 设置纹理过滤方式，GL_LINEAR为线性过滤，有插值效果
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// 设置ST纹理坐标的环绕方式，GL_CLAMP_TO_EDGE为约束纹理坐标0~1之间，超出部分为边缘拉伸效果
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return 1;
}

bool PicPreviewTexture::checkGlError(const char* op) {
	GLint error;
	for (error = glGetError(); error; error = glGetError()) {
		LOGI("error::after %s() glError (0x%x)\n", op, error);
		return true;
	}
	return false;
}

void PicPreviewTexture::dealloc() {
	LOGI("enter PicPreviewTexture::dealloc");
	if (texture) {
		// 删除纹理对象，如果不删除，会造成显存泄漏
		glDeleteTextures(1, &texture);
	}
}
