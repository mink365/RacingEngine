//
//  GameView.swift
//

import Foundation
import UIKit
import GLKit

class GameView : UIView
{
    var drawableDepthFormat:GLKViewDrawableDepthFormat = .Format24
    
    // The pixel dimensions of the CAEAGLLayer.
    private(set) var framebufferWidth:GLint = 0
    private(set) var framebufferHeight:GLint = 0
    
    // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view.
    private(set) var defaultFramebuffer:GLuint = 0
    private(set) var colorRenderbuffer:GLuint = 0
    private(set) var depthRenderbuffer:GLuint = 0
    
    var context:EAGLContext? = nil {
        willSet {
            self.deleteFramebuffer()
        }
    }
    
    var eaglLayer:CAEAGLLayer {
        get { return self.layer as! CAEAGLLayer }
    }
    
    override class func layerClass() -> AnyClass {
        return CAEAGLLayer.classForCoder()
    }
    
    override init(frame:CGRect)
    {
        super.init(frame: frame)
        
        self.initLayer()
    }

    required init?(coder aDecoder: NSCoder) {
        super.init(coder:aDecoder);
        
        self.initLayer()
    }
    
    func initLayer()
    {
        let eaglLayer = self.eaglLayer
        
        let scale = UIScreen.mainScreen().scale;
        
        eaglLayer.opaque = true;
        eaglLayer.drawableProperties = [
            kEAGLDrawablePropertyRetainedBacking : NSNumber(bool:false),
            kEAGLDrawablePropertyColorFormat : kEAGLColorFormatRGBA8
        ];
        eaglLayer.contentsScale = scale;
        
        let api: EAGLRenderingAPI = EAGLRenderingAPI.OpenGLES2
        self.context = EAGLContext(API: api)
        
        if (self.context == nil) {
            NSLog("Failed to initialize OpenGLES 2.0 context!")
            exit(1)
        }
        
        if (!EAGLContext.setCurrentContext(self.context)) {
            NSLog("Failed to set current OpenGL context!")
            exit(1)
        }
        
        self.createFramebuffer()
    }
    
    func createFramebuffer()
    {
        if (self.context == nil || self.defaultFramebuffer != 0){
            return;
        }
        
        EAGLContext.setCurrentContext(self.context)
        
        glGenBuffers(1, &self.defaultFramebuffer);
        glBindFramebuffer(GLenum(GL_FRAMEBUFFER), self.defaultFramebuffer);
        
        glGenBuffers(1, &self.colorRenderbuffer);
        glBindRenderbuffer(GLenum(GL_RENDERBUFFER), self.colorRenderbuffer);
        self.context?.renderbufferStorage(Int(GL_RENDERBUFFER), fromDrawable: self.eaglLayer);
        
        glFramebufferRenderbuffer(GLenum(GL_FRAMEBUFFER), GLenum(GL_COLOR_ATTACHMENT0), GLenum(GL_RENDERBUFFER), self.colorRenderbuffer);
        
        glGetRenderbufferParameteriv(GLenum(GL_RENDERBUFFER), GLenum(GL_RENDERBUFFER_WIDTH), &self.framebufferWidth)
        glGetRenderbufferParameteriv(GLenum(GL_RENDERBUFFER), GLenum(GL_RENDERBUFFER_HEIGHT), &self.framebufferHeight)
        
        glGenBuffers(1, &self.depthRenderbuffer)
        glBindRenderbuffer(GLenum(GL_RENDERBUFFER), self.depthRenderbuffer)
        glRenderbufferStorage(GLenum(GL_RENDERBUFFER), GLenum(GL_DEPTH_COMPONENT24_OES), framebufferWidth, framebufferHeight)
        glFramebufferRenderbuffer(GLenum(GL_FRAMEBUFFER), GLenum(GL_DEPTH_ATTACHMENT), GLenum(GL_RENDERBUFFER), self.depthRenderbuffer)
        
        let status = glCheckFramebufferStatus(GLenum(GL_FRAMEBUFFER))
        if (Int32(status) != GL_FRAMEBUFFER_COMPLETE) {
            NSLog("Failed to make complete framebuffer object %x", status)
        }

        application_initView(UInt32(framebufferWidth), UInt32(framebufferHeight),
                defaultFramebuffer, colorRenderbuffer, depthRenderbuffer,
                true, false)
    }
    
    func deleteFramebuffer() {
        if (self.context != nil) {
            EAGLContext.setCurrentContext(self.context)
            
            if (self.defaultFramebuffer > 0) {
                glDeleteFramebuffers(1, &self.defaultFramebuffer);
                self.defaultFramebuffer = 0;
            }
            
            if (self.colorRenderbuffer > 0) {
                glDeleteRenderbuffers(1, &self.colorRenderbuffer);
                self.colorRenderbuffer = 0;
            }
        }
    }
    
    func presentFramebuffer() -> Bool
    {
        var success = false
        
        if self.context != nil {
            EAGLContext.setCurrentContext(self.context)
            glBindRenderbuffer(GLenum(GL_RENDERBUFFER), self.colorRenderbuffer)
            success = self.context!.presentRenderbuffer(Int(GL_RENDERBUFFER))
        }
        
        return success
    }
    
    override func layoutSubviews() {
        // The framebuffer will be re-created at the beginning of the next setFramebuffer method call.
//        self.deleteFramebuffer()
//        
//        self.createFramebuffer()
    }
    
    override func touchesBegan(touches: Set<UITouch>, withEvent event: UIEvent?) {
        
    }
    
    override func touchesMoved(touches: Set<UITouch>, withEvent event: UIEvent?) {
        
    }
    
    override func touchesEnded(touches: Set<UITouch>, withEvent event: UIEvent?) {
        
    }
    
    override func touchesCancelled(touches: Set<UITouch>?, withEvent event: UIEvent?) {
        
    }
};