#include <folderdialog.h>
#include <QApplication>

using namespace std;

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  folderDialog dialog;
  dialog.show();
  dialog.exec();

  return (0);
}
