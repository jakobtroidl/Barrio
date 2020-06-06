#include "treemodel.h"
#include "treeitem.h"
#include <QFormLayout>

TreeModel::TreeModel(QWidget* parent, DataContainer* datacontainer, MainWidget* mainWidget): QTreeView(parent)
{
  connect(this, &QTreeView::doubleClicked, this, &TreeModel::selectItem);

  paramList = new QStandardItemModel();
  m_mainwidget = mainWidget;


  paramList->setColumnCount(2);
  paramList->setHeaderData(0, Qt::Horizontal, "Name");
  paramList->setHeaderData(1, Qt::Horizontal, "ID");
  this->setModel(paramList);

  std::map<int, Object*>* objects_map = datacontainer->getObjectsMapPtr();

  int i = 0;
  for (auto iter = objects_map->rbegin(); iter != objects_map->rend(); iter++)
  {
    Object* object_p = (*iter).second;

    if (object_p->getObjectType() != Object_t::MITO)
    {
      QString name(object_p->getName().c_str());
      QStandardItem* item_name = new QStandardItem(name);
      item_name->setEditable(false);

      int id = object_p->getHVGXID();
      QStandardItem* item_id = new QStandardItem(QString::number(id));
      item_id->setEditable(false);

      paramList->appendRow(QList<QStandardItem*>() << item_name << item_id);

      std::vector<Object*> children = object_p->getChildren();
      int j = 0;
      for (Object* child : children)
      {
        QString child_name(child->getName().c_str());
        QStandardItem* item_child_name = new QStandardItem(child_name);
        item_child_name->setEditable(false);

        int child_id = child->getHVGXID();
        QStandardItem* child_item_id = new QStandardItem(QString::number(child_id));
        child_item_id->setEditable(false);

        item_name->appendRow(QList<QStandardItem*>() << item_child_name << child_item_id);

        j++;
      }
      i++;
    }
  }

 
}

TreeModel::~TreeModel()
{

}



void TreeModel::selectItem(const QModelIndex& index)
{
  //extracting hvgx id
  int hvgx = index.siblingAtColumn(1).data().toInt();
  m_mainwidget->addGLWidget(hvgx);
  qDebug() << hvgx;
}

