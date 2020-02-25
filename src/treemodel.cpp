#include "treeitem.h"
#include "treemodel.h"
#include <QStringList>
#include "GWidget.h"
#include "node.h"
#include "edge.h"
#include "entity.h"
#include "qdebug.h" //remove it
#include "qjsonobject.h"

TreeModel::TreeModel(GraphWidget *parent ) : QAbstractItemModel(parent)
{
		Parent = parent;
        rootItem = new TreeItem("Root", parent, TreeItem::Type::Root);// , 0);
        blocks = new TreeItem("Blocks", parent, TreeItem::Type::NodesBranch);//, rootItem);
        connectors = new TreeItem("Connectors", parent, TreeItem::Type::EdgesBranch);//, rootItem);

#ifdef Aquifolium
        QList<TreeItem*> rootNodes, settingsNodes;
        QJsonObject jsonobj = parent->jsondocentities.object();
        settings = new TreeItem("Settings", parent, TreeItem::Type::SettingsBranch);//, rootItem);
        //sources = new TreeItem("Sources", parent, TreeItem::Type::Sources);//, rootItem);
        rootNodes << settings << blocks << connectors;// << sources;
        rootItem->addChild(rootNodes);
		for (int i = 0; i < Parent->mainWindow->GetSystem()->QGetAllCategoryTypes().size(); i++)
		{
			if (!rootContains(Parent->mainWindow->GetSystem()->QGetAllCategoryTypes()[i]))
			{
				TreeItem* treeitem = entityParentItemfromType(Parent->mainWindow->GetSystem()->QGetAllCategoryTypes()[i]);
				treeItems.insert(Parent->mainWindow->GetSystem()->QGetAllCategoryTypes()[i], treeitem);
				rootItem->addChild(treeitem);
			}
		}

#endif

        if (Parent->logW)
            Parent->log("Tree model created.");
}

bool TreeModel::rootContains(QString s)
{
	for (int i = 0; i < rootItem->childCount(); i++)
		if (rootItem->child(i)->Name() == s) return true; 
	return false; 
}

void TreeModel::refresh(QString part)
{/*
	//Parent = parent;
	//rootItem = new TreeItem("Anaconda", parent, TreeItem::Type::Root, 0);
	//settings = new TreeItem("Settings", parent, TreeItem::Type::SettingsBranch, rootItem);
	//globalSettings = new TreeItem("Global Settings", parent, TreeItem::Type::Item, settings);
	//solver = new TreeItem("Solver", parent, TreeItem::Type::Item, settings);
	//blocks = new TreeItem("Blocks", parent, TreeItem::Type::NodesBranch, rootItem);
	//connectors = new TreeItem("Connectors", parent, TreeItem::Type::EdgesBranch, rootItem);
	//waterQuality = new TreeItem("Water Quality", parent, TreeItem::Type::WaterQualityBranch, rootItem);
	//reactions = new TreeItem("Reactions", parent, TreeItem::Type::ReactionsBranch, waterQuality);
	//reactionNetwork = new TreeItem("Reaction Network", parent, TreeItem::Type::ReactionNetworkItem, reactions);
	//inverseModeling = new TreeItem("Inverse Modeling", parent, TreeItem::Type::InverseModelingBranch, rootItem);
	//GA = new TreeItem("Genetic Algorithm", parent, TreeItem::Type::Item, inverseModeling);
	//MCMC = new TreeItem("Markov Chain Monte Carlo", parent, TreeItem::Type::Item, inverseModeling);

	Parent->log("Tree Model refresh.");
	TreeItem* item;
	QString name;
	item = particle; name = "Particle";
	if (part.toLower() == "all" || part.toLower() == name.toLower())
	{
		qDeleteAll(item->childItems);
		item->childItems.clear();
		for each (QString e in Parent->EntityNames(name))
			new TreeItem(Parent->entity(e), item);
	}

	item = constituent; name = "Constituent";
	if (part.toLower() == "all" || part.toLower() == name.toLower())
	{
		qDeleteAll(item->childItems);
		item->childItems.clear();
		for each (QString e in Parent->EntityNames(name))
			new TreeItem(Parent->entity(e), item);
	}

	item = buildUp; name = "Build-Up";
	if (part.toLower() == "all" || part.toLower() == name.toLower())
	{
		qDeleteAll(item->children);
		item->children.clear();
		for each (QString e in Parent->EntityNames(name))
			new TreeItem(Parent->entity(e), item);
	}

	item = extrenalFlux; name = "External flux";
	if (part.toLower() == "all" || part.toLower() == name.toLower())
	{
		qDeleteAll(item->children);
		item->children.clear();
		for each (QString e in Parent->EntityNames(name))
			new TreeItem(Parent->entity(e), item);
	}

	item = reactionParameter; name = "Reaction parameter";
	if (part.toLower() == "all" || part.toLower() == name.toLower())
	{
		qDeleteAll(item->children);
		item->children.clear();
		for each (QString e in Parent->EntityNames(name))
			new TreeItem(Parent->entity(e), item);
	}

	item = parameter; name = "Parameter";
	if (part.toLower() == "all" || part.toLower() == name.toLower())
	{
		qDeleteAll(item->children);
		item->children.clear();
		for each (QString e in Parent->EntityNames(name))
			new TreeItem(Parent->entity(e), item);
	}

	item = observed; name = "Observed";
	if (part.toLower() == "all" || part.toLower() == name.toLower())
	{
		qDeleteAll(item->children);
		item->children.clear();
		for each (QString e in Parent->EntityNames(name))
			new TreeItem(Parent->entity(e), item);
	}

	update();*/
}

TreeModel::~TreeModel()
{
	delete rootItem;
}

void TreeModel::addChildFromMenu(const QString name, QModelIndex *parentIndex)
{
    TreeItem *parent = nullptr;
#ifdef Aquifolium
    if (name == "Settings")
        parent = this->settings;

#endif
#ifdef GIFMOD
	if (name == "Controller")
		parent = this->controller;
	if (name == "Sensor")
		parent = this->sensor;
	if (name == "Objective function")
		parent = this->objectiveFunction;
#endif
#ifdef GIFMod
    if (name == "Constituent")
		parent = this->constituent;
	if (name == "Particle")
		parent = this->particle;
	if (name == "Build-up")
		parent = this->buildUp;
	if (name == "External flux")
		parent = this->extrenalFlux;
	if (name == "Reaction parameter")
		parent = this->reactionParameter;
	if (name == "Parameter")
		parent = this->parameter;
	if (name == "Observation")
		parent = this->observed;
	if (name == "Evapotranspiration")
		parent = this->evapotranspiration;
	if (name == "Wells")
		parent = this->wells;
	if (name == "Tracers")
		parent = this->tracers;
	if (parent == 0) return;
#endif
	Entity* newEntity = new Entity(name, "No Name", parent->gWidget);
//	parent->addChild(new TreeItem(newEntity));
	beginInsertRows(*parentIndex, 0,1);
	endInsertRows();
	Parent->log(QString("One %1 created by tree view command.").arg(name));
	//new TreeItem(e->Name(), parentFromMenu->gWidget, TreeItem::Type::EntityItem, parentFromMenu);
}

void TreeModel::prepareToAddFromMenu(const QString  name)
{
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role == Role::TreeItemType)
	{
		TreeItem *item = itemFromIndex(index);
		return item->type;
	}
	if (role == Role::TreeParentItemType)
	{
		TreeItem *item = itemFromIndex(index);
		return singularform(item->parent()->Name());
	}

	if ((role != Qt::DisplayRole) && (role != Qt::EditRole))
		return QVariant();
	
	TreeItem *item = itemFromIndex(index);
	reset;
	return QString("%1").arg(item->data(index.column()).toString());// .arg(rowCount(index)); // Name();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
        return nullptr;
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled; 
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QString("Header %1").arg(section);

	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	TreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	TreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
	TreeItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

TreeItem* TreeModel::itemFromIndex(const QModelIndex &index) const
{
	if (index.isValid())
		return static_cast<TreeItem *>(index.internalPointer());
	else
        return nullptr;// rootItem;
}

bool TreeModel::hasChildren(const QModelIndex & parent) const
{
	return true;// itemFromIndex(parent)->childCount();
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}

void TreeModel::deleteNode(int row, QModelIndex &parent, QString name)
{
	Parent->log(QString("Deleting %1 block by tree view command.").arg(name));
	beginRemoveRows(parent, row, row);
	blocks->deleteChild(name);
	Parent->deleteNode(name);
//	blocks->Children();
//	connectors->Children();
	endRemoveRows();
}
void TreeModel::deleteNode(Node *node)
{
	Parent->log(QString("Deleting %1 block by tree view command.").arg(node->Name()));
	TreeItem *branch;
#ifdef Aquifolium
    branch = blocks;
#endif
#ifdef GIFMOD
	branch = blocks;
#endif
#ifdef GWA
	if (node->objectType.ObjectType == "Well")
		branch = wells;
	else if (node->objectType.ObjectType == "Tracer")
		branch = tracers;
#endif
	QModelIndex parent = createIndex(rootItem->indexOf(branch), 0, rootItem);
	QString name = node->Name();
	int row = branch->indexOf(name);
	QList<Edge*> edges = node->edgeList;
    foreach (Edge *edge , edges)
		deleteEdge(edge);
	beginRemoveRows(parent, row, row);
	branch->deleteChild(name);
	Parent->deleteNode(name);
	endRemoveRows();
}
void TreeModel::deleteEdge(int row, QModelIndex &parent, QString name)
{
	Parent->log(QString("Deleting %1 connector by tree view command.").arg(name));
	beginRemoveRows(parent, row, row);
	connectors->deleteChild(name);
	Parent->deleteEdge(name);
//	connectors->Children();
	endRemoveRows();
}
void TreeModel::deleteEdge(QString edgeName)
{
	QString name = edgeName;
	Parent->log(QString("Deleting %1 connector by tree view command.").arg(name));
	QModelIndex parent = createIndex(rootItem->indexOf(connectors), 0, rootItem);
	int row = connectors->indexOf(name);
	beginRemoveRows(parent, row, row);
	connectors->deleteChild(name);
	Parent->deleteEdge(name);
	//	connectors->Children();
	endRemoveRows();
}
void TreeModel::deleteEdge(Edge *edge)
{
	QString name = edge->Name();
	Parent->log(QString("Deleting %1 connector by tree view command.").arg(name));
	QModelIndex parent = createIndex(rootItem->indexOf(connectors), 0, rootItem);
	int row = connectors->indexOf(name);
	beginRemoveRows(parent, row, row);
	connectors->deleteChild(name);
	Parent->deleteEdge(name);
	//	connectors->Children();
	endRemoveRows();
}
void TreeModel::deleteEntity(Entity *entity)
{
	QString name = entity->Name();
	QString type = entity->objectType.ObjectType;

	Parent->log(QString("Deleting %1 %2 by tree view command.").arg(name).arg(type));
	//createIndex(itemFromIndex(index)->parent->Children().indexOf(itemFromIndex(index)), 0, index.internalId());
	TreeItem *branch = entityParentItemfromType(type);
	if (!branch)
	{
		Parent->log(QString("Error in deleting %1 %2 by tree view command.").arg(name).arg(type));
		return;
	}
	QModelIndex parent = createIndex(rootItem->indexOf(branch), 0, branch->parent());
	int row = branch->indexOf(name);
	beginRemoveRows(parent, row, row);
//	TreeItem *branch = entityParentItemfromType(type);
	branch->deleteChild(name);
	Parent->deleteEntity(name);
	//	refresh(type);
	endRemoveRows();
}
void TreeModel::deleteEntity(int row, QModelIndex &parent, QString type, QString name)
{
	Parent->log(QString("Deleting %1 %2 by tree view command.").arg(name).arg(type));
	//createIndex(itemFromIndex(index)->parent->Children().indexOf(itemFromIndex(index)), 0, index.internalId());
	beginRemoveRows(parent, row, row);
	TreeItem *branch = entityParentItemfromType(type);
	branch->deleteChild(name);
	Parent->deleteEntity(name);
//	refresh(type);
	endRemoveRows();
}

QString TreeModel::singularform(QString name) const
{
	if (name.right(2) != "es" && name.right(1) == "s") return name.left(name.length() - 1);
	if (name == "External fluxes") return "External flux";
	if (name == "Observations") return "Observed";
	if (name == "Particles") return "Particle";
	return name;
}

void TreeModel::add(Node *node)
{
    Parent->log("Adding one block to tree view.");
    QModelIndex parent = createIndex(rootItem->indexOf(blocks), 0, rootItem);
    int row = blocks->childCount();
    beginInsertRows(parent, row, row);
    blocks->addChild(new TreeItem(node));
    endInsertRows();
    reset;
#ifdef GIFMOD
	Parent->log("Adding one block to tree view.");
	QModelIndex parent = createIndex(rootItem->indexOf(blocks), 0, rootItem);
	int row = blocks->childCount();
	beginInsertRows(parent, row, row);
	blocks->addChild(new TreeItem(node));
	endInsertRows();
	reset;
	//emit dataChanged(createIndex(row, 0, blocks), createIndex(row + 1, 0, blocks));
#endif
#ifdef GWA
	Parent->log("Adding one well to tree view.");
	QModelIndex parent = createIndex(rootItem->indexOf(wells), 0, rootItem);
	int row = wells->childCount();
	beginInsertRows(parent, row, row);
	wells->addChild(new TreeItem(node));
	endInsertRows();
	reset;
	//emit dataChanged(createIndex(row, 0, blocks), createIndex(row + 1, 0, blocks));
#endif

}
#ifdef GWA
void TreeModel::addWell(Node *node)
{
	Parent->log("Adding one well to tree view.");
	QModelIndex parent = createIndex(rootItem->indexOf(wells), 0, rootItem);
	int row = wells->childCount();
	beginInsertRows(parent, row, row);
	wells->addChild(new TreeItem(node));
	endInsertRows();
	reset;
	//emit dataChanged(createIndex(row, 0, blocks), createIndex(row + 1, 0, blocks));
}
void TreeModel::addTracer(Node *node)
{
	Parent->log("Adding one well to tree view.");
	QModelIndex parent = createIndex(rootItem->indexOf(tracers), 0, rootItem);
	int row = tracers->childCount();
	beginInsertRows(parent, row, row);
	tracers->addChild(new TreeItem(node));
	endInsertRows();
	reset;
	//emit dataChanged(createIndex(row, 0, blocks), createIndex(row + 1, 0, blocks));
}
#endif
void TreeModel::add(Edge *edge)
{
	connectors->addChild(new TreeItem(edge));
}
TreeItem * TreeModel::entityParentItemfromType(QString type) const
{
    TreeItem *parent = nullptr;
#ifdef Aquifolium
    if (type == "Settings")
        parent = settings;
    else if (treeItems.count(type)==0)
        parent = new TreeItem(type,Parent,TreeItem::Type::Branch);

    else
        parent = treeItems[type];


#endif
#ifdef GIFMOD
	if (type == "Sensor")
		parent = this->sensor;
	if (type == "Objective function")
		parent = this->objectiveFunction;
	if (type == "Controller")
		parent = this->controller;
#endif
#ifdef GIFMod
	if (type == "Constituent")
		parent = this->constituent;
	if (type == "Particle")
		parent = this->particle;
	if (type == "Build-up")
		parent = this->buildUp;
	if (type == "External flux")
		parent = this->extrenalFlux;
	if (type == "Reaction parameter")
		parent = this->reactionParameter;
	if (type == "Parameter")
		parent = this->parameter;
	if (type == "Observation")
		parent = this->observed;
	if (type == "Evapotranspiration")
		parent = this->evapotranspiration;
	if (type == "Wells")
		parent = this->wells;
	if (type == "Tracers")
		parent = this->tracers;
#endif
	return parent;
}
void TreeModel::add(Entity *entity)
{
	QString type = entity->objectType.ObjectType;
	TreeItem *parent = entityParentItemfromType(type);
    if (parent) parent->addChild(new TreeItem(entity));
}

void TreeModel::add(Entity* entity, const QString &Branch)
{
	TreeItem* parent = entityParentItemfromType(Branch);
    if (treeItems.count(Branch)==0 && Branch!="Connectors" && Branch!="Blocks" && Branch !="Settings")
    {   treeItems.insert(Branch, parent);
        rootItem->addChild(parent);
    }
    if (parent)
        parent->addChild(new TreeItem(entity));
}


void TreeModel::Populate(GraphWidget *parent)
{
    QJsonObject jsonobj = parent->jsondocentities.object();
    foreach (QString key, jsonobj.keys()){
        QJsonValue val = jsonobj[key];
		QJsonObject valobj = val.toObject(); 
		new Entity("Settings", val.toObject()["description"].toString(), parent);
		for (QJsonObject::Iterator it = valobj.begin(); it != valobj.end(); it++)
		{
			QuanSet quanset(valobj);
			qDebug() << it.key(); 
			parent->entity("Settings")->Quans()->Append(quanset);
		}
    }

}
