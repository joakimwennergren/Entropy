import AVFoundation

var player: AVAudioPlayer?

@_cdecl("say_hello")
func playSound() {


    
    print("")


    guard let path = Bundle.main.path(forResource: "Anticlimax", ofType:"wav") else {
        return }
    let url = URL(fileURLWithPath: path)

    do {
        player = try AVAudioPlayer(contentsOf: url)
        player?.play()
        
    } catch let error {
        print(error.localizedDescription)
    }
}

