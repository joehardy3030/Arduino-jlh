//
//  ViewController.swift
//  Smart House
//
//  Created by Joseph Hardy on 5/22/15.
//  Copyright (c) 2015 Joseph Hardy. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }

    
    @IBAction func heaterOn() {
        if let url = NSURL(string: "http://10.0.0.12/arduino/on3") {
            UIApplication.sharedApplication().openURL(url)
        }
    }
    
    @IBAction func heaterOff() {
        if let url = NSURL(string: "http://10.0.0.12/arduino/off") {
            UIApplication.sharedApplication().openURL(url)
        }
    }

    @IBAction func garageDoor() {
        if let url = NSURL(string: "http://10.0.0.20/arduino/press") {
            UIApplication.sharedApplication().openURL(url)
        }
    }

    @IBAction func outletOff() {
        if let url = NSURL(string: "http://10.0.0.20/arduino/outletOff") {
            UIApplication.sharedApplication().openURL(url)
        }
    }

    @IBAction func outletOn() {
        if let url = NSURL(string: "http://10.0.0.20/arduino/outletOn") {
            UIApplication.sharedApplication().openURL(url)
        }
    }

    
}

