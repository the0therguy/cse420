#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

#define sqrt2 (sqrt(2))

typedef struct point
{
    double x,y,z;
} Point;

bool drawAxes = true;
float tyreAngle = 0;

int doorRotate = 0;

float quadAngle = -90;
float windowAngle = 0;
float doorAngle = 0;

float boxAngle = 0;
float boxScaleFactor = 1;
float tyreScaleFactor = 1;
float tinyBoxAngle = 0;
float background = 0;
float slidingDoor = 0;
int flag = 0;
float window_slide = 0;

Point pos, u, r, l;

void displayAxes()
{
    if(drawAxes)
    {
        glBegin(GL_LINES);
        {
            glColor3f(1.0, 0, 0);

            glVertex3f( 500,0,0); // a point on pos x axis
            glVertex3f(-500,0,0); // a point on neg x axis

            glColor3f(0, 1.0, 0);

            glVertex3f(0,-500,0); // a point on pos y axis
            glVertex3f(0, 500,0); // a point on neg y axis

            glColor3f(0, 0, 1.0);

            glVertex3f(0,0, 500);  // a point on pos z axis
            glVertex3f(0,0,-500);   // a point on neg z axis
        }
        glEnd();
    }
}

Point func(Point vect, Point perp, int dir)
{
    double c = cos(pi/180);
    double s = dir * sin(pi/180);
    Point point;
    point.x = c * vect.x + s * perp.x;
    point.y = c * vect.y + s * perp.y;
    point.z = c * vect.z + s * perp.z;
    c = sqrt(point.x*point.x + point.y*point.y + point.z*point.z);
    point.x /= c;
    point.y /= c;
    point.z /= c;
    return point;
}

void keyboardListener(unsigned char key, int x,int y)
{
    switch(key)
    {
    case '1':
    {
        Point l1 = func(l, r, -1);
        r = func(r, l, 1);
        l = l1;
        break;
    }
    case '2':
    {
        Point l1 = func(l, r, 1);
        r = func(r, l, -1);
        l = l1;
        break;
    }

    case '3':
    {
        Point u1 = func(u, l, -1);
        l = func(l, u, 1);
        u = u1;
        break;
    }
    case '4':
    {
        Point u1 = func(u, l, 1);
        l = func(l, u, -1);
        u = u1;
        break;
    }
    case '5':
    {
        Point r1 = func(r, u, -1);
        u = func(u, r, 1);
        r = r1;
        break;
    }
    case '6':
    {
        Point r1 = func(r, u, 1);
        u = func(u, r, -1);
        r = r1;
        break;
    }
    case '7':
    {
        tyreScaleFactor += 0.2;
        break;
    }
    case '8':
    {
        tyreScaleFactor -= 0.2;
        break;
    }
    case '9':
        {
            printf("Window angle %f\n", windowAngle);
            if(windowAngle == -54)
            {
                break;
            }
            else
            {
                windowAngle-=2;
                break;
            }
        }
    case '0':
        {
        if(windowAngle==0)
        {
            break;

        }
        else
        {
            windowAngle+=2;
            break;
        }
        }
    case 'l':
        {
            boxAngle-=5;
            break;
        }
    case 'L':
        {
            boxAngle-=5;
            break;
        }
    case 'r':
        {
            boxAngle+=5;
            break;
        }
    case 'R':
        {
            boxAngle+=5;
            break;
        }
    case 'U':
        {
            if( int(boxScaleFactor)==2)
            {
                break;
            }
            else
            {
                boxScaleFactor = boxScaleFactor+.1;
                break;
            }
        }
    case 'u':
                {
            if( int(boxScaleFactor)==2)
            {
                break;
            }
            else
            {
                boxScaleFactor = boxScaleFactor+.1;
                break;
            }
        }

    case 'd':
        if (boxScaleFactor>0)
        {
            boxScaleFactor-=.1;
            if(boxScaleFactor<0)
            {
                boxScaleFactor=0;
            }
            break;
        }
    case 'D':
        if (boxScaleFactor>0)
        {
            boxScaleFactor-=.1;
            if(boxScaleFactor<0)
            {
                boxScaleFactor=0;
            }
            break;
        }




    default:
        break;
    }
}
Point camera_position(Point vect, Point perp, int dir)
{
    vect.x = (vect.x +  4* dir*perp.x);
    vect.y = (vect.y +  4* dir*perp.y);
    vect.z = (vect.z +  4* dir*perp.z);
    return vect;

}

void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_DOWN: // down arrow
    {
        pos = camera_position(pos, l, -1);
        break;
    }

    case GLUT_KEY_UP:   // up arrow
    {
        pos = camera_position(pos, l, 1);
        break;
    }

    case GLUT_KEY_RIGHT:    // right arrow
    {
        pos = camera_position(pos, r, 1);
        break;
    }

    case GLUT_KEY_LEFT:     // left arrow
    {
        pos = camera_position(pos, r, -1);
        break;
    }

    case GLUT_KEY_PAGE_UP:
    {
        pos = camera_position(pos, u,1);
        break;
    }

    case GLUT_KEY_PAGE_DOWN:
    {
        pos= camera_position(pos,u,-1);
        break;
    }

    case GLUT_KEY_END:
    {
        if(slidingDoor == 0)
            break;
        else
        {
            slidingDoor += 5;
            break;
        }
    }

    case GLUT_KEY_HOME:
    {
        if(slidingDoor == -40)
            break;
        else
        {
            slidingDoor -= 5;
            break;
        }
    }

    default:
        break;
    }
}


void mouseListener(int button, int state, int x, int y) 	//x, y is the x-y of the screen (2D)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN) // 2 times?? in ONE click? -- solution is checking DOWN or UP
        {
            printf("Mouse Left Button Clicked\n");
        }
        else if(state == GLUT_UP)
        {
            printf("Mouse Left Button Released\n");
        }
        break;
    default:
        break;
    }
}

void displayTyre(float tyreRadius, float tyreWidth)
{
    int tyreSlices = 10;
    float sliceWidth = (2 * M_PI * tyreRadius) / tyreSlices;
    float sliceAngle = 360 / tyreSlices;

    float halfTyreWidth = tyreWidth / 2, quartTyreWidth = halfTyreWidth / 2;
    float halfSliceWidth = sliceWidth / 2;

    if(flag == 0)
    {
        for(int i = 0; i< tyreSlices; i++)
        {
            if(i%2==0)
            {
                glColor3f(1, 0, 0);
            }
            else
            {
                glColor3f(0, 0, 1);
            }

            glPushMatrix();
            glRotatef(sliceAngle*i, 1, 0, 0);
            glBegin(GL_QUADS);
            {
                glVertex3f(halfTyreWidth,-tyreRadius/2, 0);
                glVertex3f(-halfTyreWidth, -tyreRadius/2, 0);
                glVertex3f(-halfTyreWidth, -tyreRadius, 0);
                glVertex3f(halfTyreWidth, -tyreRadius, 0);
            }
            glEnd();
            glPopMatrix(); //same again for another color

        }
    }



    /*glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    {
        glVertex3f(halfTyreWidth, 0, tyreRadius);
        glVertex3f(halfTyreWidth, 0, -tyreRadius);
        glVertex3f(-halfTyreWidth, 0, -tyreRadius);
        glVertex3f(-halfTyreWidth, 0, tyreRadius);
    }
    glEnd();*/


    for(int i=0; i<tyreSlices; i++)
    {
            if(i%2==0)
            {
                glColor3f(1, 1, 1);
            }
            else
            {
                glColor3f(0, 0, 0);
            }


        glPushMatrix();
        glRotatef(sliceAngle*i, 1, 0, 0);
        glTranslatef(0, tyreRadius, 0);
        glBegin(GL_QUADS);
        {
            glVertex3f(halfTyreWidth, 0, halfSliceWidth);
            glVertex3f(halfTyreWidth, 0, -halfSliceWidth);
            glVertex3f(-halfTyreWidth, 0, -halfSliceWidth);
            glVertex3f(-halfTyreWidth, 0, halfSliceWidth);
        }
        glEnd();
        glPopMatrix();


    }
}

void displayQuad(float quadLength, float quadWidth, float xcolor, float ycolor, float zcolor) // width along x axis, length along z axis
{
    float halfQuadWidth = quadWidth / 2, halfQuadLength = quadLength / 2;

    glBegin(GL_QUADS);
    {
        /** Solid Fill Quad **/

        //glColor3f(1, 1, 0);

        //glVertex3f(halfQuadWidth, 0, halfQuadLength);
        //glVertex3f(halfQuadWidth, 0, -halfQuadLength);
        //glVertex3f(-halfQuadWidth, 0, -halfQuadLength);
        //glVertex3f(-halfQuadWidth, 0, halfQuadLength);

        /** Gradient Fill Quad **/


        glColor3f(xcolor, ycolor, zcolor);

        glVertex3f(halfQuadWidth, 0, halfQuadLength);
        glVertex3f(halfQuadWidth, 0, -halfQuadLength);

        glColor3f(xcolor, ycolor, zcolor);

        glVertex3f(-halfQuadWidth, 0, -halfQuadLength);
        glVertex3f(-halfQuadWidth, 0, halfQuadLength);

    }
    glEnd();
}

void displayGradientQuad(float quadLength, float quadWidth)
{
    float halfQuadWidth = quadWidth / 2, halfQuadLength = quadLength / 2;

    glBegin(GL_QUADS);
    {
        /** Solid Fill Quad **/
        if(flag==0)
        {
        glColor3f(1,1,1);

        glVertex3f(halfQuadWidth, 0, halfQuadLength);
        glVertex3f(halfQuadWidth, 0, -halfQuadLength);
        glColor3f(0,0,1);

        glVertex3f(-halfQuadWidth, 0, -halfQuadLength);
        glVertex3f(-halfQuadWidth, 0, halfQuadLength);
        }
        else
        {
        glColor3f(0,0,1);

        glVertex3f(halfQuadWidth, 0, halfQuadLength);
        glVertex3f(halfQuadWidth, 0, -halfQuadLength);
        glColor3f(1,1,1);

        glVertex3f(-halfQuadWidth, 0, -halfQuadLength);
        glVertex3f(-halfQuadWidth, 0, halfQuadLength);

        }

        /** Gradient Fill Quad **/
    }
    glEnd();

}

void displayWindow(float quadLength, float quadWidth,float xcolor, float ycolor, float zcolor)
{
    float halfQuadWidth = quadWidth / 2, halfQuadLength = quadLength / 2;
    glBegin(GL_QUADS);
    {
        glColor3f(xcolor, ycolor, zcolor);
        glVertex3f(0, halfQuadWidth, 0);
        glVertex3f(0, halfQuadWidth, -quadLength);
        glVertex3f(0, -halfQuadWidth, -quadLength);
        glVertex3f(0, -halfQuadWidth, 0);


    }
    glEnd();

}

void displayBackdoor(float quadLength, float quadWidth, int id)
{
    float halfQuadWidth = quadWidth / 2, halfQuadLength = quadLength / 2;
        glBegin(GL_QUADS);
    {
        /** Solid Fill Quad **/
        if(id==1)
        {
        glColor3f(1,1,1);

        glVertex3f(-quadWidth, 0, halfQuadLength);
        glVertex3f(-quadWidth, 0, -halfQuadLength);
        glColor3f(0,0,1);

        glVertex3f(0, 0, -halfQuadLength);
        glVertex3f(0, 0, halfQuadLength);
        }
        else
        {
        glColor3f(1,1,1);

        glVertex3f(quadWidth, 0, halfQuadLength);
        glVertex3f(quadWidth, 0, -halfQuadLength);
        glColor3f(0,0,1);


        glVertex3f(0, 0, -halfQuadLength);
        glVertex3f(0, 0, halfQuadLength);

        }

        /** Gradient Fill Quad **/
    }
    glEnd();

}



void displayBox(float boxLength, float boxWidth, float boxHeight, float xcolor, float ycolor, float zcolor) // length -> x axis, width -> yaxis, height -> z axis
{
    float halfBoxLength = boxLength / 2, halfBoxWidth = boxWidth / 2, halfBoxHeight = boxHeight / 2;
    glBegin(GL_QUADS);
    {
        glColor3f(xcolor, ycolor, zcolor);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);

        glColor3f(xcolor-0.35, ycolor-0.35, zcolor-0.35);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glColor3f(xcolor-0.5, ycolor-0.5, zcolor-0.5);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
    }
    glEnd();
}

void displayScene()
{
    //Primitive Object dimensions

    /** Principal Axes **/

    displayAxes();
    glRotatef(boxAngle, 0, 0, 1);

    glScalef(boxScaleFactor, boxScaleFactor, boxScaleFactor);
    //base
    float baseLength=300,baseWidth = 200, baseHeight =30;
    glPushMatrix();
    glTranslatef(0, 0, 0);   // translation applied first, rotation applied second
    displayBox(baseLength, baseWidth, baseHeight, 0, 0, 0.7);
    glPopMatrix();
    //house
    float houseLength=280, houseWidth=180, houseHeight =170;
    glPushMatrix();
    glTranslatef(0, 0, 70);    // translation applied first, rotation applied second
    displayBox(houseLength, houseWidth, houseHeight, 1.0, 0, 0);
    glPopMatrix();
    //roof
    float roofLength = 281, roofWeidth=200, roofHeight =10;
    glPushMatrix();
    glTranslatef(0, 0, 160);    // translation applied first, rotation applied second
    displayBox(roofLength, roofWeidth, roofHeight, 0.0, 1.0, 0.0);
    glPopMatrix();
    //first stairs
    float first_stairLength = 81, first_stairWidth=210, first_stairHeight=10;
    glPushMatrix();
    glTranslatef(0, 25, 10);    // translation applied first, rotation applied second
    displayBox(first_stairLength, first_stairWidth, first_stairHeight, 0.75, 0.75, 0);
    glPopMatrix();
    //2nd stairs
    float second_stairLength= 120, second_stairWidth=240, second_stairHeight=10;
    glPushMatrix();
    glTranslatef(0, 30, 0);    // translation applied first, rotation applied second
    displayBox(second_stairLength, second_stairWidth, second_stairHeight, 0, 0.75, 0.8);
    glPopMatrix();
    //3rd stairs
    float third_stairLength=160, third_stairWeidth = 270, third_stairHeight = 10;
    glPushMatrix();
    glTranslatef(0, 37, -10);    // translation applied first, rotation applied second
    displayBox(third_stairLength, third_stairWeidth, third_stairHeight, 0.75, 0.75, 0.75);
    glPopMatrix();

    //door 1st part
    float first_doorLength=125, first_doorWeidth=40;
    glPushMatrix();
    glTranslatef(slidingDoor, 0, 0);
    glTranslatef(-15, 95, 50);    // translation applied first, rotation applied second
    flag=0;
    displayGradientQuad(first_doorLength, first_doorWeidth);
    glPopMatrix();

    //door 2nd part
    glPushMatrix();
    glTranslatef(-slidingDoor, 0, 0);
    glTranslatef(20, 95, 50);    // translation applied first, rotation applied second
    flag=1;
    displayGradientQuad(first_doorLength, first_doorWeidth);
    glPopMatrix();
    //back first door
    float back_firstLength = 92, back_firstWidth = 40;
    glPushMatrix();
    glTranslatef(-40, -95, 62);    // translation applied first, rotation applied second
    flag=0;
    glRotatef(-doorAngle, 0,0,1);
    displayBackdoor(back_firstLength, back_firstWidth, 0);
    glPopMatrix();
    //back 2nd door
    glPushMatrix();
    glTranslatef(40, -95, 62);    // translation applied first, rotation applied second
    glRotatef(doorAngle, 0,0,1);
    displayBackdoor(92, 40, 1);
    glPopMatrix();

    //roof above the door
    float doorRoofL=78,doorRoofW=195,doorRoofH=5;
    glPushMatrix();
    glTranslatef(2, 0, 110);    // translation applied first, rotation applied second
    displayBox(doorRoofL, doorRoofW, doorRoofH, 0, 0, 0.7);
    glPopMatrix();
    //lampost
    float lampostL=96, lampostW=40, lampostH=300;
    glPushMatrix();
    glTranslatef(0, 0, 160);    // translation applied first, rotation applied second
    displayBox(lampostL, lampostW, lampostH, 0.7, 0.7, 0.0);
    glPopMatrix();
    //window
    float windowL=40, windowW=60;
    glPushMatrix();
    glTranslatef(142, 0, 96);    // translation applied first, rotation applied second
    glRotatef(windowAngle, 0, 1, 0); // rotates with respect to z axis
    displayWindow(windowL, windowW, 0.75, 0.75, 0);
    glPopMatrix();
    //window roof
    float windowroofL=28, windowroofW=60, windowroofH=5;
    glPushMatrix();
    glTranslatef(128, 0, 97);    // translation applied first, rotation applied second
    displayBox(windowroofL, windowroofW, windowroofH, 0, 0.75, 0.8);
    glPopMatrix();

    //sliding window roof
    glPushMatrix();
    glTranslatef(-128, 0, 97);    // translation applied first, rotation applied second
    displayBox(28, 60, 5, 0, 0.75, 0.8);
    glPopMatrix();

    //sliding window
    glPushMatrix();
    glTranslatef(-145, 0, 75);    // translation applied first, rotation applied second
    glRotatef(quadAngle, 0, 0, 1); // rotates with respect to z axis
    glTranslatef(0,0, window_slide);
    displayQuad(40, 60, 0.75, 0.75, 0);
    glPopMatrix();



    /** Tyre **/
    glPushMatrix();
    glTranslatef(0, 33, 250);
    glRotatef(tyreAngle, 0, 1, 0); // roates the tyre w.r.t y axis
    glRotatef(90, 0, 0, 1); // roates the tyre w.r.t z axis
    flag = 0;
    glScalef(tyreScaleFactor, tyreScaleFactor, tyreScaleFactor);
    displayTyre(45, 20);
    glPopMatrix();

    //small tyre
    glPushMatrix();
    glTranslatef(0, 33, 250);
    glTranslatef(0, 0, 1); // roates the tyre w.r.t z axis
    flag = 1;
    glRotatef(90, 0, 0, 1);
    glScalef(tyreScaleFactor, tyreScaleFactor, tyreScaleFactor);
    displayTyre(22.5, 10);
    glPopMatrix();

}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(background, background, background,0);	//sets background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // sets the current matrix to identity
    gluLookAt(pos.x, pos.y, pos.z, pos.x+l.x, pos.y+l.y, pos.z+l.z, u.x, u.y, u.z); // sets camera position and direction

    displayScene();

    glutSwapBuffers();
}



void animate()
{
    tinyBoxAngle += 0.05;
    glutPostRedisplay();


}
void tyre_moving_clockwise()
{
    tyreAngle +=0.15;
    glutPostRedisplay();

}
void tyre_moving_anticlockwise()
{
    tyreAngle -=0.15;
    glutPostRedisplay();

}
void tyre_moving_stop()
{
    tyreAngle =tyreAngle;
    glutPostRedisplay();
}
void slideWindowUP()
{
    if (window_slide==45)
    {
        window_slide=45;
    }
    else{
        window_slide+=0.5;
    }
    glutPostRedisplay();
}
void slideWindowDown()
{
    if (window_slide<=0)
    {
        window_slide=0;
    }
    else
    {
        window_slide-=0.1;
    }
    glutPostRedisplay();
}

void door_open()
{
    if(doorAngle>=145)
    {
        doorAngle=145;
    }
    else
    {
        doorAngle+=0.5;
    }

    glutPostRedisplay();

}

void door_close()
{
    if(doorAngle<=0)
    {
        doorAngle=0;
    }
    else
    {
        doorAngle-=0.5;
    }
    glutPostRedisplay();

}

void init()
{
    glClearColor(background, background, background,0); 	//sets background color

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity(); // sets the current matrix to identity
    gluPerspective(80,	1,	1,	1000.0);

    // to understand the following values, read the specification pdf
    double d = 1 / sqrt2;
    pos = {300, 300, 150};
    u = {0, 0, 1};
    r = {-d, d, 0};
    l = {-d, -d, 0};
}

void subMenu(int id)
{
    if(id == 3)
    {
        background = 1;
    }
    else if(id == 4)
    {
        background = 0;
    }
}
void subMenu_fan(int id)
{
    if(id==5)
    {
        glutIdleFunc(tyre_moving_anticlockwise);		//what you want to do in the idle time (when no drawing is occuring)
    }
    else if(id==6)
    {
        glutIdleFunc(tyre_moving_clockwise);		//what you want to do in the idle time (when no drawing is occuring)
    }
    else if(id==7)
    {
        glutIdleFunc(tyre_moving_stop);
    }
}

void subMenu_slidingDoor(int id)
{
    if(id==8)
    {
        glutIdleFunc(door_open);
    }
    else if(id==9)
    {
        glutIdleFunc(door_close);

    }
}

void subMenu_slidingWindow(int id)
{
    if (id==10)
    {
        glutIdleFunc(slideWindowUP);
    }
    if (id==11)
    {
        glutIdleFunc(slideWindowDown);
    }
}
void mainMenu(int id)
{
    if(id == 1)
    {
        drawAxes = true;
    }
    else if(id == 2)
    {
        drawAxes = false;
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("My OpenGL Program");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    int subMenuNo = glutCreateMenu(subMenu);
    glutAddMenuEntry("White", 3);
    glutAddMenuEntry("Black", 4);
    int subMenuNo_fan = glutCreateMenu(subMenu_fan);
    glutAddMenuEntry("Clockwise", 5);
    glutAddMenuEntry("Anti Clockwise", 6);
    glutAddMenuEntry("Stop", 7);
    int subMenuNo_slidingdoor = glutCreateMenu(subMenu_slidingDoor);
    glutAddMenuEntry("Open", 8);
    glutAddMenuEntry("Close", 9);
    int subMenuNo_slidingWindow = glutCreateMenu(subMenu_slidingWindow);
    glutAddMenuEntry("Window Open", 10);
    glutAddMenuEntry("Window Close", 11);


    glutCreateMenu(mainMenu);
    glutAddMenuEntry("Axes On", 1);
    glutAddMenuEntry("Axes Off", 2);
    glutAddSubMenu("Background", subMenuNo);
    glutAddSubMenu("Exhaust Fan", subMenuNo_fan);
    glutAddSubMenu("Rotating Door", subMenuNo_slidingdoor);
    glutAddSubMenu("Sliding Window", subMenuNo_slidingWindow);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();		//The main loop of OpenGL, this function never returns

    return 0;
}
