#ifndef MAINOPENGL_H
#define MAINOPENGL_H

// OpenGL
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_4_3_Core>

// IO
#include <QDebug>
#include <QMouseEvent>
#include <iostream>

// text includes
#include <QtGui/QImage>
#include <QPainter>

// data structures
#include <QFontMetrics>
#include <QHash>
#include <unordered_map>
#include <array>
#include <QVector3D>
#include <memory>

// file manipulations
#include <QString>
#include <QFile>


class MainOpenGL : public QOpenGLFunctions_4_3_Core
{
public:
	MainOpenGL();
	~MainOpenGL();

	bool initShader(QOpenGLShaderProgram* program, const char* vshader, const char* gshader, const char* fshader);
	bool initShader(GLuint program, const char* vshader, const char* gshader, const char* fshader);
	bool initShader(GLuint program, const char* vshader, const char* fshader);

	void GL_Error();
	unsigned int nearestPowerOfTwo(unsigned int _num);
	void initText(const QFont& _f);
	void renderText(float x, float y, float scaleX, float scaleY, const QString& text);


public:
	struct FontChar {
		int width;
		GLuint textureID;
		QOpenGLVertexArrayObject* vao;
	};

	QHash <char, FontChar>      m_characters;
	QOpenGLShaderProgram* m_program_text;
	QMatrix4x4                  m_projection;
	bool                        flag;

};

#endif // MAINOPENGL_H
