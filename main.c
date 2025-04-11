#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BLOCK_SIZE 20

typedef struct SnakeBlock
{
    int x;
    int y;
    struct SnakeBlock* next;
} SnakeBlock;

typedef struct Snake
{
    SnakeBlock* head;
    SnakeBlock* tail;
    int length;
} Snake;

SnakeBlock first = { 5, 5, NULL };
Snake snake = { &first, &first, 1};

int deque(){
    if(snake.length ==  0) return -1;
    SnakeBlock* temp = snake.head;
    snake.head = snake.head->next;
    free(temp);
    snake.length--;
    if(snake.length == 0) snake.tail = NULL;
    return 0;
}

int enque(int x,int y){
    SnakeBlock* temp = (SnakeBlock*)malloc(sizeof(SnakeBlock));
    if(temp == NULL) return -1; // Memory allocation failed
    temp->x = x;
    temp->y = y;
    snake.tail->next = temp;
    snake.tail = temp;
    snake.tail->next = NULL;
    snake.length++;
    return 0;
}

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL3 Demo", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int i=0;
    bool ateAnApple = false;
    int vx = 1;
    int vy = 0;

    // Seeding the random function
    srand(time(NULL));

    int apple[2] = {rand()%(WINDOW_WIDTH/BLOCK_SIZE), rand()%(WINDOW_HEIGHT/BLOCK_SIZE)};

    bool isGameOver = false;
    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
            }

            // Key pressed
            if (e.type == SDL_EVENT_KEY_DOWN) {
                SDL_Keycode key = e.key.key;
                
                if (key == SDLK_ESCAPE) {
                    running = 0; // Quit on ESC
                }
                if (key == SDLK_LEFT && (snake.length==1 || vx != 1)) {
                    vx = -1;
                    vy = 0;
                }
                if (key == SDLK_RIGHT && (snake.length==1 || vx != -1)) {
                    vx = 1;
                    vy = 0;
                }
                if (key == SDLK_UP && (snake.length==1 || vy != 1)) {
                    vx = 0;
                    vy = -1;
                }
                if (key == SDLK_DOWN && (snake.length==1 || vy != -1)) {
                    vx = 0;
                    vy = 1;
                }
        }

        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);


        // Rendering the snake
        SnakeBlock* current = snake.head;

        for(i = 0; i<snake.length; i++){
            SDL_FRect rect = { current->x*20, current->y*20, BLOCK_SIZE, BLOCK_SIZE };
            
            // Setting the color of head of snake to blue
            if(i==snake.length-1){
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }

            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 150,150,150,255);
            SDL_RenderRect(renderer, &rect);

            // Checked collision of head and body
            if(i != snake.length-1 && snake.tail->x == current->x && snake.tail->y == current->y){
                SDL_SetRenderScale(renderer, 10.0f, 10.0f);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
                SDL_RenderDebugText(renderer, WINDOW_WIDTH/2, WINDOW_HEIGHT/2, "Game Over!");
            }

            current = current->next;
        }

        // Rendering the apple
        SDL_FRect rect = { apple[0]*20, apple[1]*20, BLOCK_SIZE, BLOCK_SIZE };
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);

        // Updating the snake
        enque(snake.tail->x+vx,snake.tail->y+vy);

        // Checking collision between snake and apple
        if(snake.tail->x == apple[0] && snake.tail->y == apple[1]){
            ateAnApple = true;
            apple[0] = rand()%(WINDOW_WIDTH/BLOCK_SIZE);
            apple[1] = rand()%(WINDOW_HEIGHT/BLOCK_SIZE);
        } else {
            ateAnApple = false;
            deque();
        }

        // Check if snake is going out of boundary
        if(snake.tail->x >= WINDOW_WIDTH/BLOCK_SIZE){
            snake.tail->x = 0;
        }

        if(snake.tail->x < 0){
            snake.tail->x = WINDOW_WIDTH/BLOCK_SIZE - 1;
        }
        
        if(snake.tail->y >= WINDOW_HEIGHT/BLOCK_SIZE){
            snake.tail->y = 0;
        }
        
        if(snake.tail->y < 0){
            snake.tail->y = WINDOW_HEIGHT/BLOCK_SIZE - 1;
        }


        SDL_RenderPresent(renderer);

        // SDL_Delay(16); // ~60 FPS
        SDL_Delay(170); 
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
