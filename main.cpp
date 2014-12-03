//#include "widget.h"
#include <QApplication>
#include "xmlstream.h"


int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    Widget w;
//    w.show();
//    return a.exec();

    QString fileName="D:\\test.xml";
    XmlStream xs(fileName);
    xs.readFile ();
    xs.writerFile(fileName+QString(".out.xml"));
    return 0;
}
