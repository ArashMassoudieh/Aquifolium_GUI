#include "PropList.h"
#include "XString.h"
#include "qstring.h"
#include "node.h"
#include "entity.h"
#include "edge.h"
#include "mProp.h"
#include "mProplist.h"
#include "GWidget.h"
#include "utility_funcs.h"


template<>
XString PropList<Node>::getProp(const QString &propName, const QString &experimentName, Node* parentSub) const{
	if (!parentSub)
		parentSub = parent;
#ifdef GIFMod
    if (propName.contains("Particle initial"))
		return parentSub->g(experimentName);
	if (propName.contains("Constituent initial"))
		return parentSub->cg(experimentName);
#endif
	if (propName == "Name")
		return XString(parentSub->Name());
	if (propName == "Type")
		return XString(parentSub->objectType.ObjectType);
	if (propName == "SubType")
		return XString(parentSub->objectType.SubType);

	if (list.keys().contains(experimentName))
		return list.value(experimentName).getProp(propName, parentSub);
	else{
		mProp filter = parentSub->Filter();
		filter.VariableName = propName;
		mPropList mPL = parentSub->getmList(filter);
		if (mPL.size() == 0) return ".";
        QList<XString> LX = mPL[0].DefaultValuesList(nullptr,nullptr,parentSub->parent);
		XString r = (LX.count()) ? LX[0]: "";
		return r;
	}
}

template<>
XString PropList<Node>::getProp(const QString& propName, QList<Node*> nodes, const QString& experimentName) const {
	multiValues<XString> multi;
    for (Node* n : nodes)
	{
		multi.append(getProp(propName, experimentName, n));
	}
	if (multi.sameValues())
		return multi.value();
	else
		return "Different values";
}

template<>
bool PropList<Node>::setProp(const QString& propName, const XString& Value, const QString &experimentName) {
	if (propName == "Name") {
		if (Value != "") parent->setName(Value);
		return true;
	}
	if (propName == "Type") {
		parent->setObjectType(Value);
		return true;
	}
	if (propName == "SubType") {
		parent->setObjectSubType(Value);
		return true;
	}
	if (propName == "x")
	{
		if (Value.toFloat() != parent->x())
			parent->setX(Value.toDouble());
	}

	if (propName == "y")
	{
		if (Value.toFloat() != parent->y())
			parent->setY(Value.toDouble());
	}

	if (propName == "Superficial width")
	{
		if (Value.toFloat() != parent->Width())
			parent->setWidth(Value.toDouble());
	}

	if (propName == "Superficial height")
	{
		if (Value.toFloat() != parent->Height())
			parent->setHeight(Value.toDouble());
	}

	if (experimentName == "All experiments")
	{
		bool r = list[experimentName].setProp(propName, Value, parent);
        if (parent->parent->experiments)
            for (int i = 1; i < parent->parent->experiments->count(); i++)
                r |= setProp(propName, Value, parent->parent->experiments->itemText(i));
		return r;
	}
    bool out = list[experimentName].setProp(propName, Value, parent);
#ifdef DEBUG
    {
        QJsonObject json;
        compact(json);
        writetojson(json,propName);

    }
#endif
    return out;
}

template<>
XString PropList<Edge>::getProp(const QString &propName, const QString &experimentName, Edge* parentSub) const{
	if (list.keys().contains(experimentName))
		return list.value(experimentName).getProp(propName, parent);
	else{
		mProp filter = parent->Filter();
		filter.VariableName = propName;
		mPropList mPL = parent->getmList(filter);
		QList<XString> LX = mPL[0].DefaultValuesList(0, 0, parent->parent);
		XString r = LX[0];
		return r;

		return parent->getmList(filter)[0].DefaultValuesList(0, 0, parent->parent)[0];
	}

}

template<>
bool PropList<Edge>::setProp(const QString& propName, const XString& Value, const QString &experimentName) {
	QString classType = typeid(this).name();
	if (propName == "Name") {
		if (Value != "") parent->setName(Value);
		return true;
	}
	if (propName == "Type") {
		return parent->setObjectType(Value);
	}
	if (experimentName == "All experiments")
	{
		bool r = list[experimentName].setProp(propName, Value, parent);
        if (parent->parent->experiments)
        {   for (int i = 1; i < parent->parent->experiments->count(); i++)
                r |= setProp(propName, Value, parent->parent->experiments->itemText(i));
        }
		return r;
	}
	return list[experimentName].setProp(propName, Value, parent);
	}

template<>
XString PropList<Entity>::getProp(const QString &propName, const QString &experimentName, Entity* parentSub) const{
	if (list.keys().contains(experimentName))
		return list.value(experimentName).getProp(propName, parent);
	else{
		mProp filter = parent->Filter();
		filter.VariableName = propName;
		mPropList mPL = parent->getmList(filter);
		QList<XString> LX = mPL[0].DefaultValuesList(0, 0, parent->parent);
		XString r = (LX.count())? LX[0]:"Some predecessors not defined.";
		return r;

		return parent->getmList(filter)[0].DefaultValuesList(0, 0, parent->parent)[0];
	}
}

template<>
bool PropList<Entity>::setProp(const QString& propName, const XString& Value, const QString &experimentName) {
	QString classType = typeid(this).name();
	if (propName == "Name") {
		if (Value != "") parent->setName(Value);
		return true;
	}
	if (propName == "Type") {
		parent->setObjectType(Value);
		return true;
	}
	if (propName == "SubType") {
		parent->setObjectSubType(Value);
		return true;
	}
	if (experimentName == "All experiments")
	{
		bool r = list[experimentName].setProp(propName, Value, parent); 
        if (parent->parent->experiments)
        {   for (int i = 1; i < parent->parent->experiments->count(); i++)
                r |= setProp(propName, Value, parent->parent->experiments->itemText(i));
        }

        return r;
	}
	return list[experimentName].setProp(propName, Value, parent);
}
template<>
multiValues<> PropList<Node>::getPropMultiValues(const QString& propName, const QList<Node*> nodes, const QStringList &experimentsList) const {
	vector<QVariant> values;
	for (int i = 0; i<experimentsList.count(); i++)
		values.push_back(getProp(propName, nodes, experimentsList[i]));
	return multiValues<>(values);
}
