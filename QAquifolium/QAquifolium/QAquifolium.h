#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QAquifolium.h"

class QAquifolium : public QMainWindow
{
	Q_OBJECT

public:
	QAquifolium(QWidget *parent = Q_NULLPTR);

private:
	Ui::QAquifoliumClass ui;
};
