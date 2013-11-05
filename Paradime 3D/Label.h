// label.h
// class and functions for generating labels

#ifndef TEXT_TO_TEXTURE
#define TEXT_TO_TEXTURE

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_ttf.h>

class Label {
public:
	Label(const char * str,TTF_Font *font, SDL_Color colour);
	GLuint getWidth(void) { return width; }
	GLuint getHeight(void) { return height; }
	GLuint getTex(void) {return texID; }
	static void displayString(float x, float y, const char * str, TTF_Font * textFont);
	static GLuint textToTexture(const char * str,TTF_Font *font, SDL_Color colour,GLuint &width,GLuint &height);
	void draw(GLfloat x, GLfloat y);
	void draw(GLfloat x, GLfloat y, GLfloat scale);
private:
	GLuint width;
	GLuint height;
	GLuint texID;
};


#endif