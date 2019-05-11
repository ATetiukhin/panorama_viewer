#include "quick_view.h"

#include <QGuiApplication>
#include <QQuickView>

int main(int argc, char ** argv)
{
    QGuiApplication app(argc, argv);
    app.setApplicationDisplayName(QObject::tr("Panorama viewer"));
    app.setOrganizationName(QObject::tr(""));
    app.setOrganizationDomain(QObject::tr("RU"));
    app.setApplicationVersion("0.0.1");

    qmlRegisterType<quick_view_t>("SceneGraphRendering", 1, 0, "View");

    QQuickView view;
    view.setMinimumSize(QSize(300, 300));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///assets/app.qml"));
    view.resize(1024, 768);
    view.show();

    return app.exec();
}
