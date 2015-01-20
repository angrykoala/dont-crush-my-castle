//TITLE: Map Test
//PROJECT: DON´T CRUSH MY CASTLE
//AUTHOR: Andrés Ortiz
//VERSION: 0.3
//DESCRIPTION: thest od DCmC for maps
/*This test will check:
tile
tileset
tilemap
*/
#include "al_anim.h"
#include "tilemap.h"

void test(tile &t) {
    ALLEGRO_BITMAP *bmp=al_load_bitmap("spr/example_clock.png");
    tile tt(road,bmp,80);
    t=tt;
}

int main() {
    cout<<"TILEMAP TEST";
    //Final result of test, true if everything OK
    bool test_result=true;
    //ALLEGRO timer and queue pointers
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *event_queue;
    //allegro startup with image addon
    al_init();
    al_init_image_addon();
    //Declaration and assign of display,event_queue and timer
    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / 60); //60 fps timer
    //register timer and events
    al_register_event_source(event_queue,al_get_timer_event_source(timer));
    al_start_timer(timer); //begin timer
    //load example bitmap
    ALLEGRO_BITMAP *bmp=al_load_bitmap("spr/example_clock.png");
    if(bmp==NULL) {
        cout<<" || ERROR - bitmap not loaded";
        test_result=false;
    }
    //TESTING TILE
    //road tile with given bmp,512x512
    tile testtile(road,bmp,512);
    if(testtile.check()==false) test_result=false;
    if(testtile.get_size()!=512) test_result=false;
    if(testtile.type!=road) test_result=false;
    tile *testtile2=new tile(ground,bmp,256);
    testtile=*testtile2;
    delete testtile2;
    if(testtile.get_size()!=256 || testtile.type!=ground) test_result=false;
    test(testtile);
    if(testtile.get_size()!=80) test_result=false;
    //TESTING TILESET
    vector<tile_type> types;
    types.push_back(ground);
    types.push_back(road);
    types.push_back(road);
    types.push_back(road);
    //tile set test with 4 tiles of 20x20
    tileset tset("test",bmp,types,20,4);
    if(tset.size()!=4) test_result=false;
    if(tset.get_name()!="test") test_result=false;
    if(tset.get_tile_width()!=20) test_result=false;
    if(tset.add_tile(testtile)!=4) test_result=false;
    if(tset.size()!=5) test_result=false;
    tset.remove_tile(2);
    if(tset.size()!=4) test_result=false;
    if(tset.add_tile(ground,bmp)!=2) test_result=false;
    if(tset.size()!=5) test_result=false;
    vector<tile_id> v2=tset.load_from_bitmap(bmp,types,25,4);
    if(tset.size()!=9) test_result=false;
    if(v2.size()!=4) test_result=false;
    for(unsigned int i=0; i<v2.size(); i++)
        if(v2[i]!=(int) i+5) test_result=false;
    if(tset.check()==false) test_result=false;
    tileset tset2(testtile,12);
    if(tset2.get_tile_width()!=12) test_result=false;
    if(tset2.size()!=1) test_result=false;
    tileset tset3(testtile);
    if(tset3.size()!=1) test_result=false;
    if(tset3.get_tile_width()!=80) test_result=false;
    //TESTING MAP
    vector<tile_id> row;
    row.push_back(2);//ground
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
    tilemap testmap("testing map",mapids,&tset);
    if(testmap.get_name()!="testing map") test_result=false;
    if(testmap.get_width()!=4 || testmap.get_height()!=3) test_result=false;
    if(testmap.check()==false) test_result=false;
    testmap.occupy_tile(1,1);
    if(testmap.empty_section(0,0,4,1)==false) test_result=false;
    if(testmap.empty_section(1,0,1,2)==true) test_result=false;
    //TESTING BACKGROUND && foreground
    for(unsigned int i=0; i<testmap.get_width(); i++) {
        for(unsigned int j=0; j<testmap.get_height(); j++) {
            if(testmap.get_tile_id(i,j)!=mapids[i][j]) test_result=false;
            if(testmap.get_tile_type(i,j)==road && j==0) test_result=false;
            if(testmap.get_tile_type(i,j)==ground && j!=0) test_result=false;
            if(testmap.empty_tile(i,j)==true && j!=0 && (i!=1 && j!=1)) test_result=false;
            if(testmap.empty_tile(i,j)==false && j==0) test_result=false;
        }
    }
    vector<tile_id> row2;
    row2.push_back(3);//road
    vector<vector<tile_id> > mapids2;
    mapids2.push_back(row);
    mapids2.push_back(row2);
    /*
    2 3
    1 -
    3 -
    */
    tilemap testmap2("testing map2",mapids2,&tset);
    if(testmap2.get_name()!="testing map2") test_result=false;
    if(testmap2.get_width()!=2 || testmap2.get_height()!=3) test_result=false;
    if(testmap2.check()==false) test_result=false;
    for(unsigned int i=0; i<testmap2.get_width(); i++) {
        for(unsigned int j=0; j<testmap2.get_height(); j++) {
            if(testmap2.get_tile_id(i,j)!=null_tile_id && i==1 && j>=1) test_result=false;
        }
    }
    //CHECK PATH
    /* vector< pair<unsigned int,unsigned int> > dest;
     dest.push_back(make_pair(0,2));
    // testmap.set_destiny(dest);
     cout<<endl;
      for(unsigned int i=0; i<testmap.get_width(); i++) {
         for(unsigned int j=0; j<testmap.get_height(); j++) {
             cout<<testmap.get_path_value(i,j)<<" ";

         }
         cout<<endl;
     }*/
    //TODO: test map with different rowd (different size)
    //test background,foreground and path map
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    //MAP UNTESTED YET
    if(test_result==true) cout<<" - OK\n";
    else cout<<" - FAIL\n";
    return !test_result;
}

