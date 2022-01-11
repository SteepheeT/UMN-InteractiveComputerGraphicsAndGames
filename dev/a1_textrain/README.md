This is a discription file for assignment 1 - TextRain, created by Junyue Tao.

The text we use will be read from a poem inside a text file named "test.txt" inside 
'data' folder. Then each line will be stored seperately inside String Array called 
messageBox, where each position sotres one line of the poem. Then one position of 
messageBox will be randomly selected, passed to a String variable message to wait for 
further break down. 

The string message will eventually disassembled and split based on space and stored 
in array called "parts", each position of parts will be one words. Then these "words"
will be stored in a big array called "letters." The difference between directly go over
the message variable and store each letter is by this method, letters that forms one
word will go through one loop together to have a closer x position. It gives us some 
that letters that form one word will stay close to each other.

The letter will stop on darker object by detecting several pixels below itself, 
currently we are detecting 3 pixels below one letter. To make it more "robust" like the
handout said, most letters(on my machine) will land on a very thin line(one pixel wide).
To test this, press 'l' and there will be a single pixel line appear on the screen.
Once the the grayscale value is below the threshvalue, then the letter should "land" 
on it, the darker object. In addition, to make the letter react to movement of darker 
object, the ability of moving upward is added to letters.

The program also provides debugging mode, to switch to debugging mode, press space bar.
The current threshold value is displayed at the lower right cornor, pressing up arrow
will increase it and down arrow will decrease it. All changes of threshold value will
display immediatly. 

To make the scene looks "fancy and good," the video is in grayscale and texts are in
different color. Letters will have different falling speed and will change everytime
they got displayed. Letters will also maintain same position (on x-axis) until they reach the 
bottom of the screen twice. After that, their x position will be randomly generated.