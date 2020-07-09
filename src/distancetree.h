#ifndef DISTANCETREE_H
#define DISTANCETREE_H

#include <QWebChannel>
#include <QChar>
#include "vismethod.h"

class DistanceTreeData : public QObject 
{
  Q_OBJECT

public:
  DistanceTreeData(QString newickString);
  ~DistanceTreeData();

  Q_INVOKABLE QString getNewickString();
  Q_PROPERTY(QString newickString READ getNewickString);

  QString m_newickString;
};

class DistanceTree : public IVisMethod
{
public:
  DistanceTree(DataContainer* datacontainer);
  ~DistanceTree();

  QWebEngineView* getVisWidget(int ID);

private:

  DistanceTreeData* data;
  DataContainer* m_datacontainer;

  QString m_title;
  QString m_index_filename = "distancetree_index.html";

  QString createNewickString(int hvgxID, float distanceThreshold);
};

#endif
