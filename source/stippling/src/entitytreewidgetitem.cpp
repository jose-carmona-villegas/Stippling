#include "entitytreewidgetitem.h"

EntityTreeWidgetItem::EntityTreeWidgetItem(EntityTreeNode * node, EntityTreeController * controller) :  QTreeWidgetItem()
{
    _node = node;

    _controller = controller;

    //setText(0,_node->entity->name());
    //setFlags( flags() | Qt::ItemIsEditable );

    _name = new QLineEdit(_node->name());

    _visible = new QCheckBox();
    _visible->setChecked(_node->visible());

    _properties = new QPushButton();
    _properties->setText("Set");
    _properties->setToolTip("Specific configuration properties");

    _controlPoints = new QComboBox();
    _controlPoints->addItem("None");
    _controlPoints->addItem("Translation");
    _controlPoints->addItem("Resize");
    _controlPoints->addItem("Rotation");
    _controlPoints->addItem("All");
    switch(_node->paintMode())
    {
    case ENTITY:
        _controlPoints->setCurrentIndex(0);
        break;

    case TRANSLATION:
        _controlPoints->setCurrentIndex(1);
        break;

    case RESIZE:
        _controlPoints->setCurrentIndex(2);
        break;

    case ROTATION:
        _controlPoints->setCurrentIndex(3);
        break;

    case SCALE:
        //_controlPoints->setCurrentIndex(0);
        break;

    case ANY_CP:
        _controlPoints->setCurrentIndex(4);
        break;

    default:
        _controlPoints->setCurrentIndex(4);
        break;
    }

    _type = new QComboBox();
    if(!_node->isOperation())
    {
        _type->addItem("Primitive");
        _type->setCurrentIndex(0);
    }
    else
    {
        _type->addItem("Op. Union");
        _type->addItem("Op. Intersection");
        _type->addItem("Op. Difference");
        switch(_node->operation())
        {
        case BoolMesh::OP_UNION:
            _type->setCurrentIndex(0);
            break;
        case BoolMesh::OP_INTERSECTION:
            _type->setCurrentIndex(1);
            break;
        case BoolMesh::OP_DIFFERENCE:
            _type->setCurrentIndex(2);
            break;
        }
    }




    _name->setAcceptDrops(false);
    _visible->setAcceptDrops(false);
    _properties->setAcceptDrops(false);
    _controlPoints->setAcceptDrops(false);
    _type->setAcceptDrops(false);


    configDialog = 0;
}

EntityTreeWidgetItem::~EntityTreeWidgetItem()
{
    _node = 0;
    _controller = 0;

    if(_name != 0)
    {
        delete _name;
        _name = 0;
    }
    if(_visible != 0)
    {
        delete _visible;
        _visible = 0;
    }
    if(_properties != 0)
    {
        delete _properties;
        _properties = 0;
    }
    if(_controlPoints != 0)
    {
        delete _controlPoints;
        _controlPoints = 0;
    }
    if(_type != 0)
    {
        delete _type;
        _type = 0;
    }

    if(configDialog != 0)
    {
        delete configDialog;
        configDialog = 0;
    }
}

QLineEdit * EntityTreeWidgetItem::nameWidget()
{
    return _name;
}

QCheckBox * EntityTreeWidgetItem::visibleWidget()
{
    return _visible;
}

QPushButton * EntityTreeWidgetItem::properties()
{
    return _properties;
}

QComboBox * EntityTreeWidgetItem::controlPointsWidget()
{
    return _controlPoints;
}

QComboBox * EntityTreeWidgetItem::typeWidget()
{
    return _type;
}

EntityTreeNode * EntityTreeWidgetItem::node()
{
    return _node;
}

void EntityTreeWidgetItem::processNameEdition(QString name)
{
    if(_controller != 0)
    {
        _controller->setName(_node, name);
    }
}

void EntityTreeWidgetItem::processVisibilityEdition(bool visible)
{
    if(_controller != 0)
    {
        _controller->setVisible(_node, visible);
    }
}

void EntityTreeWidgetItem::openSpecificConfigurationDialog()
{
    if(configDialog != 0)
    {
        delete configDialog;
        configDialog = 0;
    }

    configDialog = new SpecificEntityConfigurationDialog();
    configDialog->loadConfiguration(_node->configuration());

    configDialog->show();
    configDialog->activateWindow();
}

void EntityTreeWidgetItem::processCPVisibilityEdition(QString choice)
{
    PaintMode mode;

    if(choice == "None")
    {
        mode = ENTITY;
    }
    else if(choice == "Translation")
    {
        mode = TRANSLATION;
    }
    else if(choice == "Resize")
    {
        mode = RESIZE;
    }
    else if(choice == "Rotation")
    {
        mode = ROTATION;
    }
    else if(choice == "Scale")
    {
        mode = SCALE;
    }
    else if(choice == "All")
    {
        mode = ANY_CP;
    }
    else
    {
        out << "EntityTreeWidgetItem#processCPVisibilityEdition() - Unrecognized paint mode" << endl;
        mode = ANY_CP;
    }

    if(_controller != 0)
    {
        _controller->setPaintMode(_node, mode);
    }
}


void EntityTreeWidgetItem::processTypeEdition(QString choice)
{
    if(choice == "Primitive")
    {
        // No need to change anything
    }
    else if(choice == "Op. Union")
    {
        _controller->setOperation(_node, BoolMesh::OP_UNION);
    }
    else if(choice == "Op. Intersection")
    {
        _controller->setOperation(_node, BoolMesh::OP_INTERSECTION);
    }
    else if(choice == "Op. Difference")
    {
        _controller->setOperation(_node, BoolMesh::OP_DIFFERENCE);
    }
    else
    {
        out << "EntityTreeWidgetItem#processTypeEdition() - Unrecognized type choice" << endl;
    }
}
