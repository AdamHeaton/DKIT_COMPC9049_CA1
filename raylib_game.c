/*******************************************************************************************
*   Working on Mac with no build errors. Some extraneous files in here, but
*   this setup is the only one I've been able to get to work with Makefile (the only way I can
*   run anything) so please ignore them and only focus on this file, raylib_game.c
*
*   Run using Makefile with the following settings 
*   Configuration: Default
*   Build target: [raylib_game]
*   Launch target [raylib_game]
********************************************************************************************/

#include "raylib.h"

//setting the variables for the start of the game to be zero
int score = 0;
int timer = 0;

// declaring variables for the ball
typedef struct Ball
{
    int radius;
    float speedX;
    float speedY;
    float x;
    float y;
};

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    //setting the window's width and height
    const int screenWidth = 800;
    const int screenHeight = 600;

    //Initialising the window
    InitWindow(screenWidth, screenHeight, "Pong Game");

    //setting the audio clips that play within the game
    InitAudioDevice(); 
    Sound beepHigh = LoadSound("resources/beepHigh.wav");         // Load WAV audio file
    Sound beepMid = LoadSound("resources/beepMid.wav");         // Load WAV audio file
    Sound beepMid2 = LoadSound("resources/beepMid2.wav");         // Load WAV audio file
    Sound beepLow = LoadSound("resources/beepLow.wav");         // Load WAV audio file
    Sound youWin = LoadSound("resources/youWin.wav");         // Load WAV audio file

    //setting variables for the ball. it will appear in the middle of the screen 
    struct Ball ball;
    ball.radius = 10;
    ball.x = screenWidth/2;
    ball.y = screenHeight/2;
    ball.speedX = 3;
    ball.speedY = 3;

    //setting variables for the border and paddle
    int borderWidth = 25;
    int borderHeight= 25;
    int paddlePosition = screenHeight/2;
    int paddleWidth = 25;
    int paddleHeight = 75;

    SetTargetFPS(60);                 // Set our game to run at 60 frames-per-second
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // While the window is open, or has not been closed yet, draw the main body of code
    {
        //This is the main control section of the game, by moving the mouse up and down the Y axis, the paddles on the screen move up and down.
        paddlePosition = GetMouseY(); 
        BeginDrawing();
            //Set the background to be plain white
            ClearBackground(WHITE);

            //These rectangles are the border around the screen
            DrawRectangle(0,0, borderWidth, screenHeight, LIGHTGRAY);
            DrawRectangle(0,0, screenWidth, borderHeight, LIGHTGRAY);
            DrawRectangle(0, screenHeight-borderHeight, screenWidth, screenHeight, LIGHTGRAY);
            DrawRectangle(screenWidth-borderWidth, 0, borderWidth, screenHeight, LIGHTGRAY);

            //Initially I included these lines for testing purposes, but quite liked the division of the screen with them so have kept them in
            DrawLine(screenWidth/2, 0, screenWidth/2, screenHeight, LIGHTGRAY);                
            DrawLine(0, screenHeight/2, screenWidth, screenHeight/2, LIGHTGRAY);                

            if ((paddlePosition - paddleHeight/2) <= borderHeight) paddlePosition = paddleHeight/2+borderHeight;    
            /* this if statement checks to see if the paddle goes off the top of the screen 
            (ie has a value LESS than zero when the paddle's height is factored in)
            and then it stays at the height of the paddle divided by two */
            if ((paddlePosition + paddleHeight/2) >= screenHeight-borderHeight) paddlePosition = screenHeight-paddleHeight/2-borderHeight;    
            /* and this does roughly the same in order to check that the paddle doesn't go off the bottom of the screen */
        
            DrawRectangle(borderWidth-paddleWidth, paddlePosition-paddleHeight/2, paddleWidth, paddleHeight, BLACK);
            /* This line of code draws our left paddle. It factors in the paddle's height and width in order to draw the paddle from 
            the centre instead of the top left.  */
            DrawRectangle((screenWidth-borderWidth), (screenHeight-paddleHeight)-(paddlePosition-paddleHeight/2), paddleWidth, paddleHeight, BLACK);
            /* This line of code draws our right paddle. It factors in the paddle's height and width in order to draw the paddle from 
            the centre instead of the top left. */

            
            /*Ball bouncing on right hand side of screen being returned. If the ball goes further than the border, it is returned. This also checks
            to see if the ball has come into contact with the paddle by comparing their Y-values, and if so, it increments the score by one, and plays a sound effect
            If the player does not gain a point, then they lose a point
            */
            if (ball.x >= screenWidth-borderWidth-ball.radius) {
                if ((ball.y < screenHeight-(paddlePosition-paddleHeight/2)) && (ball.y > screenHeight-(paddlePosition+paddleHeight/2))) {
                    score++;
                    if (score == 25) {
                        PlaySound(youWin); // this plays a sound when the game is over at 25 points. It's in this section of code to ensure that it only plays once
                    }
                    timer = 40;          
                    PlaySound(beepHigh);
                }
                else {
                    score--;
                    timer = 40;
                    PlaySound(beepLow);
                }
                ball.speedX = ball.speedX*-1; //return the ball by reversing the ball's X speed
            }

            //ball bouncing on left hand side of screen being returned. If the ball is less than the border, it will be returned to the right by reversing its speed
            //it also checks to see if the ball has come into contact with the paddle's Y-co-ordinates, and if so will increase the score counter, and if not it will 
            //decrease it.
            if (ball.x <= ball.radius+borderWidth) {
                if ((ball.y > paddlePosition-paddleHeight/2) && (ball.y < paddlePosition+paddleHeight/2)) {
                    score++;
                    if (score == 25) {
                        PlaySound(youWin); // this plays a sound when the game is over at 25 points
                    }
                    timer = 40;
                      PlaySound(beepHigh);
                }
                else 
                {
                    score--;
                    timer = 40;
                    PlaySound(beepLow);
                }
                ball.speedX = ball.speedX*-1; //return the ball by reversing the ball's X speed
            }

           //Ball bouncing on bottom section of screen being returned, incrementing bounce counter, and playing a sound alongside it
            if (ball.y >= screenHeight-borderHeight-ball.radius) {
                ball.speedY = ball.speedY*-1; //return the ball by reversing the ball's Y speed
                PlaySound(beepMid);
            }

            //ball bouncing on top section of screen being returned, incrementing bounce counter, and playing a sound alongside it
            if (ball.y <= ball.radius+borderHeight) {
                ball.speedY = ball.speedY*-1; //return the ball by reversing the ball's Y speed
                PlaySound(beepMid);
            }

            //This piece of code displays the score whenever it changes.
            if (timer > 0) {
                DrawText(TextFormat("SCORE is: %02i", score), screenWidth/6, screenHeight/3, 80, GRAY);
                timer = timer-1;
            }
            
            //DRAWING IN SECOND PADDLES]
            //If the score is between 5 and twenty there will be a set of second paddles drawn in, mirroring the existing paddles, but drawn closer to the centre of the screen
            if ((score >= 6) && (score <= 10)) {
                DrawRectangle((screenWidth/3*2-borderWidth), (screenHeight-paddleHeight)-(paddlePosition-paddleHeight/2), paddleWidth, paddleHeight, RED);
                if ((ball.x >= screenWidth/3*2-borderWidth-ball.radius) && (ball.x <= screenWidth/3*2+borderWidth-ball.radius)) {
                    if ((ball.y < screenHeight-(paddlePosition-paddleHeight/2)) && (ball.y > screenHeight-(paddlePosition+paddleHeight/2))) 
                    {
                        ball.speedX = ball.speedX*-1; //return the ball by reversing the ball's X speed
                        PlaySound(beepMid2);
                    }
                }
                DrawRectangle((screenWidth/3)+borderWidth-paddleWidth, paddlePosition-paddleHeight/2, paddleWidth, paddleHeight, BLUE);
                if ((ball.x >= screenWidth/3+borderWidth-paddleWidth-ball.radius) && (ball.x <= screenWidth/3+borderWidth+paddleWidth-ball.radius)) 
                {
                    if ((ball.y >= paddlePosition-paddleHeight/2) && (ball.y <= paddlePosition+paddleHeight/2))
                    {
                        ball.speedX = ball.speedX*-1; //return the ball by reversing the ball's X speed
                        PlaySound(beepMid2);
                    }
                }
            }

            if ((score >= 11) && (score <= 20)) {

                DrawRectangle((screenWidth/3)+borderWidth-paddleWidth, paddlePosition-paddleHeight/2, paddleWidth, paddleHeight, GREEN);
                if ((ball.x >= screenWidth/3+borderWidth-paddleWidth-ball.radius) && (ball.x <= screenWidth/3+borderWidth+paddleWidth-ball.radius)) 
                {
                    if ((ball.y >= paddlePosition-paddleHeight/2) && (ball.y <= paddlePosition+paddleHeight/2))
                    {
                        ball.speedX = ball.speedX*-1; 
                        ball.speedY = ball.speedY*-1; //reverse the ball's direction by reversing both x and y speeds
                        PlaySound(beepMid2);
                    }
                }

                DrawRectangle((screenWidth/3*2-borderWidth), (screenHeight-paddleHeight)-(paddlePosition-paddleHeight/2), paddleWidth, paddleHeight, YELLOW);
                if ((ball.x >= screenWidth/3*2-borderWidth-ball.radius) && (ball.x <= screenWidth/3*2+borderWidth-ball.radius)) {
                    if ((ball.y < screenHeight-(paddlePosition-paddleHeight/2)) && (ball.y > screenHeight-(paddlePosition+paddleHeight/2))) 
                    {
                        ball.speedX = ball.speedX*-1; //reverse the ball's direction by reversing both x and y speeds
                        ball.speedY = ball.speedY*-1;
                        PlaySound(beepMid2);
                    }
                }
            }

            //this piece of code starts to shrink the paddles if the score is 16 or over
            if ((score >= 16) && (score <= 24))
                {
                    paddleHeight = 275-score*10;
                }
            
            //Endgame. When the score is 25 the ball's speed is brought to zero, centred in the middle of the screen
            //if the player wants to play again the relevant variables are reset, otherwise if the player presses 'N' the game will end 
            //and the window will close
            if (score == 25)
            {
                DrawText(TextFormat("YOU ARE A WINNER!"), screenWidth/5, screenHeight/3*2, 40, GRAY);
                DrawText(TextFormat("PLAY AGAIN? Y/N"), screenWidth/5, screenHeight/3*2.5, 40, GRAY);
                ball.speedX = 0;
                ball.speedY = 0;
                ball.x = screenWidth/2;
                ball.y = screenHeight/2;
                if (IsKeyPressed(KEY_Y))
                {
                ball.speedX = 3;
                ball.speedY = 3;
                paddleHeight = 75;
                score = 0;
                }            
                if (IsKeyPressed(KEY_N))
                {
                    CloseWindow();;
                }            
            }

            //the code that draws the ball across the screen, adding the ball's speed to the x and y values
            ball.x += ball.speedX;
            ball.y += ball.speedY;

            DrawCircle(ball.x, ball.y, ball.radius, RED);


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
