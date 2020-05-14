#include "folderdialog.h"

#include <QFileDialog>
#include <iostream>

#include "indexer.h"
#include "ui_folderdialog.h"

folderDialog::folderDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::folderDialog) {
  ui->setupUi(this);
}

folderDialog::~folderDialog() { delete ui; }

void folderDialog::on_bOpen_accepted() {
  QString dir = QFileDialog::getExistingDirectory(
      this, tr("Open Directory"), QDir::homePath(),
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  ui->lPath->setText(dir);
  ui->bOk->setDisabled(0);
}

void folderDialog::on_bOk_accepted() {
  indexer ind(this);
  this->close();
  ind.setPath(ui->lPath->text().toStdString());
  ind.exec();
}
