#include "icons.h"

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
#define OLED1_CS 0
#define OLED2_CS 1
#define OLED_CLK 8
#define OLED_MOSI 10
#define OLED_RST -1

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64


// 2nd to last parameter is pin of RST, set to -1 because not needed
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED1_CS);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED2_CS);

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
  true
};
struct Animation CircleIcon {
  &(CIRCLEICON[0][0][0]),
  1,1,
  32,32,
  true
};
struct Animation HandshakeIcon {
  &(HANDSHAKEICON[0][0][0]),
  1,1,
  40,40,
  true
};
struct Animation DevilIcon {
  &(DEVILICON[0][0][0]),
  1,1,
  40,40,
  true
};
struct Animation SadIcon {
  &(SADICON[0][0][0]),
  1,1,
  40,40,
  true
};
struct Animation SkullIcon {
  &(SKULLICON[0][0][0]),
  1,1,
  40,40,
  true
};
struct Animation ErrorIcon {
  &(ERRORICON[0][0][0]),
  1,1,
  40,40,
  true
};

//--------

void playAnimation(
  Adafruit_SSD1306 display,
  Animation &anim,
  int x,
  int y
){
  playAnimation(display, anim, x, y, true);
}
void playFlippedAnimation(
  Adafruit_SSD1306 display,
  Animation &anim,
  int x,
  int y
) {
  bool finished = anim.currentTick >= (int)((1000.0/anim.fps)*(anim.frames));
  uint8_t frame = min((int)(anim.currentTick/(1000.0/anim.fps)), anim.frames-1);
  for(int i = 0; i < anim.height; i++){
    for(int j = 0; j < anim.width; j++){
      display.drawPixel(x+j, y+i, anim.anim[anim.frames*anim.width*anim.height - 1 - (frame*anim.width*anim.height + anim.width*i + j)]);
    }
  }
  if(anim.frames <= 1) return;
  
  if(!finished) {
    anim.currentTick += TICK_SPEED;
  } else if(anim.loop){
    anim.currentTick = 0;
  }
}
void playAnimation(
  Adafruit_SSD1306 display,
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
                  [](){ if(player1_choice == 0) player1_choice = 1; },
                  FALLING);
  Serial.println("! Pass interrupt 1");
  attachInterrupt(digitalPinToInterrupt(SPLITBUTTON2_PIN),
                  [](){ if(player2_choice == 0) player2_choice = 1; },
                  FALLING);
  Serial.println("! Pass interrupt 2");
  attachInterrupt(digitalPinToInterrupt(STEALBUTTON1_PIN),
                  [](){ if(player1_choice == 0) player1_choice = -1; },
                  FALLING);
  Serial.println("! Pass interrupt 3");
  attachInterrupt(digitalPinToInterrupt(STEALBUTTON2_PIN),
                  [](){ if(player2_choice == 0) player2_choice = -1; },
                  FALLING);
  Serial.println("! Pass interrupt 4");

  if(!display1.begin(SSD1306_SWITCHCAPVCC)){
    Serial.println("!! DISPLAY 1 FAILURE");
    delay(500);
  }
  Serial.println("! Pass display 1");
  if(!display2.begin(SSD1306_SWITCHCAPVCC)){
    Serial.println("!! DISPLAY 2 FAILURE");
    delay(500);
  }
  Serial.println("! Pass display 2");


  Serial.println("! Exiting setup ...");
  delay(4000);
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
  display.getTextBounds(text, x, y, nullptr,
                     nullptr, &w, &h);
  display.setCursor(
    xIsCenter ? x-(w/2) : x,
    yIsCenter ? y-(h/2) : y
  );
  display.write(text);
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
    selfchoice == 1 && opponentchoice == 1 ? HandshakeIcon :
    ErrorIcon;
  playAnimation(display, selectedAnimation, 
  SCREEN_WIDTH/2 - selectedAnimation.width/2,
  4);
  drawCenteredText(
    display,
        selfchoice > opponentchoice ? "YOU STOLE SUCCESSFULLY" :
      selfchoice < opponentchoice ? "YOU GOT STOLEN FROM" :
      selfchoice == -1 && opponentchoice == -1 ? "BOTH STOLE" :
      selfchoice == 1 && opponentchoice == 1 ? "BOTH SPLIT" :
      "ERROR"
    ,
    SCREEN_WIDTH/2,
    SCREEN_HEIGHT-10,
    true,
    false
  );
}
//choices : 0 = no choice yet, 1 = split, -1 = steal
void loop() {
  
  if(player1_choice == 0 || player2_choice == 0){
    for(int i = 1; i <= 2; i++){
      drawCenteredText(
        i == 1 ? display1 : display2,
        "SPLIT OR STEAL",
        SCREEN_WIDTH/2,
        SCREEN_HEIGHT-10,
        true,
        false
      );
      if(i == 1 and player1_choice == 0 || i == 2 and player2_choice == 0)
      playAnimation(
        i == 1 ? display1 : display2,
        PersonIcon,
        i == 1 ? 0 : (SCREEN_WIDTH-PersonIcon.width),
        (SCREEN_HEIGHT/2)-(PersonIcon.height/2)
      );
      else 
      playAnimation(
        i == 1 ? display1 : display2,
        CircleIcon,
        i == 1 ? 0 : (SCREEN_WIDTH-CircleIcon.width),
        (SCREEN_HEIGHT/2)-(CircleIcon.height/2)
      );
    }
  } else {
    resultScreen(display1, player1_choice, player2_choice);
    resultScreen(display2, player2_choice, player1_choice);
  }

  // ...
  display1.display();
  display2.display();
  delay(TICK_SPEED);
  display1.clearDisplay();
  display2.clearDisplay();
}
