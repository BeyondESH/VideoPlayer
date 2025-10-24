#ifndef XVIDEOVIEWTEST_H
#define XVIDEOVIEWTEST_H

#include <QMainWindow>
#include "xvideoview.h"
#include <fstream>

struct AVFrame;
QT_BEGIN_NAMESPACE
namespace Ui {
class XVideoViewTest;
}
QT_END_NAMESPACE

class XVideoViewTest : public QMainWindow
{
    Q_OBJECT

public:
    XVideoViewTest(QWidget *parent = nullptr);
    ~XVideoViewTest();
    // void timerEvent(QTimerEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;
    void initUI();
    void setMultiple(double newMultiple);

private:
    Ui::XVideoViewTest *ui;

private:
    XVideoView * _view;
    AVFrame * _frame;
    int _sdl_width;
    int _sdl_height;
    double _pix_size;
    std::ifstream _ifs;
    std::thread _draw_thread;
    std::atomic_bool _isStop;
    long long  _file_size;
    double _multiple;
signals:
    void signal_draw();
public slots:
    void slots_draw();

private slots:

    void on_action0_5X_triggered();
    void on_action0_75X_triggered();
    void on_action1_0X_triggered();
    void on_action1_25X_triggered();
    void on_action1_5X_triggered();
    void on_action2_0X_triggered();
    void on_action3_0X_triggered();
};

#endif // XVIDEOVIEWTEST_H
