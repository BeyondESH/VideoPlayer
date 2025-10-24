extern "C"{
#include <libavcodec/avcodec.h>
}
#include "xvideoviewtest.h"
#include "./ui_xvideoviewtest.h"
#include <SDL2/SDL.h>
#include <qDebug>
#include <thread>
#include <chrono>

#include <QInputDialog>



XVideoViewTest::XVideoViewTest(QWidget *parent)
    : QMainWindow(parent),_view(nullptr)
    , ui(new Ui::XVideoViewTest),_frame(nullptr),_sdl_width(0),_sdl_height(0),_pix_size(0),_isStop(false),_file_size(0),_multiple(1)
{
    ui->setupUi(this);
    initUI();
    connect(this,&XVideoViewTest::signal_draw,this,&XVideoViewTest::slots_draw);
    // 打开yuv文件
    // _ifs.open("C:/Users/19896/Videos/lucy.yuv",std::ios::binary);
    // if(_ifs.is_open()!=true){
    //     qDebug()<<"打开yuv视频失败";
    //     return;
    // }
    // //获取文件大小
    // _ifs.seekg(0,std::ios::end);
    // _file_size=_ifs.tellg();
    // _ifs.seekg(0,std::ios::beg);
    // //视频文件尺寸
    // _sdl_width=1440,_sdl_height=1080;
    // _pix_size=1.5;
    // //窗口大小调整
    // resize(_sdl_width,_sdl_height);
    // ui->videoLB->move(0,0);
    // ui->videoLB->resize(_sdl_width,_sdl_height);
    // ui->fpsLB->move(0,0);

    // _view=XVideoView::create();
    // _view->init(_sdl_width,_sdl_height,XVideoView::YUV420P);
    // // yuv_data=new unsigned char[sdl_height*sdl_width*pix_size];
    // // memset(yuv_data,0,sdl_height*sdl_width*pix_size);

    // //生成avframe对象空间
    // _frame=av_frame_alloc();
    // _frame->width=_sdl_width;
    // _frame->height=_sdl_height;
    // _frame->format=AV_PIX_FMT_YUV420P;
    // _frame->linesize[0]=_sdl_width;
    // _frame->linesize[1]=_sdl_width/2;
    // _frame->linesize[2]=_sdl_width/2;
    // //生成图像空间，默认32字节对齐
    // int res=av_frame_get_buffer(_frame,0);
    // if(res!=0){
    //     char buffer[1024];
    //     av_strerror(res,buffer,sizeof(buffer));
    //     qDebug()<<"avframe_buffer生成错误:"<<buffer;
    // }
    // // this->startTimer(1000/30);

    // _draw_thread=std::thread([&](){
    //     while(_isStop==false){
    //         emit signal_draw();
    //         std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000/(30*_multiple))));
    //         // _view->sleep(1000/120);
    //     }
    // });

    //读取文件
    _ifs.open("C:/Users/19896/Videos/lucy.raw",std::ios::binary);
    if(!_ifs.is_open()){
        qDebug()<<"打开 raw 文件失败";
        return;
    }
    // 获取文件大小
    _ifs.seekg(0,std::ios::end);
    _file_size=_ifs.tellg();
    _ifs.seekg(0,std::ios::beg);
    // 初始化数据
    _sdl_width=1440;
    _sdl_height=1080;
    _pix_size=4;
    // 调整窗口大小
    resize(_sdl_width,_sdl_height);
    ui->videoLB->resize(_sdl_width,_sdl_height);
    ui->videoLB->move(0,0);
    ui->fpsLB->move(0,0);

    _view=XVideoView::create();
    _view->init(_sdl_width,_sdl_height,XVideoView::PixFormat::ABGR,(void *)ui->videoLB->winId());

    // 创建avframe
    _frame=av_frame_alloc();
    _frame->width=_sdl_width;
    _frame->height=_sdl_height;
    _frame->format=AV_PIX_FMT_ABGR;
    // _frame->linesize[0]=_sdl_width*4;
    int res=av_frame_get_buffer(_frame,0);
    if(res!=0){
        char buffer[1024];
        av_strerror(res,buffer,sizeof(buffer));
        qDebug()<<"avframe_buffer生成错误:"<<buffer;
    }

    _draw_thread=std::thread([&](){
        while(_isStop==false){
            emit signal_draw();
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000/(30*_multiple))));
        }
    });
}

XVideoViewTest::~XVideoViewTest()
{
    _isStop=true;
    if(_draw_thread.joinable()){
        _draw_thread.join();
    }

    // delete [] yuv_data;
    _ifs.close();

    if(_frame!=nullptr){
        av_frame_free(&_frame);
    }
    delete ui;
}

void XVideoViewTest::resizeEvent(QResizeEvent *e)
{
    ui->videoLB->resize(size());
    ui->videoLB->move(0,0);
}

void XVideoViewTest::initUI()
{
    // QAction *fpsAct=ui->videomMenu->addAction("倍速播放");
    // connect(fpsAct,&QAction::triggered,this,[](){

    // });
}

void XVideoViewTest::setMultiple(double newMultiple)
{
    _multiple = newMultiple;
}

void XVideoViewTest::slots_draw()
{
    // 读取至结尾,文件光标回到开头
    if(_ifs.tellg()==_file_size){
        _ifs.seekg(0,std::ios::beg);
    }
    if(_view->isExit()==true){
        _view->close();
        exit(0);
    }

    switch (_view->fmt()) {
    case XVideoView::PixFormat::YUV420P:
    {
        _ifs.read((char *)_frame->data[0],_sdl_width*_sdl_height); //Y
        _ifs.read((char *)_frame->data[1],_sdl_width*_sdl_height*1/4); //U
        _ifs.read((char *)_frame->data[2],_sdl_width*_sdl_height*1/4); //V
        break;
    }
    case XVideoView::PixFormat::ABGR:
    case XVideoView::PixFormat::RGBA:
    {
        for (int i = 0; i < _sdl_height; ++i) {
            int offset=i*_sdl_width*4;
            _ifs.read((char *)_frame->data[0]+offset,_sdl_width*4);
        }
        break;
    }
    default:
        break;
    }

    // 绘制
    _view->drawFrame(_frame);

    // 显示帧数
    QString fps_string="fps:"+QString::number(_view->fps());
    ui->fpsLB->setText(fps_string);
    return;
}

// void XVideoViewTest::timerEvent(QTimerEvent *e)
// {
//     // yuv_file.read((char *)yuv_data,sdl_width*sdl_height*1.5);
//     // 对于avframe，yuv分开读取
//     _yuv_file.read((char *)_frame->data[0],_sdl_width*_sdl_height); //Y
//     _yuv_file.read((char *)_frame->data[1],_sdl_width*_sdl_height*1/4); //U
//     _yuv_file.read((char *)_frame->data[2],_sdl_width*_sdl_height*1/4); //V
//     if(_view->isExit()==true){
//         _view->close();
//         exit(0);
//     }
//     // _view->draw(yuv_data);
//     _view->drawFrame(_frame);
//     // qDebug()<<_frame->linesize[0]<<_frame->linesize[1]<<_frame->linesize[2];
// }

void XVideoViewTest::on_action0_5X_triggered()
{
    setMultiple(0.5);
}


void XVideoViewTest::on_action0_75X_triggered()
{
    setMultiple(0.75);
}


void XVideoViewTest::on_action1_0X_triggered()
{
    setMultiple(1);
}


void XVideoViewTest::on_action1_25X_triggered()
{
    setMultiple(1.25);
}


void XVideoViewTest::on_action1_5X_triggered()
{
    setMultiple(1.5);
}


void XVideoViewTest::on_action2_0X_triggered()
{
    setMultiple(2.0);
}


void XVideoViewTest::on_action3_0X_triggered()
{
    setMultiple(3.0);
}

