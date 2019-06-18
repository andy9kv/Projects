//Demonstrating connecting the Qt Tree View widget to another class (like a scene graph)

#include <iostream>
#include <sstream>
#include <string>
#include "window.h"
#include "ui_window.h"
#include "GUI.h"

#include "myglwidget.h"

Q_DECLARE_METATYPE(Node*);  // Tell Qt that Data is a type. This would be your Node class? 

Window::Window(QWidget *parent) :  // Window constructor
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);				// Standard Qt stuff

	model = new QStandardItemModel();  // Tree views have to be populated with models, so create one
	
	Character *c = ui->myGLWidget->c;
	Node n = *(c->getRoot());//Keeps track of stuff

	//root
	QStandardItem* root = new QStandardItem(QString::fromStdString(n.getName()));	// These would correspond to your scene graph nodes
	root->setData(QVariant::fromValue(&n)); // Store a pointer to data in the tree item so that you can retrieve it when an item is selected. 
	model->appendRow(root);						

	//rootTrans
	n = n.getChild(0);
	QStandardItem* rootTrans = new QStandardItem(QString::fromStdString(n.getName()));  
	rootTrans->setData(QVariant::fromValue(&n)); // Store a pointer to data in the tree item. 
	root->appendRow(rootTrans);	

	//head
	n = n.getChild(0);
	QStandardItem* head = new QStandardItem(QString::fromStdString(n.getName()));
	head->setData(QVariant::fromValue(&n)); // Store a pointer to data in the tree item. 
	rootTrans->appendRow(head);

	//face
	n = n.getChild(0);
	QStandardItem* face = new QStandardItem(QString::fromStdString(n.getName()));
	head->setData(QVariant::fromValue(&n)); // Store a pointer to data in the tree item. 
	head->appendRow(face);

	//ear1
	n = n.getParent()->getChild(1);
	QStandardItem* ear1 = new QStandardItem(QString::fromStdString(n.getName()));
	ear1->setData(QVariant::fromValue(&n)); // Store a pointer to data in the tree item. 
	head->appendRow(ear1);

	//ear2
	n = n.getParent()->getChild(2);
	QStandardItem* ear2 = new QStandardItem(QString::fromStdString(n.getName()));
	ear2->setData(QVariant::fromValue(&n)); // Store a pointer to data in the tree item. 
	head->appendRow(ear2);

	//puts n back as the root
	n = *(n.getParent()->getParent()->getParent());

	ui->treeView->setModel(model); // ui needs to include treeView, so create one in Qt Creator and make sure it is named treeView
								   // in the ui, I also made the header of the treeView not visible. 
	ui->treeView->expandAll();		// show all of the items at all levels 

	// When a user selects an item in the treeView have a signal sent to a method in this class, on_treeView_currentItemChanged
	connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &Window::on_treeView_currentItemChanged);

	// Set up initial sliders values
	ui->xTransSlider->setMinimum(-10); ui->xTransSlider->setMaximum(10); ui->xTransSlider->setSingleStep(1);
	ui->yTransSlider->setMinimum(-10); ui->yTransSlider->setMaximum(10); ui->yTransSlider->setSingleStep(1);
	ui->xScaleSlider->setMinimum(1); ui->xScaleSlider->setMaximum(10); ui->xScaleSlider->setSingleStep(1);
	ui->yScaleSlider->setMinimum(1); ui->yScaleSlider->setMaximum(10); ui->yScaleSlider->setSingleStep(1);
	ui->rotationSlider->setMinimum(-180); ui->rotationSlider->setMaximum(180); ui->yScaleSlider->setSingleStep(1);

	// When a user moves a slider, update the label value to show it. 
	connect(ui->xTransSlider, &QSlider::valueChanged, this, &Window::on_xTransSlider_valueChanged);
	connect(ui->yTransSlider, &QSlider::valueChanged, this, &Window::on_yTransSlider_valueChanged);
	connect(ui->xScaleSlider, &QSlider::valueChanged, this, &Window::on_xScaleSlider_valueChanged);
	connect(ui->yScaleSlider, &QSlider::valueChanged, this, &Window::on_yScaleSlider_valueChanged);
	connect(ui->rotationSlider, &QSlider::valueChanged, this, &Window::on_rotationSlider_valueChanged);

	currentNode = &n;
}

Window::~Window()
{
    delete ui;
}

void Window::keyPressEvent(QKeyEvent *e)  // If the user hits esc, close the program. 
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

// This methods automatically gets called when a different item is selected in the treeView.
// It is in a slot of this class and connected to a signal in the constructor of this class. 
void Window::on_treeView_currentItemChanged(const QItemSelection &selected, const QItemSelection &deselected)  
{
	Node* n;
	QModelIndex index = QModelIndex();
	QModelIndexList items = selected.indexes(); // It is possible to select more than one item. .

	foreach(index, items) {				// For each item selected look at the data. 
		n = index.data(Qt::UserRole + 1).value<Node*>();	// This is how to retrieve the data from an item
	}
	currentNode = n; // keep a pointer to the node that corresponds to the currently selected tree item. Makes things easier. 
	//BUG #1 - currentNode seems to have a null or empty value, I cannot access it

	/*Sets the sliders and slider value labels to the appropriate values*/
	//Translations
	ui->xTransSlider->setValue(ui->myGLWidget->c->getRootTrans()->getXTranslation());
	ui->yTransSlider->setValue(ui->myGLWidget->c->getRootTrans()->getYTranslation());
	ui->xTransSlideLabel->setNum(ui->myGLWidget->c->getRootTrans()->getXTranslation());
	ui->yTransSlideLabel->setNum(ui->myGLWidget->c->getRootTrans()->getYTranslation());
	//Scales
	ui->xScaleSlider->setValue(ui->myGLWidget->c->getRootTrans()->getXScalar());
	ui->yScaleSlider->setValue(ui->myGLWidget->c->getRootTrans()->getYScalar());
	ui->xScaleSlideLabel->setNum(ui->myGLWidget->c->getRootTrans()->getXScalar());
	ui->yScaleSlideLabel->setNum(ui->myGLWidget->c->getRootTrans()->getYScalar());
	//Rotation
	ui->rotationSlider->setValue(ui->myGLWidget->c->getRootTrans()->getRotationAngle());
	ui->rotationSlidelabel->setNum(ui->myGLWidget->c->getRootTrans()->getRotationAngle());

	//Makes the sliders visible
	ui->xTransSlider->setVisible(TRUE);
	ui->yTransSlider->setVisible(TRUE);
	ui->xScaleSlider->setVisible(TRUE);
	ui->yScaleSlider->setVisible(TRUE);
	ui->xScaleSlider->setVisible(TRUE);
	ui->rotationSlider->setVisible(TRUE);
}

// Called when the x translation slider is slid 
void Window::on_xTransSlider_valueChanged(int value){

	ui->xTransSlideLabel->setNum(value);// Set the label value

	/*translates the root transformation node in the x direction by 'value' and keeps the y translation value*/
	ui->myGLWidget->c->getRootTrans()->translate(value, ui->myGLWidget->c->getRootTrans()->getYTranslation());

	// Update the OpenGL window
	ui->myGLWidget->update();
}

// Called when the y translation slider is slid
void Window::on_yTransSlider_valueChanged(int value){

	ui->yTransSlideLabel->setNum(value);// Set the label value

	/*translates the root transformation node in the y direction by 'value'*/
	ui->myGLWidget->c->getRootTrans()->translate(0, value);

	// Update the OpenGL window
	ui->myGLWidget->update();
}

// Called when the x scale slider is slid 
void Window::on_xScaleSlider_valueChanged(int value) {

	ui->xScaleSlideLabel->setNum(value);// Set the label value

	/*scales the root transformation node in the x direction by 'value'*/
	ui->myGLWidget->c->getRootTrans()->scale(value/5, 0);
	//BUG #4 - value/5 does not seem to be redrawn, only when value/5 == a whole number

	// Update the OpenGL window
	ui->myGLWidget->update();
}

// Called when the y scale slider is slid 
void Window::on_yScaleSlider_valueChanged(int value) {

	ui->yScaleSlideLabel->setNum(value);// Set the label value

	/*scales the root transformation node in the y direction by 'value'*/
	ui->myGLWidget->c->getRootTrans()->scale(0, value/5);
	//BUG #4 - value/5 does not seem to be redrawn, only when value/5 == a whole number

	// Update the OpenGL window
	ui->myGLWidget->update();
}

// Called when the rotation slider is slid 
void Window::on_rotationSlider_valueChanged(int value) {

	ui->rotationSlidelabel->setNum(value);// Set the label value

	/*rotates the root transformation node by 'value'*/
	ui->myGLWidget->c->getRootTrans()->rotate(value);

	// Update the OpenGL window
	ui->myGLWidget->update();
}