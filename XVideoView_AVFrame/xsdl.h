#ifndef XSDL_H
#define XSDL_H
#include "xvideoview.h"
struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Window;
class XSdl:public XVideoView
{
public:
    XSdl();
    bool init(int w,int h,PixFormat fmt,void * win_id) override;
    bool draw(const unsigned char* data, int lineSize) override;
    bool draw(const unsigned char *y, int y_pitch,
              const unsigned char *u, int u_pitch,
              const unsigned char *v, int v_pitch) override;
    ~XSdl();
    bool isExit() override;
    void close() override;
private:
    SDL_Texture* _sdl_texture;
    SDL_Window* _sdl_window;
    SDL_Renderer* _sdl_renderer;
    static std::once_flag _flag;
};

#endif // XSDL_H
