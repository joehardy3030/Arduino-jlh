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

    //open the relevant url in Safari for turning on the heater
    @IBAction func heaterOn() {
        if let url = NSURL(string: "http://10.0.0.12/arduino/on3") {
            UIApplication.sharedApplication().openURL(url)
        }
    }
    
    //open the relevant url in Safari for turning off the heater
    @IBAction func heaterOff() {
        if let url = NSURL(string: "http://10.0.0.12/arduino/off") {
            UIApplication.sharedApplication().openURL(url)
        }
    }

    //open the relevant url in Safari for closing the loop on garage door opener
    @IBAction func garageDoor() {
        if let url = NSURL(string: "http://10.0.0.20/arduino/press") {
            UIApplication.sharedApplication().openURL(url)
        }
    }

    //open the relevant url in Safari for turning off the outlet for the stereo
    @IBAction func outletOff() {
        if let url = NSURL(string: "http://10.0.0.20/arduino/outletOff") {
            UIApplication.sharedApplication().openURL(url)
        }
    }

    
    // Open a NUSRLSession to flip the switch and suck in the data
    // https://www.youtube.com/watch?v=xt-Q4c1kMBQ
    @IBAction func outletOn() {
        
        let url = NSURL(string: "http://10.0.0.20/arduino/outletOn")
        
        let task = NSURLSession.sharedSession().dataTaskWithURL(url!) {(data, response, error) in
            println(NSString(data: data, encoding: NSUTF8StringEncoding))
        }
        
        task.resume()
        //if let url = NSURL(string: "http://10.0.0.20/arduino/outletOn") {
        //    UIApplication.sharedApplication().openURL(url)
       // }
    }

    
}

