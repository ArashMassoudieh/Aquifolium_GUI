#include "edge.h"
#include "node.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <GWidget.h>
#include <qdebug.h>
#include "PropModel.h"
#include "GWidget.h"

#include "qtableview.h"
#include "treemodel.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

Edge::Edge(Node *sourceNode, Node *destNode, QString ctr_type, GraphWidget *_parent)
    : arrowSize(10)
{
    setAcceptedMouseButtons(nullptr);
    quans = *_parent->metamodel()->GetItem(ctr_type.toStdString());
    source = sourceNode;
    dest = destNode;
    connector_type = ctr_type;
	model = new PropModel<Edge>(this);
	QList<Node*> list;
    foreach (Edge *e , source->edgeList)
	{
		if (e->sourceNode() == source) list.append(e->destNode());
		if (e->destNode() == source) list.append(e->sourceNode());
	}
	if (list.contains(dest))
	{
		_parent->log(QString("Duplicate connector from %1 to %2.").arg(source->Name()).arg(dest->Name()));
		delete this;
		return;
	}
    source->addEdge(this);
    dest->addEdge(this);
    adjust();
	GUI = "Connector";
	itemType = Object_Types::Connector;
//	sourceID = source->ID;
//	destID = dest->ID;

	setFlag(ItemIsSelectable);
	setFlag(ItemSendsGeometryChanges);

	setCacheMode(DeviceCoordinateCache);
	setZValue(1);
	parent = _parent;
	objectType = parent->ModelSpace; // mProp('*');
	objectType.GuiObject = "Connector";
    objectType.ObjectType = connector_type;
	QList <mProp> QL;
    QL = (*parent->mList).GetList();
    if ((*parent->mList).filter(objectType).ObjectTypes().size())
        objectType.ObjectType = (*parent->mList).filter(objectType).ObjectTypes()[0];

	updateSubType();

	props.parent = this;

	parent->MainGraphicsScene->addItem(this);
	name = QString("%1 - %2").arg(sourceNode->Name()).arg(destNode->Name());
    setName(name);
    if (parent->treeModel)
        parent->treeModel->add(this);
	parent->log(QString("One %3 connector from %1 to %2 created.").arg(sourceNode->Name()).arg(destNode->Name()).arg(objectType.SubType));
	changed();
}

bool Edge::setObjectType(const QString &type)
{
	if (objectType.ObjectType == type) return false;
	else
	{
		objectType.ObjectType = type;
		objectType.SubType = '*';
	//	objectType.SubType = (*parent->mList).filter(Filter()).SubTypes()[0];
		//update();
		if (type == "Default")
			updateSubType();
		changed();
		return true;
	}
}

mProp Edge::Filter() const
{
//	return((*parent).ModelSpace & ObjectType);
	return objectType;
}

mPropList *Edge::mList() const 
{
	return parent->mList; 
};

Node *Edge::sourceNode() const
{
    return source;
}

Node *Edge::destNode() const
{
    return dest;
}

void Edge::setBold(const bool _Bold)
{
	bold = _Bold;
}

void Edge::adjust()
{
    if (!source || !dest)
        return;

	QLineF line(mapFromItem(source, source->Width() / 2, source->Height() / 2), mapFromItem(dest, dest->Width() / 2, dest->Height() / 2));
    qreal length = line.length();

	prepareGeometryChange();

	qreal Ox, Oy, Dx, Dy;
		
	if (abs(line.dx()) < 1)
	{
		Ox = 0;
		Dx = 0;
	}
	else
	{
		Ox = line.dx() / abs(line.dx())*min(int(source->Width() / 2), int(fabs(source->Height() / 2.0 * line.dx() / (line.dy()+0.5))));
		Dx = -line.dx() / abs(line.dx())*min(int(dest->Width() / 2), int(fabs(dest->Height() / 2.0 * line.dx() / (line.dy()+0.5))));
	}
	if (abs(line.dy()) < 1)
	{
		Oy = 0;
		Dy = 0;
	}
	else
	{
		Oy = line.dy() / abs(line.dy())*min(int(source->Height() / 2), int(fabs(source->Width()/ 2.0 * line.dy() / (line.dx()+0.5))));
		Dy = -line.dy() / abs(line.dy())*min(int(dest->Height() / 2), int(fabs(dest->Width() / 2.0 * line.dy() / (line.dx()+0.5))));
	}
		QPointF edgeOffsetSource(Ox, Oy);
		QPointF edgeOffsetDest(Dx, Dy);
		sourcePoint = line.p1() + edgeOffsetSource;
		destPoint = line.p2()  + edgeOffsetDest;
}

QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 8.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

	// Draw the line itself
	if (isSelected())
		painter->setPen(QPen(Qt::green, (bold) ? 3 : 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	else if (errorDetected())
		painter->setPen(QPen(Qt::red, (bold) ? 3 : 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	else if (parent->colorCode.edges)
		painter->setPen(QPen(color.color1, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	//		painter->setPen(QPen(QColor::fromRgb(color.color1), (bold) ? 3 : 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	else
		painter->setPen(QPen(Qt::black, (bold) ? 3 : 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	if (!avoidCrossObjects)
	{

		painter->drawLine(line);

		// Draw the arrows
		double angle = ::acos(line.dx() / line.length());
		if (line.dy() >= 0)
			angle = TwoPi - angle;
//		QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
//			cos(angle + Pi / 3) * arrowSize);
//		QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
//			cos(angle + Pi - Pi / 3) * arrowSize);
		QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
			cos(angle - Pi / 3) * arrowSize);
		QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
			cos(angle - Pi + Pi / 3) * arrowSize);
		if (isSelected())
			painter->setBrush(Qt::green);
		else if (parent->colorCode.edges)
			painter->setBrush(color.color1);
//		painter->setBrush(QColor::fromRgb(color.color1));
		else
			painter->setBrush(Qt::black);
//		painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
		painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
	}
	if (avoidCrossObjects)
	{

		painter->drawLine(line);

		// Draw the arrows
		double angle = ::acos(line.dx() / line.length());
		if (line.dy() >= 0)
			angle = TwoPi - angle;

//		QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
//			cos(angle + Pi / 3) * arrowSize);
//		QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
//			cos(angle + Pi - Pi / 3) * arrowSize);
		QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
			cos(angle - Pi / 3) * arrowSize);
		QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
			cos(angle - Pi + Pi / 3) * arrowSize);
		if (isSelected())
			painter->setBrush(Qt::green);
		else
			painter->setBrush(Qt::black);
//		painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
		painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
	}
    if (isSelected() && parent->tableProp)
	{
//		parent->propModel->setParentEdge(this);
        parent->tableProp->setModel(model);
		parent->tableProp->setFocus();
	}
}
int Edge::dist(const QPointF point)
{
    int x1 = int(sourcePoint.x());
    int y1 = int(sourcePoint.y());
    int x2 = int(destPoint.x());
    int y2 = int(destPoint.y());
    int x0 = int(point.x());
    int y0 = int(point.y());
    int dist = int(abs(x0*(y2 - y1) - y0*(x2 - x1) + x2*y1 - y2*x1) / sqrt((y2 - y1) ^ 2 + (x2 - x1) ^ 2));
	return dist;
}
mPropList Edge::getmList(const mProp &_filter) const
{
	static mProp filter;
	static mPropList r;
	if (filter %= _filter) return r;
	r = mList()->filter(_filter);
	filter = _filter;
	return r;// (*parent->mList).filter(Filter() & _filter);
}

Edge::Edge(const Edge &E)
{
	setName(E.Name());
//	setID(E.ID());
//	itemType = E.itemType; 
	GUI = E.GUI;
	source = E.source;
	dest = E.dest;
	sourceID = E.sourceID;
	destID = E.destID;
	sourcePoint = E.sourcePoint;
	destPoint = E.destPoint;
	arrowSize = E.arrowSize;
	parent = E.parent;
//	propList = E.propList;
	objectType = E.objectType;
	avoidCrossObjects = E.avoidCrossObjects;
	props.list = E.props.list;
    connector_type = E.connector_type;
}

Edge Edge::operator=(const Edge &E)
{
	setName(E.Name());
//	setID(E.ID());
//	itemType = E.itemType;
	GUI = E.GUI;
	source = E.source;
	dest = E.dest;
	sourceID = E.sourceID;
	destID = E.destID;
	sourcePoint = E.sourcePoint;
	destPoint = E.destPoint;
	arrowSize = E.arrowSize;
	parent = E.parent;
//	propList = E.propList;
	objectType = E.objectType;
	avoidCrossObjects = E.avoidCrossObjects;
	props.list = E.props.list;
    connector_type = E.connector_type;
	return *this;
}

QVariant Edge::getProp(const QString &propName, const int role) const
{
	mProp filter = Filter();
	filter.VariableName = propName;
	mProp mValue = mList()->filter(filter)[0];
	if (role == TypeRole) return mValue.Delegate;
	if (role == InputMethodRole) return mValue.inputMethod;
	if (role == DefaultValuesListRole)
	{
		mProp _filter = Filter();
		return mValue.DefaultValuesStringList(mList(), &_filter, parent);
	}
	if (role == VariableTypeRole) return mValue.VariableType;
	if (role == UnitRole) return getValue(propName).unit;
	if (role == defaultUnitRole) return getValue(propName).defaultUnit;
	if (role == UnitsListRole) return getValue(propName).unitsList;
	if (role == experimentDependentRole) return mValue.ExperimentDependent;
	if (role == differentValuesRole)
	{
		if (mValue.ExperimentDependent == "No" || parent->experimentID())
			return false;
		else
			return props.getPropMultiValues(propName, parent->experimentsList()).differentValues();
	}
	if (role == allUnitsRole){
		QStringList allUnits;
		allUnits.append(getValue(propName).unit);
		allUnits.append(getValue(propName).unitsList);
		allUnits.append(getValue(propName).defaultUnit);
		return allUnits;
	}
	if (role == DescriptionCodeRole) return mValue.DescriptionCode;

	if (role == Qt::ForegroundRole)
	{
		if (warnings.keys().contains(propName))	return QBrush((QColor(255, 111, 28)));
		if (errors.keys().contains(propName)) return QBrush(Qt::red);
		if (getProp(propName, differentValuesRole).toBool())
			return QBrush(Qt::gray);
	}

	if (role == Qt::FontRole) //TextColorRole)
	{
		QFont boldFont;
		boldFont.setBold(true);
		if (propName == "Name" || propName == "Type" || propName == "SubType") return boldFont;
		if (mValue.DefaultValuesStringList(0, 0, parent).indexOf(getValue(propName)) != -1) return boldFont;
		else return QFont();
	}
	if (role == Qt::CheckStateRole)
		if ((mValue.Delegate == "CheckBox") || (mValue.VariableType == "boolean"))
			if ((getValue(propName) != "0") && (getValue(propName).toLower() != "false") && (getValue(propName).toLower() != "no")) return Qt::Checked; else return Qt::Unchecked;

	if (role == fullFileNameRole) return getValue(propName);
	if (role == Qt::DisplayRole)// || role == Qt::ToolTipRole)
	{
		if (getProp(propName, differentValuesRole).toBool())
			return QVariant("Different values.");
		if (mValue.VariableType == "filename")
			return OnlyFilenames(getValue(propName));
		else if (mValue.Delegate == "CheckBox")
			return "";
		else return getValue(propName).toStringUnit();
	}
	if (role == Qt::ToolTipRole)
	{
		QString toolTip;
		if (errors[propName].size()) toolTip.append("\n Error: ").append(errors[propName]);
		if (warnings[propName].size()) toolTip.append("\n Warning: ").append(warnings[propName]);

		if (mValue.VariableType == "filename")
			return getValue(propName).append(toolTip);
		else if (mValue.Delegate == "CheckBox")
			return toolTip;
		else return (getValue(propName).toStringUnit() == " ...") ? QVariant() : getValue(propName).toStringUnit().remove("...").append(toolTip);
	}
	if (role == VariableNameToolTipRole) return (mValue.Description != "*") ? mValue.Description : QString("");

//	if (role == Qt::ToolTipRole) return (mValue.Description != "*") ? mValue.Description : (mValue.VariableName.size() >= 12) ? mValue.VariableName : QString("");
	if (role == Qt::EditRole){
		if (getProp(propName, differentValuesRole).toBool())
			return QVariant();
		return getValue(propName);
	}
	if (role == XStringEditRole){
		return getValue(propName).list();
	}
	return QVariant();
}

XString Edge::getValue(const QString& propName) const
{
	if (propName == "Name") return Name();
	if (propName == "Type") return ObjectType().ObjectType;
	if (propName == "SubType") return ObjectType().SubType;
	if (experimentName() == "All experiments" && !getProp(propName, differentValuesRole).toBool())
		return props.getProp(propName, parent->experimentsList()[0]);
	return props.getProp(propName, experimentName());
}

bool Edge::setProp(const QString &propName, const QVariant &Value, const int role)
{
	if (role == Qt::EditRole)
	{
		if (getProp(propName, VariableTypeRole).toString().toLower() == "filename" || getProp(propName, VariableTypeRole).toString().toLower() == "directory")
			return setValue(propName, relativePathFilename(Value.toString(), parent->modelPathname()));
		else return setValue(propName, Value.toString());
	}
	if (role == XStringEditRole) return setValue(propName, Value.toStringList());
	if (role == setParamRole)
	{
		QStringList valallUnits;
		valallUnits.append(Value.toString());
		valallUnits.append(getProp(propName, allUnitsRole).toStringList());

		return setValue(propName, valallUnits);
	}

	return false;
}

bool Edge::setValue(const QString &propName, const XString &Value)
{
	QString experiment = (getProp(propName, experimentDependentRole) == "Yes") ? experimentName() : "All experiments";
	bool r = props.setProp(propName, Value, experiment);
	if (r)
		changed();
	return r;
}

QString Edge::updateSubType()
{
/*	QString r;
//	QStringList Porous;
//	Porous << "Soil" << "Darcy" << "Storage";// << "" << "";
//	if (Porous.contains(source->ObjectType().ObjectType) || Porous.contains(dest->ObjectType().ObjectType))
	if (source->isPorous() || dest->isPorous())
		r = "Porous";
	else
		r = "non-Porous";
	if (r != objectType.SubType)
	{
		objectType.SubType = r;
		changed();
		//qDebug() << source->ObjectType().ObjectType << dest->ObjectType().ObjectType << objectType.SubType;
	}
	return objectType.SubType;
*/
    return "*";
}

QMap<QString, QVariant> Edge::compact() const
{
//	//qDebug() << "Compacting: " << name << sourceNode()->Name() << destNode()->Name();

	QMap<QString, QVariant> r;
	r["Source Node"] = sourceNode()->Name();
	r["Dest Node"] = destNode()->Name();

	r["GUI"] = GUI;
	r["Name"] = name;
	r["Type"] = objectType.ObjectType;
	r["SubType"] = objectType.SubType;
	r["Arrow Size"] = arrowSize;
	r["Properties"] = props.compact();
    /*foreach (QString key , props.list.keys())
		if (!key.contains("Node")) r[key] = props.list[key].compact();*/
	return r;
}

void Edge::compact(QJsonObject &json) const
{
//	//qDebug() << "Compacting: " << name << sourceNode()->Name() << destNode()->Name();

    QJsonObject edgejson;
    edgejson["Source Node"] = sourceNode()->Name();
    edgejson["Dest Node"] = destNode()->Name();

    edgejson["GUI"] = GUI;
    edgejson["Name"] = name;
    edgejson["Type"] = objectType.ObjectType;
    edgejson["SubType"] = objectType.SubType;
    edgejson["Arrow Size"] = arrowSize;
    props.compact(edgejson);
    json[name] = edgejson;


}

Edge* Edge::unCompact(QMap<QString, QVariant> n, GraphWidget *gwidget, bool oldVersion)
{
	//qDebug() << "Loading: " << n["Name"].toString() << n["Source Node"].toString() << n["Dest Node"].toString();


	QString source = n["Source Node"].toString();
	QString dest = n["Dest Node"].toString();
	//qDebug() << gwidget->node(source)->Name();
	//qDebug() << gwidget->node(dest)->Name();
    Edge *edge = new Edge(gwidget->node(source), gwidget->node(dest), "", gwidget);

	if (!gwidget->Edges().contains(edge))
	{
		gwidget->log(QString("%1-%2 connector ignored.").arg(source).arg(dest));
		return nullptr;
	}
	edge->name = n["Name"].toString();
	edge->objectType.ObjectType = n["Type"].toString();
	edge->objectType.SubType = n["SubType"].toString();
	edge->updateSubType();
	edge->arrowSize = n["Arrow Size"].toInt();

	n.remove("Name");
	n.remove("GUI");
	n.remove("Type");
	n.remove("SubType");
	n.remove("Arrow Size");

	edge->props.list = PropList<Edge>::unCompact(n.value("Properties").toString());

	if (!edge->props.list.size() && oldVersion)
        foreach(QString key , n.keys())
		{
			QString propName = key;
			if (key == "Interface Area")
				propName = "Interface/cross sectional area";
			edge->props.setProp(propName.toLower(), XString::unCompact(n[key].toString()), "experiment1");
		}

    /*foreach (QString key , n.keys())
	edge->props.list[key] = XString::unCompact(n[key].toString());*/
	return edge;
}

Edge* Edge::unCompact(const QJsonObject &jsonobj, GraphWidget *gwidget, bool oldVersion)
{
    //qDebug() << "Loading: " << n["Name"].toString() << n["Source Node"].toString() << n["Dest Node"].toString();


    QString source = jsonobj["Source Node"].toString();
    QString dest = jsonobj["Dest Node"].toString();

    Edge *edge = new Edge(gwidget->node(source), gwidget->node(dest), "", gwidget);

    if (!gwidget->Edges().contains(edge))
    {
        gwidget->log(QString("%1-%2 connector ignored.").arg(source).arg(dest));
        return nullptr;
    }
    edge->name = jsonobj["Name"].toString();
    edge->objectType.ObjectType = jsonobj["Type"].toString();
    edge->objectType.SubType = jsonobj["SubType"].toString();
    edge->updateSubType();
    edge->arrowSize = jsonobj["Arrow Size"].toInt();

    edge->props.list = PropList<Edge>::uncompact(jsonobj);

    if (!edge->props.list.size() && oldVersion)
        foreach(QString key , jsonobj.keys())
        {
            QString propName = key;
            if (key == "Interface Area")
                propName = "Interface/cross sectional area";
            edge->props.setProp(propName.toLower(), XString::unCompact(jsonobj[key].toString()), "experiment1");
        }

    /*foreach (QString key , n.keys())
    edge->props.list[key] = XString::unCompact(n[key].toString());*/
    return edge;
}

Edge* Edge::unCompact10(QMap<QString, QVariant> n, GraphWidget *gwidget)
{
	//qDebug() << "FUNCTION CANCELED.";
	/*//qDebug() << "Loading: " << n["Name"].toString() << n["Source Node"].toString() << n["Dest Node"].toString();


	QString source = n["Source Node"].toString();
	QString dest = n["Dest Node"].toString();
	//qDebug() << gwidget->node(source)->Name();
	//qDebug() << gwidget->node(dest)->Name();
	Edge *edge = new Edge(gwidget->node(source), gwidget->node(dest), gwidget);

	if (!gwidget->Edges().contains(edge))
	{
		gwidget->log(QString("%1-%2 connector ignored.").arg(source).arg(dest));
		return nullptr;
	}
	edge->name = n["Name"].toString();
	edge->objectType.ObjectType = n["Type"].toString();
	edge->objectType.SubType = n["SubType"].toString();
	edge->updateSubType();
	edge->arrowSize = n["Arrow Size"].toInt();

	n.remove("Name");
	n.remove("GUI");
	n.remove("Type");
	n.remove("SubType");
	n.remove("Arrow Size");

    foreach (QString key , n.keys())
		edge->props.list[key] = XString::unCompactOld(n[key].toString());*/
    return new Edge(nullptr,nullptr,nullptr);
}

QStringList Edge::codes() const
{
	QStringList r;
    foreach (mProp mP , getmList(objectType).GetList())
		if (mP.VariableCode != "") r.append(mP.VariableCode);
	return r;
}
QMap<QString, condition> Edge::variableNameConditions() const
{
	QMap<QString, condition> r;
    foreach (mProp mP , getmList(objectType).GetList())
		if (mP.VariableCode != "")
		{
			condition c;
			c.Condition = mP.Condition;
			c.error = mP.error;
			c.errorDesc = mP.errorDesc;
			if (c.Condition.count()) r[mP.VariableName] = c;
		}
	return r;
}
XString Edge::val(const QString & code) const
{
	//qDebug() << code;
    foreach (mProp mP , getmList(objectType).GetList())
		if (mP.VariableCode.toLower() == code.toLower())
		{
			XString r = getValue(mP.VariableName);
//			if (r.unitsList.count() >1) return r.convertTo(r.unitsList[0]);
			if (!r.toQString().isEmpty())
				if (!r.unit.isEmpty())
				{
					if (r.toDouble() == 0)
						return r;
					else
						return r.convertToDefaultUnit();
				}
			return r;
		}
	return XString();
}
void Edge::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QPointF p = QPointF(x() + event->pos().x(), y() + event->pos().y());
	parent->edgeContextMenuRequested(this, p);

}

QStringList Edge::variableNames() const
{
	QStringList r;
    foreach (mProp mP , getmList(objectType).GetList())
		if (mP.VariableName != "") r.append(mP.VariableName);
	return r;
}
QString Edge::variableName(QString code) const
{
    foreach (mProp mP , getmList(objectType).GetList())
		if (mP.VariableCode.toLower() == code.toLower()) return mP.VariableName;
	return QString("Error: code (%1) not found.").arg(code);
}
void Edge::changed()
{
	parent->edgeChanged(this);
}

void Edge::copyProps(Node *node, QString arrayDirection, QString connectorDirection, bool copyLength)
{
	QStringList exceptionList;
	exceptionList << "id";
	if (connectorDirection.toLower().contains("v"))
		exceptionList << "width";
	if (connectorDirection.toLower().contains("h"))
		exceptionList << "a";
	if (!node->isPorous())
		exceptionList << "a";

    if (!copyLength)
        exceptionList << "d";
    
    foreach (QString code , node->codes())
	{
		if (!exceptionList.contains(code.toLower()) && codes().contains(code) &&
            node->getValue(node->variableName(code))!="")
			setProp(variableName(code), node->getValue(node->variableName(code)).list(), XStringEditRole);
	}
	if (node->isPorous())
	{
		if (connectorDirection.toLower().contains("h"))
		{
			if (variableName("a") != "")
			{
				XString area = node->getValue(node->variableName("a")).list();
				area.unit = area.unitsList[0];
				area.setNum(node->getValue(node->variableName("width")).toFloatDefaultUnit()*node->getValue(node->variableName("depth")).toFloatDefaultUnit());
				setProp(variableName("a"), area.list(), XStringEditRole);
			}
		}
		if (arrayDirection.toLower().contains("h") && connectorDirection.toLower().contains("v"))
		{
			if (variableName("a") != "")
			{
				XString area = node->getValue(node->variableName("a")).list();
				area.unit = area.unitsList[0];
				area.setNum(node->getValue(node->variableName("d")).toFloatDefaultUnit()*node->getValue(node->variableName("depth")).toFloatDefaultUnit());
				setProp(variableName("a"), area.list(), XStringEditRole);
			}
		}
		if (arrayDirection.toLower().contains("v") && connectorDirection.toLower().contains("v"))
		{
			//area already set to node's a
		}
	}
	else //non-porous
	{
		if (connectorDirection.toLower().contains("h"))
		{
			//width already set to node's width
		}
		if (connectorDirection.toLower().contains("v"))
			if (variableName("width") != "")
			{
				setProp(variableName("width"), node->getValue(node->variableName("width")).list(), XStringEditRole);
			}
	}

}

bool Edge::intersects(double x, double y)
{
	return false;
}
QString Edge::experimentName() const
{
	return parent->experimentName();
}
void Edge::copyProps(QString sourceExperiment, QString destExperiment)
{
	if (props.list.keys().contains(sourceExperiment))
		props.list[destExperiment] = props.list[sourceExperiment];
}

QString Edge::toCommand()
{
    QString cmd;
    cmd += QString("create link;");
    cmd += "Type = " + this->GetObjectType() + ",";
    cmd += "from = " + source->Name();
    cmd += ",";
    cmd += "to = " + dest->Name();
    for (map<string,Quan>::iterator it=quans.begin(); it!=quans.end(); it++)
    {
        cmd += ", ";
        cmd += QString::fromStdString(it->first) + "=" + QString::fromStdString(it->second.GetProperty());

    }
    return cmd;
}

Edge::Edge(const QString &command, GraphWidget *_parent)
    : arrowSize(10)
{
    setAcceptedMouseButtons(nullptr);

    QStringList cmdsplit = command.split(";");
    if (cmdsplit[0] == "create link")
    {
        QStringList Props = cmdsplit[1].split(",");
        QMap<QString, QString> propsmap;
        for (int i = 0; i < Props.count(); i++)
        {
            QStringList Prop = Props[i].split("=");
            if (Prop.size()>1)
                propsmap[Prop[0].trimmed()] = Prop[1].trimmed();
        }
        parent = _parent;
        Node* sourceNode = parent->node(propsmap["from"]);
        Node* destNode = parent->node(propsmap["to"]);
        if (sourceNode==nullptr)
        {
            qDebug() << "Node [" + propsmap["from"] + "] does not exist!";
            return;
        }
        if (destNode==nullptr)
        {
            qDebug() << "Node [" + propsmap["to"] + "] does not exist!";
            return;
        }
        string _type = propsmap["Type"].toStdString();
        if (_type=="")
        {
            qDebug() << "Type has not been specified!";
            return;
        }
        quans = *_parent->metamodel()->GetItem(_type);
        source = sourceNode;
        dest = destNode;
        connector_type = QString::fromStdString(_type);
        model = new PropModel<Edge>(this);
        QList<Node*> list;
        foreach (Edge *e , source->edgeList)
        {
            if (e->sourceNode() == source) list.append(e->destNode());
            if (e->destNode() == source) list.append(e->sourceNode());
        }
        if (list.contains(dest))
        {
            _parent->log(QString("Duplicate connector from %1 to %2.").arg(source->Name()).arg(dest->Name()));
            delete this;
            return;
        }
        source->addEdge(this);
        dest->addEdge(this);
        adjust();
        GUI = "Connector";
        itemType = Object_Types::Connector;
    //	sourceID = source->ID;
    //	destID = dest->ID;

        setFlag(ItemIsSelectable);
        setFlag(ItemSendsGeometryChanges);

        setCacheMode(DeviceCoordinateCache);
        setZValue(1);

        objectType = parent->ModelSpace; // mProp('*');
        objectType.GuiObject = "Connector";
        objectType.ObjectType = connector_type;
        QList <mProp> QL;
        QL = (*parent->mList).GetList();
        if ((*parent->mList).filter(objectType).ObjectTypes().size())
            objectType.ObjectType = (*parent->mList).filter(objectType).ObjectTypes()[0];

        updateSubType();

        props.parent = this;
        for (QMap<QString, QString>::iterator i = propsmap.begin(); i != propsmap.end(); ++i)
            setProp(i.key(), i.value());

        parent->MainGraphicsScene->addItem(this);
        name = QString("%1 - %2").arg(sourceNode->Name()).arg(destNode->Name());
        setName(name);
        if (parent->treeModel)
            parent->treeModel->add(this);
        parent->log(QString("One %3 connector from %1 to %2 created.").arg(sourceNode->Name()).arg(destNode->Name()).arg(objectType.SubType));
        changed();
    }
}

