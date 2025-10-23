#include "Custom.h"
#define HEIGHT 36  // Vertical dimension (rows)
#define WIDTH 36   // Horizontal dimension (columns)
#define LETTER_W 8
#define LETTER_H 16
#define STROKE 2

byte matrix[HEIGHT][WIDTH];  // [row][column]

String arrText[MAX_TEXT_LINES] = {"HI THERE"}; // Array to hold text lines

// Helper: horizontal line - draws across columns at given rows
void lineH(int r1, int r2, int c1, int c2) {
  for (int r = r1; r < r2 && r < HEIGHT; r++)
    for (int c = c1; c < c2 && c < WIDTH; c++)
      matrix[r][c] = 1;
}

// Helper: vertical line - draws down rows at given columns
void lineV(int r1, int r2, int c1, int c2) {
  for (int r = r1; r < r2 && r < HEIGHT; r++)
    for (int c = c1; c < c2 && c < WIDTH; c++)
      matrix[r][c] = 1;
}

void drawLetter(char ch, int offset) {
  ch = toupper(ch);
  int s = STROKE;
  int w = LETTER_W;
  int h = LETTER_H;
  int mid = h / 2;

  // Ensure offset doesn't exceed display width
  if (offset >= WIDTH) return;
  int maxW = min(offset + w, WIDTH);
  int availableW = maxW - offset;

  switch (ch) {
    case 'A':
      // Left vertical stroke
      lineV(s, h, offset, offset + s);
      // Right vertical stroke
      lineV(s, h, maxW - s, maxW);
      // Top horizontal
      lineH(s, s + s, offset, maxW);
      // Middle horizontal
      lineH(mid - s, mid, offset, maxW);
      break;

    case 'B':
      // Left vertical
      lineV(0, h, offset, offset + s);
      // Top horizontal
      lineH(0, s, offset, maxW - s);
      // Middle horizontal
      lineH(mid - s, mid, offset, maxW - s);
      // Bottom horizontal
      lineH(h - s, h, offset, maxW - s);
      // Top right vertical
      lineV(0, mid, maxW - s, maxW);
      // Bottom right vertical
      lineV(mid, h, maxW - s, maxW);
      break;

    case 'C':
      // Top horizontal
      lineH(0, s, offset, maxW);
      // Bottom horizontal
      lineH(h - s, h, offset, maxW);
      // Left vertical
      lineV(0, h, offset, offset + s);
      break;

    case 'D':
      // Left vertical
      lineV(0, h, offset, offset + s);
      // Top horizontal
      lineH(0, s, offset, maxW - s);
      // Bottom horizontal
      lineH(h - s, h, offset, maxW - s);
      // Right vertical
      lineV(0, h, maxW - s, maxW);
      break;

    case 'E':
      // Left vertical
      lineV(0, h, offset, offset + s);
      // Top horizontal
      lineH(0, s, offset, maxW);
      // Middle horizontal
      lineH(mid - s, mid, offset, maxW - s);
      // Bottom horizontal
      lineH(h - s, h, offset, maxW);
      break;

    case 'F':
      // Left vertical
      lineV(0, h, offset, offset + s);
      // Top horizontal
      lineH(0, s, offset, maxW);
      // Middle horizontal
      lineH(mid - s, mid, offset, maxW - s);
      break;

    case 'G':
      // Left vertical
      lineV(0, h, offset, offset + s);
      // Top horizontal
      lineH(0, s, offset, maxW);
      // Bottom horizontal
      lineH(h - s, h, offset, maxW);
      // Right vertical (bottom half)
      lineV(mid, h, maxW - s, maxW);
      // Middle horizontal (right half)
      lineH(mid - s, mid, offset + availableW / 2, maxW);
      break;

    case 'H':
      // Left vertical
      lineV(0, h, offset, offset + s);
      // Right vertical
      lineV(0, h, maxW - s, maxW);
      // Middle horizontal
      lineH(mid - s, mid, offset, maxW);
      break;

    case 'I':
      // Top horizontal
      lineH(0, s, offset, maxW);
      // Bottom horizontal
      lineH(h - s, h, offset, maxW);
      // Center vertical
      lineV(0, h, offset + availableW / 2 - s / 2, offset + availableW / 2 + s / 2);
      break;

    case 'J':
      // Top horizontal
      lineH(0, s, offset, maxW);
      // Right vertical
      lineV(0, h - s, offset + availableW / 2 - s / 2, offset + availableW / 2 + s / 2);
      // Bottom curve
      lineH(h - s, h, offset, offset + availableW / 2);
      break;

    case 'K':
      // Left vertical
      lineV(0, h, offset, offset + s);
      // Diagonal strokes
      for (int i = 0; i < h / 2; i += 2) {
        int diag_c = offset + s + i / 2;
        if (diag_c < maxW)
          lineH(i, i + s, diag_c, min(diag_c + s, maxW));
      }
      for (int i = h / 2; i < h; i += 2) {
        int diag_c = offset + s + (h - i) / 2;
        if (diag_c < maxW)
          lineH(i, min(i + s, h), diag_c, min(diag_c + s, maxW));
      }
      break;

    case 'L':
      // Left vertical
      lineV(0, h, offset, offset + s);
      // Bottom horizontal
      lineH(h - s, h, offset, maxW);
      break;

    case 'M':
      // Left vertical
      lineV(0, h, offset, offset + s);
      // Right vertical
      lineV(0, h, maxW - s, maxW);
      // Left diagonal (top half)
      for (int i = 0; i < h / 3; i += 2) {
        int diag_c = offset + s + (i * availableW) / (2 * h);
        if (diag_c < offset + availableW / 2)
          lineH(i, i + s, diag_c, min(diag_c + s, WIDTH));
      }
      // Right diagonal (top half)
      for (int i = 0; i < h / 3; i += 2) {
        int diag_c = maxW - s - (i * availableW) / (2 * h);
        if (diag_c >= offset + availableW / 2)
          lineH(i, i + s, diag_c, min(diag_c + s, WIDTH));
      }
      break;

    case 'N':
      // Left vertical
      lineV(0, h, offset, offset + s);
      // Right vertical
      lineV(0, h, maxW - s, maxW);
      // Diagonal
      for (int i = 0; i < h; i += 2) {
        int diag_c = offset + s + (i * (availableW - s)) / h;
        if (diag_c < maxW - s)
          lineH(i, min(i + s, h), diag_c, min(diag_c + s, maxW - s));
      }
      break;

    case 'O':
      // Top horizontal
      lineH(0, s, offset + s, maxW - s);
      // Bottom horizontal
      lineH(h - s, h, offset + s, maxW - s);
      // Left vertical
      lineV(s, h - s, offset, offset + s);
      // Right vertical
      lineV(s, h - s, maxW - s, maxW);
      break;

    case 'P':
      // Left vertical
      lineV(0, h, offset, offset + s);
      // Top horizontal
      lineH(0, s, offset, maxW);
      // Middle horizontal
      lineH(mid - s, mid, offset, maxW - s);
      // Right vertical (top half)
      lineV(0, mid, maxW - s, maxW);
      break;

    case 'Q':
      drawLetter('O', offset);
      // Tail diagonal
      for (int i = h - s * 6; i < h - s * 2; i += 2) {
        int diag_c = offset + availableW / 2 + (i - (h - s * 6)) / 2;
        if (diag_c < maxW)
          lineH(i, min(i + s, h), diag_c, min(diag_c + s, maxW));
      }
      break;

    case 'R':
      drawLetter('P', offset);
      // Right leg diagonal
      for (int i = mid; i < h; i += 2) {
        int diag_c = offset + s + ((i - mid) * (availableW - 2 * s)) / (h - mid);
        if (diag_c < maxW)
          lineH(i, min(i + s, h), diag_c, min(diag_c + s, maxW));
      }
      break;

    case 'S':
      // Top horizontal
      lineH(0, s, offset, maxW);
      // Middle horizontal
      lineH(mid - s, mid, offset, maxW);
      // Bottom horizontal
      lineH(h - s, h, offset, maxW);
      // Top left vertical
      lineV(0, mid, offset, offset + s);
      // Bottom right vertical
      lineV(mid, h, maxW - s, maxW);
      break;

    case 'T':
      // Top horizontal
      lineH(0, s, offset, maxW);
      // Center vertical
      lineV(0, h, offset + availableW / 2 - s / 2, offset + availableW / 2 + s / 2);
      break;

    case 'U':
      // Left vertical
      lineV(0, h - s, offset, offset + s);
      // Right vertical
      lineV(0, h - s, maxW - s, maxW);
      // Bottom horizontal
      lineH(h - s, h, offset, maxW);
      break;

    case 'V':
      // Left diagonal
      for (int i = 0; i < h - s; i += 2) {
        int left_c = offset + (i * availableW) / (2 * h);
        if (left_c < WIDTH)
          lineH(i, min(i + s, h), left_c, min(left_c + s, WIDTH));
      }
      // Right diagonal
      for (int i = 0; i < h - s; i += 2) {
        int right_c = maxW - s - (i * availableW) / (2 * h);
        if (right_c < WIDTH)
          lineH(i, min(i + s, h), right_c, min(right_c + s, WIDTH));
      }
      break;

    case 'W':
      // Left vertical
      lineV(0, h, offset, offset + s);
      // Right vertical
      lineV(0, h, maxW - s, maxW);
      // Left middle diagonal (bottom half)
      for (int i = h / 2; i < h; i += 2) {
        int diag_c = offset + s + ((i - h / 2) * availableW) / (3 * h);
        if (diag_c < offset + availableW / 2)
          lineH(i, min(i + s, h), diag_c, min(diag_c + s, WIDTH));
      }
      // Right middle diagonal (bottom half)
      for (int i = h / 2; i < h; i += 2) {
        int diag_c = maxW - s - ((i - h / 2) * availableW) / (3 * h);
        if (diag_c >= offset + availableW / 2)
          lineH(i, min(i + s, h), diag_c, min(diag_c + s, WIDTH));
      }
      break;

    case 'X':
      // Diagonal top-left to bottom-right
      for (int i = 0; i < h; i += 2) {
        int c = offset + (i * availableW) / h;
        if (c < maxW)
          lineH(i, min(i + s, h), c, min(c + s, maxW));
      }
      // Diagonal top-right to bottom-left
      for (int i = 0; i < h; i += 2) {
        int c = maxW - s - (i * availableW) / h;
        if (c >= offset && c < maxW)
          lineH(i, min(i + s, h), c, min(c + s, maxW));
      }
      break;

    case 'Y':
      // Top left diagonal
      for (int i = 0; i < h / 2; i += 2) {
        int left_c = offset + (i * availableW) / h;
        if (left_c < WIDTH)
          lineH(i, min(i + s, h / 2), left_c, min(left_c + s, WIDTH));
      }
      // Top right diagonal
      for (int i = 0; i < h / 2; i += 2) {
        int right_c = maxW - s - (i * availableW) / h;
        if (right_c < WIDTH)
          lineH(i, min(i + s, h / 2), right_c, min(right_c + s, WIDTH));
      }
      // Bottom vertical
      lineV(h / 2, h, offset + availableW / 2 - s / 2, offset + availableW / 2 + s / 2);
      break;

    case 'Z':
      // Top horizontal
      lineH(0, s, offset, maxW);
      // Bottom horizontal
      lineH(h - s, h, offset, maxW);
      // Diagonal
      for (int i = 0; i < h; i += 2) {
        int c = offset + (i * availableW) / h;
        if (c < maxW)
          lineH(i, min(i + s, h), c, min(c + s, maxW));
      }
      break;
  }
}

void textToMatrix(const char *text) {
  // Clear matrix
  for (int r = 0; r < HEIGHT; r++)
    for (int c = 0; c < WIDTH; c++)
      matrix[r][c] = 0;

  int len = strlen(text);
  for (int i = 0; i < len && i < 3; i++) {
    drawLetter(text[i], i * LETTER_W);
  }
}

void printMatrix() {
  // Print row by row
  // for (int r = 0; r < HEIGHT; r++) {
  //   for (int c = 0; c < WIDTH; c++) {
  //     Serial.print(matrix[r][c] ? "#" : " ");
  //   }
  //   Serial.println();
  // }
  for (int col = 0; col < 36; col++)
    {
      for (int row = 0; row < NUMPIXELS; row++)
      {
        if (matrix[col][row] == 1)
        {
          strip.setPixelColor(row, 0x0F0000); // Red ON
        }
        else
        {
          strip.setPixelColor(row, 0x000000); // OFF
        }
      }
      strip.show();            // Display strip
      delayMicroseconds(1326); // wait for the next column in microseconds
    }
}