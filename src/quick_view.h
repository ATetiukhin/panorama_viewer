#pragma once

#include <QQuickItem>

struct opengl_render_node_t;

struct quick_view_t : QQuickItem
{
    Q_OBJECT
public:
    explicit quick_view_t(QQuickItem * parent = nullptr);

    QSGNode * updatePaintNode(QSGNode * node, UpdatePaintNodeData *) override;

    Q_INVOKABLE void open_file(QString const & uri);

private:
    opengl_render_node_t * active_node_{ nullptr };
};
