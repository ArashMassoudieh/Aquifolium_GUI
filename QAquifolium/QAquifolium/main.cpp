#include "QAquifolium.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QAquifolium w;
	w.show();
	return a.exec();
}
