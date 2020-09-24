#include "PhysicalItem.h"

PhysicalItem::PhysicalItem(ItemType * itemType): Entity(){
    m_itemType = itemType;
}

PhysicalItem::~PhysicalItem(){

}

void PhysicalItem::update(){

}
