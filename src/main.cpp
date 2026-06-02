#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define I2C_SDA 21
#define I2C_SCL 22
#define BUTTON_JUMP 4
#define BUTTON_ACTION 15

#define GROUND_Y (SCREEN_HEIGHT - 12)
#define PLAYER_WIDTH 10
#define PLAYER_HEIGHT 10
#define OBSTACLE_WIDTH 8
#define OBSTACLE_HEIGHT 14

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

enum GameState {
  STATE_MENU,
  STATE_PLAYING,
  STATE_GAME_OVER,
};

struct Player {
  int x;
  int y;
  int vy;
  bool onGround;
};

struct Obstacle {
  int x;
  int y;
  int w;
  int h;
  bool active;
};

class GameEngine {
public:
  void begin() {
    Wire.begin(I2C_SDA, I2C_SCL);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      while (true) {
        delay(1000);
      }
    }

    pinMode(BUTTON_JUMP, INPUT_PULLUP);
    pinMode(BUTTON_ACTION, INPUT_PULLUP);

    display.clearDisplay();
    display.display();

    reset();
    state = STATE_MENU;
    lastFrame = millis();
  }

  void run() {
    uint32_t now = millis();
    if (now - lastFrame < 16) {
      return;
    }

    uint32_t dt = min(now - lastFrame, (uint32_t)50);
    lastFrame = now;

    update(dt);
    render();
  }

private:
  GameState state = STATE_MENU;
  Player player;
  Obstacle obstacle;
  uint32_t lastFrame = 0;
  uint32_t score = 0;
  uint32_t bestScore = 0;

  void reset() {
    player.x = 18;
    player.y = GROUND_Y - PLAYER_HEIGHT;
    player.vy = 0;
    player.onGround = true;

    obstacle.x = SCREEN_WIDTH;
    obstacle.y = GROUND_Y - OBSTACLE_HEIGHT;
    obstacle.w = OBSTACLE_WIDTH;
    obstacle.h = OBSTACLE_HEIGHT;
    obstacle.active = true;

    score = 0;
  }

  bool buttonPressed(int pin) {
    return digitalRead(pin) == LOW;
  }

  void update(uint32_t dt) {
    switch (state) {
      case STATE_MENU:
        if (buttonPressed(BUTTON_ACTION) || buttonPressed(BUTTON_JUMP)) {
          state = STATE_PLAYING;
          reset();
        }
        break;

      case STATE_PLAYING:
        updatePlayer(dt);
        updateObstacle(dt);
        if (checkCollision()) {
          state = STATE_GAME_OVER;
          bestScore = max(bestScore, score);
        } else {
          score += dt / 20;
        }
        break;

      case STATE_GAME_OVER:
        if (buttonPressed(BUTTON_ACTION) || buttonPressed(BUTTON_JUMP)) {
          state = STATE_MENU;
        }
        break;
    }
  }

  void updatePlayer(uint32_t dt) {
    if (player.onGround && buttonPressed(BUTTON_JUMP)) {
      player.vy = -8;
      player.onGround = false;
    }

    if (!player.onGround) {
      player.vy += 1;
      player.y += player.vy;
      if (player.y >= GROUND_Y - PLAYER_HEIGHT) {
        player.y = GROUND_Y - PLAYER_HEIGHT;
        player.vy = 0;
        player.onGround = true;
      }
    }
  }

  void updateObstacle(uint32_t dt) {
    if (!obstacle.active) {
      return;
    }

    int speed = 80 + (score / 200);
    obstacle.x -= (speed * dt) / 1000;

    if (obstacle.x + obstacle.w < 0) {
      obstacle.x = SCREEN_WIDTH + random(8, 24);
      obstacle.w = OBSTACLE_WIDTH + random(0, 4);
      obstacle.h = OBSTACLE_HEIGHT + random(0, 4);
      obstacle.y = GROUND_Y - obstacle.h;
    }
  }

  bool checkCollision() {
    int px1 = player.x;
    int py1 = player.y;
    int px2 = player.x + PLAYER_WIDTH;
    int py2 = player.y + PLAYER_HEIGHT;
    int ox1 = obstacle.x;
    int oy1 = obstacle.y;
    int ox2 = obstacle.x + obstacle.w;
    int oy2 = obstacle.y + obstacle.h;

    return px1 < ox2 && px2 > ox1 && py1 < oy2 && py2 > oy1;
  }

  void render() {
    display.clearDisplay();

    drawGround();
    drawPlayer();
    drawObstacle();
    drawHud();

    if (state == STATE_MENU) {
      display.setTextSize(1);
      display.setCursor(20, 20);
      display.print("ESP32 128x64 GAME ENGINE");
      display.setCursor(24, 38);
      display.print("PRESS ANY BUTTON");
      display.setCursor(40, 50);
      display.print("TO START");
    }

    if (state == STATE_GAME_OVER) {
      display.setTextSize(1);
      display.setCursor(34, 24);
      display.print("GAME OVER");
      display.setCursor(14, 38);
      display.print("PRESS ANY BUTTON TO RETURN");
    }

    display.display();
  }

  void drawGround() {
    display.drawFastHLine(0, GROUND_Y, SCREEN_WIDTH, SSD1306_WHITE);
    display.drawFastHLine(0, GROUND_Y + 1, SCREEN_WIDTH, SSD1306_WHITE);
  }

  void drawPlayer() {
    display.fillRect(player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT, SSD1306_WHITE);
    display.drawRect(player.x + 1, player.y + 1, PLAYER_WIDTH - 2, PLAYER_HEIGHT - 2, SSD1306_BLACK);
  }

  void drawObstacle() {
    display.fillRect(obstacle.x, obstacle.y, obstacle.w, obstacle.h, SSD1306_WHITE);
  }

  void drawHud() {
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("SCORE:");
    display.setCursor(48, 0);
    display.print(score);
    display.setCursor(0, 10);
    display.print("BEST:");
    display.setCursor(40, 10);
    display.print(bestScore);
  }
};

GameEngine engine;

void setup() {
  engine.begin();
}

void loop() {
  engine.run();
}
