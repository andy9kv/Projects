// A Qt widget class for displaying OpenGL primitives
// You will need to connect your scene graph code to this class

#ifndef MYGLWIDGET_H // Similar to #pragma once. Used to handle circular dependences. 
#define MYGLWIDGET_H // #endif is at the end of the file. 

#include <QGLWidget>
#include "GUI.h"

class MyGLWidget : public QGLWidget  // MyGLWidget is a sub-class of QGLWidget and inherits all of its functionality
{
    Q_OBJECT
public:
	Character *c;//Only Global Variable
    explicit MyGLWidget(QWidget *parent = 0);	// Constructor
    ~MyGLWidget();			// Destructor

signals:
public slots:

protected:
    void initializeGL();						// OpenGL requires some initialization. 
    void paintGL();								// Called when the OpenGL window needs to be redrawn. It calls our draw method. 
    void resizeGL(int width, int height);		// Called when the window is resized. 

    QSize minimumSizeHint() const;					// Methods for handling resizing and mouse events. Don't worry about them now. 
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void draw();	// This method is where you will traverse your scene graph and draw your animal
};

#endif // MYGLWIDGET_H

