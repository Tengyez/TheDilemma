#include "icons.h"

//non-volatile storage
#include <Preferences.h>

#define INITIALIZED_KEY "INITIALIZE"
#define TOTAL_TRIALS_KEY "NUM_TRIALS"
#define TOTAL_SPLIT_STEAL_KEY "TOTAL_SPLIT_STEAL_KEY"
#define TOTAL_SPLIT_SPLIT_KEY "TOTAL_SPLIT_SPLIT_KEY"
#define TOTAL_STEAL_STEAL_KEY "TOTAL_STEAL_STEAL_KEY"
#define NVM_NAMESPACE "nvm"


Preferences nvm;
int split_splits = -2;
int split_steals = -2;
int steal_steals = -2;

//ms between cycles
#define TICK_SPEED 50

//for SPI SSD1306 screen
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SPLITBUTTON1_PIN 4
#define SPLITBUTTON2_PIN 5
#define STEALBUTTON1_PIN 7
#define STEALBUTTON2_PIN 6

#define OLED_DC  2  
#define OLED1_CS 3
#define OLED2_CS 9
#define OLED_CLK 8    
#define OLED_MOSI 10
#define OLED_RST 20

#define BUZZER_PIN 21

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// buzzer params

const long BUZZER_LENGTH = 200;
long lastBuzzer = -BUZZER_LENGTH;

// 2nd to last parameter is pin of RST, set to -1 because not needed
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED1_CS);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, -1, OLED2_CS);

//animations
struct Animation {
  const bool* anim;
  uint8_t fps;
  uint8_t frames;
  uint8_t width;
  uint8_t height;
  bool loop;
  int currentTick = 0; //milliseconds, tracks current time
};

struct Animation PersonIcon {
  &(PERSONICON[0][0][0]),
  1,1,
  49,49,
  true, 0
};
struct Animation CircleIcon {
  &(CIRCLEICON[0][0][0]),
  1,1,
  32,32,
  true, 0
};
struct Animation HandshakeIcon {
  &(HANDSHAKEICON[0][0][0]),
  1,1,
  40,40,
  true, 0
};
struct Animation DevilIcon {
  &(DEVILICON[0][0][0]),
  1,1,
  40,40,
  true, 0
};
struct Animation SadIcon {
  &(SADICON[0][0][0]),
  1,1,
  40,40,
  true, 0
};
struct Animation SkullIcon {
  &(SKULLICON[0][0][0]),
  1,1,
  40,40,
  true, 0
};
struct Animation ErrorIcon {
  &(ERRORICON[0][0][0]),
  1,1,
  40,40,
  true, 0
};
struct Animation CheckmarkIcon {
  &(CHECKMARKICON[0][0][0]),
  1,1,
  49,49,
  true, 0
};
struct Animation VotedIcon {
  &(VOTEDICON[0][0][0]),
  1,1,
  49,53,
  true, 0
};
struct Animation LoadingAnim1 {
  &(LOADINGANIM[0][0][0]),
  12,12,
  48,48,
  false, 0
};
struct Animation LoadingAnim2 {
  &(LOADINGANIM[0][0][0]),
  12,12,
  48,48,
  false, 0
};

//statistic functions, dangerous
    // Helper function to calculate erf(x) using Abramowitz and Stegun's approximation
double erf(double x) {
    // constants
    double p = 0.3275911;
    double a1 = 0.254829592;
    double a2 = -0.284496736;
    double a3 = 1.421413741;
    double a4 = -1.453152027;
    double a5 = 1.061405429;

    // Save the sign of x
    int sign = 1;
    if (x < 0) sign = -1;
    x = abs(x);

    // A&S formula 7.1.26
    double t = 1.0 / (1.0 + p * x);
    double y = 1.0 - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * exp(-x * x);

    return sign * y;
}
    // Main function to calculate normalcdf(-infinity, z)
double normalCDF(double z) {
    return z != 0 ? 0.5 * erf(z) : 0;
}
double calculateZScore(){
  return (
    ((double)(split_splits*2 + split_steals)) / ((split_splits + split_steals + steal_steals)*2) - 0.50
  ) / (
    sqrt(0.25 / ((split_splits + split_steals + steal_steals)*2)));
}

//non-volatile memory functions
void initMemory(){
  if(!nvm.getBool(INITIALIZED_KEY, false)){
    Serial.println("Initializing default parameters");
    nvm.putBool(INITIALIZED_KEY, true);
    nvm.putInt(TOTAL_SPLIT_SPLIT_KEY, 0);
    nvm.putInt(TOTAL_SPLIT_STEAL_KEY, 0);
    nvm.putInt(TOTAL_STEAL_STEAL_KEY, 0);
  }
  split_splits = nvm.getInt(TOTAL_SPLIT_SPLIT_KEY);
  split_steals = nvm.getInt(TOTAL_SPLIT_STEAL_KEY);
  steal_steals = nvm.getInt(TOTAL_STEAL_STEAL_KEY);
  
}
void saveMemory(){
  nvm.putInt(TOTAL_SPLIT_SPLIT_KEY, split_splits);
  nvm.putInt(TOTAL_SPLIT_STEAL_KEY, split_steals);
  nvm.putInt(TOTAL_STEAL_STEAL_KEY, steal_steals);
}
//animation functions
void playAnimation(
  Adafruit_SSD1306 &display,
  Animation &anim,
  int x,
  int y
){
  playAnimation(display, anim, x, y, true);
}
void playAnimation(
  Adafruit_SSD1306 &display,
  Animation &anim,
  int x,
  int y,
  bool overwrite
){
  bool finished = anim.currentTick >= (int)((1000.0/anim.fps)*(anim.frames));
  uint8_t frame = min((int)(anim.currentTick/(1000.0/anim.fps)), anim.frames-1);
  for(int i = 0; i < anim.height; i++){
    for(int j = 0; j < anim.width; j++){
      if(!overwrite && !anim.anim[frame*anim.width*anim.height + anim.width*i + j]){
        continue;
      }
      display.drawPixel(x+j, y+i, anim.anim[frame*anim.width*anim.height + anim.width*i + j]);
    }
  }
  if(anim.frames <= 1) return;
  
  if(!finished) {
    anim.currentTick += TICK_SPEED;
  } else if(anim.loop){
    anim.currentTick = 0;
  }
}

int player1_choice = 0;
int player2_choice = 0;
bool reset = 0;

void setBuzzer(){
  lastBuzzer = millis();
}

void setup() {
  Serial.begin(115200);

  Serial.println("! Starting...");
  pinMode(SPLITBUTTON1_PIN, INPUT_PULLUP);
  Serial.println("! Pass pin 1");
  pinMode(SPLITBUTTON2_PIN, INPUT_PULLUP);
  Serial.println("! Pass pin 2");
  pinMode(STEALBUTTON1_PIN, INPUT_PULLUP);
  Serial.println("! Pass pin 3");
  pinMode(STEALBUTTON2_PIN, INPUT_PULLUP);
  Serial.println("! Pass pin 4");
  //button interrupt setup
  attachInterrupt(digitalPinToInterrupt(SPLITBUTTON1_PIN),
                  [](){ 
                    if(player1_choice == 0){ 
                      player1_choice = 1; 
                      setBuzzer();
                    }
                  },
                  FALLING);
  Serial.println("! Pass interrupt 1");
  attachInterrupt(digitalPinToInterrupt(SPLITBUTTON2_PIN),
                  [](){ 
                    if(player2_choice == 0) {
                      player2_choice = 1; 
                      setBuzzer();
                    }
                  },
                  FALLING);
  Serial.println("! Pass interrupt 2");
  attachInterrupt(digitalPinToInterrupt(STEALBUTTON1_PIN),
                  [](){ 
                    if(player1_choice == 0) {
                      player1_choice = -1; 
                      setBuzzer();
                    }
                  },
                  FALLING);
  Serial.println("! Pass interrupt 3");
  attachInterrupt(digitalPinToInterrupt(STEALBUTTON2_PIN),
                  [](){ 
                    if(player2_choice == 0) {
                      player2_choice = -1;
                      setBuzzer();
                    }
                  },
                  FALLING);
  Serial.println("! Pass interrupt 4");
  pinMode(BUZZER_PIN, OUTPUT);

  if(!display1.begin(SSD1306_SWITCHCAPVCC)){
    Serial.println("!! DISPLAY 1 FAILURE");
    for(;;);
  }
  display1.display();
  Serial.println("! Pass display 1");

  if(!display2.begin(SSD1306_SWITCHCAPVCC)){
    Serial.println("!! DISPLAY 2 FAILURE");
    for(;;);
  }
  display2.display();
  Serial.println("! Pass display 2");

  // -----
  nvm.begin(NVM_NAMESPACE, false);
  initMemory();
  Serial.println("! Pass non-volatile memory setup");
  // -----
  Serial.println("! Exiting setup ...");

  display2.setTextSize(1);      // Normal 1:1 pixel scale
  display2.setTextColor(SSD1306_WHITE); // Draw white text
  display2.setCursor(0, 0);     // Start at top-left corner
  display2.cp437(true);         // Use full 256 char 'Code Page 437' font

  
  display1.setTextSize(1);      // Normal 1:1 pixel scale
  display1.setTextColor(SSD1306_WHITE); // Draw white text
  display1.setCursor(0, 0);     // Start at top-left corner
  display1.cp437(true);         // Use full 256 char 'Code Page 437' font5

  delay(500);
}
void drawCenteredText(
  Adafruit_SSD1306 &display,
  const char *text,
  int16_t x,
  int16_t y,
  bool xIsCenter = true,
  bool yIsCenter = true
){
  uint16_t w, h; 
  int16_t x1, y1;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  display.setCursor(
    xIsCenter ? x-(w/2) : x,
    yIsCenter ? y-(h/2) : y
  );
  display.print(text);
}
void resultScreen(
  Adafruit_SSD1306 &display,
  int selfchoice,
  int opponentchoice
){
  Animation selectedAnimation = 
    selfchoice > opponentchoice ? DevilIcon :
    selfchoice < opponentchoice ? SadIcon :
    selfchoice == -1 && opponentchoice == -1 ? SkullIcon :
    selfchoice == 1 && opponentchoice == 1 ? CheckmarkIcon :
    ErrorIcon;
  const char *txt = selfchoice > opponentchoice ? "YOU STOLE SUCCESSFUL" :
      selfchoice < opponentchoice ? "YOU GOT STOLEN FROM" :
      selfchoice == -1 && opponentchoice == -1 ? "BOTH STOLE" :
      selfchoice == 1 && opponentchoice == 1 ? "BOTH SPLIT" :
      "ERROR";
  playAnimation(display, selectedAnimation, 
  SCREEN_WIDTH/2 - selectedAnimation.width/2,
  4);
  drawCenteredText(
    display, txt,
    SCREEN_WIDTH/2,
    SCREEN_HEIGHT-10,
    true, false
  );
}
void statisticsScreen(
  Adafruit_SSD1306 &display
){
  display.clearDisplay();
  display.setTextSize(1);
  drawCenteredText(
    display, 
    "TOTAL STATS",
    SCREEN_WIDTH/2, 4, 
    true, false
  );
  display.setTextSize(1);
  drawCenteredText(
    display, 
    String(String(split_splits) + " split-split").c_str(), 
    SCREEN_WIDTH/2, 20, 
    true, false
  );
  drawCenteredText(
    display, 
    String(String(split_steals) + " split-steal").c_str(), 
    SCREEN_WIDTH/2, 30, 
    true, false
  );
  drawCenteredText(
    display, 
    String(String(steal_steals) + " steal-steal").c_str(), 
    SCREEN_WIDTH/2, 40, 
    true, false
  );
}
void confidenceScreen(
  Adafruit_SSD1306 &display
){
  display.clearDisplay();
  double zScore = calculateZScore();
  double confidence = (normalCDF(abs(zScore)) - normalCDF(0))*2;
  if(zScore != 0.0){
    display.setTextSize(2);
    drawCenteredText(
      display,
      String(String(confidence*100, 2) + "%").c_str(),
      SCREEN_WIDTH/2, 20,
      true, false
    );
    display.setTextSize(1);
    drawCenteredText(
      display,
      "We are",
      SCREEN_WIDTH/2, 10,
      true, true
    );
    drawCenteredText(
      display,
      "confident that HACK",
      SCREEN_WIDTH/2, 40,
      true, true
    );
    drawCenteredText(
      display,
      String("CLUB is " + String(calculateZScore() < 0 ? "EVIL" : "GOOD")).c_str(),
      SCREEN_WIDTH/2, 50,
      true, true
    );
  } else {
    drawCenteredText(
      display,
      "equal splits & steals",
      SCREEN_WIDTH/2, SCREEN_HEIGHT/2,
      true, true
    );
    drawCenteredText(
      display,
      "unable to determine",
      SCREEN_WIDTH/2, 44,
      true, false
    );
  }
}

void checkBuzzer(){
  if(millis() - lastBuzzer < BUZZER_LENGTH ){
    tone(BUZZER_PIN, 1000);
  } else {
    noTone(BUZZER_PIN);
  }
}
//choices : 0 = no choice yet, 1 = split, -1 = steal
void loop() {
  checkBuzzer();
  if(reset==1){
    delay(4000);
    display1.clearDisplay();
    display2.clearDisplay();
    display1.display();
    display2.display();
    delay(1000);
    LoadingAnim1.currentTick = 0;
    LoadingAnim2.currentTick = 0;
    player1_choice = 0;
    player2_choice = 0;
    reset = 0;
  }
  display1.clearDisplay();
  display2.clearDisplay();
  if(player1_choice == 0 || player2_choice == 0){
    for(int i = 1; i <= 2; i++){
      drawCenteredText(
        i == 1 ? display1 : display2,
        "SPLIT",
        SCREEN_WIDTH/2,
        18,
        true,
        false
      );
      drawCenteredText(
        i == 1 ? display1 : display2,
        "OR",
        SCREEN_WIDTH/2,
        27,
        true,
        false
      );
      drawCenteredText(
        i == 1 ? display1 : display2,
        "STEAL",
        SCREEN_WIDTH/2,
        36,
        true,
        false
      );
      playAnimation(
        i == 1 ? display1 : display2,
        player1_choice == 0 ? PersonIcon : VotedIcon,
        0,
        5
      );
      playAnimation(
        i == 1 ? display1 : display2,
        player2_choice == 0 ? PersonIcon : VotedIcon,
        (SCREEN_WIDTH-(player2_choice == 0 ? PersonIcon.width : VotedIcon.width)),
        5
      );
      drawCenteredText(
        i == 1 ? display1 : display2,
        "1                  2",
        SCREEN_WIDTH/2,
        3,
        true,
        false
      );
    }
  } else {
    for(int i = 1; i <= 2; i++){
      playAnimation(
        i == 1 ? display1 : display2,
        VotedIcon,
        0,
        5
      );
      playAnimation(
        i == 1 ? display1 : display2,
        VotedIcon,
        (SCREEN_WIDTH-(player2_choice == 0 ? PersonIcon.width : VotedIcon.width)),
        5
      );
    }
    display1.display();
    display2.display();
    for(int i = 0; i < 500/TICK_SPEED; i++){
      delay(TICK_SPEED);
      checkBuzzer();
    }
    if(player1_choice > player2_choice || player2_choice > player1_choice){
      split_steals++;
    } else if(player1_choice == -1 && player2_choice == -1){
      steal_steals++;
    } else {
      split_splits++;
    }
    saveMemory();
    for(int i = 0; i < 1500/TICK_SPEED; i++){
      display1.clearDisplay();
      display2.clearDisplay();
      playAnimation(
        display1, LoadingAnim1,
        (SCREEN_WIDTH/2-LoadingAnim1.width/2),
        (SCREEN_HEIGHT/2-LoadingAnim1.height/2)
      );
      playAnimation(
        display2, LoadingAnim2,
        (SCREEN_WIDTH/2-LoadingAnim2.width/2),
        (SCREEN_HEIGHT/2-LoadingAnim2.height/2)
      );
      display1.display();
      display2.display();
      checkBuzzer();
      delay(TICK_SPEED);
    }

    resultScreen(display1, player1_choice, player2_choice);
    resultScreen(display2, player2_choice, player1_choice);
    reset = 1;
    display1.display();
    display2.display();

    delay(3200);

    statisticsScreen(display1);
    statisticsScreen(display2);

    display1.display();
    display2.display();

    delay(2000);

    confidenceScreen(display1);
    confidenceScreen(display2);

    display1.display();
    display2.display();
  }

  // ...
  display1.display();
  display2.display();


  delay(TICK_SPEED);
}
