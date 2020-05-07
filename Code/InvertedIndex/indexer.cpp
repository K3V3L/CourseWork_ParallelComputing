#include "indexer.h"
#include "ui_indexer.h"

indexer::indexer(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::indexer)
{
  ui->setupUi(this);
}

indexer::~indexer()
{
  delete ui;
}
void indexer::setPath(std::string){
  this->path = path;
}
