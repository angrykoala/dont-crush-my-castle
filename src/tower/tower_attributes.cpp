//TITLE: TOWER_ATTRIBUTES_CPP
//PROJECT: DON´T CRUSH MY CASTLE
//AUTHOR: Andrés Ortiz
//VERSION: 0.7.6
//DESCRIPTION: defines each kind of tower


#include "tower_attributes.h"
tower_attributes::tower_attributes() {
    this->bitmap=nullptr;
}
tower_attributes::tower_attributes(const XMLElement *tower_root,const ALLEGRO_TIMER *timer) {
    //if(!read_xml(attributes_root)) destroy();
    this->bitmap=nullptr;
    read_xml(tower_root,timer);
}
tower_attributes::tower_attributes(const string &name,ALLEGRO_BITMAP *bitmap,const atk_attributes &atk,unsigned int cost) {
    this->bitmap=bitmap;
    this->atk=atk;
    this->name=name;
    this->cost=cost;
}
tower_attributes::~tower_attributes() {
}
bool tower_attributes::read_xml(const XMLElement *tower_root,const ALLEGRO_TIMER *timer) {
    bool b=true;
    destroy();
    if(tower_root == nullptr) b=false;
    else if(tower_root->Value()!=tower_xml_value) b=false;
    else {
        const char *version=tower_root->Attribute("Version");
        //Compare version!!!!
        if(version==nullptr) return false;
        const XMLElement *name_element,*atk_element,*sprite_element,*cost_element;
        name_element=tower_root->FirstChildElement("Name");
        atk_element=tower_root->FirstChildElement(tower_atk_xml_value.c_str());
        sprite_element=tower_root->FirstChildElement("Sprite");
        cost_element=tower_root->FirstChildElement("Cost");
        if(name_element==nullptr || atk_element==nullptr || sprite_element==nullptr || cost_element==nullptr) return false;
        if(cost_element->QueryUnsignedText(&cost)!=XML_SUCCESS) return false;
        const char *nam=name_element->GetText();
        this->name=string(nam);
        if(name.empty()) return false;
        if(atk.read_xml(atk_element,timer)==false) return false;
        ALLEGRO_BITMAP *bmp=al_load_bitmap(sprite_element);
        if(!bmp) b=false;
        else this->bitmap=bmp;
    }
    return b;
}
bool tower_attributes::read_xml(const string &filename,const ALLEGRO_TIMER *timer) {
    XMLDocument document;
    XMLElement *element=get_root_element(filename,document);
    if(element==nullptr) return false;
    else return read_xml(element,timer);
}
unsigned int tower_attributes::get_width() const {
    return al_get_bitmap_width(bitmap);
}
unsigned int tower_attributes::get_height() const {
    return al_get_bitmap_height(bitmap);
}
void tower_attributes::resize(unsigned int width,unsigned int height) {
    resize_bitmap(bitmap,width,height);
}
void tower_attributes::destroy() {
    al_destroy_bitmap(bitmap);
    bitmap=nullptr;
    atk.destroy();
}
bool tower_attributes::check() const {
    bool b=true;
    if(bitmap==nullptr) b=false;
    if(atk.check()==false) b=false;
    if(name.empty()==true) b=false;
    return b;
}
