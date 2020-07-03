#include "SDL.h"
#include "SDL_image.h"
#include <bits/stdc++.h>

using namespace std;

// global variables

// window and rendering
const int window_width = 1280;
const int window_height = 720;
SDL_Window* window = NULL;
SDL_Renderer* screen = NULL; 
const int fps = 60;

// world map
const int world_map_width = 24;
const int world_map_height = 24;
int world_map[world_map_height][world_map_width] =
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,3,0,0,0,0,1},
    {1,0,0,0,0,0,2,0,2,0,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,0,2,0,2,0,0,0,0,3,0,0,0,0,0,3,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
    {1,0,0,0,0,0,0,2,0,2,0,0,0,0,3,0,0,0,0,0,3,0,0,1},
    {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,3,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,7,7,7,7,7,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,1},
    {1,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,1},
    {1,0,0,8,0,0,8,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,1},
    {1,0,0,8,0,0,8,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,1},
    {1,0,0,8,8,8,8,0,0,0,0,0,0,0,0,0,0,6,0,6,0,0,0,1},
    {1,4,4,4,4,4,5,5,5,0,0,0,0,0,0,0,6,0,0,0,6,0,0,1},
    {1,4,0,7,0,4,5,0,5,0,0,0,0,0,4,0,6,0,0,0,6,0,0,1},
    {1,4,0,0,0,4,5,0,5,0,0,0,0,0,5,0,0,6,0,6,0,0,0,1},
    {1,4,0,8,0,4,5,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,4,4,4,4,0,0,0,0,0,5,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// player data
double player_posX = 1.40;
double player_posY = 1.40;
double player_dirX = 1.0;
double player_dirY = 0.0;
double player_step_size = 0.5;
double cos_ang = 0.70710678;
double sin_ang = 0.70710678;

// camera data
double camera_planeX = 0.0;
double camera_planeY = 0.66;

// array to hold poinnters to loaded textures
const int number_of_textures = 8;
SDL_Texture* texture_array[number_of_textures];
const int texture_width = 64;
const int texture_height = 64;

// functions
void init(){
    // initialize sdl
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        // error has occured
        fprintf(stderr,"failed to initialize sdl: %s\n",SDL_GetError());
        return;
    }

    // create window
    window = SDL_CreateWindow("textured RayCasting Renderer",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              window_width,window_height,
                              SDL_WINDOW_RESIZABLE);
    if(window == NULL){
        // error has occured
        fprintf(stderr,"failed to create window: %s\n",SDL_GetError());
        return;
    }

    // create renderer
    screen = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(screen == NULL){
        // error has occured
        fprintf(stderr,"failed to create renderer: %s\n",SDL_GetError());
        return;
    }

    // img init
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    return;
}

void quit(){
    // destroy renderer
    SDL_DestroyRenderer(screen);
    screen = NULL;

    // destroy window
    SDL_DestroyWindow(window);
    window = NULL;

    // quit
    IMG_Quit();
    SDL_Quit();

    return;
}

SDL_Texture* load(string path){
    SDL_Surface* surf = NULL;
    surf = IMG_Load(path.c_str());

    // window surface
    SDL_Surface* window_format_surface = NULL;
    window_format_surface = SDL_GetWindowSurface(window);

    // optimized
    SDL_Surface* optimized_surf = NULL;
    optimized_surf = SDL_ConvertSurface(surf,window_format_surface->format,0);

    // free surfaces
    SDL_FreeSurface(surf);
    surf = NULL;
    SDL_FreeSurface(window_format_surface);
    window_format_surface = NULL;

    // return stuff
    SDL_Texture* return_texture = NULL;
    return_texture = SDL_CreateTextureFromSurface(screen,optimized_surf);
    if(return_texture == NULL){
        fprintf(stderr,"failed to convert surface from %s to a texture : %s\n",path.c_str(),SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(optimized_surf);
    optimized_surf = NULL;
    return return_texture; 
}

void loadMedia(){
    // loading them one by one
    texture_array[0] = load("textures/bluestone.png");
    texture_array[1] = load("textures/colorstone.png");
    texture_array[2] = load("textures/eagle.png");
    texture_array[3] = load("textures/greystone.png");
    texture_array[4] = load("textures/mossy.png");
    texture_array[5] = load("textures/purplestone.png");
    texture_array[6] = load("textures/redbrick.png");
    texture_array[7] = load("textures/wood.png");

}

void drawFloor(){
    // clear the whole screen with floor color
    SDL_SetRenderDrawColor(screen,158,158,158,255);
    SDL_RenderClear(screen);

    // rect for celling
    SDL_Rect celling = {0,0,window_width,window_height/2};
    SDL_SetRenderDrawColor(screen,98,98,98,255);
    SDL_RenderFillRect(screen,&celling);

    return;
}

void drawWalls(){
    // loop over every vertical column on window 
    for(int windowX = 0;windowX<window_width;windowX++){
        // convert to a squisher ranging from -1 to 1
        double squisher = 2*windowX/double(window_width) - 1.0;

        // get current ray direction by adding player direction to squished camera plane
        double ray_dirX = player_dirX + camera_planeX*squisher;
        double ray_dirY = player_dirY + camera_planeY*squisher;

        // the vertical dist the ray travels as it passes between 2 vertical lines is deltaY
        double deltaY = abs(ray_dirY/ray_dirX);
        double deltaX = abs(ray_dirX/ray_dirY);

        // (x,y_intercept) is the point where the ray touches first vertical line and (x_intercept,y) is the point where ray touches first horizontal line
        double x;
        double y_intercept;

        double x_intercept;
        double y;

        // value of sideX,Y depend on ray_dir
        int sideX = ray_dirX < 0? -1: 1;
        int sideY = ray_dirY < 0? -1: 1;

        if(ray_dirX < 0){
            double y_offset = (player_posX - int(player_posX)) * deltaY;
            y_intercept = player_posY + sideY*y_offset;

            x = std::floor(player_posX);
        }
        else{
            double y_offset = (1.0 - (player_posX-int(player_posX))) * deltaY;
            y_intercept = player_posY + sideY*y_offset;

            x = int(player_posX) + 1.0;
        }
        if(ray_dirY < 0){
            double x_offset = (player_posY - int(player_posY)) * deltaX;
            x_intercept = player_posX + sideX * x_offset;

            y = std::floor(player_posY);
        }
        else{
            double x_offset = (1.0 - (player_posY-int(player_posY))) * deltaX;
            x_intercept = player_posX + sideX * x_offset;

            y = int(player_posY) + 1.0;
        }

        // bools to check if wall is hit and what kind of wall is  hit
        bool wall_is_hit = false;
        bool wall_is_hit_on_vertical_face;

        int wallX; // will be used later for color
        int wallY;
        

        while(!wall_is_hit){
            if(abs(y_intercept - player_posY) < abs(y - player_posY)){ // might be wrong
                // check if wall is hit
                if(sideX > 0 && (world_map[int(y_intercept)][int(x)] != 0)){
                    wall_is_hit = true;
                    wall_is_hit_on_vertical_face = true;
                    wallX = int(x);
                    wallY = int(y_intercept);
                    //break;
                }
                else if(sideX < 0 && (world_map[int(y_intercept)][int(x-1)] != 0)){
                    wall_is_hit = true;
                    wall_is_hit_on_vertical_face = true;
                    wallX = int(x-1);
                    wallY = int(y_intercept);
                    //break;
                }                                
                else{
                    y_intercept = y_intercept + sideY*deltaY;
                    x = x + sideX; 
                }
            }
            else { // if(x_intercept < x)
                if(sideY > 0 && (world_map[int(y)][int(x_intercept)] != 0)){
                    wall_is_hit = true;
                    wall_is_hit_on_vertical_face = false;
                    wallX = int(x_intercept);
                    wallY = int(y);
                }
                else if(sideY < 0 && (world_map[int(y-1)][int(x_intercept)] != 0)){
                    wall_is_hit = true;
                    wall_is_hit_on_vertical_face = false;
                    wallX = int(x_intercept);
                    wallY = int(y-1);
                    //cout<<wallX<<" "<<wallY<<endl;
                    //return;
                }
                else{
                    x_intercept = x_intercept + sideX * deltaX;
                    y = y + sideY;
                }
            }
        }

        // now that we have location of impact lets find distance

        // lets create player as  temp origin
        double temp_player_posX = 0.0;
        double temp_player_posY = 0.0;

        double temp_wall_hitX;
        double temp_wall_hitY;

        if(wall_is_hit_on_vertical_face){
            temp_wall_hitX = x - player_posX;
            temp_wall_hitY = y_intercept - player_posY;
        }
        else{
            temp_wall_hitX = x_intercept - player_posX;
            temp_wall_hitY = y - player_posY;
        }

        double perp_dist = temp_wall_hitX * player_dirX + temp_wall_hitY * player_dirY;

        if(perp_dist < 0){
            fprintf(stderr,"perpendicular dist is negative\n");
            break;
        }
        
        // now line height
        int line_height = int(window_height*1.2/perp_dist);
        int line_start = window_height/2 - line_height/2;
        int line_end = window_height/2 + line_height/2;

        // global wall hit coordinates
        double gb_wallX;
        double gb_wallY;

        // wall co ordinates on world map
        int wall_coordinateX;
        int wall_coordinateY;
        if(wall_is_hit_on_vertical_face){
            gb_wallX = x;
            gb_wallY = y_intercept;

            wall_coordinateX = int(gb_wallX);
            wall_coordinateY = int(gb_wallY);
            if(ray_dirX < 0){
                wall_coordinateX = wall_coordinateX - 1;
            }
        }
        else{
            gb_wallX = x_intercept;
            gb_wallY = y;

            wall_coordinateX = int(gb_wallX);
            wall_coordinateY = int(gb_wallY);
            if(ray_dirY < 0){
                wall_coordinateY = wall_coordinateY - 1;
            }
        }

        // x coordinate on the wall which will be used to correspond to x coordinate on texture
        double wall_fracX;
        if(wall_is_hit_on_vertical_face){
            wall_fracX = gb_wallY - std::floor(gb_wallY);
        }
        else{
            wall_fracX = gb_wallX - std::floor(gb_wallX);
        }

        // x,y coordinate on texture 
        int texX = int(wall_fracX * double(texture_width));
        int texY = 0;
        int tex_lengthY = texture_height;

        // selecting the correct texture
        int tex_num = world_map[wall_coordinateY][wall_coordinateX] - 1;
        SDL_Texture* current_texture = texture_array[tex_num];

        // change color if it is vertical wall
        if(wall_is_hit_on_vertical_face){
            SDL_SetTextureColorMod(current_texture,128,128,128);
        }
        else{
            SDL_SetTextureColorMod(current_texture,255,255,255);
        }

        // now lets create 2 rects both of 1 pixel width
        SDL_Rect source_rect = {texX,texY,1,tex_lengthY};
        SDL_Rect dest_rect = {windowX,line_start,1,line_height};

        SDL_RenderCopy(screen,current_texture,&source_rect,&dest_rect);        
    }
}

int main(int argc, char* argv[]){
    init();

    loadMedia();
    
    // mainloop runner and event handler
    bool running = true;
    SDL_Event event;

    // main loop
    while(running){
        // fps maintainer
        Uint32 start_time = SDL_GetTicks();

        // event loop
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                running = false;
                break;
            }
            else if(event.type == SDL_KEYDOWN){
                double tempX;
                double tempY;
                double temp_dirX;
                double temp_dirY;
                switch(event.key.keysym.sym){
                    case SDLK_w:
                    tempX = player_posX + player_dirX*player_step_size;
                    tempY = player_posY + player_dirY*player_step_size;
                    if(world_map[int(tempY)][int(tempX)] == 0){
                        player_posX = tempX;
                        player_posY = tempY;
                    }
                    break;

                    case SDLK_s:
                    tempX = player_posX - player_dirX*player_step_size;
                    tempY = player_posY - player_dirY*player_step_size;
                    if(world_map[int(tempY)][int(tempX)] == 0){
                        player_posX = tempX;
                        player_posY = tempY;
                    }
                    break;

                    case SDLK_a:
                    // minus 90 deg rot of direction and then move
                    temp_dirX = 0*player_dirX + 1*player_dirY;
                    temp_dirY = -1*player_dirX + 0*player_dirY;
                    tempX = player_posX + temp_dirX*player_step_size;
                    tempY = player_posY + temp_dirY*player_step_size;
                    if(world_map[int(tempY)][int(tempX)] == 0){
                        player_posX = tempX;
                        player_posY = tempY;
                    }
                    break;

                    case SDLK_d:
                    // minus 90 deg rot of direction and then move
                    temp_dirX = 0*player_dirX + 1*player_dirY;
                    temp_dirY = -1*player_dirX + 0*player_dirY;
                    tempX = player_posX - temp_dirX*player_step_size;
                    tempY = player_posY - temp_dirY*player_step_size;
                    if(world_map[int(tempY)][int(tempX)] == 0){
                        player_posX = tempX;
                        player_posY = tempY;
                    }
                    break;

                    // now turnns
                    case SDLK_j:
                    temp_dirX = player_dirX;
                    temp_dirY = player_dirY;
                    // ang <= 90 deg
                    player_dirX = cos_ang*temp_dirX + sin_ang*temp_dirY;
                    player_dirY = -1*sin_ang*temp_dirX + cos_ang*temp_dirY;

                    // turn camera too
                    temp_dirX = camera_planeX;
                    temp_dirY = camera_planeY;
                    camera_planeX = cos_ang*temp_dirX + sin_ang*temp_dirY;
                    camera_planeY = -1*sin_ang*temp_dirX + cos_ang*temp_dirY;
                    break;
                    
                    case SDLK_l:
                    temp_dirX = player_dirX;
                    temp_dirY = player_dirY;
                    player_dirX = cos_ang*temp_dirX - sin_ang*temp_dirY;
                    player_dirY = sin_ang*temp_dirX + cos_ang*temp_dirY;

                    // turn camera too
                    temp_dirX = camera_planeX;
                    temp_dirY = camera_planeY;
                    camera_planeX = cos_ang*temp_dirX - sin_ang*temp_dirY;
                    camera_planeY = sin_ang*temp_dirX + cos_ang*temp_dirY;
                    break;
                    
                    
                    default:
                    break;
                }
            }
        }

        // draw floor and celling
        drawFloor();

        // draw walls
        drawWalls();

        // update
        SDL_RenderPresent(screen);

        // fps maintainer
        Uint32 end_time = SDL_GetTicks();
        SDL_Delay((1000/fps)>(end_time-start_time)?(1000/fps)-(end_time-start_time):0);
    }

    quit();
    return 0;
}