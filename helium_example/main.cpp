#include <QApplication>
#include <QThread>
#include "helium.h"

int main(int, char **)
{
    appInit();

    auto& window = createWindow();
    setUrlWindow(window, "https://google.com");
    showWindow(window);

    while(true) {
        processEvents();
        QThread::usleep(2);
    }

    return 0;
}
