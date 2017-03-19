#include "entitytreewidget.h"

EntityTreeWidgetItem * EntityTreeWidget::createItems(EntityTreeNode * node)
{
    // Create the item corresponding to the node
    EntityTreeWidgetItem * item = new EntityTreeWidgetItem(node, _entities);

    // Add the item to the collection of items
    _items.append(item);

    if(_entities->isSelected(item->node()))
    {
        setCurrentItem(item);
    }

    // For each of the node's children, process them recursively in the same way
    foreach(EntityTreeNode * child, node->children)
    {
        // Add the returned node's children items as children to the node's item
        item->addChild(createItems(child));
    }

    //setWidgets(item);

    return item;
}

void EntityTreeWidget::setWidgets(EntityTreeWidgetItem * item)
{
    if(item->nameWidget() != 0)
    {
        setItemWidget(item, 1, item->nameWidget());
        connect(item->nameWidget(), SIGNAL(textChanged(QString)), item, SLOT(processNameEdition(QString)));
    }
    if(item->visibleWidget() != 0)
    {
        setItemWidget(item, 2, item->visibleWidget());
        connect(item->visibleWidget(), SIGNAL(toggled(bool)), item, SLOT(processVisibilityEdition(bool)));
    }
    if(item->properties() != 0)
    {
        setItemWidget(item, 3, item->properties());
        connect(item->properties(), SIGNAL(clicked()), item, SLOT(openSpecificConfigurationDialog()));
    }
    if(item->controlPointsWidget() != 0)
    {
        setItemWidget(item, 4, item->controlPointsWidget());
        connect(item->controlPointsWidget(), SIGNAL(activated(QString)), item, SLOT(processCPVisibilityEdition(QString)));
    }
    if(item->typeWidget() != 0)
    {
        setItemWidget(item, 5, item->typeWidget());
        connect(item->typeWidget(), SIGNAL(activated(QString)), item, SLOT(processTypeEdition(QString)));
    }

    for( int i = 0; i < item->childCount(); ++i )
    {
        EntityTreeWidgetItem * child = static_cast<EntityTreeWidgetItem*>(item->child(i));
        setWidgets(child);
    }
}

EntityTreeWidget::EntityTreeWidget(QWidget * parent) : QTreeWidget(parent)
{
    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(processSelection()));
}

EntityTreeWidget::~EntityTreeWidget()
{
    _entities = 0;

    // The items are deleted in the inverse order due to the fact that because they are QObjects,
    // Qt's default behaviour is to also delete its children (if they are not deleted), and by
    // doing it in that order, the entities are found first, then the operations. That way Qt does
    // not have to delete anything (Qt use a wrong delete, and won't delete the EntityTreeWidgetItem
    // part, just the QObject part, and that will cause a segmentation fault. This happens because
    // multiple inheritance was used) and there won't be segmentation faults caused by attempting
    // to delete what Qt deleted automatically.
    for(int i=_items.size()-1; i >= 0; --i)
    {
        if(_items.at(i) != 0)
        {
            //out << "About to delete the item " << _items.at(i)->nameWidget()->text() << endl;
            delete _items.at(i);
            _items.replace(i, 0);
            //out << "deleted" << endl;
        }
    }

    _items.clear();
}

void EntityTreeWidget::setEntityTreeController(EntityTreeController * entities)
{
    _entities = entities;

    setColumnCount(6);

    QStringList headers;
    headers.append("");
    headers.append("Name");
    headers.append("Visible");
    headers.append("Properties");
    headers.append("Control Points");
    headers.append("Type");

    setHeaderLabels(headers);

    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setDragEnabled(true);
    this->viewport()->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
    this->setDragDropMode(QAbstractItemView::InternalMove);

    update();
}

void EntityTreeWidget::dropEvent ( QDropEvent * event )
{
    //out << "dropEvent() called!" << endl;

    QTreeWidgetItem * temp = this->itemAt(event->pos());
    EntityTreeWidgetItem * item = static_cast<EntityTreeWidgetItem*>(temp);

    if(_entities->selectedNode() != 0)
    {
        if(item != 0)
        {
            // If the item below is valid...
            if(item->node()->isOperation() && item->node()->numberOfChildren() < 2)
            {
                // Accept the selected node as child if possible
                _entities->pruneAndGraft(_entities->selectedNode(), item->node());
            }
        }
        else
        {
            // If there's no valid item below to try to parent the selected node as child...
            if(_entities->selectedNode()->hasParent())
            {
                _entities->pruneAndGraftAsPrimitive(_entities->selectedNode());
            }
        }
    }

    event->ignore();
}

void EntityTreeWidget::update()
{
    //out << "treeWidget.update()" << endl;
    setCurrentItem(0);

    // The items are deleted in the inverse order due to the fact that because they are QObjects,
    // Qt's default behaviour is to also delete its children (if they are not deleted), and by
    // doing it in that order, the entities are found first, then the operations. That way Qt does
    // not have to delete anything (Qt use a wrong delete, and won't delete the EntityTreeWidgetItem
    // part, just the QObject part, and that will cause a segmentation fault. This happens because
    // multiple inheritance was used) and there won't be segmentation faults caused by attempting
    // to delete what Qt deleted automatically.
    for(int i=_items.size()-1; i>=0; --i)
    {
        //out << "update() - destroying item " << _items.at(i)->nameWidget()->text() << endl;
        if(_items.at(i) != 0)
        {
            delete _items.at(i);
            _items.replace(i, 0);
        }
    }

    _items.clear();

    foreach(EntityTreeNode * n, _entities->getFirstLevelEntities())
    {
        EntityTreeWidgetItem * item = createItems(n);
        addTopLevelItem(item);
        setWidgets(item);
    }
}

void EntityTreeWidget::updateSelected()
{
    //out << "treeWidget.updateSelected()" << endl;
    if(_entities->selected() != 0)
    {
        //out << "controller thinks that " << _entities->selected()->name() << " is currently selected" << endl;
        foreach(EntityTreeWidgetItem * item, _items)
        {
            if(_entities->isSelected(item->node()))
            {
                setCurrentItem(item);
                break;
            }
        }
    }
}

void EntityTreeWidget::processSelection()
{
    //out << "processSelection()" << endl;
    QTreeWidgetItem * temp = currentItem();
    EntityTreeWidgetItem * current = static_cast<EntityTreeWidgetItem*>(temp);

    if((current != 0) && (current->node() != 0) && (_entities != 0))
    {
        if(!_entities->isSelected(current->node()))
        {
            _entities->select(current->node()->id());
        }
    }
}

