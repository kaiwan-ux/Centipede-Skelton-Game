/*=======================

Name: Abdul Saboor
Roll No: 23I-3039
Project: Programming Fundamentals

=========================*/
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX=960;
const int resolutionY=960;
const int boxPixelsX=32;
const int boxPixelsY=32;
const int gameRows=resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid
const int max_Rows_player=5;

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// The Score counter; this will calcuate the score
int score=0;

const int x = 0;
const int y = 1;
const int exists = 2;


bool Keys(const sf::Keyboard::Key& key);
bool SpaceKey(sf::RenderWindow& window);

bool bullet_touch_Mushroom(float bullet[], int mushroom[]);
bool player_touch_Centipede(float player[], int centipede[][3], int length);

void Start_Mushrooms();
void Start_Centipede();

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite);
void drawMushrooms(sf::RenderWindow& window, sf::Sprite& mushroomSprite);
void drawCentipede(sf::RenderWindow& window, sf::Sprite& SpriteofCentipede);

void Bullet_touch_Centipede(float bullet[]);
void Bullet_touch_Mushroom(float bullet[]);
void Check_Bullet(float bullet[]);
const int number_Mushrooms = 26;
const int mushroom_size = 32;
int G_Mushroom[number_Mushrooms][3];

void moveBullet(float bullet[], sf::Clock& bulletClock);
void handleCollision(sf::Clock& centipedeClock, bool& Left);
void originalMoveCentipede(sf::Clock& centipedeClock);
void Centipedemove(bool Left);

void drawMenu(sf::RenderWindow& window, sf::Font& font);

const int SizeofCentipede = 32;
const int LengthofCentipede= 12; // Move the declaration here
int G_centipede[LengthofCentipede][3];

// Some of the Musical Variables: Declared Here
sf::Music kill;
sf::Music death;
sf::Music fire;
sf::Music Touch;


int main(){


    srand(time(0));

    // Declaring RenderWindow.
    sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);

    // Used to resize your window if it's too big or too small. Use according to your needs.
    window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.

    // Used to position your window on every launch. Use according to your needs.
    window.setPosition(sf::Vector2i(100, 0));

    // Musical are here:
    sf::Music bgMusic;
    bgMusic.openFromFile("Music/field_of_hopes.ogg");
    bgMusic.play();
    bgMusic.setVolume(50);

    kill.openFromFile("Sound Effects/kill.wav");
    kill.setVolume(50);

    death.openFromFile("Sound Effects/death.wav");
    death.setVolume(50);
 
    fire.openFromFile("Sound Effects/fire1.wav");
    fire.setVolume(45);
   
    Touch.openFromFile("Sound Effects/newBeat.wav");
    Touch.setVolume(60);

// Font is declared here:
    sf::Font font;
    font.loadFromFile("Font/Arial.ttf");

    sf::Text score_Text;
    score_Text.setFont(font);
    score_Text.setCharacterSize(33);
    score_Text.setFillColor(sf::Color::White);
    score_Text.setPosition(resolutionX - 170, 7);
   

    // Initializing Background.
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    backgroundTexture.loadFromFile("Textures/background.png");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.80));

    // Initializing Player and Player Sprites.
    float player[2] = {};
    player[x] = (gameColumns / 2) * boxPixelsX;
    player[y] = (gameColumns * 3 / 4) * boxPixelsY;
    sf::Texture playerTexture;
    sf::Sprite playerSprite;
    playerTexture.loadFromFile("Textures/player.png");
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Initializing Bullet and Bullet Sprites.
    float bullet[3] = {};
    bullet[x] = player[x];
    bullet[y] = player[y] - boxPixelsY;
    bullet[exists] = true;
    sf::Clock bulletClock;
    sf::Texture bulletTexture;
    sf::Sprite bulletSprite;
    bulletTexture.loadFromFile("Textures/bullet.png");
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
 
    // Initializing  Mushrooms and Mushroom Sprites
    sf::Texture mushroom_Texture;
    mushroom_Texture.loadFromFile("Textures/mushroom.png");
    sf::Sprite mushroom_Sprite;
    mushroom_Sprite.setTexture(mushroom_Texture);
    mushroom_Sprite.setTextureRect(sf::IntRect (0, 0,mushroom_size, mushroom_size));
 
    // Initializing  Centipede and Centipede Sprites
    sf::Texture TextureofCentipede;
    TextureofCentipede.loadFromFile("Textures/c_body_left_walk.png");
    sf::Sprite SpriteofCentipede;
    sf::Clock centipedeClock;
    SpriteofCentipede.setTexture(TextureofCentipede);
    SpriteofCentipede.setTextureRect(sf::IntRect(0, 0, SizeofCentipede, SizeofCentipede));
 
       
    int a=0;
    a=rand()%10;

    Start_Mushrooms();
    Start_Centipede();
    int currentLevel = 1;
   
   // Some of the Flags are declared here:
    bool inMenu=1;
    bool gameStarted=0;
    bool playerAlive=1;
    bool firstTime=1;


while (window.isOpen()) {
   

    sf::Event event;
    // Work: Make a startup menu; before the game:
    while(window.pollEvent(event)){
        if(event.type == sf::Event::Closed){
            window.close();
        }
// The User: Enter S, the Game Starts, and E or Exist
        if(event.type == sf::Event::KeyPressed){
            if(inMenu){
                if(event.key.code == sf::Keyboard::Key::S){
                   
                    inMenu = 0;
                    gameStarted = 1;
                }                                
                 else if(event.key.code == sf::Keyboard::Key::E){
                 
                    window.close();  }
          }
         }
        }
   
    window.clear();

    if(inMenu){
        drawMenu(window, font);
       
    } else if(gameStarted){
   
    // Work: This creates the bullet: when space key is pressed        
    if(SpaceKey(window) && !bullet[exists]){
    bullet[x]=player[x];
    bullet[y]=player[y] - boxPixelsY;
    bullet[exists] = 1;
    }
   
// These Control the movement of the player accroding to arrows:
    if (Keys(sf::Keyboard::Left) && player[x] > 1) {
    player[x]=player[x]-1;
    }
   
    if (Keys(sf::Keyboard::Right) && player[x] < resolutionX - boxPixelsX) {
    player[x]=player[x]+1;
    }
   
    if (Keys(sf::Keyboard::Up) && player[y] > 1 && player[y] > (gameRows - max_Rows_player) * boxPixelsY) {
        player[y]=player[y]-1;
        }
   
    if (Keys(sf::Keyboard::Down) && player[y] < resolutionY - boxPixelsY) {
    player[y]=player[y]+1;
    }

// Work: This shows the Score on the Screen:
    score_Text.setString("Score: " + std::to_string(score));
    window.draw(backgroundSprite);
   
   
    drawPlayer(window, player, playerSprite);
// These Draw the Player as well as the bullet:
    if (bullet[exists]){

    moveBullet(bullet, bulletClock);
    drawBullet(window, bullet, bulletSprite);
   
    }

// Some of the Function are been called;
    drawMushrooms(window, mushroom_Sprite);
    drawCentipede(window, SpriteofCentipede);
    Check_Bullet( bullet);
    originalMoveCentipede(centipedeClock);
   
   // The Score is drawn;
    window.draw(score_Text);
   
   
   // Work: This condition runs when the player touches the centipede and
   // the player goes to sleep for 3 second and the menu is shown again:
    if (player_touch_Centipede(player, G_centipede, LengthofCentipede)){
            death.play();
            gameStarted = 0;
            inMenu = 1;
            sf::sleep(sf::seconds(3));
        }
     
    sf::Event e;
    while (window.pollEvent(e)){
    if (e.type == sf::Event::Closed){
    return 0;
    }
    }
                   
        }
       
        window.display();
        window.clear();
    }

   
return 0;
}

// Work: Check when the arrows buttons are pressed,
// and move or handle the player's according to it;
bool Keys(const sf::Keyboard::Key& key){
    return sf::Keyboard::isKeyPressed(key);
}


// Work: Check when the Space key is pressed,
// and it will fire the bullet
bool SpaceKey(sf::RenderWindow& window){
return sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
}


// Work: when the bullet touches the mushroom; this bool fucntion will return
// the 1 which means that bullet has touches the mushroom
bool bullet_touch_Mushroom(float bullet[], int mushroom[]){
    float bullet_Left=bullet[0];        
    float bullet_Right=(bullet_Left+boxPixelsX);
    float bullet_Top=bullet[1];        
    float bullet_Bottom=(bullet_Top+boxPixelsY);

    float mushroom_Left=mushroom[0];    
    float mushroom_Right=(mushroom_Left+mushroom_size);
    float mushroom_Top=mushroom[1];    
    float mushroom_Bottom=(mushroom_Top+mushroom_size);

    bool CheckX = ((bullet_Right > mushroom_Left) && (bullet_Left < mushroom_Right));
    bool CheckY = ((bullet_Bottom > mushroom_Top) && (bullet_Top < mushroom_Bottom));

    return (CheckX&&CheckY);
}


// Work: when the player touches the centipede; this bool fucntion will return
// the 1 which means that player has touches the centipede
bool player_touch_Centipede(float player[], int centipede[][3], int length){
    float playerLeft=player[x];
    float playerRight=playerLeft+boxPixelsX;
    float playerTop=player[y];
    float playerBottom=playerTop+boxPixelsY;
   
// Applied loop, because there are 12 segments of centipede not just one
    for (int i=0;i<length;i++){
        if (centipede[i][exists]){
            float centipede_Left =centipede[i][x];
            float centipede_Right =(centipede_Left+SizeofCentipede);
            float centipede_Top =centipede[i][y];
            float centipede_Bottom =(centipede_Top+SizeofCentipede);

           
            bool CheckX = ((playerRight > centipede_Left) && (playerLeft < centipede_Right));
            bool CheckY = ((playerBottom > centipede_Top) && (playerTop < centipede_Bottom));

            if (CheckX && CheckY){
            // collides
                return 1;
            }
        }
    }
// means no collision
    return 0;
}


// Work: This will generate the Mushrooms at the random postion;
void Start_Mushrooms(){
    int i = 0;
   
        do{
       
    // We have done gameRows-max_Rows_player
// to not generate mushroom in the player restriced area;
        G_Mushroom[i][y] = (rand() % (gameRows-max_Rows_player)*boxPixelsY);
        G_Mushroom[i][x] = (rand() % gameColumns*boxPixelsX);
       
        // This like makes sure that ith mushroom exists
        G_Mushroom[i][exists] = 1;
        i++;
    }while(i<number_Mushrooms);
   
return;
}


// Work: Generate the Centipede at the random location on the grid,
// but it is not in the player area
void Start_Centipede(){

    int CheckX = (rand() % (gameColumns-LengthofCentipede)*boxPixelsX);
    int CheckY = (rand() % (gameRows-max_Rows_player)*boxPixelsY);
    int i=0;
   
    do{
        G_centipede[i][y] = CheckY;
        G_centipede[i][x] = (CheckX + i * boxPixelsX);
       
// This like makes sure that ith segment of the Centipede exists
        G_centipede[i][exists] = 1;
        i++;
    }while(i<LengthofCentipede);
   
    return;
}


// Work: This fucntion will draw the player
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite){
playerSprite.setPosition(player[x], player[y]);
window.draw(playerSprite);
return;
}


// Work: This function will draw the bullet,
// initially from where the player is placed
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite){
bulletSprite.setPosition(bullet[x], bullet[y]);
window.draw(bulletSprite);
return;
}


// Work: Draw the Mushrooms, where the ith Mushroom's flag is already true;
void drawMushrooms(sf::RenderWindow& window, sf::Sprite& mushroom_Sprite){
    int count = 0;
    while(count<number_Mushrooms){
        if(G_Mushroom[count][exists]){
            mushroom_Sprite.setPosition(G_Mushroom[count][x], G_Mushroom[count][y]);
            window.draw(mushroom_Sprite);
        }
        count=count+1;
    }
    return; }


// Work: This fucntion will draw the segments of the centipede,
// where the ith Centipede's flag is true
void drawCentipede(sf::RenderWindow& window, sf::Sprite& SpriteofCentipede){
    int count=0;
   
    do{
// When the flag is true, centipede's segment will be draw  
        if(G_centipede[count][exists]){
            SpriteofCentipede.setPosition(G_centipede[count][x], G_centipede[count][y]);
            window.draw(SpriteofCentipede);    
    }
    count=count+1;
   
    }while(count<LengthofCentipede);
   
    return ;
}


// Work: Removes the Mushroom when the bullet touches the mushroom,
// for each ith mushroom it checks
void Bullet_touch_Mushroom(float bullet[]){
    int count=0;
   
    while(count<number_Mushrooms){
   
    // calls the bulletmushroomtouch fucntion and check the existence of mushroom
        if (G_Mushroom[count][exists] && bullet_touch_Mushroom(bullet, G_Mushroom[count])){
            bullet[exists] = 0;
            G_Mushroom[count][exists] = 0;
            score=score+1;

// Done for the Music of kill, that Mushroom is been destroyed
            if (!kill.openFromFile("Sound Effects/kill.wav")) {
                cout<<"Error loading sound file!\n";
            } else {
                kill.play();
            }
        }
        count++;
    }
    return;
}


// Work: Removes the Segment of Centipede where the bullet has hit the centipede,
// it is done by equating the flag for existence of segment of centipede to zero
void Bullet_touch_Centipede(float bullet[]){
    int i=0;
    while(i<LengthofCentipede){
   
     // This check wheather the Bullet has hit the Centipede or not;
      bool bullet_Exists = bullet[exists];
      bool bullet_Above = (bullet[y]<G_centipede[i][y]+SizeofCentipede);
      bool bullet_Below = (bullet[y]+boxPixelsY>G_centipede[i][y]);
      bool bullet_Left = (bullet[x]<G_centipede[i][x]+SizeofCentipede);
      bool bullet_Right = (bullet[x]+boxPixelsX>G_centipede[i][x]);

        if (G_centipede[i][exists] && (bullet_Exists && bullet_Above && bullet_Below && bullet_Left && bullet_Right)){
            score=score+10;
           
           // The Music is been played when segment of Centipede is removed
            if (!fire.openFromFile("Sound Effects/fire1.wav")){
                cout<<"Error loading sound file!\n";
            } else {
                fire.play();  }
           
           // both bullet and segment of centipede are been removed;
           bullet[exists]=0;            
           G_centipede[i][exists]=0;
            break;
        }
        i++;
}
return;
}


// Work: This function call both of the fucntions;
void Check_Bullet(float bullet[]){
    Bullet_touch_Mushroom(bullet);
    Bullet_touch_Centipede(bullet);
}


// Work: This function moves the bullet and removes it
// when it is going out of the windown screen or grid
void moveBullet(float bullet[], sf::Clock& bulletClock){
    if (bulletClock.getElapsedTime().asMilliseconds() < 5)
    return;
    bulletClock.restart();
    bullet[y] = bullet[y]-10;
    if (bullet[y] < -32)
    bullet[exists] = 0;
    return;
}


//  Work: This the Function to handle touches or collision between the centipede and
// mushrooms, and update the direction and position.
void handleCollision(bool& Left){
    bool collisionCheck = false;
// Work: This loop check for the existence of each segment of centipede
    for(int i=0;i<LengthofCentipede;i++){
        if(G_centipede[i][exists]){
            int centipede_Left = G_centipede[i][x] + (Left ? -1 : 1) * boxPixelsX;
            int centipede_Top = G_centipede[i][y];
            int centipede_Right = (centipede_Left + SizeofCentipede);
            int centipede_Bottom = (centipede_Top + SizeofCentipede);

            //Work: This loop check wheather each segment of Centipede
// touches the Mushroom
            for(int j=0;j<number_Mushrooms;j++){
           
            // Work: This will calculate the area occupied by the centipede
                if (G_Mushroom[j][exists]) {
                    int mushroomRight=(G_Mushroom[j][x]+mushroom_size);
                    int mushroomBottom=(G_Mushroom[j][y]+mushroom_size);

                   
                    bool collisionX=((centipede_Left<mushroomRight) && (centipede_Right>G_Mushroom[j][x]));
                    bool collisionY=((centipede_Top<mushroomBottom) && (centipede_Bottom>G_Mushroom[j][y]));

                    if(collisionX && collisionY){
                        collisionCheck = true;
                        if (!Touch.openFromFile("Sound Effects/newBeat.wav")){
                           
                            cout<<"Error loading sound file!\n";
                        } else {
                            Touch.play();
                        }

                        break;
                   }
                 }
          }
        }
        }

    // Work: Handle collision by changing direction and moving down
      if(collisionCheck){
           Left = !Left; // Reverse direction
           int j=0;
            while (j < LengthofCentipede){
            G_centipede[j][y]=G_centipede[j][y]+boxPixelsY;
            j++;
         }
      }
        // Check if the centipede is about to move out of the grid
        switch(Left){
        case 1:
            if(G_centipede[0][x]<-360){
                // Move the entire centipede down, if touches the wall of the screen;
                int count=0;
                while(count < LengthofCentipede){
                    if (G_centipede[count][exists]){
                        G_centipede[count][y]=G_centipede[count][y]+boxPixelsY;
                    }
                    count++;
                }

                // Changes the direction after been hit by the wall
                Left = !Left;
            }
            break;
        case 0:
         
            if (G_centipede[LengthofCentipede - 1][x] > resolutionX && G_centipede[0][x]>960){
                // Move the entire centipede down, if touches the wall of the screen
                int count=0;
                while(count < LengthofCentipede){
                    if (G_centipede[count][exists]){
                        G_centipede[count][y]=G_centipede[count][y]+boxPixelsY;
                    }
                    count++;
                }
// changes the direction after been hit by the wall
                Left = !Left;
            }
            break;
        }
       

       
    return;
}


// Work: Function to move the centipede
// along the x-axis;
void Centipedemove(bool Left){
    // Move the centipede and handle collision
    int i=0;
    while(i<LengthofCentipede){
        if (G_centipede[i][exists]){
            Left ? (G_centipede[i][x]=G_centipede[i][x]-1) : (G_centipede[i][x]=G_centipede[i][x]+1);
        }
        i++;
    }
}


// Work: The original function can now call these two functions separately
void originalMoveCentipede(sf::Clock& centipedeClock){
    static bool Left = 1;
    const int centipedeMoveInterval = 1;

    if (centipedeClock.getElapsedTime().asMilliseconds() > centipedeMoveInterval) {
        handleCollision(Left);
        Centipedemove(Left);
        centipedeClock.restart();
    }
}


// Work: This fucntion will the draw the Startup Menu;
void drawMenu(sf::RenderWindow& window, sf::Font& font) {
    sf::Text start_Text("Start", font, 55);
    sf::Text options_Text("Options", font, 55);
    sf::Text exit_Text("Exit", font, 55);

    start_Text.setPosition(360, 340);
    options_Text.setPosition(360, 450);
    exit_Text.setPosition(360, 570);

    window.draw(start_Text);
    window.draw(options_Text);
    window.draw(exit_Text);
   
    return;
}
