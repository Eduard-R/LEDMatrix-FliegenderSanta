#include <Adafruit_GFX.h>
#include <RGBmatrixPanel.h>
#define CLK 50
#define LAT 10
#define OE  51
#define A   A0
#define B   A1
#define C   A2
#define D   A3
#define w 32
#define h 32
#define interval 40
#define nh 15
#define nm 10
#define nv  8
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true);
// Farben
uint16_t ch = matrix.Color333(2, 2, 2);
uint16_t cm = matrix.Color333(4, 4, 4);
uint16_t cv = matrix.Color333(7, 7, 7);
uint16_t red   = matrix.Color333(7, 0, 0);
uint16_t white = matrix.Color333(7, 7, 7);
uint16_t brown = matrix.Color333(4, 2, 1);
uint16_t dark  = matrix.Color333(2, 1, 0);
uint16_t black = matrix.Color333(0, 0, 0);
uint16_t yellow = matrix.Color333(7, 7, 2);
uint16_t blue = matrix.Color333(0, 0, 7);
uint16_t green = matrix.Color333(0, 7, 0);
// Schneeflocken
int sh[nh][2];
int sm[nm][2];
int sv[nv][2];
// Timing
unsigned long waittime = 0;
byte step = 0;
// Wind
int wind = 0;
unsigned long windTimer = 0;
// Santa
int santaX = -40;
int santaBaseY = 6;
int santaPhase = 0;
void setup() {
  randomSeed(analogRead(7) * analogRead(8) + analogRead(9));
  matrix.begin();
  for (int i = 0; i < nh; i++) { sh[i][0] = random(w); sh[i][1] = random(h); }
  for (int i = 0; i < nm; i++) { sm[i][0] = random(w); sm[i][1] = random(h); }
  for (int i = 0; i < nv; i++) { sv[i][0] = random(w); sv[i][1] = random(h); }
}
void fallSnow(int &x, int &y, int speedDiv) {
  if (step % speedDiv != 0) return;
  y++;
  if (random(3) == 0) x += wind;
  if (x < 0) x = 0;
  if (x >= w) x = w - 1;
  if (y >= h) {
    y = 0;
    x = random(w);
  }
}
void drawBigFlake(int x, int y, uint16_t c) {
  matrix.drawPixel(x, y, c);
  if (x + 1 < w) matrix.drawPixel(x + 1, y, c);
  if (y + 1 < h) matrix.drawPixel(x, y + 1, c);
}
// 🦌 Rentier (nach rechts schauend)
void drawReindeer(int x, int y, bool legsUp) {
  matrix.fillRect(x, y + 2, 6, 3, brown);      // Körper
  matrix.fillRect(x + 3, y + 2, 3, 2, brown);  // Kopf rechts
  matrix.drawPixel(x + 6, y + 2, dark);        // Nase rechts
  // Geweih rechts
  matrix.drawPixel(x + 3, y + 0, dark);
  matrix.drawPixel(x + 4, y + 1, dark);
  matrix.drawPixel(x + 5, y, dark);
  matrix.drawPixel(x + 5, y + 1, dark);
  // Beine
  if (legsUp) {
    matrix.drawPixel(x + 1, y + 5, dark);
    matrix.drawPixel(x + 4, y + 4, dark);
  } else {
    matrix.drawPixel(x + 1, y + 4, dark);
    matrix.drawPixel(x + 4, y + 5, dark);
  }
}
// 🎅 Santa + Schlitten + Rentiere
void drawSanta(int x, int y, bool legsUp) {
  // Sack
  matrix.fillCircle(x + 3, y + 3, 2, brown);
  
  // Schlitten
  matrix.fillRect(x, y + 6, 12, 3, brown);
  matrix.drawLine(x + 2, y + 9, x + 2, y + 10, white);
  matrix.drawLine(x + 9, y + 9, x + 9, y + 10, white);
  // Kufen unten
  matrix.drawLine(x, y + 11, x + 11, y + 11, white);
  matrix.drawPixel(x + 12, y + 10, white);

  // Santa (weiter rechts)
  matrix.fillRect(x + 6, y + 2, 4, 4, red);
  matrix.drawPixel(x + 7, y + 1, white);
  matrix.drawPixel(x + 8, y + 1, white);
  matrix.drawPixel(x + 7, y, white);
  matrix.drawPixel(x + 8, y, white);
  matrix.drawPixel(x + 7, y - 1, red);
  matrix.drawPixel(x + 8, y - 1, red);
  matrix.drawPixel(x + 6, y - 1, red);
  matrix.drawPixel(x + 5, y, white);
  
  // Zugleine
  matrix.drawLine(x + 12, y + 7, x + 24, y + 6, white);
  matrix.drawPixel(x + 25, y + 6, white);
  
  // Rentiere VOR dem Schlitten (weiter rechts)
  drawReindeer(x + 18, y + 3, !legsUp);
  drawReindeer(x + 26, y + 2, legsUp);
}
void loop() {
  unsigned long t = millis();
  if (t < waittime) return;
  waittime = t + interval;
  matrix.fillScreen(0);

  // Mond oben links
  matrix.fillCircle(4, 4, 3, white);

  // Stadt
  matrix.fillRect(0, 31, 3, -5, blue);
  matrix.fillRect(3, 31, 3, -10, red);
  matrix.fillRect(6, 31, 3, -8, dark);
  matrix.fillRect(9, 31, 4, -6, green);
  matrix.fillRect(13, 31, 2, -11, blue);
  matrix.fillRect(15, 31, 3, -9, brown);
  matrix.fillRect(18, 31, 5, -4, red);

  // Fenster
  matrix.drawPixel(0, 27, yellow);
  matrix.drawPixel(3, 28, white);
  matrix.drawPixel(2, 29, yellow);
  matrix.drawPixel(3, 23, white);

  // Zufällig von KI
  matrix.drawPixel(5, 27, yellow);
  matrix.drawPixel(18, 27, white);
  matrix.drawPixel(11, 30, yellow);
  matrix.drawPixel(9, 25, white);
  matrix.drawPixel(14, 19, yellow);
  matrix.drawPixel(2, 28, white);
  matrix.drawPixel(20, 29, yellow);
  matrix.drawPixel(7, 31, white);
  matrix.drawPixel(16, 26, white);
  matrix.drawPixel(3, 20, white);
  matrix.drawPixel(12, 29, yellow);
  matrix.drawPixel(19, 28, white);
  matrix.drawPixel(1, 25, yellow);
  matrix.drawPixel(10, 27, white);
  matrix.drawPixel(15, 30,  white);
  matrix.drawPixel(6, 23, white);
  matrix.drawPixel(21, 26, yellow);
  matrix.drawPixel(4, 22, white);
  matrix.drawPixel(13, 28, yellow);
  matrix.drawPixel(8, 31, white);
  matrix.drawPixel(7, 27, white);

  // Bäume
  matrix.drawPixel(26, 31, dark);
  matrix.drawPixel(26, 30, green);
  matrix.drawPixel(25, 30, green);
  matrix.drawPixel(27, 30, green);
  matrix.drawPixel(26, 29, green);
  matrix.drawPixel(29, 31, dark);
  matrix.fillRect(28, 30, 3, -1, green);
  
  step = (step + 1) % 6;
  if (t - windTimer > 3000) {
    windTimer = t;
    wind = random(-1, 2);
  }
  // Schnee
  for (int i = 0; i < nh; i++) {
    fallSnow(sh[i][0], sh[i][1], 3);
    matrix.drawPixel(sh[i][0], sh[i][1], ch);
  }
  for (int i = 0; i < nm; i++) {
    fallSnow(sm[i][0], sm[i][1], 2);
    drawBigFlake(sm[i][0], sm[i][1], cm);
  }
  for (int i = 0; i < nv; i++) {
    fallSnow(sv[i][0], sv[i][1], 1);
    drawBigFlake(sv[i][0], sv[i][1], cv);
  }
  // Santa bewegen
  santaX++;
  santaPhase++;
  int santaY = santaBaseY + sin(santaPhase * 0.15) * 2;
  bool legsUp = (santaPhase % 10) < 5;
  drawSanta(santaX, santaY, legsUp);
  if (santaX > w + 30) {
    santaX = -40;
    santaPhase = 0;
  }
  matrix.swapBuffers(false);
}