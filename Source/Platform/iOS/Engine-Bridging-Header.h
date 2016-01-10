//
//  Examples-Swift.h
//  RacingEngine
//
//  Created by rui kou on 16/1/5.
//
//

#ifndef Examples_Swift_h
#define Examples_Swift_h

#include <stdint.h>

extern void application_init(const char* resource_dir);
extern void application_initView(uint32_t width, uint32_t height, uint32_t framebuffer,
                            uint32_t colorRenderbuffer, uint32_t depthRenderbuffer, bool depth, bool stencil);
extern void application_update();

extern void view_touch_begin(float x, float y);
extern void view_touch_moved(float x, float y);
extern void view_touch_ended(float x, float y);
extern void view_touch_cancled(float x, float y);

#endif /* Examples_Swift_h */
