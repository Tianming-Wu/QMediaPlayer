#include "container.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Container container;

    QUrl videourl = QUrl("");

    qsizetype cpos = 1;
    QStringList ar = app.arguments();
    while(cpos < argc) {
        if(ar[cpos] == "-f" || ar[cpos]== "--file") {
            if(cpos + 1 < argc) {
                if(videourl.isEmpty()) videourl = QUrl::fromLocalFile(ar[cpos+1]);
                else return 1;
                cpos += 2;
            } else return 1;
        } else if(ar[cpos] == "-u" || ar[cpos] == "--url") {
            if(cpos + 1 < argc) {
                if(videourl.isEmpty()) videourl = ar[cpos+1];
                else return 1;
                cpos += 2;
            }
        }
    }

    if(!videourl.isEmpty()) container.openVideo(videourl);
    qDebug() << "source: " << videourl;

    container.show();
    container.run();
    return app.exec();
}
