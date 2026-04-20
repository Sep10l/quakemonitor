#include "mainwindow.h"
#include "../../source/repos/RaspPIconaf/src/run.h"
#include <tmap.h>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QImageReader>
#include <QToolButton>
#include <QDebug>
#include <QStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowSize(QSize(1920, 1080));
    w.setFixedSize(w.getWindowSize());
    w.show();

    int quakemonitor = quakemonitor_run();

    QString appDir = QCoreApplication::applicationDirPath();

    QPixmap homePixmap(appDir + "/assets/icons/home.png");
    QPixmap settingsPixmap(appDir + "/assets/icons/settings.png");
    QPixmap userPixmap(appDir + "/assets/icons/user_icon.png");
    QPixmap mapPixmap(appDir + "/assets/icons/map.png");

    QImage homeImage = homePixmap.toImage().convertToFormat(QImage::Format_ARGB32);
    QImage settingsImage = settingsPixmap.toImage().convertToFormat(QImage::Format_ARGB32);
    QImage userImage = userPixmap.toImage().convertToFormat(QImage::Format_ARGB32);
    QImage mapImage = mapPixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    QIcon homeIcon = w.makeImageBackgroundTransparent(&homeImage);
    QIcon settingsIcon = w.makeImageBackgroundTransparent(&settingsImage);
    QIcon userIcon = w.makeImageBackgroundTransparent(&userImage);
    QIcon mapIcon = w.makeImageBackgroundTransparent(&mapImage);

    // Home button && icon
    w.createButton("HOME",
                   w.home_button,
                   w.getButtonCorrectSize(w.home_button, w.defaultws, "HOME", homeIcon),
                   &homeIcon,
                   w.defaultws,
                   Qt::ToolButtonTextUnderIcon,
                   w.beige
    );

    // Settings button && icon
    w.createButton("",
                   w.settings_button,
                   w.getButtonCorrectSize(w.settings_button, w.defaultws, NULL, settingsIcon),
                   &settingsIcon,
                   w.defaultws,
                   Qt::ToolButtonTextUnderIcon,
                   w.beige
    );

    // User button && icon
    w.createButton("ME",
                   w.user_button,
                   w.getButtonCorrectSize(w.user_button, w.defaultws, "ME", userIcon),
                   &userIcon,
                   w.defaultws,
                   Qt::ToolButtonTextUnderIcon,
                   w.beige
    );

    // Map button & icon
    w.createButton("MAP",
                   w.map_button,
                   w.getButtonCorrectSize(w.map_button, w.defaultws, "MAP", mapIcon),
                   &mapIcon,
                   w.defaultws,
                   Qt::ToolButtonTextUnderIcon,
                   w.beige
    );

    // Fix hardcoded values, good for now
    w.setButtonLocations();
    w.setButtonSizes(5);

    QPalette pal = w.palette();
    pal.setColor(QPalette::Window, w.beige);
    w.setPalette(pal);

    // qDebug() << settings_button->

    // qDebug() << "Button size:" << home_button->size();
    // qDebug() << QImageReader::supportedImageFormats();

    QVBoxLayout* layout = new QVBoxLayout;

    // YOU HAVE TO ADD EVERY OBJECT TO THE LAYOUT AFTER
    //layout->addWidget(home_button);

    w.setLayout(NULL);
    w.setLayout(layout);

    if (quakemonitor == 1)
    {
        qDebug() << "The application initialization failed. If this problem persists, talk to the application developer";
        return 1;
    }

    return QCoreApplication::exec();
}
