#ifndef INDEXER_H
#define INDEXER_H

#include <QDialog>
#include <string>
#include <indextable.h>
#include <filequeue.h>

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

private:
  Ui::indexer *ui;
indexTable * table;
fileQueue * fq;
  std::string path;
};

#endif // INDEXER_H
