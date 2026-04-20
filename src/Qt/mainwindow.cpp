#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFont>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QSize MainWindow::getWindowSize()
{
    return windowSize;
}

void MainWindow::setWindowSize(const QSize& size)
{
    windowSize = size;
}

QSize MainWindow::getButtonCorrectSize(QToolButton* button, const QSize& actualSize, const char* text, const QIcon& icon)
{
    if (icon.isNull() && !text) return QSize(0,0);

    QFontMetrics metrics(button->font());

    if (!icon.isNull() && text) return QSize(metrics.horizontalAdvance(text) + 4, metrics.height() + icon.actualSize(actualSize).height() + 4);
    if (!icon.isNull() && !text  ) return QSize(icon.actualSize(actualSize).width() + 4, icon.actualSize(actualSize).height() + 4);
    if (icon.isNull() && text) return QSize(metrics.horizontalAdvance(text), metrics.height());
    return QSize(metrics.horizontalAdvance(text), metrics.height());
}

int MainWindow::calculateTargetPoint(int window_coord, int divisor, int button_size)
{
    return ((window_coord * button_size) - (window_coord / divisor));
}

void MainWindow::createButton(const QString& text, QToolButton* button, const QSize& resize, const QIcon* icon, const QSize& icon_size, Qt::ToolButtonStyle buttonStyle, const QColor color)
{
    if (!button) return;

    button->setText(text);

    if (resize.width() != 0 || resize.height() != 0) {
        button->resize(resize);
    }

    if (icon) {
        button->setIcon(*icon);
        button->setIconSize(icon_size);
    }
    // else qDebug() << "Icon " << icon.name() << " points to null";

    button->show();
    button->setToolButtonStyle(buttonStyle);

    QPalette pal = button->palette();
    pal.setColor(QPalette::Button, color);
    // pal.setColor(QPalette::ButtonText, textColor)
    button->setAutoFillBackground(true);
    button->setPalette(pal);

    button->setAutoRaise(true);
    button->setFocusPolicy(Qt::NoFocus);
}

void MainWindow::setButtonLocations()
{
    home_button->move(125, 75);
    settings_button->move(calculateTargetPoint(width(), 17, 1) - settings_button->width() - 30,
                          calculateTargetPoint(height(), 17, 1) - settings_button->height() - 100);
    user_button->move(165, 400);
    map_button->move(145, 725);
}

void MainWindow::setButtonSizes(int val)
{
    QFont font;

    if (home_button) {
        home_button->setFixedSize(home_button->size() * val);
        home_button->setIconSize(home_button->iconSize() * val);
        font = home_button->font();
        font.setPointSize(18);
        home_button->setFont(font);
    }

    if (settings_button) {
        settings_button->setFixedSize(settings_button->size() * val);
        settings_button->setIconSize(settings_button->iconSize() * val);
        font = settings_button->font();
        font.setPointSize(18);
        settings_button->setFont(font);
    }

    if (user_button) {
        user_button->setFixedSize(user_button->size() * val);
        user_button->setIconSize(user_button->iconSize() * val);
        font = user_button->font();
        font.setPointSize(18);
        user_button->setFont(font);
    }

    if (map_button) {
        map_button->setFixedSize(map_button->size() * val);
        map_button->setIconSize(map_button->iconSize() * val);
        font = map_button->font();
        font.setPointSize(18);
        map_button->setFont(font);
    }
}

QSize MainWindow::getButtonSize(QToolButton* button)
{
    return button->size();
}

QIcon MainWindow::makeImageBackgroundTransparent(QImage* const image, int tolerance)
{
    QRgb targetWhite = qRgb(255, 255, 255);

    for (int y{0}; y < image->height(); ++y) {
        for (int x{0}; x < image->width(); ++x) {
            QRgb current = image->pixel(x, y);

            if (qAbs(qRed(current) - qRed(targetWhite)) < tolerance &&
                qAbs(qGreen(current) - qGreen(targetWhite)) < tolerance &&
                qAbs(qBlue(current) - qBlue(targetWhite)) < tolerance) {

                image->setPixel(x, y, qRgba(0, 0, 0, 0));
            }
        }
    }

    return QIcon(QPixmap::fromImage(*image));
}
