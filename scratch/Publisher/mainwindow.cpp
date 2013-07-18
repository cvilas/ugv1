#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../Agent/AgentMessenger.h"
#include "../../AgentExamples/AgentExamples.h"
#include "../../AgentExamples/example_t.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _pUi(new Ui::MainWindow)
{
    _pUi->setupUi(this);

    _pUi->statusBar->addWidget(&_errorInfo);
    _pUi->urlEdit->setText(BUS_URL.c_str());
    _pUi->chnEdit->setText(EXAMPLE_CHANNEL.c_str());
}

MainWindow::~MainWindow()
{
    delete _pUi;
}

void MainWindow::on_publishBtn_clicked()
{
    Ugv1::AgentMessenger publisher( _pUi->urlEdit->text().toStdString() );

    if( !publisher.isGood() )
    {
        _errorInfo.setText("Error setting up publisher");
        return;
    }

    // the message
    example_t my_data;
    my_data.timestamp = 0;

    my_data.position[0] = 1;
    my_data.position[1] = 2;
    my_data.position[2] = 3;

    my_data.orientation[0] = 1;
    my_data.orientation[1] = 0;
    my_data.orientation[2] = 0;
    my_data.orientation[3] = 0;

    my_data.num_ranges = 15;
    my_data.ranges.resize(my_data.num_ranges);
    for(int i = 0; i < my_data.num_ranges; i++)
    {
        my_data.ranges[i] = i;
    }

    my_data.name = "example string";
    my_data.enabled = true;


    publisher.publish(_pUi->chnEdit->text().toStdString(), &my_data);
}
