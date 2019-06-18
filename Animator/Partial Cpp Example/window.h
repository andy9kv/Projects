// window.h
// The Window class has the main Qt components and links together the GUI with your scene graph

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QtWidgets>
#include <QSlider>

class Node;

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

protected:
    void keyPressEvent(QKeyEvent *event);

	signals:
	

	public slots :
		void on_treeView_currentItemChanged(const QItemSelection &selected, const QItemSelection &deselected);  // Called when a different item in the tree view is selected. 
		void on_xTransSlider_valueChanged(int value); // Called when the xTransSlider is slid.
		void on_yTransSlider_valueChanged(int value); // Called when the yTransSlider is slid. 
		void on_xScaleSlider_valueChanged(int value); // Called when the xScaleSlider is slid.
		void on_yScaleSlider_valueChanged(int value); // Called when the yScaleSlider is slid.
		void on_rotationSlider_valueChanged(int value); // Called when the rotationSlider is slid. 
private:
    Ui::Window *ui;				// A Qt internal mechanism
	QStandardItemModel* model;  // Links Qt with your scene graph 
	Node* currentNode;			// A pointer to the currently selected "node"
	
};

#endif // WINDOW_H
