#ifndef INDEXER_H
#define INDEXER_H

#include <QDialog>
#include <string>
#include <indextable.h>
#include <filequeue.h>
#include <QListWidgetItem>

namespace Ui {
  class indexer;
}

class indexer : public QDialog
{
  Q_OBJECT

public:
  explicit indexer(QWidget *parent = nullptr);
  ~indexer();
  void setPath(std::string);

private slots:
  void on_bIndex_clicked();

  void on_bFind_clicked();

  void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
  std::string key;
  Ui::indexer *ui;
indexTable * table = nullptr;
fileQueue * fq;
  std::string path;
};

#endif // INDEXER_H
