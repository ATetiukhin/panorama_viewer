#include "quick_view.h"

#include "opengl_renderer.h"

#include <QQuickWindow>

quick_view_t::quick_view_t(QQuickItem * const parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents);
    setAcceptedMouseButtons(Qt::AllButtons);
}

QSGNode * quick_view_t::updatePaintNode(QSGNode * node, UpdatePaintNodeData *)
{
    QSGRenderNode * render_node = static_cast<QSGRenderNode *>(node);
    if (!render_node) {
        active_node_ = new opengl_render_node_t(this);
        render_node = active_node_;
    }

    return render_node;
}

void quick_view_t::wheelEvent(QWheelEvent * const event) {

    QPoint const number_degrees = event->angleDelta() / 8;
    if (active_node_ && !number_degrees.isNull()) {
        active_node_->set_zoom(number_degrees.y() / 15);
    }

    event->accept();
    window()->update();
}

void quick_view_t::mousePressEvent(QMouseEvent * const event)
{
    position_ = event->globalPos();
    last_position_ = position_;

    event->accept();
}

void quick_view_t::mouseMoveEvent(QMouseEvent * const event)
{
    if (active_node_)
        active_node_->set_move(event->globalPos() - last_position_);

    last_position_ = position_;
    window()->update();
    event->accept();
}

void quick_view_t::open_file(QString const & uri)
{
    QString const filename = QUrl(uri).toLocalFile();
    QImage image(filename);
    if (image.isNull()) {
        qWarning("Failed to load image %s", qPrintable(filename));
        return;
    }

    if (active_node_)
        active_node_->set_image(image);

    window()->update();
}
