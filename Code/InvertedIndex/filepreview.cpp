#include "filepreview.h"

#include <fstream>
#include <iostream>

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

  bool phrase = keys->size() - 1;
  std::cout << phrase << std::endl;
  if (!ui->textEdit->isReadOnly()) {
    ui->textEdit->moveCursor(QTextCursor::Start);
    QColor color = QColor(Qt::yellow);
    QString RS = "[^a-z^A-Z]";
    for (std::string word : *keys) {
      RS += QString::fromStdString(word) + "[^a-z^A-Z]";
    }
    QRegExp RE = QRegExp(RS);

    RE.setCaseSensitivity(Qt::CaseInsensitive);
    RE.setMinimal(1);
    while (ui->textEdit->find(RE)) {
      extra.format.setBackground(color);

      extra.cursor = ui->textEdit->textCursor();
      if (phrase) {
      } else {
        extra.cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        extra.cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        extra.cursor.select(QTextCursor::WordUnderCursor);
      }
      extraSelections.append(extra);
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
