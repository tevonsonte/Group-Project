#ifndef config_h
#define config_h

// Force Redraw timer
#define refresh 16.6   // fps timer

// window dimensions
#define title "Breakout Graphic Game by Group 11"
const int winWidth = 800;
const int winHeight = 600;

// wall settings
const int wallWidth = 700;                          // wall width
const int wallHeight = 180;                         // wall height
const float wallX = (winWidth - wallWidth) / 2.0f;  // wall start point x
const float wallY = wallX + 40;                     // wall start point y
const int wallSpace = 3;                            // space between bricks
const int wallColmns = 15;                            // number of wall columns
const int wallRows = 10;                            // number of wall rows

// pedal Settings
const float pedalWidth = 150.0f;
const float pedalHeight = 12.0f;

// ball settings
const float BallRadius = 6.0f;
const int CircleParts = 50; // draws ball and the life symbols

#endif