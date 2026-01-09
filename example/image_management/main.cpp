#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include "workflow.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    int result = 0;

    if (argc > 1) {
        result = Workflow::executeWorkflow(argv[1]);
    } else {
        Workflow::WorkflowType type = Workflow::runInteractiveMenu();
        result = Workflow::executeWorkflow(type);
    }

    if (result == 0) {
        qDebug() << "\n演示成功完成!";
    }

    QTimer::singleShot(100, &app, &QCoreApplication::quit);
    return app.exec();
}
