#include "folderdialog.h"
#include "ui_folderdialog.h"
#include "iostream"
#include <QString>
#include <QFileDialog>

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

void folderDialog::on_bOpen_accepted()
{
  QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  ui->lPath->setText(dir);
  ui->bOk->setDisabled(0);
}
