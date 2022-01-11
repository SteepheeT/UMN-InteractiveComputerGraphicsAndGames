/** CSci-4611 Assignment 2:  Car Soccer
 */

#include "car_soccer.h"
#include "config.h"


// Remember in C++, the .h file list all the functions and member variables that are part of the class!
// Look there first to understand what is part of the CarSoccer class, then look below to see how each
// function is implemented.
bool test = false;

CarSoccer::CarSoccer() : GraphicsApp(1024,768, "Car Soccer") {
    // If you are having trouble driving the car with the keybaord, you can set this to true to use
    // the mouse instead.  The mouse controls are based on the postion of the mouse cursor on the window.
    // There is a "dead zone" in the middle of the window, and if you move the mouse up/down or left/right
    // outside of that zone, it is like pushing the up/down and/or left/right keys on the keyboard
    use_mouse_ = false;
        
    // Define a search path for finding data files (images and shaders)
    searchPath_.push_back(".");
    searchPath_.push_back("./data");
    searchPath_.push_back(DATA_DIR_INSTALL);
    searchPath_.push_back(DATA_DIR_BUILD);
}

CarSoccer::~CarSoccer() {
}


void CarSoccer::OnMouseMove(const Point2& pos, const Vector2& delta)
{
    mouse_pos_ = PixelsToNormalizedDeviceCoords(pos);
}

void CarSoccer::OnSpecialKeyDown(int key, int scancode, int modifiers) {
    if (key == GLFW_KEY_SPACE) {
        // Here's where you could call some form of launch_ball();
        ball_.Reset();
    }
}

/// This is a little utility function that is helpful.  It treats the arrow keys like a joystick or D-pad on a game controller
/// and returns the direction you are pressing as a 2D vector, taking into account the fact that you might be holding
/// down more than one key at a time.
Vector2 CarSoccer::joystick_direction() {
    Vector2 dir;
    // car_.set_left(false);
    // car_.set_right(false);

    if (use_mouse_) {
        // threshold defines the size of the "dead zone" in the middle of the screen
        // if the mouse's x,y position falls outside of this, then it is like pushing
        // the corresponding key on the keyboard
        const float threshold = 0.2f;
        dir[0] = 0;
        if (mouse_pos_[0] < -threshold) {
            dir[0] = -1;
        }
        else if (mouse_pos_[0] > threshold) {
            dir[0] = 1;
        }
        dir[1] = 0;
        if (mouse_pos_[1] < -threshold) {
            dir[1] = -1;
        }
        else if (mouse_pos_[1] > threshold) {
            dir[1] = 1;
        }
    }
    else {
        // the default user interface is to use the arrow keys on the keyboard.
        // like a D-pad on a game controller, you can hold more than one key down at a time if you want.
        Matrix4 R_Y;
        Vector3 rotateV, originV;
        
    }
    return dir;
}

// dt is for "Delta Time", the elapsed time in seconds since the last frame
void CarSoccer::UpdateSimulation(double dt) {
    Vector2 dpad_dir = joystick_direction();
    // std::cout << "D-Pad Direction: " << dpad_dir << std::endl;
    

    // Here's where you shound do your "simulation", updating the positions of the
    // car and ball based on the elapsed time and checking for collisions.  Filling
    // in this routine is the main part of the assignment.+


    
    if (!test) {
        ball_.set_velocity(Vector3(4, 10, 5));
        test = !test;
    }
    
    if (ball_.velocity().x() < 0.01f) {
        ball_.velocity()[0] = 0;
    }
    if (ball_.velocity().y() < 0.1f) {
        ball_.velocity()[1] = 0;
    }
    if (ball_.velocity().z() < 0.01f) {
        ball_.velocity()[2] = 0;
    }

    // -------------------- Ball Bounce & Movement start --------------------

    //*********************************
    // The code only works at the given field size which is 100*80, half size 50/40
    // Unable to dynamically follow size of the gound
    //*********************************

    // This block below will detect if the ball hits the left/right wall and set it bounce back
    if (40 - abs(ball_.position().x()) <= ball_.radius()) {
        Vector3 reboundVector, normalVector;
        Point3 pBall, pContact;
        // if the ball hits right bound
        if (ball_.position().x() >= 0) {
            ball_.set_position(Point3(40 - ball_.radius(), ball_.position().y(), ball_.position().z()));
            pBall = ball_.position();
            pContact = Point3(ball_.position().x()+ball_.radius(), ball_.position().y(), ball_.position().z());
            normalVector = pBall - pContact;
            normalVector.Normalize();
        }
        // if the ball hits left bound
        else {
            ball_.set_position(Point3(-40 + ball_.radius(), ball_.position().y(), ball_.position().z()));
            pBall = ball_.position();
            pContact = Point3(ball_.position().x() - ball_.radius(), ball_.position().y(), ball_.position().z());
            normalVector = pBall - pContact;
            normalVector.Normalize();
        }
        reboundVector = ball_.velocity() - 2 * (ball_.velocity().Dot(normalVector)) * normalVector;
        ball_.set_velocity(0.8 * reboundVector);
    }

    // This block below will detect if the ball hits the front/back wall and set it bounce back
    if (50 - abs(ball_.position().z()) <= ball_.radius()) {
        Vector3 reboundVector, normalVector;
        Point3 pBall, pContact;

        // check if the ball front bound
        if (ball_.position().z() >= 0) {
            if ((ball_.position().x() >= -10 && ball_.position().x() <= 10) && ball_.position().y() <= 10) {
                
                ball_.Reset();
                car_.Reset();
            }
            else {
                ball_.set_position(Point3(ball_.position().x(), ball_.position().y(), 50 - ball_.radius()));

                pBall = ball_.position();
                pContact = Point3(ball_.position().x(), ball_.position().y(), ball_.position().z() + ball_.radius());
                normalVector = pBall - pContact;
                normalVector.Normalize();
                reboundVector = ball_.velocity() - 2 * (ball_.velocity().Dot(normalVector)) * normalVector;
                ball_.set_velocity(0.8 * reboundVector);
            }
        }
        // the ball hit the back bound
        else {
            if ((ball_.position().x() >= -10 && ball_.position().x() <= 10) && ball_.position().y() <= 10) {
                
                ball_.Reset();
                car_.Reset();
            }
            else {
                ball_.set_position(Point3(ball_.position().x(), ball_.position().y(), -50 + ball_.radius()));

                pBall = ball_.position();
                pContact = Point3(ball_.position().x(), ball_.position().y(), ball_.position().z() - ball_.radius());
                normalVector = pBall - pContact;
                normalVector.Normalize();
                reboundVector = ball_.velocity() - 2 * (ball_.velocity().Dot(normalVector)) * normalVector;
                ball_.set_velocity(0.8 * reboundVector);
            }
        }
        
    }

    // This detect if the ball hits the celling
    if (35 - abs(ball_.position().y()) <= ball_.radius()) {
        Vector3 reboundVector, normalVector;
        Point3 pBall, pContact;

        ball_.set_position(Point3(ball_.position().x(), 35 - ball_.radius(), ball_.position().z()));

        pBall = ball_.position();
        pContact = Point3(ball_.position().x(), ball_.position().y() + ball_.radius(), ball_.position().z());
        normalVector = pBall - pContact;
        normalVector.Normalize();
        reboundVector = ball_.velocity() - 2 * (ball_.velocity().Dot(normalVector)) * normalVector;

        ball_.set_velocity(reboundVector);
    }

    // This detect if the ball hits the ground
    if (ball_.position().y() - 0 <= ball_.radius()) {
        Vector3 reboundVector, normalVector;
        Point3 pBall, pContact;

        ball_.set_position(Point3(ball_.position().x(), ball_.radius(), ball_.position().z()));

        pBall = ball_.position();
        pContact = Point3(ball_.position().x(), 0, ball_.position().z());
        normalVector = pBall - pContact;
        normalVector.Normalize();
        reboundVector = ball_.velocity() - 2 * (ball_.velocity().Dot(normalVector)) * normalVector;
        
        ball_.set_velocity(0.85*reboundVector);
    }

    // If ball leaves the ground, then apply gravity to it
    if (ball_.position().y() > 0) {
        Vector3 gravitySpdAdjust;
        gravitySpdAdjust = ball_.velocity() + Vector3(0, -20.0f, 0) * dt;
        ball_.set_velocity(gravitySpdAdjust);
 
    }
    
    // -------------------- Ball Bounce & Movement end --------------------

    // -------------------- Car Movement Restriction start --------------------
    
    // This will prevent car exceed left/right bound
    if (40 - abs(car_.position().x()) <= (car_.size().x() / 2.0f)) {
        if (car_.position().x() > 0) {
            car_.set_position(Point3(40 - car_.size().x() / 2.0f, car_.position().y(), car_.position().z()));
        }
        else {
            car_.set_position(Point3(-40 + car_.size().x() / 2.0f, car_.position().y(), car_.position().z()));
        }
    }

    // This will prevent car exceed front/back bound
    if (50 - abs(car_.position().z()) <= (car_.size().z() / 2.0f)) {
        if (car_.position().z() > 0) {
            car_.set_position(Point3(car_.position().x(), car_.position().y(), 50 - car_.size().z() / 2.0f));
        }
        else {
            car_.set_position(Point3(car_.position().x(), car_.position().y(), -50 + car_.size().z() / 2.0f));
        }
    }

    // -------------------- Car Movement Restriction end --------------------
    
    // -------------------- Car-Ball Collosion Detection start --------------------   |||||||| Needs adjust later

    // This allows car correctly kick the ball away
    if ((ball_.position() - car_.position()).Length() <= ball_.radius() + car_.collision_radius()) {
        Vector3 car2ball_Normal_Vector;
        Vector3 reboundSpd;
        Vector3 relativeSpd;
        car2ball_Normal_Vector = ball_.position() - car_.position();
        car2ball_Normal_Vector.Normalize();
        ball_.set_position(car_.position() + car2ball_Normal_Vector * (ball_.radius() + car_.collision_radius()));

        relativeSpd = (ball_.velocity() - car_.velocity());

        reboundSpd = relativeSpd - 2 * (relativeSpd.Dot(car2ball_Normal_Vector)) * car2ball_Normal_Vector;
        
        ball_.set_velocity(reboundSpd);
        
    }

    // -------------------- Car-Ball Collosion Detection start --------------------

    // Update position of the ball based on adjusted velocity
    ball_.set_position(ball_.position() + ball_.velocity() * 5.0f * dt);

    // Example: This is not the "correct way" to drive the car, but this code
    // will at least move the car around for testing
    // float metersPerSec = 10.0f;
    

    // The car will slowly stop if we don't hold up or down arrow
    // *******************
    // If suddenly switching direction, the car will keep the velocity and instantly move 
    // *******************
    float carFriction = 8.0f;
    if (car_.speed() < 1 && car_.speed() > -1) {
        car_.set_speed(0);
    }
    else if (car_.speed() > 1) {
        car_.set_speed(car_.speed() - carFriction * dt);
    }
    else if(car_.speed() < -1){
        car_.set_speed(car_.speed() + carFriction * dt);
    }
    
    Matrix4 R_Y;
    Vector3 rotateV, carV;
    
    if (IsKeyDown(GLFW_KEY_UP)) {
        
        if (car_.speed() <= 5 && (!IsKeyDown(GLFW_KEY_LEFT || !IsKeyDown(GLFW_KEY_RIGHT)))) {
            if (car_.speed() < 0) {
                // do nothing and let fraction slow the car down
            }
            else {
                car_.set_speed(car_.speed() + 1.0f);
            }
        }
        if (IsKeyDown(GLFW_KEY_LEFT)) {
            
            R_Y = Matrix4::RotationY(GfxMath::ToRadians(car_.speed()));
            rotateV = R_Y * car_.forward();
            rotateV.Normalize();
            carV = car_.forward();
            carV.Normalize();
            car_.set_forward(carV + rotateV);
        }
        else if (IsKeyDown(GLFW_KEY_RIGHT)) {

            R_Y = Matrix4::RotationY(GfxMath::ToRadians(-car_.speed()));
            rotateV = R_Y * car_.forward();
            rotateV.Normalize();
            carV = car_.forward();
            carV.Normalize();
            car_.set_forward(carV + rotateV);
        }
    }

    if (IsKeyDown(GLFW_KEY_DOWN) ) {
        if (car_.speed() >= -5 && (!IsKeyDown(GLFW_KEY_LEFT || !IsKeyDown(GLFW_KEY_RIGHT)))) {
            if (car_.speed() > 0) {
                // do nothing and let fraction slow the car down
            }
            else{
                car_.set_speed(car_.speed() - 1.0f);
            }
        }
        // car_.set_forward(Vector3(0, 0, 1));
        if (IsKeyDown(GLFW_KEY_LEFT)) {
            //std::cout << "inside per frame left" << std::endl;
            R_Y = Matrix4::RotationY(GfxMath::ToRadians(car_.speed()));
            rotateV = R_Y * car_.forward();
            rotateV.Normalize();
            carV = car_.forward();
            carV.Normalize();
            car_.set_forward(carV + rotateV);
        }
        else if (IsKeyDown(GLFW_KEY_RIGHT)) {
            //std::cout << "inside per frame right" << std::endl;
            R_Y = Matrix4::RotationY(GfxMath::ToRadians(-car_.speed()));
            rotateV = R_Y * car_.forward();
            rotateV.Normalize();
            carV = car_.forward();
            carV.Normalize();
            car_.set_forward(carV + rotateV);
        }
    }

    car_.set_position(car_.position() + car_.speed() * car_.forward() * 3 * dt);
}

void CarSoccer::InitOpenGL() {
    // Set up the camera in a good position to see the entire field
    projMatrix_ = Matrix4::Perspective(60, aspect_ratio(), 1, 1000);
    modelMatrix_ = Matrix4::LookAt(Point3(0,60,70), Point3(0,0,10), Vector3(0,1,0));
 
    // Set a background color for the screen (don't worry if you get a depricated warning on this line in OSX)
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    
    // Load some image files we'll use
    fieldTex_.InitFromFile(Platform::FindFile("pitch.png", searchPath_));
    crowdTex_.InitFromFile(Platform::FindFile("crowd.png", searchPath_));
}


void CarSoccer::DrawUsingOpenGL() {
    // Draw the crowd as a fullscreen background image
    quickShapes_.DrawFullscreenTexture(Color(1,1,1), crowdTex_);

    // Draw the car and the ball
    car_.Draw(quickShapes_, modelMatrix_, viewMatrix_, projMatrix_);
    ball_.Draw(quickShapes_, modelMatrix_, viewMatrix_, projMatrix_);
    
    // Draw the field with the field texture on it.
    Color col(16.0f/255.0f, 46.0f/255.0f, 9.0f/255.0f);
    Matrix4 M = Matrix4::Translation(Vector3(0.0f, -0.201f, 0.0f)) * Matrix4::Scale(Vector3(50.0f, 1.0f, 60.0f));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, col);
    M = Matrix4::Translation(Vector3(0.0f, -0.2f, 0.0f)) * Matrix4::Scale(Vector3(40.0f, 1.0f, 50.0f));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, Color(1,1,1), fieldTex_);
    
    // You should add drawing the goals and the boundary of the playing area
    // using quickShapes_.DrawLines()

    Color boundCol = Color(1, 1, 1);

    Matrix4 BackBound;
    std::vector<Point3> backBound;
    backBound.push_back(Point3(-40, 35, -50));
    backBound.push_back(Point3(40, 35, -50));
    backBound.push_back(Point3(40, 0, -50));
    backBound.push_back(Point3(-40, 0, -50));
    quickShapes_.DrawLines(modelMatrix_ , viewMatrix_, projMatrix_, boundCol, backBound, QuickShapes::LinesType::LINE_LOOP, 0.1);

    Matrix4 LeftBound;
    std::vector<Point3> leftBound;
    leftBound.push_back(Point3(-40, 0, -50));
    leftBound.push_back(Point3(-40, 0, 50));
    leftBound.push_back(Point3(-40, 35, 50));
    leftBound.push_back(Point3(-40, 35, -50));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, boundCol, leftBound, QuickShapes::LinesType::LINE_LOOP, 0.1);

    Matrix4 RightBound;
    std::vector<Point3> rightBound;
    rightBound.push_back(Point3(40, 0, -50));
    rightBound.push_back(Point3(40, 0, 50));
    rightBound.push_back(Point3(40, 35, 50));
    rightBound.push_back(Point3(40, 35, -50));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, boundCol, rightBound, QuickShapes::LinesType::LINE_LOOP, 0.1);

    Matrix4 FrontBound;
    std::vector<Point3> frontBound;
    frontBound.push_back(Point3(-40, 35, 50));
    frontBound.push_back(Point3(40, 35, 50));
    frontBound.push_back(Point3(40, 0, 50));
    frontBound.push_back(Point3(-40, 0, 50));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, boundCol, frontBound, QuickShapes::LinesType::LINE_LOOP, 0.1);

    Color blueGoalCol = Color(0, 1, 1);
    std::vector<Point3> blueGoalBound;
    blueGoalBound.push_back(Point3(10, 0, -50));
    blueGoalBound.push_back(Point3(10, 10, -50));
    blueGoalBound.push_back(Point3(-10, 10, -50));
    blueGoalBound.push_back(Point3(-10, 0, -50));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, blueGoalCol, blueGoalBound, QuickShapes::LinesType::LINE_LOOP, 0.1);

    Color redGoalCol = Color(1, 0.3, 0);
    std::vector<Point3> redGoalBound;
    redGoalBound.push_back(Point3(10, 0, 50));
    redGoalBound.push_back(Point3(10, 10, 50));
    redGoalBound.push_back(Point3(-10, 10, 50));
    redGoalBound.push_back(Point3(-10, 0, 50));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, redGoalCol, redGoalBound, QuickShapes::LinesType::LINE_LOOP, 0.1);

    std::vector<Point3> blueGoalNet;
    for (int row = 9; row >= 1; row--) {
        blueGoalNet.push_back(Point3(10, row, -50));
        blueGoalNet.push_back(Point3(-10, row, -50));
    }
    for (int col = 8; col >= -8; col-=2) {
        blueGoalNet.push_back(Point3(col, 10, -50));
        blueGoalNet.push_back(Point3(col, -10, -50));
    }
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, blueGoalCol, blueGoalNet, QuickShapes::LinesType::LINES, 0.1);

    std::vector<Point3> redGoalNet;
    for (int row = 9; row >= 1; row--) {
        redGoalNet.push_back(Point3(10, row, 50));
        redGoalNet.push_back(Point3(-10, row, 50));
    }
    for (int col = 8; col >= -8; col -= 2) {
        redGoalNet.push_back(Point3(col, 10, 50));
        redGoalNet.push_back(Point3(col, -10, 50));
    }
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, redGoalCol, redGoalNet, QuickShapes::LinesType::LINES, 0.1);
}