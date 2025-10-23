#include "xvideoview.h"
#include "xsdl.h"
#include <QString>
extern "C"{
#include <libavcodec/avcodec.h>
}

XVideoView *XVideoView::create(RenderType type)
{
    switch (type) {
    case XVideoView::RenderType::SDL:
        return new XSdl();
    default:
        break;
    }
    return nullptr;
}

bool XVideoView::drawFrame(AVFrame *frame)
{
    if(frame==nullptr||frame->data[0]==nullptr){
        return false;
    }
    bool result=false;
    switch(frame->format){
        case AV_PIX_FMT_YUV420P:{
            result=draw(frame->data[0],frame->linesize[0],
                        frame->data[1],frame->linesize[1],
                        frame->data[2],frame->linesize[2]);
            break;
        }
        case AV_PIX_FMT_BGRA:{
            result=draw(frame->data[0],frame->linesize[0]);
            break;
        }
        default:{
            break;
        }
    }

    if(result==false){
        return false;
    }

    //统计帧数
    auto nowTimePoint=std::chrono::high_resolution_clock::now();
   if(_fps_count==0){
        _beg_ms=nowTimePoint;
    }
    _fps_count++;
    auto duration=nowTimePoint-_beg_ms;
    auto diff_ms=std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    if(diff_ms>=1000){
        _fps=static_cast<int>(static_cast<double>(_fps_count)*1000/diff_ms);
        _fps_count=0;
        _beg_ms=nowTimePoint;
    }
    return true;
}

void XVideoView::scale(int w, int h)
{
    if (_width <= 0 || _height <= 0) return;
    double scaleW = static_cast<double>(w) / static_cast<double>(_width);
    double scaleH = static_cast<double>(h) / static_cast<double>(_height);
    double scale = (scaleW < scaleH) ? scaleW : scaleH;

    int sw = static_cast<int>(_width * scale);
    int sh = static_cast<int>(_height * scale);
    // 防止结果为 0
    _scale_width  = (sw > 0) ? sw : 1;
    _scale_height = (sh > 0) ? sh : 1;
}

int XVideoView::scale_width() const
{
    return _scale_width;
}

int XVideoView::scale_height() const
{
    return _scale_height;
}

void XVideoView::sleep(long long ms)
{
    auto beg=std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ms; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        auto now=std::chrono::high_resolution_clock::now();
        auto diff_ms=std::chrono::duration_cast<std::chrono::milliseconds>(now-beg).count();
        if(diff_ms>=ms){
            return;
        }
    }
}

int XVideoView::fps() const
{
    return _fps;
}
