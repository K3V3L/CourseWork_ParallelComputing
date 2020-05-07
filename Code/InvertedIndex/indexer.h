#ifndef INDEXER_H
#define INDEXER_H

#include <QDialog>
#include <string>

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

private:
  Ui::indexer *ui;
  std::string path;
};

#endif // INDEXER_H
