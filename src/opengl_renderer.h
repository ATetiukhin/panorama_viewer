#pragma once

#include <qsgrendernode.h>

class QQuickItem;
class QOpenGLTexture;
class QOpenGLShaderProgram;
class QOpenGLBuffer;

#include <QImage>

struct opengl_render_node_t final : QSGRenderNode
{
    explicit opengl_render_node_t(QQuickItem * item);
    ~opengl_render_node_t();

    void render(RenderState const * state) override;
    void releaseResources() override;
    StateFlags changedStates() const override;
    RenderingFlags flags() const override;
    QRectF rect() const override;

    void set_image(QImage const & image);

private:
    void make_object();

    QVector<GLfloat> vertex_data_;
    QImage image_;

    QQuickItem * item_;

    QOpenGLShaderProgram * shader_program_;
    QOpenGLTexture * texture_;
    QOpenGLBuffer * vbo_;
};
