#include "PihaQComponentTreeItem.h"

#include "PihaQDeviceTreeItem.h"
#include "PihaQComponentTreeItemEditorFactory.h"

namespace Piha
{

QComponentTreeItem::QComponentTreeItem( QDeviceTreeItem* parent, Component* component )
	:	QTreeWidgetItem( parent ),
		mComponent(component)
{
	QIcon icon = QComponentTreeItemEditorFactory::getComponentIcon( component );
	setIcon( 0, icon );

	QWidget* editor = QComponentTreeItemEditorFactory::createTreeItemEditor( component, NULL );
	if ( editor )
		treeWidget()->setItemWidget( this, 4, editor );
	
	updateContents();
}

QComponentTreeItem::~QComponentTreeItem()
{
}

void QComponentTreeItem::updateContents()
{
	Component* component = getComponent();
	std::string typeName = component->getTypeName();
	capitalizeFirstLetter(typeName);
	setText( 0, QString::fromUtf8( typeName.c_str() ) );
	setText( 1, QString::number( component->getIndexInParentDevice() ) );
	if ( !component->getName().empty() )
	{
		std::string name = component->getName();
		capitalizeFirstLetter(name);
		setText( 2, QString::fromUtf8( name.c_str() ) );
	}
	else
	{
		setText( 2, "<Unnamed>" );
	}
	
	QString state = QComponentTreeItemEditorFactory::getComponentState( component );
	setText( 3, state );

	QComponentEditorWidget* editor = static_cast<QComponentEditorWidget*>( treeWidget()->itemWidget( this, 4 ) );
	if ( editor )
		editor->updateContents();
}

void QComponentTreeItem::onItemClicked( int /*column*/ )
{
}

void QComponentTreeItem::capitalizeFirstLetter( std::string& text )
{
	if ( text.empty() )
		return;
	text[0] = static_cast<char>(toupper( text[0] ));
}

}