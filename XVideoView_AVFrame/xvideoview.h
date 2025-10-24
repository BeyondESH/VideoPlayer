#ifndef XVIDEOVIEW_H
#define XVIDEOVIEW_H

#include <mutex>
#include <chrono>
struct AVFrame;

class XVideoView
{
public:
    enum RenderType{
        SDL=0,
    };
    static XVideoView * create(RenderType type=SDL);
    enum PixFormat{
        RGBA=0,
        ARGB,
        YUV420P,
        ABGR,
    };

    // 初始化渲染窗口 线程安全
    // @para w 窗口宽度
    // @para h 窗口高度
    // @para fmt 绘制的像素格式
    // @para win_id 窗口句柄，若为空则创建新窗口
    // @return 是否创建成功
    virtual bool init(int w,int h,PixFormat fmt=PixFormat::RGBA,void * win_id=nullptr)=0;

    // 渲染图像 线程安全
    // @para data 渲染的二进制数据
    // @para lineSize 一行数据的字节数，对于YUV420p格式Y为一行字节数
    // lineSize<=0 就根据宽度和像素自动算出大小
    // @return 渲染是否成功
    virtual bool draw(const unsigned char *data, int lineSize=0)=0;

    virtual bool draw(const unsigned char *y, int y_pitch,
                      const unsigned char *u, int u_pitch,
                      const unsigned char *v, int v_pitch)=0;

    bool drawFrame(AVFrame * frame);

    void scale(int w, int h);

    //清理所有申请的资源
    virtual void close()=0;


    int scale_width() const;

    int scale_height() const;

    void sleep(long long ms);

    //处理窗口退出事件
    virtual bool isExit()=0;
    int fps() const;

    PixFormat fmt() const;

protected:
    int _fps=0;
    int _width=0; //材质宽度
    int _height=0; //材质高度
    PixFormat _fmt=RGBA;
    std::mutex _mutex;
    int _scale_width=0; //实际显示宽度
    int _scale_height=0; //实现显示高度
    std::chrono::time_point<std::chrono::high_resolution_clock> _beg_ms=std::chrono::high_resolution_clock::now(); //计时开始时间
    int _fps_count =0;  //统计显示次数
};

#endif // XVIDEOVIEW_H
