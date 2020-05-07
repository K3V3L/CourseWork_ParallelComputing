#include "folderdialog.h"
#include "ui_folderdialog.h"

folderDialog::folderDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::folderDialog)
{
  ui->setupUi(this);
}

folderDialog::~folderDialog()
{
  delete ui;
}
