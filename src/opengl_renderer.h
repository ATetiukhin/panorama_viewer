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

    void set_image(QImage const & image);
    void set_zoom(int zoom);
    void set_move(QPoint const & delta);

private:
    void make_texture();
    QMatrix4x4 const & projection_matrix();
    QMatrix4x4 const & model_view_matrix();

    QQuickItem * item_;

    QImage image_;

    QVector<GLfloat> vertex_data_;

    QOpenGLShaderProgram * shader_program_;
    QOpenGLTexture * texture_;
    QOpenGLBuffer * vbo_;

    QMatrix4x4 projection_matrix_;
    QMatrix4x4 model_view_matrix_;

    int matrix_uniform_;
    float x_;
    float y_;
    float z_;
};
