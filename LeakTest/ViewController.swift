//
//  ViewController.swift
//  LeakTest
//
//  Created by Pictune on 11/17/20.
//

import UIKit
import AudioKit

class ViewController: UIViewController {
    
    var playButton: UIButton!
    var effect: AKCostelloReverb!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        playButton = UIButton(frame: CGRect(x: 0, y: 0, width: 100, height: 100))
        playButton.backgroundColor = UIColor(white: 1, alpha: 0.5)
        view.addSubview(playButton)
        
        playButton.addTarget(self, action: #selector(pressed(sender:)), for: .touchUpInside)
    }
    
    @objc func pressed(sender: UIButton) {
        let filename = Bundle.main.url(forResource: "track1", withExtension: "wav")
        let player = try! AKAudioPlayer(file: AKAudioFile(forReading: filename!))
        player.completionHandler = completion
        print("before AKCostelloReverb(player)")
        effect = AKCostelloReverb(player)
        
        let mixer = AKMixer(effect)
        AudioKit.output = mixer
        print("before try! AudioKit.start()")
        try! AudioKit.start()
        
        player.play()
    }
    
    func completion() {
        do {
            print("before effect.detach()")
            effect.detach()
            print("before AudioKit.stop()")
            try AudioKit.stop()
        } catch {}
    }

}

