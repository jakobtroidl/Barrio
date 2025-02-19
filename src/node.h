#ifndef NODE_H
#define NODE_H

#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include "edge.h"

enum class Node_t { AXON, DENDRITE, BOUTON, SPINE, MITO, SYNAPSE, ASTROCYTE, GLYCOGEN, NONE };

class Edge;

class Node
{
public:
	Node(int nID, long idxID, float x, float y, float z, Node_t node_type = Node_t::NONE);
	~Node();
	int getID() { return m_nID; }
	long getIdxID() { return m_idxID; }

	Node_t getNodeType() { return m_node_t; }
	QVector3D get3DPosition() { return m_nodeXYZ; }
	void addEdge(Edge* e);
	std::map<int, Edge*> getAdjEdges() { return m_adjEdges; }

	QVector2D getProjectedVertex();
	// force based layout related functions
	QVector2D getLayoutedPosition() { return m_layouted; }
	void addToLayoutedPosition(QVector2D newPos);
	void resetLayout(QMatrix4x4 rotationMatrix);

	void addToForceSum(QVector2D newForce);
	QVector2D getForceSum() { return m_forceSum; }

	void resetForce();

	// hashmap
	void setHashMap(std::pair<int, int> cell);
	std::pair<int, int>  getHashMapCell();


	// GEM
	double getLocalTemp() { return m_localTemp; }
	void setLocalTemp(double tinit) { m_localTemp = tinit; }
	QVector2D getImpulse() { return impulse; }
	void updateImpulse(QVector2D newImpulse) { impulse = newImpulse; }
	void updateSkew(double newSkew) { skew_gauge = newSkew; }
	double getSkew() { return skew_gauge; }

	int getDeg() { return m_adjEdges.size(); }
private:
	int                     m_nID;      // hvgx ID
	long                     m_idxID;    // index in buffer

	// could belong to a skeleton or just one neurite node
	bool                    m_is_skeleton;

	QVector3D               m_nodeXYZ;  // original 3D position

	Node_t                  m_node_t;   // node type

	std::map<int, Edge*>    m_adjEdges;


	// hashmap
	std::pair<int, int>     m_hashMapCell;

	// GEM Algorithm
	QVector2D               impulse;  // impulse vector
	double                  m_localTemp;  // local temperature
	double                  skew_gauge;

	// force directed layout
	QVector2D               m_forceSum; // force to be applied on 2D position
	QVector2D               m_layouted; // 2D position after applying force

	QMatrix4x4              m_rotationMatrix;
};

#endif // NODE_H
