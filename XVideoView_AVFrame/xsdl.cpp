#include "xsdl.h"
#include <SDL2/SDL.h>
#include <QDebug>
XSdl::XSdl():_sdl_renderer(nullptr),_sdl_texture(nullptr),_sdl_window(nullptr)
{

}

std::once_flag XSdl::_flag;

bool XSdl::init(int w, int h, PixFormat fmt, void *win_id)
{
    if(w<=0||h<=0){
        return false;
    }
    //初始化sdl
    bool init_res=true;
    std::call_once(_flag,[&init_res](){
        int res=SDL_Init(SDL_INIT_VIDEO);
        if(res!=0){
            qDebug()<<"SDL初始化失败:"<<SDL_GetError();
            init_res=false;
        }
        //设定缩放算法，处理锯齿
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");
    });
    if(init_res==false){
        return false;
    }

    std::lock_guard<std::mutex> sdl_lock(_mutex);
    _width=w,
    _height=h;
    _fmt=fmt;

    //清理
    if(_sdl_texture!=nullptr){
        SDL_DestroyTexture(_sdl_texture);
    }
    if(_sdl_renderer!=nullptr){
        SDL_DestroyRenderer(_sdl_renderer);
    }

    //创建窗口
    if(_sdl_window==nullptr){
        //新建窗口
        if(win_id==nullptr){
            _sdl_window=SDL_CreateWindow("视频",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,_width,_height,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
        }
        else{
            //渲染到控件
            _sdl_window=SDL_CreateWindowFrom(win_id);
        }
    }
    if(_sdl_window==nullptr){
        qDebug()<<"创建窗口失败:"<<SDL_GetError();
        return false;
    }


    //创建渲染器
    _sdl_renderer=SDL_CreateRenderer(_sdl_window,-1,SDL_RENDERER_ACCELERATED);
    if(_sdl_renderer==nullptr){
        qDebug()<<"创建渲染器失败:"<<SDL_GetError();
        return false;
    }

    // 确定像素格式
    SDL_PixelFormatEnum sdl_fmt;
    switch (fmt) {
    case XVideoView::PixFormat::ARGB:
        sdl_fmt=SDL_PIXELFORMAT_ARGB32;
        break;
    case XVideoView::PixFormat::RGBA:
        sdl_fmt=SDL_PIXELFORMAT_RGBA8888;
        break;
    case XVideoView::PixFormat::YUV420P:
        sdl_fmt=SDL_PIXELFORMAT_IYUV;
        break;
    default:
        qDebug()<<"像素格式错误";
        return false;
    }

    // 创建材质
    _sdl_texture=SDL_CreateTexture(_sdl_renderer,sdl_fmt,SDL_TEXTUREACCESS_STREAMING,_width,_height);
    if(_sdl_texture==nullptr){
        qDebug()<<"创建材质失败:"<<SDL_GetError();
        return false;
    }
    return true;
}

bool XSdl::draw(const unsigned char *data, int lineSize)
{
    if(data==nullptr){
        return false;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    if(_sdl_renderer==nullptr||_sdl_window==nullptr||_sdl_texture==nullptr||_width<=0||_height<=0){
        return false;
    }

    if(lineSize<=0){
        switch (_fmt) {
        case XVideoView::PixFormat::ARGB:
        case XVideoView::PixFormat::RGBA:
            lineSize=_width*4;
            break;
        case XVideoView::PixFormat::YUV420P:
            lineSize=_width;
            break;
        default:
            return false;
        }
    }
    //更新内存材质到显存
    if((SDL_UpdateTexture(_sdl_texture,nullptr,data,lineSize))!=0){
        qDebug()<<"更新材质失败:"<<SDL_GetError();
        return false;
    }
    //清理渲染器
    if((SDL_RenderClear(_sdl_renderer))!=0){
        qDebug()<<"清理渲染器失败:"<<SDL_GetError();
        return false;
    }
    //材质加载到渲染器
    if(_scale_height<=0){
        _scale_height=_height;
    }
    if(_scale_width<=0){
        _scale_width=_width;
    }
    int window_width, window_height;
    SDL_GetWindowSize(_sdl_window,&window_width,&window_height);
    scale(window_width, window_height);
    SDL_Rect rect;
    rect.x=(window_width-_scale_width)/2;
    rect.y=(window_height-_scale_height)/2;
    rect.w=_scale_width; //渲染的宽度，缩放
    rect.h=_scale_height; //渲染的高度，缩放
    if((SDL_RenderCopy(_sdl_renderer,_sdl_texture,nullptr,&rect))!=0){
        qDebug()<<"材质加载到渲染器失败:"<<SDL_GetError();
        return false;
    }
    //开始渲染
    SDL_RenderPresent(_sdl_renderer);
    return true;
}

bool XSdl::draw(const unsigned char *y, int y_pitch, const unsigned char *u, int u_pitch, const unsigned char *v, int v_pitch)
{
    //检查参数
    if(y==nullptr||u==nullptr||v==nullptr){
        return false;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    if(_sdl_renderer==nullptr||_sdl_window==nullptr||_sdl_texture==nullptr||_width<=0||_height<=0){
        return false;
    }

    //更新内存材质到显存
    if((SDL_UpdateYUVTexture(_sdl_texture,nullptr,y,y_pitch,u,u_pitch,v,v_pitch))!=0){
        qDebug()<<"更新材质失败:"<<SDL_GetError();
        return false;
    }
    //清理渲染器
    if((SDL_RenderClear(_sdl_renderer))!=0){
        qDebug()<<"清理渲染器失败:"<<SDL_GetError();
        return false;
    }
    //材质加载到渲染器
    if(_scale_height<=0){
        _scale_height=_height;
    }
    if(_scale_width<=0){
        _scale_width=_width;
    }
    int window_width, window_height;
    SDL_GetWindowSize(_sdl_window,&window_width,&window_height);
    scale(window_width, window_height);
    SDL_Rect rect;
    rect.x=(window_width-_scale_width)/2;
    rect.y=(window_height-_scale_height)/2;
    rect.w=_scale_width; //渲染的宽度，缩放
    rect.h=_scale_height; //渲染的高度，缩放
    if((SDL_RenderCopy(_sdl_renderer,_sdl_texture,nullptr,&rect))!=0){
        qDebug()<<"材质加载到渲染器失败:"<<SDL_GetError();
        return false;
    }
    //开始渲染
    SDL_RenderPresent(_sdl_renderer);
    return true;
}

XSdl::~XSdl()
{
    close();
}

bool XSdl::isExit()
{
    SDL_Event e;
    // 使用PollEvent非阻塞地处理所有待处理事件
    while(SDL_PollEvent(&e)){
        if(e.type==SDL_EventType::SDL_QUIT){
            return true; // 收到退出事件
        }
        // 这里可以添加对其他事件的处理，例如窗口缩放等
    }
    return false; // 没有收到退出事件
}

void XSdl::close()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if(_sdl_texture!=nullptr){
        SDL_DestroyTexture(_sdl_texture);
        _sdl_texture=nullptr;
    }
    if(_sdl_renderer!=nullptr){
        SDL_DestroyRenderer(_sdl_renderer);
        _sdl_renderer=nullptr;
    }
    if(_sdl_window!=nullptr){
        SDL_DestroyWindow(_sdl_window);
        _sdl_window=nullptr;
    }
}


