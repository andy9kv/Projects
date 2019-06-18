// Implementation of the MyGLWidget class
// Your scene graph will be created and called from here and OpenGL calls to display it. 

#include <QtWidgets>
#include <QtOpenGL>  // Note that some of the OpenGL calls are Qt specific and not standard OpenGL

#include "myglwidget.h"

MyGLWidget::MyGLWidget(QWidget *parent)					 // Constructor 
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
	c = new Character();
}   // Call to parent constructor

MyGLWidget::~MyGLWidget()	// Destructor
{
	// Normally you destroy what you created, but I'm not worried about it just now. 
}

////////////////////////////////////////////////////////////////////////////
// A few more Qt methods that you don't need to worry about yet. 
QSize MyGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MyGLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}
///////////////////////////////////////////////////////////////////////////////


// Initializing OpenGL
void MyGLWidget::initializeGL()
{
	qglClearColor(Qt::darkBlue);	// Set the background color. Note this isn't standard OpenGL. It's Qt. 

	// We'll add more code here later in the semester
}

// This method gets called whenever the widget needs updating. 
void MyGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT); // Clear the window using the background color set in initialization
 
    draw();
}

// Actual code for building and drawing the primitives
void MyGLWidget::draw()
{
	//BUG #2 - Does not clear the screen before a new drawing occurs
	//Global Variable c
	GUI gui = GUI(c);
	gui.draw(gui.build());
}

/////////////////////////////////////////////////////////////////
// More Qt methods that you do not have to worry about just now
void MyGLWidget::resizeGL(int width, int height)
{

	glViewport(0, 0, this->width(), this->height());  // lower left coordinate and width and height

										//glOrtho describes a transformation that produces a parallel projection.
	glOrtho(-10, 10, -10, 10, -1, 1);   // left, right, bottom, top, near, far

}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
   
}

//////////////////////////////////////////////////////////////////////////
