//
//  UI.swift
//  Symbios
//
//  Created by Joakim Wennergren on 2023-08-29.
//

import Foundation
import UIKit

var _view = UIView()
var _screen = UIScreen()
class SecondViewController: UIViewController {
    
    override func didRotate(from fromInterfaceOrientation: UIInterfaceOrientation) {
        var text=""
        switch UIDevice.current.orientation{
        case .portrait:
            text="Portrait"
        case .portraitUpsideDown:
            text="PortraitUpsideDown"
        case .landscapeLeft:
            text="LandscapeLeft"
            print("in landscape left")
            let temp = _view.frame.size.height
            _view.frame.size.height = temp + 30
        case .landscapeRight:
            text="LandscapeRight"
            print("in landscape left")
            let temp = _view.frame.size.height
            _view.frame.size.height = temp + 30
        default:
            text="Another"
        }
        print(text)
    }
    
    override func viewDidAppear(_ animated: Bool) {
        
        let orientation = UIDevice.current.orientation
        
        if(orientation == .portrait)
        {
            
        } else if (orientation == .landscapeLeft )
        {
            print("in landscape left")
            let temp = _view.frame.size.height
            _view.frame.size.height = temp + 30
        } else if(orientation == .landscapeRight)
        {
            print("in landscape right")
            let temp = _view.frame.size.height
            _view.frame.size.height = temp + 30
        }
        
        super.viewDidLoad()
    
    }

    
}

@_cdecl("get_native_bounds")
func getNativeBounds(view: UIView, screen: UIScreen) -> UIViewController
{
    _view = view
    _screen = screen
    let controller = SecondViewController();
    
    return controller;
}
