#include "rendervertexdata.h"

RenderVertexData::RenderVertexData()
{

}

RenderVertexData::~RenderVertexData()
{
	for (auto iter = m_programs.begin(); iter != m_programs.end(); iter++) {
		GLuint program = (*iter).second;
		glDeleteProgram(program);
	}

	for (auto iter = m_vbo.begin(); iter != m_vbo.end(); iter++) {
		QOpenGLBuffer vbo = (*iter).second;
		vbo.destroy();
	}

	for (auto iter = m_vao.begin(); iter != m_vao.end(); iter++) {
		GLuint vao = (*iter).second;
		glDeleteVertexArrays(1, &vao);
	}
}

void RenderVertexData::initOpenGLFunctions()
{
	initializeOpenGLFunctions();

}

GLuint RenderVertexData::createProgram(std::string program_name)
{
	m_programs[program_name] = glCreateProgram();
	return m_programs[program_name];
}

bool RenderVertexData::compileShader(std::string program_name, const char* vshader, const char* gshader, const char* fshader)
{
	return initShader(m_programs[program_name], vshader, gshader, fshader);
}

bool RenderVertexData::compileShader(std::string program_name, const char* vshader, const char* fshader)
{
	return initShader(m_programs[program_name], vshader, fshader);
}

void RenderVertexData::useProgram(std::string program_name)
{
	glUseProgram(m_programs[program_name]);
}


void RenderVertexData::vboBind(std::string name)
{
	m_vbo[name].bind();
}

void RenderVertexData::vboRelease(std::string name)
{
	m_vbo[name].release();
}

void RenderVertexData::vboAllocate(std::string name, const void* data, int count)
{
	m_vbo[name].allocate(data, count);
}

void  RenderVertexData::vboWrite(std::string name, int offset, const void* data, int count)
{
	m_vbo[name].write(offset, data, count);
}

void RenderVertexData::vboCreate(std::string name, Buffer_Type type, Buffer_Usage_Type usage)
{
	if (type == Buffer_Type::VERTEX)
		m_vbo[name] = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	else if (type == Buffer_Type::INDEX)
		m_vbo[name] = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

	m_vbo[name].create();

	if (usage == Buffer_Usage_Type::DYNAMIC_DRAW)
		m_vbo[name].setUsagePattern(QOpenGLBuffer::DynamicDraw);
	else if (usage == Buffer_Usage_Type::STATIC)
		m_vbo[name].setUsagePattern(QOpenGLBuffer::StaticDraw);

}

QOpenGLBuffer RenderVertexData::getVBO(std::string name)
{
	return m_vbo[name];
}

//************** VAO
void RenderVertexData::vaoCreate(std::string name)
{
	glGenVertexArrays(1, &m_vao[name]);
}

void RenderVertexData::vaoBind(std::string name)
{
	glBindVertexArray(m_vao[name]);
}

void RenderVertexData::vaoRelease()
{
	glBindVertexArray(0);
}
