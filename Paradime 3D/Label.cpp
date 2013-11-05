// label.cpp

#include <iostream>
#include "Label.h"


Label::Label(const char * str, TTF_Font *font, SDL_Color colour) {
	texID = textToTexture(str, font, colour, width, height);
}

// textToTexture
// A static member function: does not need an instance of the label class
GLuint Label::textToTexture(const char * str, TTF_Font *font, SDL_Color colour, GLuint &w,GLuint &h) {
	SDL_Surface *stringImage;
	stringImage = TTF_RenderText_Blended(font,str,colour);

	if (stringImage == NULL)
		//exitFatalError("String surface not created.");
		std::cout << "String surface not created." << std::endl;


	w = stringImage->w;
	h = stringImage->h;
	GLuint colours = stringImage->format->BytesPerPixel;

	GLuint format, internalFormat;
	if (colours == 4) {   // alpha
		if (stringImage->format->Rmask == 0x000000ff)
			format = GL_RGBA;
	    else
		    format = GL_BGRA;
	} else {             // no alpha
		if (stringImage->format->Rmask == 0x000000ff)
			format = GL_RGB;
	    else
		    format = GL_BGR;
	}
	internalFormat = (colours == 4) ? GL_RGBA : GL_RGB;

	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0,
                    format, GL_UNSIGNED_BYTE, stringImage->pixels);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

	// SDL surface was used to generate the texture but is no longer
	// required. Release it to free memory
	SDL_FreeSurface(stringImage);
	return texture;
}

// This function assumes that strings are dynamic:
// creating and deleting textures for the string
// Strings that remain throughout the game should only be generated once
// or should be generated at compile time and loaded as fixed textures
// Generating textures during init at run time can make it easier to change
// text, while using artist generated textures can allow for a much more
// professional quality finish on the graphics
void Label::displayString(float x, float y, const char * str,TTF_Font * textFont)
{

	SDL_Color colour = { 255, 255, 0 };
	GLuint width, height;
	GLuint tex = Label::textToTexture(str, textFont, colour, width, height);

	// Draw texture here
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
	  glTexCoord2d(0,1); // TextureLoader has origin at top not bottom
      glVertex3f (x,y, 0.0); // first corner
	  glTexCoord2d(1,1);
	  glVertex3f (x+0.002f*width, y, 0.0); // second corner
	  glTexCoord2d(1,0);
      glVertex3f (x+0.002f*width, y+0.002f*height, 0.0); // third corner
	  glTexCoord2d(0,0);
      glVertex3f (x, y+0.002f*height, 0.0); // fourth corner
    glEnd();

	glDisable(GL_TEXTURE_2D);

	glDeleteTextures(1, &tex);
}

// Draw a label with text scaled by scale factor
void Label::draw(GLfloat x, GLfloat y, GLfloat scale)
{
	// Draw texture here
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, texID);
    glBegin(GL_QUADS);
	  glTexCoord2d(0,1); // TextureLoader has origin at top not bottom
      glVertex2f (x,y); // first corner
	  glTexCoord2d(1,1);
	  glVertex2f (x+scale*width, y); // second corner
	  glTexCoord2d(1,0);
      glVertex2f (x+scale*width, y+scale*height); // third corner
	  glTexCoord2d(0,0);
      glVertex2f (x, y+scale*height); // fourth corner
    glEnd();

	glDisable(GL_TEXTURE_2D);
}

// Use default scale factor
// default scale has been selected to render legible text with
// default OpenGL projection (-1 to +1 visible view frustum)
void Label::draw(GLfloat x, GLfloat y)
{
	draw(x,y, 0.002);
}