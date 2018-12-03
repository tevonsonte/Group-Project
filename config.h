#ifndef config_h
#define config_h

// redraw timer
#define refresh 16.6

// window dimensions
//set window title
#define title "Breakout Graphic Game by Group 11"
const int winWidth = 800;
const int winHeight = 600;

// wall settings
const int wallWidth = 700;                          // wall width
const int wallHeight = 180;                         // wall height
const float wallX = (winWidth - wallWidth) / 2.0f;  // wall x starting coord
const float wallY = wallX + 40;                     // wall y starting coord
const int wallSpace = 3;                            // space between bricks
const int wallColmns = 15;                          // number of wall columns
const int wallRows = 10;                           // number of wall rows

// pedal Settings
const float pedalWidth = 150.0f;
const float pedalHeight = 12.0f;

// ball settings
const float BallRadius = 6.0f;
const int CircleParts = 50; // variable used for both heart and ball graphics

#endif