#include <SDL.h>
#include <SDL_image.h>
#include<windows.h>
#include <stdio.h>
#include <string>
#include<stdlib.h>
#include<time.h>

using namespace std;

typedef long long ll;
char fileselection1[][10]= {"q1.txt","q2.txt","q3.txt","q4.txt","q5.txt","q6.txt","q7.txt","q8.txt","q9.txt","q10.txt"};
char fileselection2[][10]= {"a1.txt","a2.txt","a3.txt","a4.txt","a5.txt","a6.txt","a7.txt","a8.txt","a9.txt","a10.txt"};
char SudokuToSolve[10][10];
char SudokuSolved[10][10];
char OnDuty[10][10];


int low=0,high=9;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
bool flag=false;

bool init();
bool loadMedia();
void close();
void InitSudokuToSolve();
bool CheckOnDuty(char c);
void initplay();
void go_up();
void go_down();
void go_left();
void go_right();
void showinvalid();
bool update(int x);
bool solved_or_not();
void start();


SDL_Texture* loadTexture( string path );

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Texture* gTexture[9][9];
SDL_Texture* allimages[10];
SDL_Texture* sudokusign;
SDL_Texture* invalid;
SDL_Texture* box;
SDL_Texture* green;
SDL_Texture* congrats;


ll box_pos_x=206,box_pos_y=1;
ll initiali=0,initialj=0;
bool CheckOnDuty(char c)
{
    if(SudokuToSolve[initiali][initialj]!='*')
    {
        showinvalid();
        return false;
    }
    ll ok=0;
    for(ll i=0; i<9; i++)
    {
        if(OnDuty[initiali][i]==c)
        {
            ok=1;
            break;
        }
        if(OnDuty[i][initialj]==c)
        {
            ok=1;
            break;
        }
    }
    if(ok)showinvalid();
    else
    {
        OnDuty[initiali][initialj]=c;
        return update(c-'0');
    }
    return false;
}
bool solved_or_not()
{
    ll ok=1;
    for(ll i=0; i<9; i++)
    {
        for(ll j=0; j<9; j++)
        {
            if(OnDuty[i][j]!=SudokuSolved[i][j])ok=0;
        }
    }
    if(ok)
    {
        flag=true;
        return true;
    }
    else return false;
}

void InitSudokuToSolve()
{
    FILE *f1,*f2;
    srand(time(0));
    int a=(rand()%(high-low+1))+low;//generating a random puzzle
    //printf("%d\n",a);

    f1=fopen(fileselection1[a],"r");//opening one file to scan the puzzle
    f2=fopen(fileselection2[a],"r");//another file to keep the solution of the puzzle

    ll i,j;
    //scanning the puzzle
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            char c;
            fscanf(f1,"%c",&c);
            if(c!='\n')SudokuToSolve[i][j]=c;
            else
            {
                fscanf(f1,"%c",&c);
                SudokuToSolve[i][j]=c;
            }
        }
    }
    //scanning the solution of the puzzle
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            char c;
            fscanf(f2,"%c",&c);
            if(c!='\n')SudokuSolved[i][j]=c;
            else
            {
                fscanf(f2,"%c",&c);
                SudokuSolved[i][j]=c;
            }
        }
    }

    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            OnDuty[i][j]=SudokuToSolve[i][j];
        }
        OnDuty[i][j]='\0';
    }


}

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        gWindow = SDL_CreateWindow( "SUDOKU 9*9", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    bool success = true;
    box=loadTexture("box.png");
    invalid=loadTexture("invalid.png");
    sudokusign=loadTexture("sudoku.png");
    green=loadTexture("green.png");
    congrats=loadTexture("congrats.png");

    allimages[0]=loadTexture("0.png");
    allimages[1]=loadTexture("1.png");
    allimages[2]=loadTexture("2.png");
    allimages[3]=loadTexture("3.png");
    allimages[4]=loadTexture("4.png");
    allimages[5]=loadTexture("5.png");
    allimages[6]=loadTexture("6.png");
    allimages[7]=loadTexture("7.png");
    allimages[8]=loadTexture("8.png");
    allimages[9]=loadTexture("9.png");

    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            //printf("%d ",SudokuToSolve[i][j]);
            if(SudokuToSolve[i][j]=='*')gTexture[i][j]=allimages[0];
            else if(SudokuToSolve[i][j]=='1')gTexture[i][j]=allimages[1];
            else if(SudokuToSolve[i][j]=='2')gTexture[i][j]=allimages[2];
            else if(SudokuToSolve[i][j]=='3')gTexture[i][j]=allimages[3];
            else if(SudokuToSolve[i][j]=='4')gTexture[i][j]=allimages[4];
            else if(SudokuToSolve[i][j]=='5')gTexture[i][j]=allimages[5];
            else if(SudokuToSolve[i][j]=='6')gTexture[i][j]=allimages[6];
            else if(SudokuToSolve[i][j]=='7')gTexture[i][j]=allimages[7];
            else if(SudokuToSolve[i][j]=='8')gTexture[i][j]=allimages[8];
            else if(SudokuToSolve[i][j]=='9')gTexture[i][j]=allimages[9];
        }
        //printf("\n");
    }

    for(ll i=0; i<9; i++)
    {
        for(ll j=0; j<9; j++)
        {
            if( gTexture[i][j] == NULL )
            {
                printf( "%d %d Failed to load texture image!\n",i,j );
                success = false;
            }
        }
    }
    return success;
}

void close()
{

    for(ll i=0; i<9; i++)
    {
        for(ll j=0; j<9; j++)
        {
            SDL_DestroyTexture( gTexture[i][j] );
            gTexture[i][j] = NULL;
        }
    }

    SDL_DestroyTexture(box);
    SDL_DestroyTexture(green);
    SDL_DestroyTexture(invalid);
    SDL_DestroyTexture(sudokusign);
    for(ll i=0; i<10; i++)
    {
        SDL_DestroyTexture(allimages[i]);
    }

    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture( string path )
{

    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0, 0 ) );
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}
void start()
{

    int i=0,i2;

    for(i=0,i2=800; i<=400; i+=1,i2-=1)
    {
        SDL_SetRenderDrawColor( gRenderer, 13,11, 31, 0xFF );
        SDL_RenderClear( gRenderer );
        SDL_Texture* meh=loadTexture("meh.png");
        SDL_Rect mehrect= {400,160,200,150};
        SDL_RenderSetViewport(gRenderer,&mehrect);
        SDL_RenderCopy(gRenderer,meh,NULL,NULL);
        SDL_Rect starting1= {i,260,200,80};
        SDL_RenderSetViewport( gRenderer, &starting1 );
        SDL_RenderCopy( gRenderer, sudokusign, NULL, NULL);
        SDL_Rect starting2= {i2,260,200,80};
        SDL_RenderSetViewport( gRenderer, &starting2 );
        SDL_RenderCopy( gRenderer, sudokusign, NULL, NULL);
        SDL_RenderPresent( gRenderer );
        Sleep(10);
    }
    Sleep(1000);
}

void initplay()
{
    //Clear screen
    SDL_SetRenderDrawColor( gRenderer, 13,11, 31, 0xFF );
    SDL_RenderClear( gRenderer );

    SDL_Rect topleftsign= {1,1,200,80};
    SDL_RenderSetViewport( gRenderer, &topleftsign );
    SDL_RenderCopy( gRenderer, sudokusign, NULL, NULL);
    SDL_Rect boardrect[10][10];



    ll i1=0,j1=0;
    for(ll j=1,i1=0; j<=529; j+=66,i1++)
    {
        for(ll i=206,j1=0; i<=734; i+=66,j1++)
        {
            boardrect[i1][j1]= {i,j,64,64};
            SDL_RenderSetViewport( gRenderer, &boardrect[i1][j1] );
            SDL_RenderCopy( gRenderer, gTexture[i1][j1], NULL, NULL );
            if(SudokuToSolve[i1][j1]!='*')
            {
                SDL_Rect greenrect= {i,j,64,64};
                SDL_RenderSetViewport(gRenderer,&greenrect);
                SDL_RenderCopy(gRenderer,green,NULL,NULL);
            }
        }
    }

    SDL_Rect boxrect= {box_pos_x,box_pos_y,66,66};
    SDL_RenderSetViewport(gRenderer,&boxrect);
    SDL_RenderCopy(gRenderer,box,NULL,NULL);

    SDL_RenderPresent( gRenderer );
}

void showinvalid()
{
    //Clear screen
    SDL_SetRenderDrawColor( gRenderer, 13,11, 31, 0xFF );
    SDL_RenderClear( gRenderer );

    SDL_Rect topleftsign= {1,1,200,80};
    SDL_RenderSetViewport( gRenderer, &topleftsign );
    SDL_RenderCopy( gRenderer, sudokusign, NULL, NULL);
    SDL_Rect boardrect[10][10];



    ll i1=0,j1=0;
    for(ll j=1,i1=0; j<=529; j+=66,i1++)
    {
        for(ll i=206,j1=0; i<=734; i+=66,j1++)
        {
            boardrect[i1][j1]= {i,j,64,64};
            SDL_RenderSetViewport( gRenderer, &boardrect[i1][j1] );
            SDL_RenderCopy( gRenderer, gTexture[i1][j1], NULL, NULL );
            if(SudokuToSolve[i1][j1]!='*')
            {
                SDL_Rect greenrect= {i,j,64,64};
                SDL_RenderSetViewport(gRenderer,&greenrect);
                SDL_RenderCopy(gRenderer,green,NULL,NULL);
            }
        }
    }
    SDL_Rect invalidrect= {1,500,200,80};
    SDL_RenderSetViewport(gRenderer,&invalidrect);
    SDL_RenderCopy(gRenderer,invalid,NULL,NULL);

    SDL_Rect boxrect= {box_pos_x,box_pos_y,66,66};
    SDL_RenderSetViewport(gRenderer,&boxrect);
    SDL_RenderCopy(gRenderer,box,NULL,NULL);

    SDL_RenderPresent( gRenderer );
}

bool update(int x)
{
    gTexture[initiali][initialj]=allimages[x];

    initplay();
    return solved_or_not();
}

void go_up()
{

    if(box_pos_y-66<1)return;
    initiali--;
    box_pos_y-=66;
    //Clear screen
    SDL_SetRenderDrawColor( gRenderer, 13,11, 31, 0xFF );
    SDL_RenderClear( gRenderer );

    SDL_Rect topleftsign= {1,1,200,80};
    SDL_RenderSetViewport( gRenderer, &topleftsign );
    SDL_RenderCopy( gRenderer, sudokusign, NULL, NULL);
    SDL_Rect boardrect[10][10];



    ll i1=0,j1=0;
    for(ll j=1,i1=0; j<=529; j+=66,i1++)
    {
        for(ll i=206,j1=0; i<=734; i+=66,j1++)
        {
            boardrect[i1][j1]= {i,j,64,64};
            SDL_RenderSetViewport( gRenderer, &boardrect[i1][j1] );
            SDL_RenderCopy( gRenderer, gTexture[i1][j1], NULL, NULL );
            if(SudokuToSolve[i1][j1]!='*')
            {
                SDL_Rect greenrect= {i,j,64,64};
                SDL_RenderSetViewport(gRenderer,&greenrect);
                SDL_RenderCopy(gRenderer,green,NULL,NULL);
            }
        }
    }
    SDL_Rect boxrect= {box_pos_x, box_pos_y,66,66};
    SDL_RenderSetViewport(gRenderer,&boxrect);
    SDL_RenderCopy(gRenderer,box,NULL,NULL);
    SDL_RenderPresent( gRenderer );

}
void go_down()
{
    if(box_pos_y+66>529)return;
    initiali++;
    box_pos_y+=66;
    //Clear screen
    SDL_SetRenderDrawColor( gRenderer, 13,11, 31, 0xFF );
    SDL_RenderClear( gRenderer );

    SDL_Rect topleftsign= {1,1,200,80};
    SDL_RenderSetViewport( gRenderer, &topleftsign );
    SDL_RenderCopy( gRenderer, sudokusign, NULL, NULL);
    SDL_Rect boardrect[10][10];



    ll i1=0,j1=0;
    for(ll j=1,i1=0; j<=529; j+=66,i1++)
    {
        for(ll i=206,j1=0; i<=734; i+=66,j1++)
        {
            boardrect[i1][j1]= {i,j,64,64};
            SDL_RenderSetViewport( gRenderer, &boardrect[i1][j1] );
            SDL_RenderCopy( gRenderer, gTexture[i1][j1], NULL, NULL );
            if(SudokuToSolve[i1][j1]!='*')
            {
                SDL_Rect greenrect= {i,j,64,64};
                SDL_RenderSetViewport(gRenderer,&greenrect);
                SDL_RenderCopy(gRenderer,green,NULL,NULL);
            }
        }
    }
    SDL_Rect boxrect= {box_pos_x, box_pos_y,66,66};
    SDL_RenderSetViewport(gRenderer,&boxrect);
    SDL_RenderCopy(gRenderer,box,NULL,NULL);
    SDL_RenderPresent( gRenderer );
}
void go_left()
{
    if(box_pos_x-66<206)return;
    initialj--;
    box_pos_x-=66;
    //Clear screen
    SDL_SetRenderDrawColor( gRenderer, 13,11, 31, 0xFF );
    SDL_RenderClear( gRenderer );
    SDL_Rect topleftsign= {1,1,200,80};
    SDL_RenderSetViewport( gRenderer, &topleftsign );
    SDL_RenderCopy( gRenderer, sudokusign, NULL, NULL);
    SDL_Rect boardrect[10][10];



    ll i1=0,j1=0;
    for(ll j=1,i1=0; j<=529; j+=66,i1++)
    {
        for(ll i=206,j1=0; i<=734; i+=66,j1++)
        {
            boardrect[i1][j1]= {i,j,64,64};
            SDL_RenderSetViewport( gRenderer, &boardrect[i1][j1] );
            SDL_RenderCopy( gRenderer, gTexture[i1][j1], NULL, NULL );
            if(SudokuToSolve[i1][j1]!='*')
            {
                SDL_Rect greenrect= {i,j,64,64};
                SDL_RenderSetViewport(gRenderer,&greenrect);
                SDL_RenderCopy(gRenderer,green,NULL,NULL);
            }
        }
    }
    SDL_Rect boxrect= {box_pos_x, box_pos_y,66,66};
    SDL_RenderSetViewport(gRenderer,&boxrect);
    SDL_RenderCopy(gRenderer,box,NULL,NULL);
    SDL_RenderPresent( gRenderer );
}
void go_right()
{
    if(box_pos_x+66>734)return;
    initialj++;
    box_pos_x+=66;
    //Clear screen
    SDL_SetRenderDrawColor( gRenderer, 13,11, 31, 0xFF );
    SDL_RenderClear( gRenderer );

    SDL_Rect topleftsign= {1,1,200,80};
    SDL_RenderSetViewport( gRenderer, &topleftsign );
    SDL_RenderCopy( gRenderer, sudokusign, NULL, NULL);
    SDL_Rect boardrect[10][10];



    ll i1=0,j1=0;
    for(ll j=1,i1=0; j<=529; j+=66,i1++)
    {
        for(ll i=206,j1=0; i<=734; i+=66,j1++)
        {
            boardrect[i1][j1]= {i,j,64,64};
            SDL_RenderSetViewport( gRenderer, &boardrect[i1][j1] );
            SDL_RenderCopy( gRenderer, gTexture[i1][j1], NULL, NULL );
            if(SudokuToSolve[i1][j1]!='*')
            {
                SDL_Rect greenrect= {i,j,64,64};
                SDL_RenderSetViewport(gRenderer,&greenrect);
                SDL_RenderCopy(gRenderer,green,NULL,NULL);
            }
        }
    }
    SDL_Rect boxrect= {box_pos_x, box_pos_y,66,66};
    SDL_RenderSetViewport(gRenderer,&boxrect);
    SDL_RenderCopy(gRenderer,box,NULL,NULL);
    SDL_RenderPresent( gRenderer );
}

int main( int argc, char* args[] )
{
    freopen("out.txt","w",stdout);
    start:
        flag=false;
    InitSudokuToSolve();
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            bool quit = false;
            SDL_Event e;
            start();
            initplay();
            while( !quit )
            {
                while( SDL_PollEvent( &e ) != 0 )
                {
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    else if( e.type == SDL_KEYDOWN )
                    {
                        switch( e.key.keysym.sym )
                        {
                        case SDLK_UP:
                            go_up();
                            break;
                        case SDLK_DOWN:
                            go_down();
                            break;
                        case SDLK_LEFT:
                            go_left();
                            break;
                        case SDLK_RIGHT:
                            go_right();
                            break;
                        case SDLK_w:
                            go_up();
                            break;
                        case SDLK_s:
                            go_down();
                            break;
                        case SDLK_a:
                            go_left();
                            break;
                        case SDLK_d:
                            go_right();
                            break;
                        case SDLK_1:
                            quit=CheckOnDuty('1');
                            break;
                        case SDLK_2:
                            quit=CheckOnDuty('2');
                            break;
                        case SDLK_3:
                            quit=CheckOnDuty('3');
                            break;
                        case SDLK_4:
                            quit=CheckOnDuty('4');
                            break;
                        case SDLK_5:
                            quit=CheckOnDuty('5');
                            break;
                        case SDLK_6:
                            quit=CheckOnDuty('6');
                            break;
                        case SDLK_7:
                            quit=CheckOnDuty('7');
                            break;
                        case SDLK_8:
                            quit=CheckOnDuty('8');
                            break;
                        case SDLK_9:
                            quit=CheckOnDuty('9');
                            break;
                        case SDLK_KP_1:
                            quit=CheckOnDuty('1');
                            break;
                        case SDLK_KP_2:
                            quit=CheckOnDuty('2');
                            break;
                        case SDLK_KP_3:
                            quit=CheckOnDuty('3');
                            break;
                        case SDLK_KP_4:
                            quit=CheckOnDuty('4');
                            break;
                        case SDLK_KP_5:
                            quit=CheckOnDuty('5');
                            break;
                        case SDLK_KP_6:
                            quit=CheckOnDuty('6');
                            break;
                        case SDLK_KP_7:
                            quit=CheckOnDuty('7');
                            break;
                        case SDLK_KP_8:
                            quit=CheckOnDuty('8');
                            break;
                        case SDLK_KP_9:
                            quit=CheckOnDuty('9');
                            break;
                        default:
                            showinvalid();
                            break;

                        }
                    }
                }
            }
            quit=false;
            if(flag)
            {
                while(!quit)
                {
                    while(SDL_PollEvent(&e)!=0)
                    {
                        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 0 );
                        SDL_RenderClear( gRenderer );
                        SDL_Rect congrats_rect= {47,120,903,360};
                        SDL_RenderSetViewport(gRenderer,&congrats_rect);
                        SDL_RenderCopy(gRenderer,congrats,NULL,NULL);

                        SDL_RenderPresent( gRenderer );
                        if(e.type==SDL_QUIT)
                        {
                            quit=true;
                        }
                        else if( e.type == SDL_KEYDOWN )
                        {
                            switch( e.key.keysym.sym )
                            {
                                case  	 SDLK_TAB:
                                    close();
                                    goto start;
                                    break;
                                default :
                                    break;

                            }
                        }


                    }
                }
            }


        }
    }
    close();

    return 0;
}
