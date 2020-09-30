#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QWidget>
#include <QtOpenGL>

#include "mainopengl.h"
#include "graphmanager.h"
#include "abstractionspace.h"
#include "openglmanager.h"
#include "performancerate.h"
#include "inputform.h"
#include "globalParameters.h"


enum BufferNames { COUNTER_BUFFER = 0, LINKED_LIST_BUFFER };

struct ListNode {
  QVector4D color;
  GLfloat depth;
  GLuint next;
};

struct SharedGLResources
{
  QOpenGLBuffer* vertex_vbo;
  QOpenGLBuffer* mesh_index_vbo;
  QOpenGLBuffer* skeleton_index_vbo;

  int            index_count;

  GLuint*        mito_cell_distance_colormap;
  GLuint*        image_stack_volume;

  QOpenGLBuffer* slice_vertex_vbo;

  float         cell_opacity;
  bool          show_slice;
  float         slice_depth;

  QVector<int>*   highlighted_objects;
  QVector<int>*   highlighted_group_boxes;
};

class GLWidget : public QOpenGLWidget, MainOpenGL
{
  Q_OBJECT

public:
  GLWidget(int hvgx_id, SharedGLResources* resources, bool isOverviewWidget, QWidget* parent = 0);
  ~GLWidget();
  void init(DataContainer* data_container);

  OpenGLManager* getOpenGLManager() { return m_opengl_mngr; }
  DataContainer* getDataContainer() { return m_data_container; }
  float getZoomFactor() { return m_camera_distance; }

  int pickObject(QMouseEvent* event);
  void insertInTable(int);
  void getToggleCheckBox(std::map<Object_t, std::pair<int, int>>);

  void drawScene();

  void updateHighlightedSSBO();
  void updateVisibilitySSBO();
  
  void setVisibleStructures();

  void update_synapse_distance_threshold(double distance);

  void updateVisParameters();

  void resetCamera();
 


  void pass1();
  void pass2();
  void clearBuffers();

  void initMeshShaderStorage(int width, int height);
  void initSelectionFrameBuffer(int width, int height);

  int processSelection(float x, float y);



  void load3DTexturesFromRaw(QString path, GLuint& texture, GLenum texture_unit, int sizeX, int sizeY, int sizeZ);

public slots:
  void getSliderX(int value);
  void getSliderY(int value);
  void getIntervalID(int ID);

  void getActiveGraphTab(int);
  void reset_layouting(bool);

  void lockRotation2D();

  void getFilteredType(QString value, bool);
  void getFilteredID(QString value);

  void getFilterWithChildren(bool value);
  void getFilterWithParent(bool value);
  void getFilterWithSynapses(bool value);

  void getDepth(int d);

  void getNodeSizeEncoding(QString);
  void getColorEncoding(QString);
  void get2DtextureEncoding(QString);

  void getItemChanged(QListWidgetItem*);
  void getDoubleClickedTableView(QModelIndex);
  void highlightSelected(QModelIndex);

  void getDeletedHVGXID(int);
  void getFitlerButtonClicked(bool);
  void getResetFitlerButtonClicked(bool);
  void clearSelectedObjectsTable();

  void hideSelectedObjects();
  void getHVGXIDAtSelectedRow(int);
  void togglWeightedAstroCoverage();
  void getglycogenMappedSelectedState(QString, bool);
  void getProximityTypeState(QString, bool);

  void getFilteredListByProximity();
  void updateMinProximity(double);

  void getMappingTreeWidget(QTreeWidget*);
  void selectAllVisible();
  void updateProximitySSBO();

  void autoRotation();
  void startRotation();
  void prepareResize();

signals:
  void setAbstractionData(AbstractionSpace* space_instance);
  void setHoveredID(int);
  void setHoveredName(QString);
  void object_clicked(QList<QStandardItem*>);
  void clearRowsTable();
  void RemoveRowAt_GL(QModelIndex);
  void GetIDFrom(QModelIndex);
  void checkAllListWidget_GL();
  void updateFPS(QString);
  void updateFrameTime(QString);
  void signalCheckByType(std::map<QString, int>);

protected:
  void initializeGL() Q_DECL_OVERRIDE;
  void resizeGL(int w, int h) Q_DECL_OVERRIDE;
  void paintGL() Q_DECL_OVERRIDE;
  void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
  void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
  void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;
  void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

  void updateMVPAttrib(QOpenGLShaderProgram* program);

  void saveScreenshot();

  AbstractionSpace* m_2dspace;
  bool                                m_2D;

  /* mesh */
  DataContainer* m_data_container;
  OpenGLManager* m_opengl_mngr;

  struct WidgetUniforms               m_uniforms;

  /* matrices */
  QMatrix4x4                          m_projection;
  QMatrix4x4                          m_mMatrix;
  QMatrix4x4                          m_vMatrix;
  QMatrix4x4                          m_model_noRotation;
  QMatrix4x4                          m_rotationMatrix;

  QVector3D                           m_center;
  QVector3D                           m_eye;
  QVector3D                           m_cameraUpDirection;

  /* rotation */
  QPoint                              m_lastPos;
  double                              m_camera_distance;
  QQuaternion                         m_rotation;
  QVector3D                           m_rotationAxis;
  QVector3D                           m_translation;

  /* mouse pad */
  int                                 m_yaxis;
  int                                 m_xaxis;

  // force directed layout
  bool                                m_FDL_running;

  QTimer* m_refresh_timer;
  QTimer* m_lockRotation2D_timer;
  QTimer* m_auto_rotation_timer;

  std::set<int>                       m_selectedObjects;
  bool                                m_hover;
  bool                                m_hide_toggle;
  int                                 m_active_graph_tab;
  PerformanceRate                     m_performaceMeasure;
  QVector3D                           m_filterByProximityType;
  bool                                m_auto_rotate;
  float                               m_rot_ydiff;
  float                               m_xy_slice_z;

  SharedGLResources*                   m_shared_resources;

  int                                 m_selected_hvgx_id;

  QOpenGLShaderProgram*                m_mesh_program;
  QOpenGLVertexArrayObject             m_mesh_vao;

  QOpenGLShaderProgram*                m_slice_program;
  QOpenGLVertexArrayObject             m_slice_vao;

  std::vector<int>                    m_visible_structures; // list of hvgx ids with visible structures
  GLuint                              m_visibility_ssbo;
  GLuint                              m_highlighted_ssbo;

  GLuint                              m_selectionFrameBuffer;
  GLuint                              m_selectionRenderBuffer;

  /* order independent transparency vars*/
  QOpenGLVertexArrayObject            m_fsQuad_vao;
  GLuint                              oit_buffers[2], headPtrTex;
  GLuint                              mesh_shader_pass_idx_1, mesh_shader_pass_idx_2;
  GLuint                              clear_oit_buffers;
  GLint                               m_maxNodes;

  std::vector<GLuint>*                 headPtrClearBuf;

  int                                 m_width, m_height;
  bool                                m_init;
  bool                                m_is_overview_widget;

  double                              m_distance_threshold;

  //GLuint                              m_mito_cell_distance_colormap;


  GLuint        m_image_stack_texture;



};


#endif // GLWIDGET_H
