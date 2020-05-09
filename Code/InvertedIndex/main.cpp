#include <iostream>
#include <QMutex>
#include <dirent.h>
#include <fstream>
#include <map>
#include <queue>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <folderdialog.h>
#include <QApplication>
#include <filequeue.h>
#include <indextable.h>
#include <util.h>

using namespace std;





int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  folderDialog dialog;
  dialog.show();
  dialog.exec();

  return (0);
}
