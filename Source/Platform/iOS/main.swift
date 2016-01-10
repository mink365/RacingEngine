//
//  main.swift
//  RacingEngine
//
//  Created by rui kou on 16/1/4.
//
//

import Foundation
import UIKit

class MyApplication: UIApplication {
    override func sendEvent(event: UIEvent) {
        super.sendEvent(event)
//        print("Event sent: \(event)");
    }
}

UIApplicationMain(Process.argc, Process.unsafeArgv,
    NSStringFromClass(MyApplication), NSStringFromClass(AppDelegate))