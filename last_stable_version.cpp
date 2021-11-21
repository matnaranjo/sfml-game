#include <SFML/Graphics.hpp>
#include<SFML/Network.hpp>
#include<SFML/System.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<vector>
#include<string>
#include<math.h>

void direction (float& xv, float& yv, int vl, sf::Vector2f char_pos, sf::Vector2f char_size, std::vector<sf::RectangleShape> blocks, sf::RectangleShape crystal, std::vector<sf::RectangleShape> enemies, int& jump, float wx, float wy){
    sf::Vector2f crystal_size= crystal.getSize();
    sf::Vector2f crystal_pos = crystal.getPosition();

    // solving changes in speed when crashing against a block.
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

    for (int j=0; j<enemies.size();j++){
        sf::Vector2f e_pos = enemies[j].getPosition();
        sf::Vector2f e_size = enemies[j].getSize();
        if ((char_pos.x<e_pos.x+e_size.x) && (char_pos.x+char_size.x>e_pos.x) && (char_pos.y+char_size.y==e_pos.y)){
            jump=0;
        }
        if ((char_pos.x<e_pos.x+e_size.x) && (char_pos.x+char_size.x>e_pos.x) && (char_pos.y==e_pos.y+e_size.y)){
            yv=0;
        }
    }

    // Solving changes in speed when crashing against the crystal.
    if ((char_pos.x<crystal_pos.x+crystal_size.x) && (char_pos.x+char_size.x>crystal_pos.x) && (char_pos.y+char_size.y==crystal_pos.y)){
            jump=0;
        }
    if ((char_pos.x<crystal_pos.x+crystal_size.x) && (char_pos.x+char_size.x>crystal_pos.x) && (char_pos.y==crystal_pos.y+crystal_size.y)){
            yv=0;
        } 
    if (((char_pos.y<crystal_pos.y+crystal_size.y) && (char_pos.y+char_size.y>crystal_pos.y) && (char_pos.x==crystal_pos.x+crystal_size.x)) || ((char_pos.y<crystal_pos.y+crystal_size.y) && (char_pos.y+char_size.y>crystal_pos.y) && (char_pos.x+char_size.x==crystal_pos.x))) {
            xv=0;
        }

    // gravity
    if (yv<vl){
        yv+=wy*0.00092592592;
        if (yv>vl){
            yv=vl;
        }
    }

    // If the character is not jumping, we are able to move it in the map right or left.
    if (jump==0){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            if (xv<vl){
                xv+=wx*0.00052083333;
            }
            if (xv>vl){
                xv=vl;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            if (xv>-vl){
                xv-=wx*0.00052083333;
            }
            if (xv<-vl){
                xv=-vl;
            }
        }
    }

    // if whe have not done a double jump, we will be able to do it.
    if (jump<2){
        if (yv>-(wy*0.00925925925)){
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                yv=-wy*0.01481481481;
                jump+=1;
            }
        }
    }

    // If we dont press any key, the speed will decrease to 0
    if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && jump==0){
        if (xv>0){
            xv-=wx*0.00052083333;
            if (xv<0){
                xv=0;
            }
        }
        if (xv<0){
            xv+=wx*0.00052083333;
            if(xv>0){
                xv=0;
            }
        }
    }

    // if the character is jumping, horizontal speed will decrease.
    if (jump>0){
        if (xv>0){
            xv-=wx*0.00015625;
            if (xv<1){
                xv=wx*0.00052083333;
            }
        }
        if (xv<0){
            xv+=wx*0.00015625;
            if(xv>-1){
                xv=-wx*0.00052083333;
            }
        }
    }
    
}

void follow_pointer (float x_mouse, float y_mouse, sf::Vector2f block_pos, sf::Vector2f block_size, sf::Vector2f& shooting_thing_pos, sf::Vector2f& components, float wx, float wy){
    float x_center_character = block_pos.x+block_size.x/2;
    float y_center_character = block_pos.y+block_size.y/2;
    float x_distance = x_mouse-x_center_character;
    float y_distance = y_mouse-y_center_character;
    float distance = sqrt(pow(x_distance,2.0)+pow(y_distance,2.0));
    float x_component = x_distance/distance;
    float y_component = y_distance/distance;

    components = sf::Vector2f(x_component, y_component);

    shooting_thing_pos = sf::Vector2f(x_center_character+(x_component*(wx*0.016))-((wx*0.0015625)), y_center_character+(y_component*(wx*0.016))-((wx*0.0015625)));

    if (y_component>0){
        if (x_component>0 && x_component<0.787){
            x_component = 0.787;
            y_component = 0.616;
            shooting_thing_pos = sf::Vector2f(x_center_character+(x_component*(wx*0.016))-((wx*0.0015625)), y_center_character+(y_component*(wx*0.016))-((wx*0.0015625)));
        }
        if (x_component>-0.787 && x_component<=0 ){
            x_component = -0.787;
            y_component = 0.616;
            shooting_thing_pos = sf::Vector2f(x_center_character+(x_component*(wx*0.016))-((wx*0.0015625)), y_center_character+(y_component*(wx*0.016))-((wx*0.0015625)));
        }
        components = sf::Vector2f(x_component, y_component);
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

void bullets_dir(sf::Vector2f shooting_thing_pos, sf::Vector2f components, std::vector<sf::CircleShape>& bullets, std::vector<sf::Vector2f>& bullet_dir, float& shoot_t, float& load_t, float dt, int& bullets_avail, float wx, float wy){

    if (shoot_t<0.6){
        shoot_t+=dt;
    }

    if (bullets_avail>0 && shoot_t>=0.3){
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
            bullets.push_back(sf::CircleShape(wx*0.0015625));
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
        bullets_avail =200;
    }
}

void bullets_move(std::vector<sf::CircleShape>& bullets, std::vector<sf::Vector2f>& bullet_dir, std::vector<sf::RectangleShape> blocks,std::vector<sf::RectangleShape> enemies,std::vector<float>& enemies_lf, float dt, float wx, float wy){
    int bullets_speed=wx*0.01041666666; 
    int multi =60;
    float x_bullet_vel;
    float y_bullet_vel; 
    bool first_contact = false;
    sf::Vector2f contact;
    float time_hit;
    sf::Vector2f normal;
    sf::Vector2f bull_center;
    sf::Vector2f bull_dir_vector;
    std::vector<int> deleted_b;
    std::vector<int> hit_enemy;

    
    for (int bd=0; bd<bullets.size();bd++){
        x_bullet_vel= dt*bullets_speed*multi*bullet_dir[bd].x;
        y_bullet_vel= dt*bullets_speed*multi*bullet_dir[bd].y;
        bull_center= sf::Vector2f(bullets[bd].getPosition().x+(wx*0.00078125),bullets[bd].getPosition().y+(wy*0.00138888888));
        bull_dir_vector = sf::Vector2f(x_bullet_vel, y_bullet_vel);
        for (int e=0; e<enemies.size(); e++){
            if (RayVRect(bull_center,bull_dir_vector, enemies[e], contact, time_hit, normal) && time_hit<=1){
                deleted_b.push_back(bd);
                hit_enemy.push_back(e);
                first_contact = true;
                break;
            }
        }
        if (!first_contact){
            for (int b=0; b<blocks.size(); b++){
                if (RayVRect(bull_center,bull_dir_vector, blocks[b], contact, time_hit, normal) && time_hit<=1){
                    deleted_b.push_back(bd);
                    break;
                }
            }
        }
        
        first_contact=false;
        bullets[bd].move(sf::Vector2f(x_bullet_vel, y_bullet_vel));
    }
    for (int h=0; h<hit_enemy.size();h++){
        enemies_lf[h] -=0.25;
    }
    for (int d=0; d<deleted_b.size();d++){
        bullets.erase(bullets.begin()+deleted_b[d]);
        bullet_dir.erase(bullet_dir.begin());
    }
    deleted_b.clear();
    
}

void spawn_enemies(float dt, float& spawn_timer, std::vector<sf::RectangleShape>& enemieshitbox, std::vector<sf::RectangleShape>& enemies, std::vector<sf::RectangleShape>& enemies_lb, std::vector<float>& enemies_lp, std::vector<int>& enemy_class, std::vector<float>& hit_timmer,std::vector<bool>& hitting, float wx, float wy){

    float spawn_pos[4][2] = {{0,wy*0.83333333333},{0,0.33333333333*wy},
                            {wx*0.96875,0.33333333333*wy},{wx*0.96875,wy*0.83333333333}};

    float spawn_pos_enemy[4][2]= {{wx*0.00520833333,wy*0.83333333333},{wx*0.00520833333,0.33333333333*wy},
                                {wx*0.97395833333,0.33333333333*wy},{wx*0.97395833333,wy*0.83333333333 }};

    float spawn_pos_lb[4][2] = {{0,wy*0.81},{0,0.31*wy},
                            {wx*0.96875,0.31*wy},{wx*0.96875,wy*0.81}};

    int rand_spawn_pos;
    int spawn_time =5;
    spawn_timer += dt;
    if (spawn_timer>10){
        std::cout<<" ";
        rand_spawn_pos = rand()%4;
        spawn_timer=0;
        // enemies hitbox.
        enemieshitbox.push_back(sf::RectangleShape(sf::Vector2f(wx*0.03125,wy*0.05555555555)));
        enemieshitbox[enemieshitbox.size()-1].setPosition(sf::Vector2f(spawn_pos[rand_spawn_pos][0],spawn_pos[rand_spawn_pos][1]));
        enemieshitbox[enemieshitbox.size()-1].setFillColor(sf::Color::Yellow);

        // enemies life bar
        enemies_lb.push_back(sf::RectangleShape(sf::Vector2f(wx*0.03125,wy*0.00277777777)));
        enemies_lb[enemies_lb.size()-1].setPosition(sf::Vector2f(spawn_pos_lb[rand_spawn_pos][0],spawn_pos_lb[rand_spawn_pos][1]));
        enemies_lb[enemies_lb.size()-1].setFillColor(sf::Color::Red);

        // enemies
        enemies.push_back(sf::RectangleShape(sf::Vector2f(wx*0.02083333333,wy*0.05555555555)));
        enemies[enemies.size()-1].setPosition(sf::Vector2f(spawn_pos_enemy[rand_spawn_pos][0],spawn_pos_enemy[rand_spawn_pos][1]));
        enemies[enemies.size()-1].setFillColor(sf::Color::Magenta);

        // enemies life point
        enemies_lp.push_back(1);

        // enemies hit timer
        hit_timmer.push_back(0);

        // enemies hit animation
        hitting.push_back(false);



        if ((spawn_pos[rand_spawn_pos][0])==0){
            enemy_class.push_back(0);
        }
        else{
            enemy_class.push_back(1);
        }
    }
}

void enemies_movement(std::vector<sf::RectangleShape>& enemieshitbox, std::vector<sf::RectangleShape>& enemies, std::vector<sf::RectangleShape>& enemies_lb,std::vector<bool> hitting, std::vector<int>enemy_class, std::vector<sf::RectangleShape> blocks, sf::RectangleShape crystal, sf::RectangleShape character, int multi, float dt){
    int enemy_speed = 4;
    float x_chang;dt*multi*enemy_speed;
    float y_chang;dt*multi*enemy_speed;
    sf::Vector2f contact_point_enemies;
    sf::Vector2f contact_normal_enemies;
    float time_enemies;

    for (int e=0;e<enemieshitbox.size();e++){

        x_chang = dt*multi*enemy_speed;
        y_chang = dt*multi*enemy_speed;

        if (enemy_class[e]==0){
            x_chang*=1;
        }
        else{
            x_chang*=-1;
        }

        for (int i =0;i<blocks.size()-4;i++){
            if (dynamicrectvrect(enemies[e], blocks[i],contact_point_enemies, contact_normal_enemies, time_enemies, x_chang, y_chang )){
                x_chang += contact_normal_enemies.x*std::abs(x_chang)*(1-time_enemies);
                y_chang += contact_normal_enemies.y*std::abs(y_chang)*(1-time_enemies);
        }
        }

        if (dynamicrectvrect(enemies[e], crystal,contact_point_enemies, contact_normal_enemies, time_enemies, x_chang, y_chang )){
                x_chang += contact_normal_enemies.x*std::abs(x_chang)*(1-time_enemies);
                y_chang += contact_normal_enemies.y*std::abs(y_chang)*(1-time_enemies);
        }
        if (dynamicrectvrect(enemies[e], character,contact_point_enemies, contact_normal_enemies, time_enemies, x_chang, y_chang )){
                x_chang += contact_normal_enemies.x*std::abs(x_chang)*(1-time_enemies);
                y_chang += contact_normal_enemies.y*std::abs(y_chang)*(1-time_enemies);
        }

        enemieshitbox[e].move(sf::Vector2f(x_chang, y_chang));
        enemies[e].move(sf::Vector2f(x_chang, y_chang));
        enemies_lb[e].move(sf::Vector2f(x_chang, y_chang));

    }

}

void enemies_update(std::vector<sf::RectangleShape>& enemieshitbox, std::vector<sf::RectangleShape>& enemies, std::vector<sf::RectangleShape>& enemies_lb,std::vector<float>& enemies_lp, std::vector<int>& enemy_class, std::vector<float>& hit_timmer, std::vector<bool>& hitting,float wx , float wy, int& score){
    sf::Vector2f lb_size (wx*0.03125,wy*0.00277777777);
    std::vector<int> deleted_e;
    for (int e=0; e<enemies.size(); e++){
        float lp_fraction = enemies_lp[e];
        enemies_lb[e].setSize (sf::Vector2f (lb_size.x*lp_fraction, lb_size.y));

        if(enemies_lp[e]<=0){
            deleted_e.push_back(e);
            score+=10;
        }
    }

    for (int d=0; d<deleted_e.size();d++){
        enemieshitbox.erase(enemieshitbox.begin()+deleted_e[d]);
        enemies.erase(enemies.begin()+deleted_e[d]);
        enemies_lb.erase(enemies_lb.begin()+deleted_e[d]);
        enemies_lp.erase(enemies_lp.begin()+deleted_e[d]);
        enemy_class.erase(enemy_class.begin()+deleted_e[d]);
        hit_timmer.erase(hit_timmer.begin()+deleted_e[d]);
        hitting.erase(hitting.begin()+deleted_e[d]);

    }
    deleted_e.clear();

}

void enemies_hit (std::vector<sf::RectangleShape>& enemieshitbox, std::vector<float>& hit_timmer, std::vector<bool>& hitting, sf::RectangleShape crystal, sf::RectangleShape& crystal_lb, float& crystal_lp, sf::RectangleShape character, sf::RectangleShape& character_lb, float& character_lp, float dt, float wx, float wy){
    int color;
    float time_to_hit = 1.5;
    sf::Vector2f crystal_pos = crystal.getPosition();
    sf::Vector2f crystal_size = crystal.getSize();
    sf::Vector2f character_pos = character.getPosition();
    sf::Vector2f character_size = character.getSize();
    sf::Vector2f enemies_pos;
    sf::Vector2f enemies_size;
    float character_lb_lenght=0.02604166666*wx;
    float crystal_lb_lenght=wx/19.2;
    float character_lb_height = character_lb.getSize().y;
    float crystal_lb_height = crystal_lb.getSize().y;

    for (int e=0; e<hitting.size(); e++){
        enemies_pos = enemieshitbox[e].getPosition();
        enemies_size = enemieshitbox[e].getSize();
        if ((  enemies_pos.x<character_pos.x+character_size.x  &&  enemies_pos.x+enemies_size.x>character_pos.x  &&  enemies_pos.y<character_pos.y+character_size.y  &&  enemies_pos.y+enemies_size.y>character_pos.y  ) || (  enemies_pos.x<crystal_pos.x+crystal_size.x  &&  enemies_pos.x+enemies_size.x>crystal_pos.x  &&  enemies_pos.y<crystal_pos.y+crystal_size.y  &&  enemies_pos.y+enemies_size.y>crystal_pos.y  )){
            hitting[e] = true;
        }
        if (hitting[e]){
            hit_timmer[e] += dt; 
            color = 255-(255*(hit_timmer[e]/time_to_hit));
            if (color<0){
                color=0;
            }
            enemieshitbox[e].setFillColor(sf::Color (255,color,0));
            if (color==0){
                if (  enemies_pos.x<character_pos.x+character_size.x  &&  enemies_pos.x+enemies_size.x>character_pos.x  &&  enemies_pos.y<character_pos.y+character_size.y  &&  enemies_pos.y+enemies_size.y>character_pos.y  ){
                    character_lp-=0.2;
                    character_lb.setSize(sf::Vector2f(character_lb_lenght*character_lp,character_lb_height));
                }
                if (  enemies_pos.x<crystal_pos.x+crystal_size.x  &&  enemies_pos.x+enemies_size.x>crystal_pos.x  &&  enemies_pos.y<crystal_pos.y+crystal_size.y  &&  enemies_pos.y+enemies_size.y>crystal_pos.y  ){
                    crystal_lp-=0.1;
                    crystal_lb.setSize(sf::Vector2f(crystal_lb_lenght*crystal_lp,crystal_lb_height));
                }
                hit_timmer[e]=0;
                hitting[e]=false;
                enemieshitbox[e].setFillColor(sf::Color (255,255,0));
            }

        }

    }
     
}



int main (){

    int window_widht=1600;
    int window_height=900;
    float wx, wy;

    // initializing window.
    // sf::RenderWindow window(sf::VideoMode(window_widht, window_height), "SFML works!");
    sf::RenderWindow window(sf::VideoMode(window_widht, window_height), "SFML works!", sf::Style::Fullscreen);
    window.setFramerateLimit(200);
    wx = window.getSize().x;
    wy = window.getSize().y;

    float x_vel=0;
    float y_vel=0;
    float x_change;
    float y_change;
    float x_mouse;
    float y_mouse;
    float time;
    int multi=60;
    int vel_limit=wx*0.00520833333;
    int jump;
    bool collision;
    float shoot_time = 0;
    float reload_time = 0;
    int bullets_available=0;
    float spawn_timer;
    int score = 0;
    sf::Vector2f shooting_thing_pos;
    sf::Vector2f contact_point;
    sf::Vector2f contact_normal;
    sf::Vector2f components;
    sf::Font font;
    font.loadFromFile("./font/ARCADECLASSIC.TTF");
    std::string score_string;
    sf::Text score_text;
    sf::Text start_text;
    sf::Text pause_text;
    bool pause = false;
    bool intro = true;
    

    std::vector<sf::RectangleShape> blocks; //saves the information of every block in the screen.
    std::vector<sf::CircleShape> bullets; //bullet vector.
    std::vector<sf::Vector2f> bullets_components; //saves bullet direction in order to update position.
    std::vector<sf::RectangleShape> enemies1; //enemies actual size.
    std::vector<sf::RectangleShape> enemies1hitbox; // enemies range to hit, if crystal or character gets too close, they will hit after charging 1 sec.
    std::vector<sf::RectangleShape> enemies_life_bar; //each enemy life bar 
    std::vector<float> enemies_life_points; //life points of every enemy
    std::vector<int> enemy_class; // 0 for spawning left, 1 for spawning right.
    std::vector<float> enemies_timer; // enemies time to attack.
    std::vector<bool> enemies_hitting; //true if the animation is happening, false if it is not happening;
    
    // position and size of every block in the screen
    float pos_size[22][4] = {{0,wy*0.88888888888,wx*0.21875,wy*0.11111111111},
                            {wx*0.40625,wy*0.72222222222,wx*0.1875,wy*0.05555555555},
                            {wx*0.78125,wy*0.88888888888,wx*0.21875,wy*0.11111111111},
                            {wx*0.09375,wy*0.66666666666,wx*0.09375,wy*0.05555555555},
                            {wx*0.15625,wy*0.61111111111,wx*0.15625,wy*0.05555555555},    
                            {wx*0.8125,wy*0.66666666666,wx*0.09375,wy*0.05555555555},
                            {wx*0.6875,wy*0.61111111111,wx*0.15625, wy*0.05555555555}, 
                            {0,wy*0.38888888888,wx*0.15625,wy*0.05555555555}, 
                            {wx*0.34375,wy*0.38888888888,wx*0.3125,wy*0.05555555555},
                            {wx*0.84375,wy*0.38888888888,wx*0.15625,wy*0.05555555555}, 
                            {wx*0.125,wy*0.16666666666,wx*0.25,wy*0.05555555555}, 
                            {wx*0.21875,wy*0.11111111111,wx*0.0625 ,wy*0.16666666666},
                            {wx*0.625,wy*0.16666666666,wx*0.25,wy*0.05555555555}, 
                            {wx*0.71875,wy*0.11111111111,wx*0.0625 ,wy*0.16666666666}, 
                            {-wx*0.00520833333,-wy*0.00925925925,wx*1.01041666667,wy*0.00925925925}, 
                            {wx,-wy*0.00925925925,wx*0.00520833333,wy*1.01851851852}, 
                            {-wx*0.00520833333,wy,wx*1.01041666667,wy*0.00925925925}, 
                            {-wx*0.00520833333,-wy*0.00925925925,wx*0.00520833333,wy*1.01851851852},
                            {0,wy*0.83333333333,wx*0.03125,wy*0.05555555555},
                            {0,wy*0.33333333333,wx*0.03125,wy*0.05555555555},
                            {wx*0.96875,wy*0.33333333333,wx*0.03125,wy*0.05555555555},
                            {wx*0.96875,wy*0.83333333333,wx*0.03125,wy*0.05555555555},};
    int rows = sizeof(pos_size)/sizeof(pos_size[0]); //Y size of pos_size array

    // all blocks are saved in blocks vector given the positions and sizes in pos_size.
    for (int j=0;j<rows;j++){
        blocks.push_back(sf::RectangleShape(sf::Vector2f(pos_size[j][2],pos_size[j][3])));
        blocks[j].setPosition(sf::Vector2f(pos_size[j][0],pos_size[j][1]));
        blocks[j].setFillColor(sf::Color::Blue);
    }

    // initializing clock.
    sf::Clock clock;
    float dt;
    dt = clock.restart().asSeconds();

    // character is created
    sf::RectangleShape character(sf::Vector2f(0.015625*wx, wy/27));
    character.setPosition(sf::Vector2f(wx/9.6,wy/10.8));
    character.setFillColor(sf::Color(67, 217, 222));
    sf::Vector2f character_pos = character.getPosition();
    sf::Vector2f character_size = character.getSize();
    sf::RectangleShape character_life_bar(sf::Vector2f(0.02604166666*wx, wy*0.00277777777));
    character_life_bar.setPosition(sf::Vector2f(wx*0.09895833333,wy*0.08333333333));
    character_life_bar.setFillColor(sf::Color::Red);
    float character_life_points = 1;
    

    // crystal is created
    sf::RectangleShape crystal(sf::Vector2f(wx/19.2, (wy*5)/36));
    crystal.setPosition(sf::Vector2f(wx*0.47395833333, wy*0.85185185185));
    crystal.setFillColor(sf::Color (92,181,116));
    sf::Vector2f crystal_pos = crystal.getPosition();
    sf::Vector2f crystal_size = crystal.getSize();
    sf::RectangleShape crystal_life_bar(sf::Vector2f(wx/19.2, wy*0.0029));
    crystal_life_bar.setPosition(sf::Vector2f(wx*0.47395833333, wy*0.83));
    crystal_life_bar.setFillColor(sf::Color::Red);
    float crystal_life_points = 1;

    // shooting thing is created
    sf::CircleShape shooting_thing(wx*0.0015625);
    shooting_thing_pos= sf::Vector2f(character_pos.x+(character_size.x/2)+(wx*0.015625), character_pos.y+(character_size.y/2));
    shooting_thing.setFillColor(sf::Color::White);
    shooting_thing.setPosition(shooting_thing_pos);

    // score 
    score_text.setFont(font);
    score_text.setCharacterSize(wx*0.015625);
    score_text.setPosition(sf::Vector2f(wx*0.02, wy*0.03));
    score_text.setFillColor(sf::Color::White);

    // pause chart
    sf::RectangleShape pause_chart(sf::Vector2f(wx*0.1875, wy*0.22222222222));
    pause_chart.setPosition(sf::Vector2f(wx*0.40625,wy*0.38888888888));
    pause_chart.setOutlineThickness(3);
    pause_chart.setOutlineColor(sf::Color::White);
    pause_chart.setFillColor(sf::Color::Black);

    // pause button 
    sf::RectangleShape pause_button(sf::Vector2f(wx*0.0625,wy*0.05555555555));
    pause_button.setPosition(sf::Vector2f(wx*0.46875,wy*0.44444444444));
    sf::Vector2f pause_button_pos = pause_button.getPosition();
    sf::Vector2f pause_button_size = pause_button.getSize();
    pause_button.setOutlineThickness(3);
    pause_button.setOutlineColor(sf::Color::White);
    pause_button.setFillColor(sf::Color::Black);

    // intro menu
    sf::RectangleShape intro_menu(sf::Vector2f(wx*0.1875, wy*0.44444444444));
    intro_menu.setPosition(sf::Vector2f(wx*0.40625,wy*0.27777777777));
    intro_menu.setOutlineThickness(3);
    intro_menu.setOutlineColor(sf::Color::White);
    intro_menu.setFillColor(sf::Color::Black);

    // start button
    sf::RectangleShape start_button(sf::Vector2f(wx*0.0625,wy*0.05555555555));
    start_button.setPosition(sf::Vector2f(wx*0.46875,wy*0.44444444444));
    sf::Vector2f start_button_pos = start_button.getPosition();
    sf::Vector2f start_button_size = start_button.getSize();
    start_button.setOutlineThickness(3);
    start_button.setOutlineColor(sf::Color::White);
    start_button.setFillColor(sf::Color::Black);

    // start text
    start_text.setFont(font);
    start_text.setCharacterSize(wx*0.015625);
    start_text.setPosition(sf::Vector2f(wx*0.477,wy*0.454));
    start_text.setString("Start");
    start_text.setFillColor(sf::Color::White);

    // pause text
    pause_text.setFont(font);
    pause_text.setCharacterSize(wx*0.015625);
    pause_text.setPosition(sf::Vector2f(wx*0.477,wy*0.454));
    pause_text.setString("Pause");
    pause_text.setFillColor(sf::Color::White);


    // game loop.
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // main menu
        if (intro)
        {
            // getting mouse position for shoting thing to follow.
            x_mouse = sf::Mouse::getPosition(window).x;
            y_mouse = sf::Mouse::getPosition(window).y;
            window.clear(sf::Color(0,0,0,255));
            window.draw(intro_menu);
            window.draw(start_button);
            window.draw(start_text);

            if (x_mouse>=start_button_pos.x && y_mouse>=start_button_pos.y && x_mouse<=start_button_pos.x+start_button_size.x && y_mouse<=start_button_pos.y+start_button_size.y){
                start_button.setOutlineThickness(5);
                start_text.setCharacterSize(wx*0.017);
                start_text.setPosition(sf::Vector2f(wx*0.476,wy*0.453));
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    intro=false;
                }
            }
            else{
                start_text.setPosition(sf::Vector2f(wx*0.477,wy*0.454));
                start_text.setCharacterSize(wx*0.015625);
                start_button.setOutlineThickness(3);
            }
        }
        
        else{
        // pause verification 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)){
            pause=true;
        }


        if (!pause){
    
            // getting mouse position for shoting thing to follow.
            x_mouse = sf::Mouse::getPosition(window).x;
            y_mouse = sf::Mouse::getPosition(window).y;

            // set changes in position before checking block and crystal collision
            direction(x_vel, y_vel, vel_limit, character_pos, character_size, blocks,crystal,enemies1, jump, wx, wy);
            x_change = dt*multi*x_vel;
            y_change = dt*multi*y_vel;

            // collision with blocks.
            for (int i =0;i<blocks.size();i++){
                if (dynamicrectvrect(character, blocks[i],contact_point, contact_normal, time, x_change, y_change )){
                    x_change += contact_normal.x*std::abs(x_change)*(1-time);
                    y_change += contact_normal.y*std::abs(y_change)*(1-time);
            }
            }
            // collision with enemies
            for (int i =0;i<enemies1.size();i++){
                if (dynamicrectvrect(character, enemies1[i],contact_point, contact_normal, time, x_change, y_change )){
                    x_change += contact_normal.x*std::abs(x_change)*(1-time);
                    y_change += contact_normal.y*std::abs(y_change)*(1-time);
            }
            }
            //collision with crystal.
            if (dynamicrectvrect(character, crystal,contact_point, contact_normal, time, x_change, y_change )){
                    x_change += contact_normal.x*std::abs(x_change)*(1-time);
                    y_change += contact_normal.y*std::abs(y_change)*(1-time);
            }
            // setting changes after checking collisions.
            character.move(x_change, y_change);
            character_life_bar.move(x_change, y_change);

            // new values of character after solving collisions with crystal and blocks
            character_pos = character.getPosition();
            character_size = character.getSize();

            // set position of shooting thing pointing to pinter position
            follow_pointer(x_mouse, y_mouse, character_pos, character_size, shooting_thing_pos, components, wx, wy);
            shooting_thing.setPosition(shooting_thing_pos);
            // creating bullet on the screen, moving it in the direction the shooting thing was pointing and deleting them after hitting blocks and enemies.
            bullets_dir(shooting_thing_pos, components, bullets,bullets_components, shoot_time, reload_time, dt, bullets_available, wx, wy);
            bullets_move(bullets, bullets_components, blocks,enemies1, enemies_life_points,dt, wx, wy);
            // spawning enemies, and moving them based on the patterns choosen.
            spawn_enemies(dt,spawn_timer,enemies1hitbox, enemies1,enemies_life_bar,enemies_life_points, enemy_class, enemies_timer,enemies_hitting, wx, wy);
            enemies_movement(enemies1hitbox, enemies1,enemies_life_bar,enemies_hitting, enemy_class, blocks,crystal,character, multi, dt);
            enemies_update(enemies1hitbox, enemies1, enemies_life_bar, enemies_life_points,enemy_class, enemies_timer,enemies_hitting, wx, wy, score);
            enemies_hit(enemies1hitbox, enemies_timer, enemies_hitting, crystal, crystal_life_bar, crystal_life_points, character, character_life_bar, character_life_points, dt, wx, wy);

            score_string = "Score  " + std::to_string(score);
            score_text.setString(score_string);


            // draw everything in the screen.
            window.clear(sf::Color(0,0,0,255));
            for (int e=0; e<enemies1hitbox.size();e++){
                window.draw(enemies1hitbox[e]);
            }
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
            window.draw(crystal);
            window.draw(character_life_bar);
            window.draw(crystal_life_bar);
            for (int e=0; e<enemies_life_bar.size();e++){
                window.draw(enemies_life_bar[e]);
            }

            window.draw(score_text);

        }

        else{
            // getting mouse position for shoting thing to follow.
            x_mouse = sf::Mouse::getPosition(window).x;
            y_mouse = sf::Mouse::getPosition(window).y;
            window.clear(sf::Color(0,0,0,255));
            window.draw(pause_chart);
            window.draw(pause_button);
            window.draw(pause_text);
            if (x_mouse>=pause_button_pos.x && y_mouse>=pause_button_pos.y && x_mouse<=pause_button_pos.x+pause_button_size.x && y_mouse<=pause_button_pos.y+pause_button_size.y){
                pause_button.setOutlineThickness(5);
                pause_text.setCharacterSize(wx*0.017);
                pause_text.setPosition(sf::Vector2f(wx*0.476,wy*0.453));
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    pause=false;
                }
            }
            else{
                pause_text.setPosition(sf::Vector2f(wx*0.477,wy*0.454));
                pause_text.setCharacterSize(wx*0.015625);
                pause_button.setOutlineThickness(3);
            }
        }
        
        }
        window.display();

        // restarting the clock.
        dt = clock.restart().asSeconds();
    }

    

}