#include <KApplication>
#include <KCmdLineArgs>
#include <KAboutData>

#include "MainWindow.h"

int main(int argc, char** argv) {
    KAboutData aboutData( "YoutubeSaver", 0,
      ki18n("Youtube Saver"), "1.0",
      ki18n("Saves Youtube vids loaded in Firefox"),
      KAboutData::License_GPL,
      ki18n("Copyright (c) 2011 Lim Yuen Hoe") );
    KCmdLineArgs::init( argc, argv, &aboutData );

    KApplication app;
    MainWindow m;
    m.resize(450,400);
    m.show();

    return app.exec();
}