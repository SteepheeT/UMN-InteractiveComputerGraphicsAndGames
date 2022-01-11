/*
 * CSci-4611 Assignment #1 Text Rain
 */


/* Note: if Processing's video library does not support your particular combination of webcam and
   operating system, then the Sketch may hang in the setup() routine when the list of available
   image capture devices is requestd with "Capture.list()".  If this happens, you can skip all of
   the camera initilization code and just run in movie mode by setting the following global 
   variable to true.
   
   If you having trouble accessing the cameara on a mac laptop due to a security issue.  Some
   students found this post helpful: 
   https://github.com/processing/processing-video/issues/134#issuecomment-664778394
 */
boolean forceMovieMode = false;

// Global vars used to access video frames from either a live camera or a prerecorded movie file
import processing.video.*;
String[] cameraModes;
Capture cameraDevice;
Movie inputMovie;
boolean initialized = false;

// Both modes of input (live camera and movie) will update this same variable with the lastest
// pixel data each frame.  Use this variable to access the new pixel data each frame!
PImage inputImage, lineImage;

//---------------------------------
int movementThreshVal= 128, messageSize = 0;
PFont testf;
Letter[] letters;
String[] messageBox;
String message;
Boolean debugMode = false, lineDisplay = false;

// Called automatically by Processing, once when the program starts up
void setup(){
  size(1280, 720);  
  inputImage = createImage(width, height, RGB);
  parameterSetup();
  readFromFile();
  lettersFallenDown(message);
  

  if (!forceMovieMode) {
    println("Querying avaialble camera modes.");
    cameraModes = Capture.list();
    println("Found " + cameraModes.length + " camera modes.");
    for (int i=0; i<cameraModes.length; i++) {
      println(" " + i + ". " + cameraModes[i]); 
    }
    // if no cameras were detected, then run in offline mode
    if (cameraModes.length == 0) {
      println("Starting movie mode automatically since no cameras were detected.");
      initializeMovieMode(); 
    }
    else {
      println("Press a number key in the Processing window to select the desired camera mode.");
    }
  }
}

// Called automatically by Processing, once per frame
void draw() {
  // start each frame by clearing the screen
  background(0);

  if (!initialized) {
    // IF NOT INITIALIZED, DRAW THE INPUT SELECTION MENU
    drawMenuScreen();      
  }
  else {
    // IF WE REACH THIS POINT, WE'RE PAST THE MENU AND THE INPUT MODE HAS BEEN INITIALIZED
    // GET THE NEXT FRAME OF INPUT DATA FROM LIVE CAMERA OR MOVIE  
    if ((cameraDevice != null) && (cameraDevice.available())) {
      // Get image data from cameara and copy it over to the inputImage variable
      cameraDevice.read();
      inputImage.copy(cameraDevice, 0,0,cameraDevice.width,cameraDevice.height, 0,0,inputImage.width,inputImage.height);
    }
    else if ((inputMovie != null) && (inputMovie.available())) {
      // Get image data from the movie file and copy it over to the inputImage variable
      inputMovie.read();
      inputImage.copy(inputMovie, 0,0,inputMovie.width,inputMovie.height, 0,0,inputImage.width,inputImage.height);
    }
    
    // DRAW THE INPUTIMAGE ACROSS THE ENTIRE SCREEN
    // Note, this is like clearing the screen with an image.  It will cover up anything drawn before this point.
    // So, draw your text rain after this!
    
    pushMatrix();
    scale(-1,1);
    translate(-width,0);
    image(inputImage,0,0);
    popMatrix();
    
    lineDisplay();
    filter(GRAY);
    debugModeDisplay();

    threshDisplay();
    
    
    
    // DRAW THE TEXT RAIN, ETC.
    // TODO: Much of your implementation code should go here. At this point, the latest pixel data from the
    // live camera or movie file will have been copied over to the inputImage variable.  So, if you access
    // the pixel data from the inputImage variable, your code should always work, no matter which mode you run in.
    
    // The following for loop allows a semtence movement from the top of the screen at the same time, but letters are
    // in different order
    for(int i=0; i<letters.length; i++){
      // Display all letters
      letters[i].display();
      // check if the letter is within the screen or ourside      
      if(letters[i].y >= height-4){
        // Now the letter is outside the screen, call reset funciton to reset its location
        letters[i].reset();
      }
      else{
        // if still in, then call movement function, attempting to let it movement down
        letters[i].movement();  
      }
    }
  }
}


// Called automatically by Processing once per frame
void keyPressed() {
  if (!initialized) {
    // CHECK FOR A NUMBER KEY PRESS ON THE MENU SCREEN    
    if ((key >= '0') && (key <= '9')) { 
      int input = key - '0';
      if (input == 0) {
        initializeMovieMode();
      }
      else if ((input >= 1) && (input <= 9)) {
        initializeLiveCameraMode(input);
      }
    }
  }
  else {
    // CHECK FOR KEYPRESSES DURING NORMAL OPERATION
    // TODO: Fill in your code to handle keypresses here..
    if (key == CODED) {
      if (keyCode == UP) {
        // up arrow key pressed
        // pressing up arrow will increase the thresh val by 5 per time, but not over 255.
        if(movementThreshVal > 245 && movementThreshVal <= 255){
          movementThreshVal = 255;
        }
        else if(movementThreshVal <= 245){
          movementThreshVal += 10;
        }
        if(movementThreshVal == 255){
            println("[message] letter movement thresh value has reached max number 255.");
        }
      }
      else if (keyCode == DOWN) {
        // down arrow key pressed
        // pressing down arrow will decrease the thresh val by 5 per time, but not below 0.
        if(movementThreshVal >= 0 && movementThreshVal < 10){
          movementThreshVal = 0;
        }
        else if(movementThreshVal >= 10){
          movementThreshVal -= 10;
        }
        if(movementThreshVal == 0){
            println("[LOG] letter movement thresh value has reached minimum number 0.");
        }
      }
      println("[INSTANT] letter movement thresh value chaged, now is: " + movementThreshVal);
    }
    else if (key == ' ') {
      // spacebar pressed
      if(debugMode == false){
        debugMode = true;
      }
      else{
        debugMode = false;
      }
    }
    else if (key == 'l' || key == 'L'){
        lineDisplay = !lineDisplay;
    }
  }
}

// Loads a movie from a file to simulate camera input.
void initializeMovieMode() {
  String movieFile = "TextRainInput.mov";
  println("Simulating camera input using movie file: " + movieFile);
  inputMovie = new Movie(this, movieFile);
  inputMovie.loop();
  initialized = true;
}

// Starts up a webcam to use for input.
void initializeLiveCameraMode(int cameraMode) {
  println("Activating camera mode #" + cameraMode + ": " + cameraModes[cameraMode-1]);
  cameraDevice = new Capture(this, cameraModes[cameraMode-1]);
  cameraDevice.start();
  initialized = true;
}

// Draws a quick text-based menu to the screen
void drawMenuScreen() {
  int y=10;
  text("Press a number key to select an input mode", 20, y);
  y += 40;
  text("O: Offline mode, test with TextRainInput.mov movie file instead of live camera feed.", 20, y);
  y += 40; 
  for (int i = 0; i < min(9,cameraModes.length); i++) {
    text(i+1 + ": " + cameraModes[i], 20, y);
    y += 40;
  }  
}

void threshDisplay(){
  fill(255);
  text(movementThreshVal, width-80, height-10);
}

void readFromFile(){
  messageBox = loadStrings("test.txt");
  messageSize = messageBox.length;
  int idx = (int)random(0,messageSize);
  message = messageBox[idx];
}

void iniLetters(){
  letters = new Letter[message.length()];
  for(int i=0; i<message.length(); i++){
    letters[i] = new Letter((int)random(100,1000),0,message.charAt(i));
  }
}

void parameterSetup(){
  strokeWeight(1);
  testf = createFont("Arial",16,true);
  fill(0);
  textFont(testf,36);
}

void lineDisplay(){
  if(lineDisplay){
    line(0,100,1280,100);
  }
}

void debugModeDisplay(){
  if(debugMode){
    filter(THRESHOLD, 0.57);
  }
}

void lettersFallenDown(String msg){
  String[] parts = msg.split(" ");
  
  int partsSize = parts.length;
  int totalSize = 0,i=0;
  
  // this function will arrange multiple spaces for different word combinations
  // but possible used only one time , then this space will
  
  for(int idx = 0; idx < partsSize; idx++){
    totalSize = parts[idx].length() + totalSize;
  }
  letters = new Letter[totalSize];
  
  // this for loop will go over envery parts in one sentence
  for(int idx = 0; idx < partsSize; idx++){
    // this will allocate another letter array for one specific parts and its letters
    // after this line use letters[index] to call one specific letter
    for(int j = 0; j < parts[idx].length(); j++, i++){
      letters[i] = new Letter(wordRandomLeftBound(idx) + (i * (int)random(4,8)),0,parts[idx].charAt(j));
    }
  } 
}

int wordRandomLeftBound(int idx){
  
  return idx * ((int)random(0,width/20) + width/10);
  
}

class Letter{
  char letter;
  float x,y;
  float downSpd = random(5,10), upSpd;
  boolean move = true;
  color btmColor1,btmColor2,btmColor3,upColor4,lettColor;
  float gscaleVal1,gscaleVal2,gscaleVal3,gscaleVal4;
  
  Letter(float x_, float y_, char letter_){
    x = x_;
    y = y_;
    letter = letter_;
    lettColor = color(random(0,256),random(0,256),random(0,256));
  }
  
  void display(){
    fill(0);
    textAlign(LEFT);
    fill(lettColor);
    text(letter,x,y);
  }
  // This function controls letter movement by checking (2) pixels below each letter
  // It should also maintains the distance between letters and objects (vertically move up and down) 
  void movement(){
    btmColor1 = get((int)x,(int)(y+1));
    btmColor2 = get((int)x,(int)(y+2));
    btmColor3 = get((int)x,(int)(y+3));
    gscaleVal1 = red(btmColor1);
    gscaleVal2 = red(btmColor2);
    gscaleVal3 = red(btmColor3);
    if(gscaleVal1 > movementThreshVal  && gscaleVal2 > movementThreshVal && gscaleVal3 > movementThreshVal){
      letterFall();
    }
    else{
      moveUpward();
    }
  }

  void reset(){
    y = 0;
    downSpd = random(7,10);
  }

  void letterFall(){
    y += downSpd;
  }
  
  void moveUpward(){
    if(x>0 && y>0){
      btmColor1 = get((int)x,(int)(y+1));
      btmColor2 = get((int)x,(int)(y+2));
      btmColor3 = get((int)x,(int)(y+3));
      upColor4 = get((int)x,(int)(y-2));
      gscaleVal1 = red(btmColor1);
      gscaleVal2 = red(btmColor2);
      gscaleVal3 = red(btmColor3);
      gscaleVal4 = red(upColor4);
      if(gscaleVal1 < movementThreshVal && gscaleVal2 < movementThreshVal){
        upSpd = 10;
      }
      else if(gscaleVal2 < movementThreshVal && gscaleVal3 < movementThreshVal){
        upSpd = 8;
      }
      y -= upSpd;
    }
  }
}
