#include <iostream>
#include <dirent.h>
#include <filequeue.h>
#include <folderdialog.h>
#include <indextable.h>
#include <stdio.h>
#include <string.h>
#include <util.h>

#include <QApplication>
#include <QMutex>
#include <fstream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  folderDialog dialog;
  dialog.show();
  dialog.exec();

  return (0);
}
