#ifndef FILEPREVIEW_H
#define FILEPREVIEW_H

#include <QDialog>
#include <string>

namespace Ui {
class filePreview;
}

class filePreview : public QDialog {
  Q_OBJECT

 public:
  explicit filePreview(QWidget* parent = nullptr);
  void loadFile(std::string fileName, std::string key);
  void highlight(std::string key);
  ~filePreview();

 private:
  Ui::filePreview* ui;
};

#endif  // FILEPREVIEW_H
