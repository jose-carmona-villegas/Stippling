/**
 * @file main.cpp
 * @brief Application entry point
 *
 * @version 0.1
 * @author José Ignacio Carmona Villegas <joseicv@correo.ugr.es>
 * @date 7/October/2013
 *
 * @thanks Germán Arroyo Moreno <arroyo@ugr.es>
 *
 */


#include <QtGui>
#include <QApplication>
#include "mainwindow.h"



class StipplingApplication : public QApplication
{
public:
    StipplingApplication(int& argc, char ** argv) : QApplication(argc, argv)
    {

    }

    virtual ~StipplingApplication()
    {

    }

    // reimplemented from QApplication so we can throw exceptions in slots
    virtual bool notify(QObject * receiver, QEvent * event)
    {
        try
        {
            return QApplication::notify(receiver, event);
        }
        catch(std::exception& e)
        {
            qCritical() << endl << "Exception thrown:" << e.what() << endl;
        }
        return false;
    }
};


int main(int argc, char *argv[])
{
    StipplingApplication app(argc, argv);
    MainWindow window;

    window.showMaximized();
    //window.resize(1000,700);
    //window.show();

    return app.exec();
}
