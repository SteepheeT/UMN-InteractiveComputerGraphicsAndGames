float t = 0.0;

void setup()
{
  size(600, 310);
  smooth();
}

void draw() {
  background(255);
  drawFloor();
  drawRobot();
  t += 0.05;
}

int x=0;

void drawRobot() {
  // Everything we draw is relative to the global (0,0,0)
  pushMatrix(); // Saving the CTM (world) on the stack
    x +=1;
    if(x>600){
      x = 0;
    }
    translate(x,200); // get into the pielvs coordinate system
    drawTorso(); 
    
    pushMatrix(); // Save the CTM (pielvs)
      translate(0, -65 -2); // Go to the neck coordinate system
      drawHead();
    popMatrix(); // Restore CTM to pielvs
    
    pushMatrix(); // Save the CTM (pielvs)
      translate(25+6+2, -62); // Go to left shoulder coordinate system
      rotate(bounce(radians(-90), radians(-120), 3.0, t));
      drawArmPart();
      pushMatrix(); // Save CTM left shoulder
        translate(0, 26+2); // Get into left elbow coordinate system
        rotate(bounce(radians(0), radians(-60), 3.0, t));
        drawArmPart();
        pushMatrix(); // Save CTM left elbow
          translate(0, 26+2);
          rotate(bounce(radians(0), radians(-60), 3.0, t));
          drawHand();
        popMatrix(); // Restore CTM to left elbow
      popMatrix(); // Restore CTM to left shoulder
    popMatrix(); // Restore CTM to pielvs
    
    pushMatrix(); // Save the CTM (pielvs)
      translate(-25-6-2, -62); // Go to left shoulder coordinate system
      rotate(bounce(radians(90), radians(120), 3.0, t));
      drawArmPart();
      pushMatrix(); // Save CTM left shoulder
        translate(0, 26+2); // Get into left elbow coordinate system
        rotate(bounce(radians(0), radians(60), 3.0, t));
        drawArmPart();
        pushMatrix(); // Save CTM left elbow
          translate(0, 26+2);
          rotate(bounce(radians(0), radians(60), 3.0, t));
          drawHand();
        popMatrix(); // Restore CTM to left elbow
      popMatrix(); // Restore CTM to left shoulder
    popMatrix(); // Restore CTM to pielvs
    
  popMatrix(); // Restore CTM to the world;
  
}


// HELPER ROUTINES FOR DRAWING THE ROBOT'S BODY PARTS

// 50x65 rectangle with origin at bottom center (pelvis area)
void drawTorso() {
  noStroke();
  fill(38, 38, 200);
  rect(-25, -65, 50, 65); // body
}

// 38x30 rectangle with origin at bottom center
void drawHead() {
  noStroke();
  fill(38, 38, 200);
  rect(-19, -30, 38, 30);
  fill(222, 222, 249);
  ellipse(-8, -18, 12, 12); // left eye
  ellipse( 8, -18, 12, 12); // right eye
}

// 12x26 rectangle with origin at top center
void drawArmPart() {
  noStroke();
  fill(38, 38, 200);
  rect(-6, 0, 12, 26);  
}

// 12x20 ellipse with origin at the top center
void drawHand() {
  noStroke();
  fill(122, 122, 249);
  ellipse(0, 10, 12, 20);
}

// 16x40 rectangle with origin at top center
void drawLegPart() {
  noStroke();
  fill(38, 38, 200);  
  rect(-8, 0, 16, 40);
}

// 26x12 ellipse with origin at top center
void drawFoot() {
  noStroke();
  fill(122, 122, 249);
  ellipse(0, 6, 26, 12);
}


void drawFloor()
{
  noStroke();
  fill(100, 255, 100);
  rect(0, 300, 600, 10);   
}

// "Bounces" between two angles based on the current time.
// a1 = the first angle
// a2 = the second angle
// secondsPerHalfCycle = if a full cycle starts at a1, goes to a2, and then back to a1 again,
//   this is half of that time, i.e., the time to go from a1 to a2.
// t = the current time
float bounce(float a1, float a2, float secondsPerHalfCycle, float t) {
  float numCyclesAsFloat = t / secondsPerHalfCycle;
  float fractionOfCurrentCycle = numCyclesAsFloat % 1;
  float wholeCycles = numCyclesAsFloat - fractionOfCurrentCycle;  
  float cycleStartAngle = a1;
  float cycleStopAngle = a2;
  if (wholeCycles % 2 == 1) {
    // reverse the start and stop angles every odd cycle
    cycleStartAngle = a2;
    cycleStopAngle = a1;
  }
  // linearly interpolate between the startAngle and stopAngle
  return cycleStartAngle + fractionOfCurrentCycle * (cycleStopAngle - cycleStartAngle);
}
