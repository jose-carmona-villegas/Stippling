#ifndef ENTITYTREEWIDGET_H
#define ENTITYTREEWIDGET_H

#include <QTreeWidget>
#include <QDropEvent>

#include "entitytreewidgetitem.h"

class EntityTreeWidget : public QTreeWidget
{
    Q_OBJECT

private:
    EntityTreeController * _entities;
    QVector<EntityTreeWidgetItem *> _items;

    EntityTreeWidgetItem * createItems(EntityTreeNode * node);
    void setWidgets(EntityTreeWidgetItem * item);

public:
    EntityTreeWidget(QWidget * parent = 0);
    ~EntityTreeWidget();

    void setEntityTreeController(EntityTreeController * entities);

protected:
    //Qt::DropActions supportedDropActions() const;
    //QStringList mimeTypes () const;
    //QMimeData * mimeData ( const QList<QTreeWidgetItem *> items ) const;
    //bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action);
    void dropEvent ( QDropEvent * event );
    //void dragMoveEvent(QDragMoveEvent * event);

public slots:
    void update();
    void updateSelected();

private slots:
    void processSelection();
};

#endif // ENTITYTREEWIDGET_H
