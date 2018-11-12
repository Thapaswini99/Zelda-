#include <cstdlib>
#include <unistd.h>
#include<math.h>
#include <cstring>
#include <string>
#include <sstream>
#include "main.h"
#include "timer.h"
#include "ball.h"
#include "ocean.h"
#include "stone.h"
#include "gift.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/
Ball ball1;
float camera_rotation_angle = 0;
int score = 0;
int health_points = 0;
float ex = -10*sin(camera_rotation_angle*M_PI/180.0f);
float ey = 5;
float ez = -10*cos(camera_rotation_angle*M_PI/180.0f);
float tx = 0 ;
float ty = 0;
float tz = 100;
float ux = 0;
float uy = 1;
float uz = 0;
float flag = 0;
int j;

Ocean ocean;
Stone sn[600];
Gift g[600];

int view = 0;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;


Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //printf("%f\n",ex);
    //printf("yoooooooooo\n");

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( ex, ey, ez);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (tx, ty, tz);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (ux, uy, uz);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ocean.draw(VP);
    ball1.draw(VP);
    for(j=1;j<=500;j++)
        sn[j].draw(VP);

    for(j=1;j<=500;j++)
        g[j].draw(VP);


}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int top_view = glfwGetKey(window, GLFW_KEY_T);
    int tower_view = glfwGetKey(window, GLFW_KEY_U);
    int n_view = glfwGetKey(window, GLFW_KEY_H);
    int b_view = glfwGetKey(window, GLFW_KEY_B);
    int f_view = glfwGetKey(window, GLFW_KEY_F);
    if(top_view) view = 1;
    else if (tower_view) view = 2;
    else if (n_view) view = 0;
    else if (b_view) view = 3;
    else if (f_view) view = 4;
    int a = glfwGetKey(window, GLFW_KEY_A);
    int d = glfwGetKey(window, GLFW_KEY_D);
    int b = glfwGetKey(window, GLFW_KEY_B);
    if (left) {
        ball1.rotation1 = ball1.rotation1 + 0.4;
    }
    if (right) {
        ball1.rotation1 = ball1.rotation1 - 0.4;
    }
    if (up) {
        ball1.position.z = ball1.position.z + 0.05*cos(ball1.rotation1 * M_PI/180.0f);
        ball1.position.x = ball1.position.x + 0.05*sin(ball1.rotation1 * M_PI/180.0f);
    }
    if (down) {
        ball1.position.z = ball1.position.z - 0.05*cos(ball1.rotation1 * M_PI/180.0f);
        ball1.position.x = ball1.position.x - 0.05*sin(ball1.rotation1 * M_PI/180.0f);
    }
    for(j=1;j<=500;j++)
    {

        if(detect_collision(ball1.bounding_box(),sn[j].bounding_box())){
            score -= 1;
        }
    }
    for(j=1;j<=500;j++)
    {

        if(detect_collision(ball1.bounding_box(),g[j].bounding_box())){
            score += 1;
        }
    }
    if (a){
      camera_rotation_angle += 1;
    }
    if (d){
      camera_rotation_angle -= 1;
    }
    if(view == 0){
      ex = -ball1.position.x+10*sin(camera_rotation_angle*M_PI/180.0f);
      ey = 5;
      ez = -ball1.position.z+10*cos(camera_rotation_angle*M_PI/180.0f);
      tx = ball1.position.x;
      ty = 0;
      tz = ball1.position.z;
      ux = 0;
      uy = 1;
      uz = 0;

    }
    if(view == 1) {
      ex = ball1.position.x -1;
      ey = 30 ;
      ez = ball1.position.z -1;
      tx = ball1.position.x;
      tz = ball1.position.z;
      ty = 0;
    }
    else if(view == 2){
      ex = 0;
      ez = -30;
      ey = 10;
      tx = 0 ;
      ty = 0;
      tz = 30;
    }
    else if( view == 3){
      tx = ball1.position.x;
      tz = ball1.position.z +  5 * cos(ball1.rotation1 * M_PI/180.0);
      ex = tx;
      ey = 1;
      ty = 0;
      ez =  ball1.position.z;
    }
    else if ( view == 4){
      tx = ball1.position.x;
      tz = ball1.position.z +  5 * cos(ball1.rotation1 * M_PI/180.0);
      ex = tx;
      ey = 3;
      ty = 0;
      ez =  ball1.position.z -  5 * cos(ball1.rotation1 * M_PI/180.0);;


    }
    else{
      ex = -10*sin(camera_rotation_angle*M_PI/180.0f);
      ez = -10*cos(camera_rotation_angle*M_PI/180.0f);
      ey = 5;
    }
    //cout <<
}

void tick_elements() {
    ball1.tick();
    //camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    float p,q,r;
    ocean       = Ocean(0,-500.0,0,COLOR_WATER);
    ball1       = Ball(0, 0.001, 0,COLOR_RED);
    for(j=1;j<=500;j++){
        p = rand() % 500 -500;
        q = rand() % 500;
        sn[j] = Stone(p,0.5,q,COLOR_WHITE);
      }
    for(j=1;j<=500;j++){
        p = rand() % 500 -500;
        q = rand() % 500;
        g[j] = Gift(p,0.5,q,COLOR_PINK);
      }

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 1200;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);

            string Result;
            stringstream convert;
            convert << score;
            Result = convert.str();

            const char *one = "Score ";
            const char *two = Result.c_str();
            const char *three = "   Health Points ";
            string Result1;
            stringstream convert1;
            convert1 << health_points;
            Result1 = convert1.str();
            const char *four = Result1.c_str();
            string total( string(one) + two + string(three) + four);
            glfwSetWindowTitle(window, total.c_str());
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (sqrt(pow((a.x-b.x),2)+pow((a.z-b.z),2))  <= 0.75);
}

void reset_screen() {
    // float top    = screen_center_y + 4 / screen_zoom;
    // float bottom = screen_center_y - 4 / screen_zoom;
    // float left   = screen_center_x - 4 / screen_zoom;
    // float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::perspective((GLfloat)0.9f, (GLfloat) 2.0f, 0.01f, 9000.0f);
}
