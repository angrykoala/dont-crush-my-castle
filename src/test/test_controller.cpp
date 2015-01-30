//TITLE: TEST_CONTROLLER
//PROJECT: DON´T CRUSH MY CASTLE
//AUTHOR: Andrés Ortiz
//VERSION: 0.5
//DESCRIPTION: will test different game controllers
/*This test will check:
game_objects
player_controller
*/

#include "player_controller.h"
#include "game_master.h"

int main() {
    cout<<"GAME CONTROLLERS TEST";
    bool test_result=true;
    //ALLEGRO display,timer and queue pointers
    ALLEGRO_TIMER *timer;
    //allegro startup with image addon
    al_init();
    al_init_image_addon();
    timer = al_create_timer(1.0 / 60); //60 fps timer
    al_start_timer(timer); //begin timer
    ALLEGRO_BITMAP *bmp=al_load_bitmap("spr/example_clock.png");
    resize_bitmap(bmp,100,100);
    //ENEMY
    al_anim anim(bmp,50,50,2,timer);
    enemy_attributes testenemy_attr("minion test",100,10,2,5);
    map<enemy_animation,al_anim> vanim;
    vanim.insert(make_pair(idle_anim,anim));
    vanim.insert(make_pair(up_anim,anim));
    vanim.insert(make_pair(down_anim,anim));
    vanim.insert(make_pair(left_anim,anim));
    vanim.insert(make_pair(right_anim,anim));
    vanim.insert(make_pair(dead_anim,anim));
    testenemy_attr.animation=vanim;
    enemy testenemy(testenemy_attr,0,0,0,timer);
    if(testenemy.check()==false) test_result=false;
    //TOWER
    atk_attributes attr(bmp,anim,10,8,3,2,shoot_atk);
    tower_attributes attr2("towerTest",bmp,attr,110);
    tower towertest(attr2,0,0,timer);
    if(towertest.check()==false) test_result=false;
    //GAMEOBJECTS
    game_objects go;
    if(go.enemy_size()!=0 || go.tower_size()!=0) test_result=false;
    if(go.check()==false) test_result=false;
    go.add_enemy(testenemy);
    tower_id tid=go.add_tower(towertest);
    if(go.check()==false) test_result=false;
    if(go.enemy_size()!=1 || go.tower_size()!=1) test_result=false;
    list<enemy>::iterator it=go.get_first_enemy();
    if(it->get_name()!="minion test") test_result=false;
    if(it->is_active()==false) test_result=false;
    it->kill();
    it++;
    if(go.get_last_enemy()!=it) test_result=false;
    tower *tt=go.get_tower(tid);
    if(tt->get_name()!="towerTest") test_result=false;
    if(tt->can_attack()==true) test_result=false;
    if(tt->is_active()==false) test_result=false;
    if(go.enemy_size()!=1) test_result=false;
    for(int i=0; i<180; i++) {
        go.update_towers();
        go.update_enemies();
    }
    vector<tower_id> v=go.update_towers();
    if(v.size()!=1) test_result=false;
    if(go.get_tower(tid)->can_attack()==false) test_result=false;
    if(tt->can_attack()==false) test_result=false;
    if(go.enemy_size()!=0 || go.tower_size()!=1) test_result=false;
    go.remove_tower(tid);
    if(go.check()==false) test_result=false;
    if(go.tower_size()!=0) test_result=false;
    //PLAYER CONTROLLER
    ALLEGRO_BITMAP *bmp3=al_load_bitmap("spr/example_clock.png");
    tower_attributes attr3("towerTest",bmp3,attr,110);
    tower_set towers("tset",attr3,timer);
    ALLEGRO_BITMAP *bmp2=al_load_bitmap("spr/example_clock.png");
    vector<tile_type> types;
    types.push_back(ground);
    types.push_back(road);
    types.push_back(road);
    types.push_back(road);
    //tile set test with 4 tiles of 20x20
    tileset tiles("test",bmp2,types,20,4);
    vector<tile_id> row;
    row.push_back(0);//ground
    row.push_back(1);//road
    row.push_back(3);//road
    vector<vector<tile_id> > mapids;
    mapids.push_back(row);
    mapids.push_back(row);
    mapids.push_back(row);
    mapids.push_back(row);
    /*Map (with the tset "test)
    2 2 2 2
    1 1 1 1
    3 3 3 3
    */
    //i=vertical
    //j=horizontal
    set<pair<unsigned int,unsigned int> > dest;
    dest.insert(make_pair(0,2));
    tilemap testmap("testing map",mapids,&tiles,dest);
    player_controller pc(towers,go,testmap);
    if(pc.check()==false) test_result=false;
    if(pc.get_towers_size()!=towers.get_size()) test_result=false;
    if(pc.get_tower_names().size()!=towers.get_size()) test_result=false;
    if(pc.build_tower("towerTest",1,1)==false) test_result=false;
    if(pc.build_tower("towerTest",10,10)==true) test_result=false;
    if(pc.build_tower("towerTest",10,30)==true) test_result=false;
    if(pc.spawned_towers()!=1) test_result=false;
    if(pc.can_build(10,10)==true || pc.can_build(10,30)==true) test_result=false;
    if(pc.can_build(30,10)==false) test_result=false;
    if(pc.check()==false) test_result=false;
    if(pc.get_tower(12,8)->get_name()!="towerTest") test_result=false;
    pc.remove_tower(1,1);
    if(pc.spawned_towers()!=0) test_result=false;
    if(pc.check()==false) test_result=false;
    al_destroy_bitmap(bmp2); //this is not destryed because tileset copy original bitmap with slice
    al_destroy_timer(timer);
    if(test_result==true) cout<<" - OK\n";
    else cout<<" - FAIL\n";
    return !test_result;
}