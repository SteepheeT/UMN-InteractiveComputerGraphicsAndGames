# My solution to Assignment 2
This is a discription file for assignment 2 - CarSoccer, created by Junyue Tao.

In this assignment, you will be able to control a small car represented by the small red
cuboid on the playground. Please forgive me that do not add any decoration on it to help
you distinguish the front and back side of the car, but I'm pretty sure that's not a big
problem here.

### Control the vehicle
You can directly control the car using arrow key on your keyboard. Pushing uparrow will
let the car increase its speed toward the current direction. For ease of handling, cars 
do not have very high speeds. Using left/right arrows to turn your car. This is exactly
like dirving a real car using a third-person view. HOWEVER, BE CAREFUL WITH TURNING. 
Just like real car, if you suddenly choose to go backwards while moving forward. Sending
this insturction to your car will not instantly allows you to go back, instrad, your car
will still move forward for a little bit before satisfiys your wish. Same thing happends
with moving back then suddenly switch to froward.

The steering radius of the car is also related to the speed. The faster the speed, the
greater the radius. For ease of play, I didn't set the maximum speed to very high,
otherwise I would immediately hit the wall. Oh, I don't want to see a terrible car
accident.

### Jumping Balls
Balls fly around the field and bounce off invisible walls or cars. To simulate friction 
and power loss, each collision will speed the ball 0.85 times faster than before. But I 
love collisions between balls and cars, so I love them, and collisions between balls and 
cars don't slow things down. :)

The ball is also affected by gravity while in the air, so if it doesn't hit the invisible
ceiling, it will bounce lower and lower like a ball in the world. 

In addition, before the modification, if the vehicle and the ball were very close to the
wall, there was a chance that the ball would get stuck in the car and shake up and down
after a slight collision. This is also mentioned in the handout of Assignment2 and has
been fixed in this version.

### Goal and reset
Several goal checks are made for each collision between front and back walls. If the ball
is found to have successfully hit the goal range, it is considered a successful goal, the
ball will reset and give an initial speed and direction, and the car will return to 
original position.

### Enjoy
Some of the features mentioned in Handout are basically implemented, so I hope you enjoy
this little game. :)