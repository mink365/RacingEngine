//
//  GameLoop.swift
//

import Foundation
import UIKit

class GameLoop : NSObject
{
    var doSomething: ()->()!
    var displayLink: CADisplayLink!
    var frameInternal: Int!
    
    init(frameInternal: Int, doSomething: ()->()) {
        self.doSomething = doSomething
        self.frameInternal = frameInternal
        
        super.init()
        
        start()
    }
    
    func handleTimer() {
        self.doSomething()
    }
    
    func start() {
        displayLink = CADisplayLink(target: self, selector: Selector("handleTimer"))
        displayLink.frameInterval = self.frameInternal
        displayLink.addToRunLoop(NSRunLoop.mainRunLoop(), forMode: NSRunLoopCommonModes)
    }
    
    func stop() {
        displayLink.paused = true
        displayLink.removeFromRunLoop(NSRunLoop.mainRunLoop(), forMode: NSRunLoopCommonModes)
        displayLink = nil
    }
}