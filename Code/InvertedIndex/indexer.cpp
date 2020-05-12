#include "indexer.h"
#include "ui_indexer.h"
#include <dirent.h>
#include <filequeue.h>
#include <string>
#include <indextable.h>
#include <fstream>
#include <iostream>
#include <util.h>
#include <QDebug>
#include <atomic>
#include <thread>
#include <mutex>
#include <filepreview.h>

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
void indexer::setPath(std::string path){
  this->path = path;
}
void threadFunc(fileQueue *fq, indexTable *table, Ui::indexer *ui, std::atomic<unsigned> * counter, unsigned size, std::mutex * m) {
  std::string *filename = nullptr;
  unsigned pos;
  while ((filename = fq->get())) {
    pos = 0;
    std::ifstream in(*filename);
    std::cout << *filename << std::endl;
    std::string contents((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
    std::vector<std::string> *words = util::strToWords(contents);
    tableEntry *entry = new tableEntry;
    for (std::string word : *words) {
      // std::cout << word <<  *filename << std::endl;
      entry->pos = pos;
      entry->path = *filename;
      table->insert(word, entry);
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
}
void indexer::on_bIndex_clicked()
{
  ui->bIndex->setDisabled(1);
  this->fq = new fileQueue;
  unsigned threads = 4;
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(this->path.c_str())) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir(dir)) != NULL) {
      if (ent->d_name[0] == '.')
        continue;
      //          std::cout << ent->d_name << std::endl;
      fq->add(std::string(this->path + "/" + ent->d_name));
    }
    closedir(dir);
  } else {
    qDebug() << "could not open directory";
  }

  if (this->table)
      delete this->table;
  this->table = new indexTable;
  std::atomic<unsigned> * counter = new std::atomic<unsigned>;
  *counter=0;
  std::thread *indexers = new std::thread[threads];
  unsigned qSize = this->fq->getSize();
  std::mutex * m = new std::mutex;
  ui->listWidget->clear();
  for (unsigned i = 0; i < threads; i++) {
      indexers[i] = std::thread(threadFunc, this->fq, this->table, ui, counter, qSize, m);
    }
  for (unsigned i = 0; i < threads; i++) {
      indexers[i].join();
    }
    ui->listWidget->setDisabled(0);
  ui->bFind->setDisabled(0);
  ui->lineEdit->setDisabled(0);
  delete counter;
  delete m;
  delete[] indexers;
  delete fq;
  ui->bIndex->setEnabled(1);
}

void indexer::on_bFind_clicked()
{
  this->key = ui->lineEdit->text().toStdString();
  ui->listWidget->clear();
   ui->bFind->setDisabled(1);
   ui->lineEdit->setDisabled(1);
   std::vector<std::string> *results = this->table->get(ui->lineEdit->text().toStdString());
  std::cout << "Found " << results->size() << " matches" << std::endl;
  for (auto &i : *results)
    {
    std::cout << i << std::endl;
    ui->listWidget->addItem(QString::fromStdString(i));
    }
  delete results;
  ui->bFind->setEnabled(1);
  ui->lineEdit->setEnabled(1);
}

void indexer::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
   std::cout << "clicked : " << item->text().toStdString() << std::endl;
   filePreview preview;
   preview.loadFile(item->text().toStdString(), this->key);
   preview.exec();

//   FinalWIndow DrawWindow;
//       DrawWindow.setMode(g_mode);
//       DrawWindow.setNumber(n);
//       DrawWindow.setData(rects);
//       this->close();
//       DrawWindow.exec();
}
