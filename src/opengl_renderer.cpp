#include "opengl_renderer.h"

#include <QQuickItem>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <QtMath>

namespace
{
    GLfloat constexpr coords[4][3] = {
        { +1, -1, -1 },
        { -1, -1, -1 },
        { -1, +1, -1 },
        { +1, +1, -1 },
    };

    template <typename T> int sgn(T val)
    {
        return (T(0) < val) - (val < T(0));
    }

    struct camera_t final
    {
        explicit camera_t()
            : eye(QVector3D(0.0, 0.0, 1.0))
            , at(QVector3D(0.0, 0.0, 0.0))
            , up(QVector3D(0.0, 1.0, 0.0))
        {
        }

        QVector3D eye;
        QVector3D at;
        QVector3D up;
    };

    camera_t const camera;

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
    , x_(0)
    , y_(0)
    , z_(0)
{
    for (int j = 0; j < 4; ++j) {
        // vertex position
        vertex_data_.append(coords[j][0]);
        vertex_data_.append(coords[j][1]);
        vertex_data_.append(coords[j][2]);

        // texture coordinate
        vertex_data_.append(j == 0 || j == 3);
        vertex_data_.append(j == 0 || j == 1);
    }
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

void opengl_render_node_t::render(RenderState const * const /*state*/)
{
    if (!shader_program_) {
        ::init(shader_program_);
        matrix_uniform_ = shader_program_->uniformLocation("matrix");
    }

    QOpenGLFunctions * const f = QOpenGLContext::currentContext()->functions();

    if (!image_.isNull()) {
        f->glViewport(0, 0, item_->width(), item_->height());
        make_texture();

        shader_program_->bind();
        shader_program_->setUniformValue(matrix_uniform_, projection_matrix() * model_view_matrix());
        shader_program_->enableAttributeArray(0);
        shader_program_->enableAttributeArray(1);

        shader_program_->setAttributeBuffer(0, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
        shader_program_->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

        texture_->bind();
        f->glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(coords) / sizeof(coords[0]));
    }
}

void opengl_render_node_t::set_image(QImage const & image)
{
    image_ = image;

    x_ = 0;
    y_ = 0;
    z_ = 0;
}

void opengl_render_node_t::set_zoom(int const zoom)
{
    float constexpr x0 = -15.0f;
    float constexpr x1 = +15.0f;

    float constexpr y0 = -0.5f;
    float constexpr y1 = +1.5f;

    z_ += sgn(zoom) * (y0 + (y1 - y0) / (x1 - x0) * (zoom - x0));
    z_ = qBound(y0, z_, y1);
}

void opengl_render_node_t::set_move(QPoint const& delta)
{
    float constexpr coefficient_move = 1.0f / 50.f;

    x_ += -delta.x() / item_->width() * coefficient_move;
    y_ += delta.y() / item_->height() * coefficient_move;

    x_ = qBound(-1.0f, x_, 1.0f);
    y_ = qBound(-1.0f, y_, 1.0f);
}

void opengl_render_node_t::make_texture()
{
    delete texture_;
    texture_ = new QOpenGLTexture(image_);
    texture_->setMinificationFilter(QOpenGLTexture::Nearest);
    texture_->setMagnificationFilter(QOpenGLTexture::Linear);
    texture_->setWrapMode(QOpenGLTexture::Repeat);

    vbo_->create();
    vbo_->bind();
    vbo_->allocate(vertex_data_.constData(), vertex_data_.count() * sizeof(GLfloat));
}

QMatrix4x4 const& opengl_render_node_t::projection_matrix()
{
    qreal const width = item_->width();
    qreal const height = item_->height();

    projection_matrix_.setToIdentity();

    qreal const aspect_ratio = width / height;
    projection_matrix_.perspective(45.0, aspect_ratio, 0.1f, 20.0f);

    return projection_matrix_;
}

QMatrix4x4 const& opengl_render_node_t::model_view_matrix()
{
    model_view_matrix_.setToIdentity();
    model_view_matrix_.lookAt(camera.eye, camera.at, camera.up);
    model_view_matrix_.translate(x_, y_, z_);

    return model_view_matrix_;
}
