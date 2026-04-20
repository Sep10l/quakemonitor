#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QStyleOption>
#include <QToolButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    QSize getWindowSize();
    void setWindowSize(const QSize& size);
    QSize getButtonCorrectSize(QToolButton* button, const QSize& actualSize, const char* text, const QIcon& icon);
    void createButton(const QString& text, QToolButton* button, const QSize& resize, const QIcon* icon, const QSize& icon_size=QSize(32, 32), Qt::ToolButtonStyle buttonStyle=Qt::ToolButtonStyle::ToolButtonTextBesideIcon, const QColor color=Qt::white);
    int calculateTargetPoint(int window_coord, int divisor, int button_size);
    void setButtonLocations();
    void setButtonSizes(int val);
    QSize getButtonSize(QToolButton* button);
    QIcon makeImageBackgroundTransparent(QImage* const image, int tolerance=35);

    const QSize defaultws{24,24};

    QToolButton* home_button = new QToolButton(this);
    QToolButton* settings_button = new QToolButton(this);
    QToolButton* user_button = new QToolButton(this);
    QToolButton* map_button = new QToolButton(this);

    const QColor beige{166, 137, 88};

private:
    Ui::MainWindow *ui;
    QSize windowSize = QSize{ 800, 600 };
};

#endif // MAINWINDOW_H
