#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QChar>
#include "vismethod.h"
#include "../globalParameters.h"
#include "../datacontainer.h"

class HistogramData : public QObject
{
  Q_OBJECT

public:
  HistogramData(QString json_data, int* number_of_bins, DataContainer* data_container, GlobalVisParameters* m_global_vis_parameters);
  ~HistogramData();

  Q_INVOKABLE QString getData();
  Q_PROPERTY(QString json_string READ getData);

  Q_INVOKABLE int getNumberOfBins();
  Q_PROPERTY(int number_of_bins READ getNumberOfBins);



  /*Q_INVOKABLE void setSelectedNode(QString name);
  Q_INVOKABLE QString getSelectedNode();
  Q_PROPERTY(QString new_selected_node READ getSelectedNode);

  Q_INVOKABLE void setHighlightedStructure(const QString& name);
  Q_INVOKABLE void removeHighlightedStructure(const QString& name_to_remove);
  Q_INVOKABLE void removeAllHighlightedStructures();*/

  void setJsonString(QString json) { m_json_string = json; };
  void setHvgxID(int ID) { m_hvgx_id = ID; };

  int m_hvgx_id;
  int m_number_of_bins;
  QString m_json_string;

  GlobalVisParameters* m_global_vis_parameters;
  DataContainer* m_datacontainer;
};

class Histogram : public IVisMethod
{
public:
  Histogram(Histogram* distanceTree);
  Histogram(GlobalVisParameters* visparams, DataContainer* datacontainer, int number_of_bins);
  ~Histogram();

  QString createJSONString(int mito_id);

  //inherited functions
  QWebEngineView* initVisWidget(int ID, SpecificVisParameters params);
  QWebEngineView* getWebEngineView();
  bool            update();
  Histogram*      clone();

  VisType         getType();

private:

  HistogramData* data;
  DataContainer* m_datacontainer;

  QString m_title;
  QString m_index_filename = "web/histogram_index.html";

  GlobalVisParameters* m_global_vis_parameters;
  QWebEngineView* m_web_engine_view;
};

#endif

