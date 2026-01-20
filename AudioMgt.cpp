#include <iostream>
#include <vector>
#include <memory>
#include <typeinfo>
#include <string>
using namespace std;
// Base class with virtual functions for polymorphism
class MediaFile {
protected:
    string filename;
    double duration;    // in seconds
    long fileSize;  // in bytes
public:
    MediaFile(const string& name, double dur, long size)
        : filename(name), duration(dur), fileSize(size) {
        cout << "MediaFile created: " << filename << endl;
    }    
    // Virtual destructor - essential for inheritance
    virtual ~MediaFile() {
        cout << "MediaFile destroyed: " << filename << endl;
    }    
    // Pure virtual functions - make class abstract
    virtual void play() const = 0;
    virtual void pause() const = 0;
    virtual void stop() const = 0;
    virtual string getMediaType() const = 0;    
    // Virtual function with default implementation
    virtual void displayInfo() const {
        cout << "File: " << filename << endl;
        cout << "Duration: " << duration << "s" << endl;
        cout << "Size: " << fileSize << " bytes" << endl;
        cout << "Type: " << getMediaType() << endl;
    }    
    // Non-virtual utility methods
    string getFilename() const { return filename; }
    double getDuration() const { return duration; }
    long getFileSize() const { return fileSize; }
};
// Audio file implementation
class AudioFile : public MediaFile {
private:
    int bitRate;
    string audioFormat;    
public:
    AudioFile(const string& name, double dur, long size, int rate, const string& format)
        : MediaFile(name, dur, size), bitRate(rate), audioFormat(format) {
        cout << "AudioFile created: " << audioFormat << " format" << endl;
    }    
    ~AudioFile() {
        cout << "AudioFile destroyed: " << filename << endl;
    }    
    void play() const override {
        cout << "🎵 Playing audio: " << filename << " (" << audioFormat << ")" << endl;
        cout << "Bitrate: " << bitRate << " kbps" << endl;
    }    
    void pause() const override {
        cout << "⏸️ Audio paused: " << filename << endl;
    }    
    void stop() const override {
        cout << "⏹️ Audio stopped: " << filename << endl;
    }    
    string getMediaType() const override {
        return "Audio (" + audioFormat + ")";
    }    
    // Audio-specific method
    void adjustVolume(int volume) const {
        cout << "🔊 Volume set to " << volume << "% for " << filename << endl;
    }    
    int getBitRate() const { return bitRate; }
    string getAudioFormat() const { return audioFormat; }
};
// Video file implementation
class VideoFile : public MediaFile {
private:
    string resolution;
    int frameRate;
    string videoCodec;    
public:
    VideoFile(const string& name, double dur, long size, const string& res, int fps, const string& codec)
        : MediaFile(name, dur, size), resolution(res), frameRate(fps), videoCodec(codec) {
        cout << "VideoFile created: " << resolution << " @ " << frameRate << "fps" << endl;
    }    
    ~VideoFile() {
        cout << "VideoFile destroyed: " << filename << endl;
    }    
    void play() const override {
        cout << "🎬 Playing video: " << filename << endl;
        cout << "Resolution: " << resolution << " @ " << frameRate << "fps" << endl;
        cout << "Codec: " << videoCodec << endl;
    }    
    void pause() const override {
        cout << "⏸️ Video paused: " << filename << endl;
    }    
    void stop() const override {
        cout << "⏹️ Video stopped: " << filename << endl;
    }    
    string getMediaType() const override {
        return "Video (" + videoCodec + ")";
    }    
    // Video-specific method
    void changeQuality(const string& newRes) const {
        cout << "📺 Quality changed to " << newRes << " for " << filename << endl;
    }    
    string getResolution() const { return resolution; }
    int getFrameRate() const { return frameRate; }
    string getVideoCodec() const { return videoCodec; }
};

int main()
{
  AudioFile audio1("song.mp3", 240.0, 5000000, 320, "MP3");
  audio1.displayInfo();
  audio1.play();
  audio1.adjustVolume(75);
  audio1.pause();
  audio1.stop();
  cout << "------------------------" << endl;

  VideoFile video1("movie.mp4", 7200.0, 1500000000, "1920x1080", 30, "H.264");
  video1.displayInfo();
  video1.play();
  video1.changeQuality("1280x720");
  video1.pause();
  video1.stop();
  cout << "------------------------" << endl;

  // Experiment with base class pointers:
  // Store different file types in MediaFile pointers
  
  unique_ptr<MediaFile> mediaFiles[2] = {
      make_unique<AudioFile>("podcast.aac", 3600.0, 30000000, 256, "AAC"),
      make_unique<VideoFile>("documentary.mkv", 5400.0, 2000000000, "3840x2160", 60, "HEVC")
  };
  for (const auto& media : mediaFiles) {
      media->displayInfo();
      media->play();
      media->pause();
      media->stop();
      cout << "------------------------" << endl;
  }

  // Call virtual methods through base class pointers
  // Verify that correct derived class methods are called
}