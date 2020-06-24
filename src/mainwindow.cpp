#include "glwidget.h"
#include "glycogenanalysismanager.h"
#include "glycogencluster.h"
#include "object.h"
#include "openglmanager.h"

#include "mainwindow.h"
#include <chrono>
#include <map>
#include <QPixmap>
#include "ui_mainwindow.h"

namespace fs = std::filesystem;

//------------------------------------------------------
//
MainWindow::MainWindow(QWidget* parent, InputForm* input_form) :
  QMainWindow(parent),
  mainwindow_ui(new Ui::MainWindow)
{
  mainwindow_ui->setupUi(this);
  //getGLWidget()->init(input_form);

  m_data_container = new DataContainer(input_form);

  m_mainWidget = new MainWidget(m_data_container, input_form, mainwindow_ui->centralwidget);
  mainwindow_ui->gridLayout_5->addWidget(m_mainWidget, 0, 0);
 
  setupSignalsNSlots();

  m_currentSelectedCluster = 0;
  m_clusters = 0;

  QSlider* synapse_threshold_slider = mainwindow_ui->horizontalSlider_3;
  connect(synapse_threshold_slider, SIGNAL(valueChanged(int)), m_mainWidget, SLOT(on_synapse_distance_slider_changed(int)));
 
  m_treemodel = new TreeModel(mainwindow_ui->groupBox_16, m_data_container, m_mainWidget);
  mainwindow_ui->verticalLayout_15->addWidget(m_treemodel);

  
}

//------------------------------------------------------
//
MainWindow::~MainWindow()
{
  delete mainwindow_ui;
  delete tableView;
  delete m_currentSelectedCluster;
  delete m_clusters;
}

//------------------------------------------------------
//
void MainWindow::setupSignalsNSlots()
{
  QObject::connect(this, SIGNAL(update_glycogen_clustering_timing(QString)),
    mainwindow_ui->glycogen_clustering_ms, SLOT(setText(QString)));


  QObject::connect(this, SIGNAL(update_glycogen_cluter_mapping_timing(QString)),
    mainwindow_ui->glycogen_cluster_mapping_ms, SLOT(setText(QString)));

  QObject::connect(this, SIGNAL(update_glycogen_granules_mapping_timing(QString)),
    mainwindow_ui->glycogen_granules_mapping_ms, SLOT(setText(QString)));

  QObject::connect(mainwindow_ui->displayVisButton, SIGNAL(released()), this, SLOT(on_display_vis_button_clicked()));

  
}

void MainWindow::onAddWebWidget()
{
}

//------------------------------------------------------
//
void MainWindow::on_object_clicked(QList<QStandardItem*> items)
{
  tableView->appendRow(items);
}

//------------------------------------------------------
//
void MainWindow::clearTable()
{
  tableView->removeRows(0, tableView->rowCount(), QModelIndex());

}

//------------------------------------------------------
//
void MainWindow::RemoveRowAt(QModelIndex selectedIndex)
{
  QModelIndex hvgxID_index = tableView->index(selectedIndex.row(), 0);
  QVariant hvgxID_item = tableView->data(hvgxID_index);
  int hvgxID = hvgxID_item.toInt();
  getDeletedData(hvgxID);
  tableView->removeRows(selectedIndex.row(), 1, QModelIndex());
}

void MainWindow::getIDAt(QModelIndex selectedIndex)
{
  qDebug() << "getIDAt";
  QModelIndex hvgxID_index = tableView->index(selectedIndex.row(), 0);
  QVariant hvgxID_item = tableView->data(hvgxID_index);
  int hvgxID = hvgxID_item.toInt();

  getIDAtSelectedRow(hvgxID);
}

//------------------------------------------------------
//
void MainWindow::on_mitoRadioButton_toggled(bool checked)
{
  mainwindow_ui->glycogenMapToBoutonsCheckBox->setEnabled(!checked);
  mainwindow_ui->glycogenMapToSpinesCheckBox->setEnabled(!checked);
}

//------------------------------------------------------
//
void MainWindow::on_boutonsSpinesRadioButton_toggled(bool checked)
{
  mainwindow_ui->glycogenMapToBoutonsCheckBox->setEnabled(checked);
  mainwindow_ui->glycogenMapToSpinesCheckBox->setEnabled(checked);
}

//------------------------------------------------------
//
void MainWindow::getPath(QString pathLabel)
{
  mainwindow_ui->listPaths->addItem(pathLabel);
}

//------------------------------------------------------
//
void MainWindow::getSelectedPathIndex(QModelIndex index)
{
  QString pathLabel = mainwindow_ui->listPaths->item(index.row())->text();
  signalPathLabel(pathLabel);
}

//------------------------------------------------------
//
void MainWindow::deleteSelectedPath(QModelIndex index)
{
  // return label to mousepad to remove it from paths list
  QListWidgetItem* item = mainwindow_ui->listPaths->item(index.row());
  QString pathLabel = item->text();
  delete mainwindow_ui->listPaths->takeItem(index.row());
  signalDeletedPathLabel(pathLabel);
}



//------------------------------------------------------
//
void  MainWindow::on_glycogenMappingTreeWidget_itemChanged(QTreeWidgetItem* item, int column)
{
  bool state = true;
  if (item->checkState(0) == Qt::Unchecked)
    state = false;

  QString ID_str = item->text(0); // highlighted immediately
  glycogenMappedSelectedState(ID_str, state);
}

//------------------------------------------------------
//
void MainWindow::on_filterByProximityListWidget_itemChanged(QListWidgetItem* item)
{
  bool flag = true;
  if (item->checkState() == Qt::Unchecked)
    flag = false;

  signalProximityTypeState(item->text(), flag);
}

//------------------------------------------------------
//
void MainWindow::on_listWidget_itemChanged(QListWidgetItem*)
{
  signalMappingTreeWidget(mainwindow_ui->glycogenMappingTreeWidget);
}

void MainWindow::on_display_vis_button_clicked()
{
  bool axons = mainwindow_ui->checkBox->isChecked();
  bool dends = mainwindow_ui->checkBox_2->isChecked();
  bool mitos = mainwindow_ui->checkBox_3->isChecked();
  bool syn = mainwindow_ui->checkBox_5->isChecked();

  bool sliceView = mainwindow_ui->checkBox_12->isChecked();

  VisConfiguration config;
  config.axons = axons;
  config.dends = dends;
  config.mitos = mitos;
  config.syn = syn;
  config.sliceView = sliceView;

  m_mainWidget->setupMainWidget(config);
}
