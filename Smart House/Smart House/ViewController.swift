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
        let alertController = UIAlertController(title: "Heater On", message: "Heater On",
            preferredStyle: UIAlertControllerStyle.Alert)
        alertController.addAction(UIAlertAction(title: "OK", style: UIAlertActionStyle.Default, handler: nil))
        self.presentViewController(alertController, animated: true, completion: nil)
    }

}

