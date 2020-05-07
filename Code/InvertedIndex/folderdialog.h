#ifndef FOLDERDIALOG_H
#define FOLDERDIALOG_H

#include <QDialog>

namespace Ui {
  class folderDialog;
}

class folderDialog : public QDialog
{
  Q_OBJECT

public:
  explicit folderDialog(QWidget *parent = nullptr);
  ~folderDialog();

private slots:
  void on_bOpen_accepted();

private:
  Ui::folderDialog *ui;
};

#endif // FOLDERDIALOG_H
