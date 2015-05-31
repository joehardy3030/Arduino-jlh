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

    @IBOutlet weak var textLabel: UILabel!
    
    //open the relevant url in Safari for turning on the heater
    @IBAction func heaterOn() {
        let url = NSURL(string: "http://10.0.0.12/arduino/on3")
        var dataString:String = "No connection"
        let task = NSURLSession.sharedSession().dataTaskWithURL(url!) {(data, response, error) in
            dataString = String(NSString(data: data, encoding: NSUTF8StringEncoding)!)
            
            dispatch_async(dispatch_get_main_queue()) {
                // Update the UI on the main thread.
                self.textLabel.text = dataString
            } //dispatch_async
        } //NSURLSession
        task.resume()
    }
    
    //open the relevant url in Safari for turning off the heater
    @IBAction func heaterOff() {
        let url = NSURL(string: "http://10.0.0.12/arduino/off")
        var dataString:String = "No connection"
        let task = NSURLSession.sharedSession().dataTaskWithURL(url!) {(data, response, error) in
            dataString = String(NSString(data: data, encoding: NSUTF8StringEncoding)!)
            
            dispatch_async(dispatch_get_main_queue()) {
                // Update the UI on the main thread.
                self.textLabel.text = dataString
            } //dispatch_async
        } //NSURLSession
        task.resume()
    }

    //open the relevant url in Safari for closing the loop on garage door opener
    @IBAction func garageDoor() {
        let url = NSURL(string: "http://10.0.0.20/arduino/press")
        var dataString:String = ""
        let task = NSURLSession.sharedSession().dataTaskWithURL(url!) {(data, response, error) in
            dataString = String(NSString(data: data, encoding: NSUTF8StringEncoding)!)
            
            dispatch_async(dispatch_get_main_queue()) {
                // Update the UI on the main thread.
                self.textLabel.text = dataString
            } //dispatch_async
        } //NSURLSession
        task.resume()
        
//        if let url = NSURL(string: "http://10.0.0.20/arduino/press") {
//            UIApplication.sharedApplication().openURL(url)
//        }
    }

    @IBAction func outletOn(sender: AnyObject) {
        let url = NSURL(string: "http://10.0.0.20/arduino/outletOn")
        var dataString:String = ""
        let task = NSURLSession.sharedSession().dataTaskWithURL(url!) {(data, response, error) in
            dataString = String(NSString(data: data, encoding: NSUTF8StringEncoding)!)
            
            dispatch_async(dispatch_get_main_queue()) {
                // Update the UI on the main thread.
                self.textLabel.text = dataString
            } //dispatch_async
        } //NSURLSession
        task.resume()
    } //outletOn
    

    // https://www.youtube.com/watch?v=xt-Q4c1kMBQ
    // Open a NUSRLSession to flip the switch and suck in the data
    @IBAction func outletOff() {
        let url = NSURL(string: "http://10.0.0.20/arduino/outletOff")
        var dataString:String = ""
        let task = NSURLSession.sharedSession().dataTaskWithURL(url!) {(data, response, error) in
            dataString = String(NSString(data: data, encoding: NSUTF8StringEncoding)!)
        
            dispatch_async(dispatch_get_main_queue()) {
                // Update the UI on the main thread.
                self.textLabel.text = dataString
             } //dispatch_async
        } //NSURLSession
        task.resume()
     } //outletOff()

} //UI controller

