#pragma once

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <filesystem>

#include "mainopengl.h"
#include "glwidget.h"
#include "datacontainer.h"
#include "globalParameters.h"

struct RelatedWidgets {
	QWidget* vis_params;
	QGroupBox* overview;
};

class MainWidget : public QOpenGLWidget, MainOpenGL
{
	Q_OBJECT;
public:
	MainWidget(DataContainer* datacontainer, InputForm* input_form, QMap<int, QJsonObject>* vis_settings, RelatedWidgets relatedWidgets, QMap<int, QGroupBox*>* selection_box, QWidget* parent = 0);

	bool addWidgetGroup(int ID, bool isOverviewWidget, QMap<int, CameraSettings> cameraSettings);
	void updateOverviewWidget();

	Vis getVisInfo(int id);

	QMap<int, CameraSettings> allGLCameraSettings();

	bool deleteAllInfoVisWidgets();

	bool deleteAllWidgets(bool deleteGeneralInfoVisWidgets);

	void addStandardItem(int id, QList<QStandardItem*> items);

	bool addInfoVisWidget(int ID, QGroupBox* groupBox, IVisMethod* visMethod, QJsonObject settings);
	bool addGLWidget(int ID, QGroupBox* groupBox, bool isOverviewWidget, bool use_camera_settings, CameraSettings settings);

	bool useCameraSettings(QMap<int, CameraSettings> map, int ID);
	void setSilhouetteEnabled(bool enabled);

	void showSlice(bool showSlice);
	void show3DRenderings(bool show3DRenderings);

	void updateInfoVisViews();
	void updateGroupBoxStyle();

	void setNumberOfBinsForHistogram(int bins);
	void setColormap(QString colormap);
	void setVisMethod(Vis vis);
	void setColorcodingEnabled(bool enabled);

	void setupVisParams(Vis vis_method, QJsonObject settings);

	void clearWidget(QWidget* widget_to_clear);

	int getSelectedStructures();
	int setVisMethodBasedOnID();

	void highlightVisBoxes(int box_vis_id);

	void addStructure();
	void deleteStructure();


	void   on_opacity_slider_changed(int value);
	void   on_slice_position_slider_changed(int value);
	void   set_slice_position(int value);


	void initializeGL() override;
	void paintGL() override;
	void updateWidgets();
	void resizeGL(int width, int height) override;
	void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

	SelectedVisMethods getSelectedVisMethods();

protected:
	void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;
public slots:
	void OnWidgetClose();

	// params slots
	void histogram_slider_changed(int bins);
	void on_synapse_distance_slider_changed(int value);

private:
	DataContainer* m_datacontainer;
	InputForm* m_input_form; // bad

	std::map<int, QGroupBox*>   m_groupboxes;
	std::map<int, GLWidget*>    m_opengl_views;
	std::map<int, IVisMethod*>  m_infovis_views;
	std::map<int, QFrame*>      m_seperation_elements;

	QVector<int>                m_all_selected_structures;

	QMap<int, QList<QStandardItem*>> m_selected_standard_items;
	QMap<int, QJsonObject>* m_vis_settings;
	QMap<int, QGroupBox*>* m_selection_box;

	QGridLayout* m_main_layout;
	QGroupBox* m_overview;

	QWidget* m_vis_params_widget;

	SharedGLResources m_shared_resources;

	// shared resource variables
	QOpenGLBuffer m_mesh_index_vbo;
	QOpenGLBuffer m_mesh_vertex_vbo;
	QOpenGLBuffer m_skeleton_index_vbo;

	GLuint        m_mito_cell_distance_colormap;
	GLuint        m_image_stack_texture;

	QOpenGLBuffer m_slice_vertex_vbo;
	GLuint        m_slice_texture;

	AbstractionSpace* m_abstraction_space;

	SelectedVisMethods m_vis_methods;
	SpecificVisParameters m_specific_vis_parameters;

	QString m_selected_colormap = "0_colormap_tom.png";

	void initSharedVBOs();
	void initSharedSlice();
	void initSharedMeshVBOs();
	void initColormaps();
	void init3DVolumeTexture();

	void init_1D_texture(GLuint& texture, GLenum texture_unit, GLvoid* data, int size);
	void load3DTexturesFromRaw(QString path, GLuint& texture, GLenum texture_unit, int sizeX, int sizeY, int sizeZ);

	QImage* getColorMap(QString name);
	QString getColorValues(QImage* image);

	int m_lastID;
	int m_current_row = 0;
	int m_current_col = 0;

	int m_max_cols = 4;

	int m_number_of_selected_structures = 0;
	NumberOfEntities m_number_of_entities;

	QString m_low_detail_name = "Low Cardinality";
	QString m_medium_detail_name = "Medium Cardinality";
	QString m_high_detail_name = "High Cardinality";
};

