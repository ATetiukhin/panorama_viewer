#include "opengl_renderer.h"

#include <QQuickItem>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

namespace
{
    // row: vertex position (vec3) texture coordinate (vec2)
    GLfloat constexpr coords[][5] = {
        { +0.5, -0.5, 0.0, 1, 1 },
        { -0.5, -0.5, 0.0, 0, 1 },
        { -0.5, +0.5, 0.0, 0, 0 },

        { -0.5, +0.5, 0.0, 0, 0 },
        { +0.5, +0.5, 0.0, 1, 0 },
        { +0.5, -0.5, 0.0, 1, 1 },
    };

    void init(QOpenGLShaderProgram *& shader_program)
    {
        delete shader_program;
        shader_program = new QOpenGLShaderProgram;

        if (!shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/assets/texture.vert"))
            qFatal("Vertex shader compilation failed");

        if (!shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/assets/texture.frag"))
            qFatal("Vertex shader compilation failed");

        shader_program->bindAttributeLocation("vertex", 0);
        shader_program->bindAttributeLocation("tex_coord", 1);

        if (!shader_program->link())
            qFatal("Shader program is not linked");

        if (!shader_program->bind())
            qFatal("Shader program is not bind");

        shader_program->setUniformValue("texture", 0);
    }
}

opengl_render_node_t::opengl_render_node_t(QQuickItem * const item)
    : item_(item)
    , shader_program_(nullptr)
    , texture_(nullptr)
    , vbo_(new QOpenGLBuffer)
    , matrix_uniform_(-1)
{
}

opengl_render_node_t::~opengl_render_node_t()
{
    releaseResources();
}

void opengl_render_node_t::releaseResources()
{
    delete shader_program_;
    shader_program_ = nullptr;

    delete vbo_;
    vbo_ = nullptr;

    delete texture_;
    texture_ = nullptr;
}

void opengl_render_node_t::render(RenderState const * const state)
{
    if (!shader_program_)
    {
        ::init(shader_program_);
        matrix_uniform_ = shader_program_->uniformLocation("matrix");
    }

    QOpenGLFunctions * const f = QOpenGLContext::currentContext()->functions();

    if (!image_.isNull()) {
        make_object();

        shader_program_->bind();
        shader_program_->setUniformValue(matrix_uniform_, *state->projectionMatrix() * *matrix());

        shader_program_->enableAttributeArray(0);
        shader_program_->enableAttributeArray(1);

        shader_program_->setAttributeBuffer(0, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
        shader_program_->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

        f->glDrawArrays(GL_TRIANGLES, 0, sizeof(coords) / sizeof(coords[0]));
    }
}

QSGRenderNode::StateFlags opengl_render_node_t::changedStates() const
{
    return BlendState;
}

QSGRenderNode::RenderingFlags opengl_render_node_t::flags() const
{
    return BoundedRectRendering | DepthAwareRendering;
}

QRectF opengl_render_node_t::rect() const
{
    return QRect(0, 0, item_->width(), item_->height());
}

void opengl_render_node_t::set_image(QImage const & image)
{
    image_ = image;
}

void opengl_render_node_t::make_object()
{
    delete texture_;
    texture_ = new QOpenGLTexture(image_);
    texture_->setMinificationFilter(QOpenGLTexture::Nearest);
    texture_->setMagnificationFilter(QOpenGLTexture::Linear);
    texture_->setWrapMode(QOpenGLTexture::Repeat);
    texture_->bind();

    vbo_->create();
    vbo_->bind();
    vbo_->allocate(coords, sizeof(coords));
}
