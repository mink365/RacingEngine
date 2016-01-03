//
//  GameViewController.swift
// 

import Foundation
import UIKit

class GameViewController : UIViewController {
    var timeSinceLastUpdate:NSTimeInterval = 0.020
    var gameLoop:GameLoop!
    var __view:GameView!
    
    var context:EAGLContext {
        get { return self.__view.context! }
    }
    
    override func loadView() {
        super.loadView()
        
        self.view = GameView(frame:CGRect(origin: CGPoint(x:0, y:0), size: CGSize(width:414, height:800)))
        self.__view = self.view as! GameView
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.gameLoop = GameLoop(frameInternal: 1, doSomething: self.updateInternal)
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        
        if self.isViewLoaded() && (self.view.window != nil) {
            self.view = nil
            
            self.tearDownGL()
            
            if EAGLContext.currentContext() === self.context {
                EAGLContext.setCurrentContext(nil)
            }
            self.__view.context = nil
        }
    }
    
    private func updateInternal() {
        self.update()
        
        self.__view.presentFramebuffer()
        
        self.glView(self.__view, drawInRect: CGRect())
    }
    
    func update() {
        NSLog("xxxx")
    }
    
    func glView(view: GameView, drawInRect rect: CGRect) {
        
    }
    
    func tearDownGL() {
    
    }
}