#include "fantasia.h"

// https://github.com/newdigate/teensy-variable-playback
#include <TeensyVariablePlayback.h>

#define MAX_NUM_SOUND_BANKS 2
#define MAX_NUM_SOUND_FILES 16

#define DUMP_POTS 1

Fantasia fantasia;

// Audio Declarations
AudioControlSGTL5000     sgtl5000_1;     //xy=1125,313

// GUItool: begin automatically generated code
// AudioPlaySdWav           wavPlayer;     //xy=217,501
AudioPlaySdResmp         wavPlayer;     //xy=217,501
AudioOutputI2S           i2s1;           //xy=435,502
AudioConnection          patchCord1(wavPlayer, 0, i2s1, 0);
AudioConnection          patchCord2(wavPlayer, 1, i2s1, 1);
// GUItool: end automatically generated code




File root;
enum {
SOUND_BANK_A,
SOUND_BANK_B,
};
String sounds[MAX_NUM_SOUND_BANKS][MAX_NUM_SOUND_FILES];
int soundBank = SOUND_BANK_A;
int soundFile = 0; // display value
String soundFilename;




long oldEncPos = -999;
long encPos = -999;
bool encFlag = 0; // is display decimal flag displayed
char displayValue = 0;

int playratePotValue = 0;
int startPotValue = 0;
int sizePotValue = 0;
int spreadPotValue = 0;
bool loopWav = true;
bool btn1Down = false;

void setup() {  
  // open the serial port at 9600 bps:
  Serial.begin(9600); 
  Serial.println("Booting Sampler");
  
  audioSetup();

  if (fantasia.sdCardSetup()){
    Serial.println("SD Card ready");
  } else {
    Serial.println("Failed to load SD Card");
  }

  loadSoundFiles();
  wavPlayer.playWav("SDTEST1.WAV");
  Serial.println("Playing sound file...");

  Serial.println("Seting up fantasia...");
  fantasia.setup();
  Serial.println("Done");

  Serial.print("SOUND_BANK:"); Serial.println(soundBank);
}

void loop() {

  fantasia.update();
  // Controller values can be used directly after update fantasia:
  // fantasia.pot1, fantasia.btn2, encPos, fantasia.encPressed

  update();
  // Values can be used after being updated: encPos...
  
  // change sample speed
  playratePotValue = max(fantasia.pot1, 20);
  wavPlayer.setPlaybackRate(getPlaybackRate(playratePotValue));
}


//local update for this specific patch
void update() {
  
  updateEncoder();

#if DUMP_POTS
  Serial.print("potA:");
  Serial.print(fantasia.pot1);
  Serial.print(" potB:");
  Serial.print(fantasia.pot2);
  Serial.print(" potC:");
  Serial.print(fantasia.pot3);
  Serial.print(" potD:");
  Serial.print(fantasia.pot4);
  Serial.print(" btnA:");
  Serial.print(fantasia.btn1);
  Serial.print(" btnB:");
  Serial.print(fantasia.btn2);
  Serial.print(" btnC:");
  Serial.print(fantasia.btn3);
  Serial.print(" btnD:");
  Serial.print(fantasia.btn4);
  Serial.print(" display:");
  Serial.print(int(displayValue));
  Serial.print(" dot:");
  Serial.print(encFlag);
  Serial.print(" enc:");
  Serial.print(encPos);
  Serial.print(" encPressed:");
  Serial.print(fantasia.encPressed);
  Serial.print(" wav:");
  Serial.print(soundFilename);
  Serial.println();
#endif
}

void audioSetup() {
  Serial.println("Setting up audio...");
  AudioMemory(24);
  sgtl5000_1.enable();
  sgtl5000_1.volume(1);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.micGain(36); //NEEDED?

  wavPlayer.enableInterpolation(true);
  wavPlayer.setLoopType(loop_type::looptype_repeat);
  playratePotValue = analogRead(fantasia.potPin1);
  startPotValue = analogRead(fantasia.potPin2);
  wavPlayer.setPlaybackRate(getPlaybackRate(playratePotValue));
  wavPlayer.setLoopStart(getStartPosition(startPotValue));

  Serial.println("Audio ready");
}

void printDirectory(File dir, int numTabs) {
  while( true ) {
    File entry = dir.openNextFile();
    
    if (!entry) { break; }

    for(uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    
    Serial.print(entry.name());
    
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    
    entry.close();
  }
}

void loadSoundFiles() {
    Serial.println("Loading sound files...");
    Serial.println("Reading root...");
    root = SD.open("/");
    // printDirectory( root, 0 );

    int soundIndex = 0;

    while(true) {
      File entry = root.openNextFile();
      if(!entry) { break; }

      String s(entry.name());
      if (s.endsWith(".wav") || s.endsWith(".WAV")) {
        sounds[soundBank][soundIndex] = entry.name();
        Serial.print(entry.name());
        Serial.print(" : ");
        Serial.println(sounds[soundBank][soundIndex]);
        soundIndex++;
      }
      else {
        Serial.print(entry.name());
        Serial.println(" not supported.");
      }
      entry.close();
    }
    root.close();
    Serial.println("Loading sound files done.");
}

void changeSoundFile( String filename ) {
  Serial.print("change sound: ");
  Serial.println(filename);
  int length = filename.length() + 1;
  char buf [length];
  filename.toCharArray(buf, length);
  wavPlayer.stop();
  wavPlayer.playWav(buf);
}

void updateEncoder() {

  int pos = fantasia.enc.read() / 4;

  if (fantasia.encPressed) { 
    encFlag = !encFlag;
    soundBank = int(encFlag);
    soundFilename = sounds[soundBank][soundFile];
  }

  if (pos != oldEncPos) {
    int hex = abs(pos) % 16;
    if(pos < 0) {
      hex = 16 - hex;
    }

    // safety measure to avoid reading empty slots
    // if( sounds[soundBank][hex].length() <= 0 ) return;
    // if(sounds[soundBank][hex].length() <=0 && pos > 0) updateEncoder(hex+1);
    // if(sounds[soundBank][hex].length() <=0 && pos < 0) updateEncoder(hex-1);

    displayValue = hex;

    soundFilename = sounds[soundBank][hex];
    if (soundFilename.length() != 0) {
      changeSoundFile(soundFilename);
    }
    encPos = pos;
    oldEncPos = pos;
  }
    
  fantasia.segmentDisplay.displayHex(displayValue, encFlag);
}

double getPlaybackRate(int16_t analog) {
  return (analog - 512.0) / 512.0 * 2;
}

double getStartPosition(int16_t analog) {
  return (analog - 512.0) / 512.0;
}
