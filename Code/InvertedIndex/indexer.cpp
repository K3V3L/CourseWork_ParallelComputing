#include "indexer.h"
#include <QDebug>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>

#include "dirent.h"
#include "filepreview.h"
#include "filequeue.h"
#include "indextable.h"
#include "ui_indexer.h"
#include "util.h"

indexer::indexer(QWidget* parent) : QDialog(parent), ui(new Ui::indexer) {
  ui->setupUi(this);
}

indexer::~indexer() { delete ui; }

void indexer::setPath(std::string path) { this->path = path; }

void threadFunc(fileQueue* fq, indexTable* table, Ui::indexer* ui,
                std::atomic<unsigned>* counter, unsigned size, std::mutex* m) {
  std::multimap<std::string, tableEntry>* tmpTable =
      new std::multimap<std::string, tableEntry>;
  std::string* filename = nullptr;
  unsigned pos;
  while ((filename = fq->get())) {
    pos = 0;
    std::ifstream in(*filename);
    std::cout << *filename << std::endl;
    std::string contents((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
    std::vector<std::string>* words = util::strToWords(contents);
    tableEntry* entry = new tableEntry;
    for (std::string word : *words) {
      // std::cout << word <<  *filename << std::endl;
      entry->pos = pos;
      entry->path = *filename;
      tmpTable->insert(std::pair<std::string, tableEntry>(word, *entry));
      pos++;
    }
    delete entry;
    delete words;
    (*counter)++;
    std::cout << *counter / (float)size * 100 << std::endl;
    m->lock();
    ui->listWidget->addItem(QString::fromStdString(*filename));
    ui->progressBar->setValue(*counter / (float)size * 100);
    m->unlock();
    delete filename;
  }
  table->merge(tmpTable);
  delete tmpTable;
}

void indexer::on_bIndex_clicked() {
  ui->bIndex->setDisabled(1);
  this->fq = new fileQueue;
  unsigned threads = 1;
  DIR* dir;
  struct dirent* ent;
  if ((dir = opendir(this->path.c_str())) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir(dir)) != NULL) {
      if (ent->d_name[0] == '.') continue;
      //          std::cout << ent->d_name << std::endl;
      fq->add(std::string(this->path + "/" + ent->d_name));
    }
    closedir(dir);
  } else {
    qDebug() << "could not open directory";
  }

  if (this->table) delete this->table;
  this->table = new indexTable;
  std::atomic<unsigned>* counter = new std::atomic<unsigned>;
  *counter = 0;
  std::thread* indexers = new std::thread[threads];
  unsigned qSize = this->fq->getSize();
  std::mutex* m = new std::mutex;
  ui->listWidget->clear();
  std::chrono::steady_clock::time_point start =
      std::chrono::steady_clock::now();
  for (unsigned i = 0; i < threads; i++) {
    indexers[i] =
        std::thread(threadFunc, this->fq, this->table, ui, counter, qSize, m);
  }
  for (unsigned i = 0; i < threads; i++) {
    indexers[i].join();
  }
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << " threads : " << threads << " "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     start)
                   .count()
            << std::endl;

  ui->listWidget->setDisabled(0);
  ui->bFind->setDisabled(0);
  ui->lineEdit->setDisabled(0);
  delete counter;
  delete m;
  delete[] indexers;
  delete fq;
  ui->bIndex->setEnabled(1);
}

void indexer::on_bFind_clicked() {
  this->key = ui->lineEdit->text().toStdString();
  ui->listWidget->clear();
  ui->bFind->setDisabled(1);
  ui->lineEdit->setDisabled(1);
  std::vector<std::string>* results =
      this->table->get(ui->lineEdit->text().toStdString());
  std::cout << "Found " << results->size() << " matches" << std::endl;
  for (auto& i : *results) {
    std::cout << i << std::endl;
    ui->listWidget->addItem(QString::fromStdString(i));
  }
  delete results;
  ui->bFind->setEnabled(1);
  ui->lineEdit->setEnabled(1);
}

void indexer::on_listWidget_itemDoubleClicked(QListWidgetItem* item) {
  std::cout << "clicked : " << item->text().toStdString() << std::endl;
  filePreview preview;
  preview.loadFile(item->text().toStdString(), this->key);
  preview.exec();
}
