#include "distancematrix.h"

DistanceMatrix::DistanceMatrix(DistanceMatrix* matrix)
{
	m_datacontainer = matrix->m_datacontainer;
	m_global_vis_parameters = matrix->m_global_vis_parameters;
}

DistanceMatrix::DistanceMatrix(GlobalVisParameters* visparams, DataContainer* datacontainer)
{
	m_datacontainer = datacontainer;
	m_global_vis_parameters = visparams;
}

DistanceMatrix::~DistanceMatrix()
{
	// do nothing
}

QWebEngineView* DistanceMatrix::initVisWidget(int ID, SpecificVisParameters params)
{
	m_settings = params.settings;

	QString json = getJSONString(&m_global_vis_parameters->selected_objects, m_global_vis_parameters->distance_threshold);
	data = new DistanceMatrixData(json, m_datacontainer, m_global_vis_parameters);

	m_web_engine_view = new QWebEngineView();
	QWebChannel* channel = new QWebChannel(m_web_engine_view->page());
	m_web_engine_view->page()->setWebChannel(channel);
	channel->registerObject(QStringLiteral("distancematrix_data"), data);
	m_web_engine_view->load(getHTMLPath(m_index_filename));

	return m_web_engine_view;
}

bool DistanceMatrix::update()
{
	QString json = getJSONString(&m_global_vis_parameters->selected_objects, m_global_vis_parameters->distance_threshold);
	data->setJsonString(json);
	return true;
}

QWebEngineView* DistanceMatrix::getWebEngineView()
{
	return m_web_engine_view;
}

DistanceMatrix* DistanceMatrix::clone()
{
	return new DistanceMatrix(this);
}

bool DistanceMatrix::update_needed()
{
	return true;
}

VisType DistanceMatrix::getType()
{
	return VisType::DISTANCE_MATRIX;
}

void DistanceMatrix::setSpecificVisParameters(SpecificVisParameters params)
{
}



QString DistanceMatrix::getJSONString(QList<int>* selected_mitos, double distanceThreshold)
{
	QString related_synapses = "related-synapses";
	QString surrounding_synapses = "surrounding-synapses";
	QString synapse_param = m_settings.value("params").toString();

	QJsonArray json;
	std::map<int, Object*>* objects = m_datacontainer->getObjectsMapPtr();
	std::vector<Object*> synapses = m_datacontainer->getObjectsByType(Object_t::SYNAPSE);

	// search all distances to respective synapses to mito
	QList<int> selected_synapses;
	for each (int mito_id in *selected_mitos)
	{
		Object* mito = objects->at(mito_id);

		int cell_id = mito->getParentID();
		Object* cell = objects->at(cell_id);
		std::map<int, double>* distance_map = mito->get_distance_map_ptr();

		if (synapse_param == related_synapses)
		{
			for (int syn_id : *cell->getSynapseIDs())
			{
				double distance_to_mito = distance_map->at(syn_id);
				if (!selected_synapses.contains(syn_id))
				{
					selected_synapses.append(syn_id);
				}
			}
		}
		else if (synapse_param == surrounding_synapses)
		{
			for (auto& syn : synapses)
			{
				double distance_to_mito = distance_map->at(syn->getHVGXID());
				if (distance_to_mito < distanceThreshold && !selected_synapses.contains(syn->getHVGXID()))
				{
					selected_synapses.append(syn->getHVGXID());
				}
			}
		}
	}

	// build json document
	for each (int mito_id in *selected_mitos)
	{
		Object* mito = objects->at(mito_id);

		int cell_id = mito->getParentID();
		Object* cell = objects->at(cell_id);

		QJsonObject mito_object;
		std::map<int, double>* distance_map = mito->get_distance_map_ptr();
		mito_object.insert("name", QJsonValue::fromVariant(mito->getName().c_str()));

		QJsonArray syn_array;
		for each (int syn_id in selected_synapses)
		{
			Object* syn = objects->at(syn_id);
			double distance_to_mito = distance_map->at(syn_id);

			QJsonObject syn_object;
			syn_object.insert("name", QJsonValue::fromVariant(syn->getName().c_str()));

			if (synapse_param == related_synapses)
			{
				if (cell->hasSynapse(syn_id))
				{
					syn_object.insert("distance", QJsonValue::fromVariant(distance_to_mito));
				}
				else
				{
					syn_object.insert("distance", QJsonValue::fromVariant(100.0));
				}
			}
			else if (synapse_param == surrounding_synapses)
			{
				syn_object.insert("distance", QJsonValue::fromVariant(distance_to_mito));
			}


			syn_array.push_back(syn_object);

		}
		mito_object.insert("synapses", syn_array);
		json.push_back(mito_object);
	}
	QJsonDocument doc(json);
	return doc.toJson(QJsonDocument::Compact);
}

DistanceMatrixData::DistanceMatrixData(QString json_string, DataContainer* datacontainer, GlobalVisParameters* visparameters)
{
	m_json_string = json_string;

	m_datacontainer = datacontainer;
	m_global_vis_parameters = visparameters;
}

DistanceMatrixData::~DistanceMatrixData()
{
	// do nothing for now
}

Q_INVOKABLE QString DistanceMatrixData::getData()
{
	return Q_INVOKABLE m_json_string;
}

Q_INVOKABLE void DistanceMatrixData::setHighlightedStructure(const QString& name)
{
	int hvgx_id = m_datacontainer->getIndexByName(name);
	if (!m_global_vis_parameters->highlighted_objects.contains(hvgx_id))
	{
		m_global_vis_parameters->highlighted_objects.append(hvgx_id);
	}
	m_global_vis_parameters->needs_update = true;
	return Q_INVOKABLE void();
}

Q_INVOKABLE void DistanceMatrixData::removeHighlightedStructure(const QString& name_to_remove)
{
	int hvgx_id = m_datacontainer->getIndexByName(name_to_remove);
	QVector<int>* highlighted = &m_global_vis_parameters->highlighted_objects;
	if (highlighted->contains(hvgx_id))
	{
		QMutableVectorIterator<int> it(*highlighted);
		while (it.hasNext())
		{
			if (it.next() == hvgx_id) {
				it.remove();
			}
		}
	}
	m_global_vis_parameters->needs_update = true;
	return Q_INVOKABLE void();
}

Q_INVOKABLE void DistanceMatrixData::setHighlightedFrame(const QString& name)
{

	int hvgx = m_datacontainer->getIndexByName(name);
	if (!m_global_vis_parameters->highlighted_group_boxes.contains(hvgx))
	{
		m_global_vis_parameters->highlighted_group_boxes.append(hvgx);
	}
	m_global_vis_parameters->needs_update = true;
	return Q_INVOKABLE void();
}

Q_INVOKABLE void DistanceMatrixData::removeHighlightedFrame(const QString& name_to_remove)
{

	int hvgx_id = m_datacontainer->getIndexByName(name_to_remove);
	QVector<int>* highlighted = &m_global_vis_parameters->highlighted_group_boxes;
	if (highlighted->contains(hvgx_id))
	{
		QMutableVectorIterator<int> it(*highlighted);
		while (it.hasNext())
		{
			if (it.next() == hvgx_id) {
				it.remove();
			}
		}
	}
	m_global_vis_parameters->needs_update = true;
	return Q_INVOKABLE void();
}
