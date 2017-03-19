#ifndef ENTITYTREEWIDGETITEM_H
#define ENTITYTREEWIDGETITEM_H

#include "entitytreenode.h"
#include "entitytreecontroller.h"
#include "specificentityconfigurationdialog.h"

#include <QTreeWidgetItem>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QComboBox>



class EntityTreeWidgetItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT

private:
    QLineEdit * _name;
    QCheckBox * _visible;
    QPushButton * _properties;
    QComboBox * _controlPoints;
    QComboBox * _type;

    EntityTreeNode * _node;

    EntityTreeController * _controller;

    SpecificEntityConfigurationDialog * configDialog;

public:
    EntityTreeWidgetItem(EntityTreeNode * node, EntityTreeController * controller = 0);
    ~EntityTreeWidgetItem();

    QLineEdit * nameWidget();
    QCheckBox * visibleWidget();
    QPushButton * properties();
    QComboBox * controlPointsWidget();
    QComboBox * typeWidget();
    EntityTreeNode * node();

public slots:
    void processNameEdition(QString name);
    void processVisibilityEdition(bool visible);
    void openSpecificConfigurationDialog();
    void processCPVisibilityEdition(QString choice);
    void processTypeEdition(QString choice);
};

#endif // ENTITYTREEWIDGETITEM_H
