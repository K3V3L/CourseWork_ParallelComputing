#include "filepreview.h"

#include <fstream>

#include "ui_filepreview.h"
#include "util.h"

filePreview::filePreview(QWidget* parent)
    : QDialog(parent), ui(new Ui::filePreview) {
  ui->setupUi(this);
}

filePreview::~filePreview() { delete ui; }

void filePreview::highlight(std::string key) {
  std::vector<std::string>* keys = util::strToWords(key);
  QTextEdit::ExtraSelection extra;
  QList<QTextEdit::ExtraSelection> extraSelections;

  for (std::string word : *keys) {
    if (!ui->textEdit->isReadOnly()) {
      ui->textEdit->moveCursor(QTextCursor::Start);
      QColor color = QColor(Qt::yellow);
      QRegExp RE = QRegExp("[^a-z^A-Z]" + QString::fromStdString(key) + "[^a-z^A-Z]");
      RE.setMinimal(1);
      while (ui->textEdit->find(RE)) {
        extra.format.setBackground(color);

        extra.cursor = ui->textEdit->textCursor();
        extraSelections.append(extra);
      }
    }
  }
  delete keys;
  ui->textEdit->setExtraSelections(extraSelections);
  ui->textEdit->moveCursor(QTextCursor::Start);
}

void filePreview::loadFile(std::string fileName, std::string key) {
  std::ifstream in(fileName);
  std::string contents((std::istreambuf_iterator<char>(in)),
                       std::istreambuf_iterator<char>());
  ui->textEdit->setText(QString::fromStdString(contents));
  if (!key.empty()) this->highlight(key);
  this->setWindowTitle("Preview: " + QString::fromStdString(fileName));
}
