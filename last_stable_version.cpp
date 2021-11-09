#include <SFML/Graphics.hpp>
#include<SFML/Network.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<vector>
#include<string>
#include<math.h>

void direction (float& xv, float& yv, int vl, sf::Vector2f char_pos, sf::Vector2f char_size, std::vector<sf::RectangleShape> blocks, int& jump){

    for (int j =0;j<blocks.size();j++){
        sf::Vector2f b_pos = blocks[j].getPosition();
        sf::Vector2f b_size = blocks[j].getSize();
        if ((char_pos.x<b_pos.x+b_size.x) && (char_pos.x+char_size.x>b_pos.x) && (char_pos.y+char_size.y==b_pos.y)){
            jump=0;
        }
        if ((char_pos.x<b_pos.x+b_size.x) && (char_pos.x+char_size.x>b_pos.x) && (char_pos.y==b_pos.y+b_size.y)){
            yv=0;
        }

        if (((char_pos.y<b_pos.y+b_size.y) && (char_pos.y+char_size.y>b_pos.y) && (char_pos.x==b_pos.x+b_size.x)) || ((char_pos.y<b_pos.y+b_size.y) && (char_pos.y+char_size.y>b_pos.y) && (char_pos.x+char_size.x==b_pos.x))) {
            xv=0;
        }
    }

    if (yv<vl){
        yv+=1;
        if (yv>vl){
            yv=vl;
        }
    }

    if (jump==0){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            if (xv<vl){
                xv+=1;
            }
            if (xv>vl){
                xv=vl;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            if (xv>-vl){
                xv-=1;
            }
            if (xv<-vl){
                xv=-vl;
            }
        }
    }

    if (jump<2){
        if (yv>-10){
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                yv=-16;
                jump+=1;
            }
        }
    }

    if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && jump==0){
        if (xv>0){
            xv-=1;
            if (xv<0){
                xv=0;
            }
        }
        if (xv<0){
            xv+=1;
            if(xv>0){
                xv=0;
            }
        }
    }

    if (jump>0){
        if (xv>0){
            xv-=0.3;
            if (xv<1){
                xv=1;
            }
        }
        if (xv<0){
            xv+=0.3;
            if(xv>-1){
                xv=-1;
            }
        }
    }
}

void follow_pointer (float x_mouse, float y_mouse, sf::Vector2f block_pos, sf::Vector2f block_size, sf::Vector2f& shooting_thing_pos, sf::Vector2f& components){
    float x_center_character = block_pos.x+block_size.x/2;
    float y_center_character = block_pos.y+block_size.y/2;
    float x_distance = x_mouse-x_center_character;
    float y_distance = y_mouse-y_center_character;
    float distance = sqrt(pow(x_distance,2.0)+pow(y_distance,2.0));
    float x_component = x_distance/distance;
    float y_component = y_distance/distance;

    components = sf::Vector2f(x_component, y_component);

    shooting_thing_pos = sf::Vector2f(x_center_character+x_component*30-3, y_center_character+y_component*30-3);
    if (y_component>0){
        if (x_component>0 && x_component<0.746){
            shooting_thing_pos = sf::Vector2f(x_center_character+22.36-3, y_center_character+20-3);
        }
        if (x_component>-0.746 && x_component<=0 ){
            shooting_thing_pos = sf::Vector2f(x_center_character-22.36-3, y_center_character+20-3);
        }
    }
}


bool RayVRect(sf::Vector2f& ray_origin, sf::Vector2f& ray_dir, sf::RectangleShape block, sf::Vector2f& contact, float& T_H_Near, sf::Vector2f& normal){

    normal.x = 0;
    normal.y = 0;

    sf::Vector2f block_pos = block.getPosition();
    sf::Vector2f block_size = block.getSize();

    sf::Vector2f t_near = sf::Vector2f((block_pos-ray_origin).x /ray_dir.x, (block_pos-ray_origin).y /ray_dir.y);
    
    sf::Vector2f t_far = sf::Vector2f((block_pos+block_size-ray_origin).x /ray_dir.x, (block_pos+block_size-ray_origin).y /ray_dir.y);


    if (std::isnan(t_far.y) || std::isnan(t_far.x)){
        return false;
    } 
	if (std::isnan(t_near.y) || std::isnan(t_near.x)){
        return false;
    }
    if (t_near.x>t_far.x){
        std::swap(t_near.x, t_far.x);
    } 
    if ((ray_origin.x==block_pos.x && ray_dir.x<0)||(ray_origin.x==(block_pos.x+block_size.x) && ray_dir.x>0)){
        std::swap(t_near.x, t_far.x);
    }
    if (t_near.y>t_far.y){
        std::swap(t_near.y,t_far.y);
    } 
    if ((ray_origin.y==block_pos.y && ray_dir.y<0)||(ray_origin.y==(block_pos.y+block_size.y) && ray_dir.y>0)){
        std::swap(t_near.y,t_far.y);
    }

    if (t_near.x > t_far.y || t_near.y > t_far.x){
        return false;
    } 
    T_H_Near = std::max(t_near.x,t_near.y);

    float t_hit_far = std::min(t_far.x, t_far.y);

    if (t_hit_far < 0){
        return false;
    }

    contact= ray_origin+(sf::Vector2f(T_H_Near*ray_dir.x, T_H_Near*ray_dir.y));

    if (t_near.x > t_near.y){
        if (ray_dir.x<0){
            normal.x=1;
            normal.y=0;
        }
        else{
            normal.x=-1;
            normal.y=0;
        }
    }
    else if(t_near.x<t_near.y){
        if (ray_dir.y<0){
            normal.x=0;
            normal.y=1;
        }
        else{
            normal.x=0;
            normal.y=-1;
        }
    }

    return true;
}

bool dynamicrectvrect(sf::RectangleShape& dynamic, sf::RectangleShape& block, sf::Vector2f& contact, sf::Vector2f& normal, float& time, float&xchange, float& ychange){

    sf::Vector2f change = sf::Vector2f(xchange, ychange);

    sf::Vector2f block_pos = block.getPosition();
    sf::Vector2f block_size = block.getSize();

    sf::Vector2f character_pos = dynamic.getPosition();
    sf::Vector2f character_size = dynamic.getSize();
    sf::Vector2f dynamic_center = sf::Vector2f(character_pos.x+(character_size.x/2), character_pos.y+(character_size.y/2));

    // if the velocity is 0, there will be no collision.
    if(change.x==0 && change.y==0){
        return false;
    }
    
    sf::Vector2f expanded_pos = sf::Vector2f(block_pos.x - (character_size.x/2), block_pos.y - (character_size.y/2) );
    sf::Vector2f expanded_size = sf::Vector2f(block_size.x + character_size.x, block_size.y + character_size.y);

    sf::RectangleShape expanded_block(expanded_size);
    expanded_block.setPosition(expanded_pos);

    if (RayVRect(dynamic_center, change, expanded_block, contact, time,  normal)){
        if (time<=1){
            return true;
        }   
    }
    return false;
}

void bullets_dir(sf::Vector2f shooting_thing_pos, sf::Vector2f components, std::vector<sf::CircleShape>& bullets, std::vector<sf::Vector2f>& bullet_dir, float& shoot_t, float& load_t, float dt, int& bullets_avail){

    if (shoot_t<0.6){
        shoot_t+=dt;
    }

    if (bullets_avail>0 && shoot_t>=0.6){
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
            bullets.push_back(sf::CircleShape(3));
            bullet_dir.push_back(components);
            bullets[bullets.size()-1].setPosition(shooting_thing_pos);
            bullets[bullets.size()-1].setFillColor(sf::Color::White);
            bullets_avail-=1;
            shoot_t=0;
        } 
    }
    if(bullets_avail==0 && load_t<2){
        load_t +=dt;
    }
    if (bullets_avail==0 && load_t>=2){
        load_t =0;
        bullets_avail =10;
    }
}

void bullets_move(std::vector<sf::CircleShape>& bullets, std::vector<sf::Vector2f>& bullet_dir, std::vector<sf::RectangleShape> blocks, float dt){
    int bullets_speed=20; 
    int multi =60;
    float x_bullet_vel;
    float y_bullet_vel; 
    sf::Vector2f contact;
    float time_hit;
    sf::Vector2f normal;
    sf::Vector2f bull_center;
    sf::Vector2f bull_dir_vector;
    std::vector<int> deleted;

    
    for (int bd=0; bd<bullets.size();bd++){
        x_bullet_vel= dt*bullets_speed*multi*bullet_dir[bd].x;
        y_bullet_vel= dt*bullets_speed*multi*bullet_dir[bd].y;
        bull_center= sf::Vector2f(bullets[bd].getPosition().x+1.5,bullets[bd].getPosition().y+1.5 );
        bull_dir_vector = sf::Vector2f(x_bullet_vel, y_bullet_vel);
        for (int b=0; b<blocks.size(); b++){
            if (RayVRect(bull_center,bull_dir_vector, blocks[b], contact, time_hit, normal) && time_hit<=1){
                deleted.push_back(bd);
                break;
            }
        }
        bullets[bd].move(sf::Vector2f(x_bullet_vel, y_bullet_vel));
    }
    for (int d=0; d<deleted.size();d++){
        bullets.erase(bullets.begin()+deleted[d]);
        bullet_dir.erase(bullet_dir.begin());
    }
    deleted.clear();
    
}

void spawn_enemies(float dt, float& spawn_timer, std::vector<sf::RectangleShape>& enemies){

    float spawn_pos[4][2] = {{0,900},{0,360},
                            {1860,360},{1860,900}};
    int rand_spawn_pos;
    int spawn_time =5;
    spawn_timer += dt;

    if (spawn_timer>5){
        rand_spawn_pos = rand()%4;
        spawn_timer=0;
        enemies.push_back(sf::RectangleShape(sf::Vector2f(60,60)));
        enemies[enemies.size()-1].setPosition(sf::Vector2f(spawn_pos[rand_spawn_pos][0],spawn_pos[rand_spawn_pos][1]));
        enemies[enemies.size()-1].setFillColor(sf::Color::Yellow);
    }
}

int main (){

    int window_widht=1000;
    int window_height=600;
    float x_vel=0;
    float y_vel=0;
    float x_change;
    float y_change;
    float x_mouse;
    float y_mouse;
    float time;
    int multi=60;
    int vel_limit=10;
    int jump;
    bool collision;
    float shoot_time = 0;
    float reload_time = 0;
    int bullets_available=0;
    float spawn_timer;
    sf::Vector2f shooting_thing_pos;
    sf::Vector2f contact_point;
    sf::Vector2f contact_normal;
    sf::Vector2f components;

    std::vector<sf::RectangleShape> blocks;
    std::vector<sf::CircleShape> bullets;
    std::vector<sf::Vector2f> bullets_components;
    std::vector<sf::RectangleShape> enemies1;
    std::vector<sf::RectangleShape> enemies1hitbox;
    std::vector<sf::RectangleShape> enemies2;

    float pos_size[18][4] = {{0,960,420,120},{780,780,360,60 },{1500,960,420,120},
                            {180,720,180 ,60}, { 300,660 ,300 ,60 }, { 1560,720 ,180 ,60 },
                            { 1320, 660, 300, 60}, { 0,420 ,300 ,60 }, { 660, 420,600 ,60 },
                            { 1620,420 ,300 ,60 }, { 240,180 ,480 ,60 }, {420 ,120 ,120 ,180 },
                            { 1200, 180, 480,60 }, { 1380, 120,120 ,180 }, { -10,-10 ,1940 ,10 }, 
                            { 1920, -10,10 ,1100 }, { -10, 1080,1940 ,10 }, { -10,-10 ,10 ,1100 } };
    int rows = sizeof(pos_size)/sizeof(pos_size[0]);

    for (int j=0;j<rows;j++){
        blocks.push_back(sf::RectangleShape(sf::Vector2f(pos_size[j][2],pos_size[j][3])));
        blocks[j].setPosition(sf::Vector2f(pos_size[j][0],pos_size[j][1]));
        blocks[j].setFillColor(sf::Color::Blue);
    }

    sf::Clock clock;
    float dt;

    sf::RenderWindow window(sf::VideoMode(window_widht, window_height), "SFML works!", sf::Style::Fullscreen);
    window.setFramerateLimit(200);

    sf::RectangleShape character(sf::Vector2f(30.f, 40.f));
    character.setPosition(sf::Vector2f(200.f,100.f));
    character.setFillColor(sf::Color::Red);
    sf::Vector2f character_pos = character.getPosition();
    sf::Vector2f character_size = character.getSize();

    sf::CircleShape shooting_thing(3);
    shooting_thing_pos= sf::Vector2f(character_pos.x+(character_size.x/2)+30, character_pos.y+(character_size.y/2));
    shooting_thing.setFillColor(sf::Color::White);
    shooting_thing.setPosition(shooting_thing_pos);

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
        }

        x_mouse = sf::Mouse::getPosition(window).x;
        y_mouse = sf::Mouse::getPosition(window).y;

        direction(x_vel, y_vel, vel_limit, character_pos, character_size, blocks, jump);

        x_change = dt*multi*x_vel;
        y_change = dt*multi*y_vel;

        for (int i =0;i<blocks.size();i++){
            if (dynamicrectvrect(character, blocks[i],contact_point, contact_normal, time, x_change, y_change )){
                x_change += contact_normal.x*std::abs(x_change)*(1-time);
                y_change += contact_normal.y*std::abs(y_change)*(1-time);
        }
        }

        character.move(x_change, y_change);

        character_pos = character.getPosition();
        character_size = character.getSize();

        follow_pointer(x_mouse, y_mouse, character_pos, character_size, shooting_thing_pos, components);
        shooting_thing.setPosition(shooting_thing_pos);
        bullets_dir(shooting_thing_pos, components, bullets,bullets_components, shoot_time, reload_time, dt, bullets_available);
        bullets_move(bullets, bullets_components, blocks, dt);
        spawn_enemies(dt,spawn_timer,enemies1);


        window.clear(sf::Color(0,0,0,255));

        for (int e=0; e<enemies1.size();e++){
            window.draw(enemies1[e]);
        }

        for (int i =0;i<blocks.size();i++){
            window.draw(blocks[i]);
        }
    
        for(int b=0;b<bullets.size();b++){
            window.draw(bullets[b]);
        }

        window.draw(shooting_thing);
        window.draw(character);

        window.display();

        dt = clock.restart().asSeconds();
    }

    

}