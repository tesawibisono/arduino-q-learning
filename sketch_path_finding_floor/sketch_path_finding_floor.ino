//*** Q LEARNING - PATH FINDING FLOOR ***//
// This code lights up LED strips based on a simple Q-learning algorithm,
// where the strips represent states, and the goal is to reach a target state.
// The documentation and comments should help you understand the purpose and
// functionality of each part of the code.
//
// *** Original Tutorial: https://alidemir1.github.io/qlearning-post/ 
// *** Modified by Tesalonika Wibisono (info@tesalonika.com)


// Library and Pin Definitions: Include the Adafruit NeoPixel library
// and define the pins for each LED strip.

#include <Adafruit_NeoPixel.h>

// Pin definitions for each LED strip
#define LED_PIN1  2
#define LED_PIN2  3
#define LED_PIN3  4
#define LED_PIN4  5
#define LED_PIN5  6
#define LED_PIN6  7
#define LED_PIN7  8

// Number of LEDs per strip
#define LED_COUNT 4

// Initialize each LED strip
Adafruit_NeoPixel strip1(LED_COUNT, LED_PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_COUNT, LED_PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3(LED_COUNT, LED_PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4(LED_COUNT, LED_PIN4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip5(LED_COUNT, LED_PIN5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip6(LED_COUNT, LED_PIN6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip7(LED_COUNT, LED_PIN7, NEO_GRB + NEO_KHZ800);

// Q-learning Matrices: Define the reward (Q) matrix and the mapping (R) matrix
// for the environment.

// Reward matrix for Q-learning
float Q[7][7] = {
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0}
};

// Mapping matrix for the environment (-1: wall, 0: connection, 100: target)
float R[7][7] = {
  {-1, 0, -1, -1, -1, -1, -1},
  {0, -1, 0, 0, -1, -1, -1},
  {-1, 0, -1, -1, 0, -1, 100},
  {-1, 0, -1, -1, 0, -1, -1},
  {-1, -1, 0, 0, -1, 0, -1},
  {-1, -1, -1, -1, 0, -1, -1},
  {-1, -1, 0, -1, -1, -1, -1},
};

// Possible actions from each state
int M[7][3] = {
  {1}, {0, 2, 3}, {1, 4, 6}, {1, 4}, {2, 3, 5}, {4}, {2}
};

// Array of pointers to LED strips for easy access
Adafruit_NeoPixel* strips[] = {&strip1, &strip2, &strip3, &strip4, &strip5, &strip6, &strip7};

// Variables for Q-learning
int action;
int state;
int i, j, l, m;
int k = 0;
int r;
float gamma = 0.8;
float Qmax;
float a;
float b;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  // Initialize all LED strips
  for (i = 0; i < 7; i++) {
    strips[i]->begin();
    strips[i]->show(); // Initialize all pixels to 'off'
  }
}

// Function to light up a specific LED strip with a color for a duration
void lightUp(int stripIndex, uint32_t color, int duration) {
  strips[stripIndex]->setPixelColor(0, color);
  strips[stripIndex]->show();
  delay(duration);
  strips[stripIndex]->setPixelColor(0, 0); // Turn off
  strips[stripIndex]->show();
  delay(100);
}

void loop() {
  // Training phase: repeat for 10 episodes
  while (k < 10) {
    state = 0; // Start from the initial state
    Serial.println(state);
    lightUp(state, strip1.Color(0, 255, 255), 300); // Light up the initial state

    // Continue until reaching the target state (state 6)
    while (state != 6) {
      // Select an action based on the current state
      if (state == 0) {
        action = 1;
      }
      else if (state == 1) {
        r = rand() % 3;
        action = M[1][r];
      }
      else if (state == 2) {
        r = rand() % 3;
        action = M[2][r];
      }
      else if (state == 3) {
        r = rand() % 2;
        action = M[3][r];
      }
      else if (state == 4) {
        r = rand() % 3;
        action = M[4][r];
      }
      else if (state == 5) {
        action = 4;
      }
      else if (state == 6) {
        action = 2;
      }
      
      // Find the maximum Q value for the next state
      a = -10;
      for (i = 0; i < 7; i++) {
        if (a < Q[action][i]) {
          a = Q[action][i];
        }
      }
      Qmax = a * gamma; // Discounted future reward
      Q[state][action] = R[state][action] + Qmax; // Update Q value
      state = action; // Move to the next state

      Serial.println(state);
      lightUp(state, strip1.Color(0, 255, 255), 300); // Light up the new state
    }
    k++; // Increment episode counter
  }

  // Exploitation phase: follow the learned policy
  while (1) {
    state = 0; // Start from the initial state
    Serial.println(state);
    lightUp(state, strip1.Color(0, 255, 255), 300); // Light up the initial state

    b = -10;
    while (state != 6) {
      // Find the action with the highest Q value for the current state
      for (i = 0; i < 7; i++) {
        if (b <= Q[state][i]) {
          b = Q[state][i];
          l = i;
        }
      }
      
      state = l; // Move to the next state
      Serial.println(state);
      lightUp(state, strip1.Color(0, 255, 255), 300); // Light up the new state
    }
  }
}
