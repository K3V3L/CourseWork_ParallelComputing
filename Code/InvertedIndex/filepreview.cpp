#include "filepreview.h"
#include "util.h"
#include "ui_filepreview.h"

#include <fstream>

filePreview::filePreview(QWidget* parent)
    : QDialog(parent), ui(new Ui::filePreview) {
  ui->setupUi(this);
}

filePreview::~filePreview() { delete ui; }
void filePreview::highlight(std::string key) {
  util::strToWords(key);
  QTextEdit::ExtraSelection extra;
  QList<QTextEdit::ExtraSelection> extraSelections;

  for (std::string word : *(util::strToWords(key))) {
    if (!ui->textEdit->isReadOnly()) {
      ui->textEdit->moveCursor(QTextCursor::Start);
      QColor color = QColor(Qt::gray).lighter(130);

      while (ui->textEdit->find(QString::fromStdString(key))) {
        extra.format.setBackground(color);

        extra.cursor = ui->textEdit->textCursor();
        extraSelections.append(extra);
      }
    }
  }

  ui->textEdit->setExtraSelections(extraSelections);
  ui->textEdit->textCursor().setPosition(QTextCursor::Start);
  extra.cursor.setPosition(QTextCursor::Start);
}

void filePreview::loadFile(std::string fileName, std::string key) {
  std::ifstream in(fileName);
  std::string contents((std::istreambuf_iterator<char>(in)),
                       std::istreambuf_iterator<char>());
  ui->textEdit->setText(QString::fromStdString(contents));
  if (!key.empty()) this->highlight(key);
}
