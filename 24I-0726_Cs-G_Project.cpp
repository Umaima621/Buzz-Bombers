#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.

const int resolutionX = 960;
const int resolutionY = 640;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; 
const int gameColumns = resolutionY / boxPixelsY; 
int beesHandled = 0;
bool sessionOver = false;

int gameGrid[gameRows][gameColumns] = {};
void start_game(RenderWindow& window);
void start_level2(RenderWindow& window);
void start_level3(RenderWindow& window);
void beemovesprite(RenderWindow& window, Sprite RegularbeeSprite[], Vector2f RegularbeeDirection[], float Regularbee_moveSpeed, Clock RegularbeeClock[], int Regularbee_direction[], bool& isRegularbeePaused, int size, bool bee_active[], Sprite obstaclesSprite[], bool isobstaclesvisible[], int flower); 
void birdmovesprite(Sprite& birdSprite, Vector2f& birdDirection, float bird_moveSpeed, Clock& birdClock, bool& isbirdPaused); 
bool Fastbee_collision(RenderWindow& window, Sprite& bulletSprite, Sprite Fast_beeSprite[], Sprite honeycomb_redSprite[], int i);

void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite);
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock);
void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite);
void drawbird(RenderWindow&, Sprite& birdSprite);
void drawFastbee(RenderWindow& window, Sprite Fast_beeSprite[], int fastsize);
bool collision(RenderWindow& window, Sprite& bulletSprite, Sprite RegularbeeSprite[], Sprite honeycombSprite[], int i, bool ishoneycombvisible[]);
void fastbeemovesprite(Sprite Fast_beeSprite[], Vector2f Fast_beeDirection[], float Fast_bee_moveSpeed, Clock Fast_beeClock[], bool& isFast_beePaused, int fastsize);
void drawRegularbee(RenderWindow& window, Sprite RegularbeeSprite[], int size, bool bee_active[], Clock RegularbeeClock[], int Regularbee_direction[]);
bool honey_bee_collision(RenderWindow& window, Sprite RegularbeeSprite[], Sprite honeycombSprite[], int size);
bool honey_bullet_collision(RenderWindow& window, Sprite& bulletSprite, Sprite honeycombSprite[], int i, bool ishoneycombvisible[]);
bool bird_collision(RenderWindow& window, Sprite& bulletSprite, Sprite& birdSprite, int bird_hit);

int main()
{
    srand(time(0));
    // Title and window
    bool MenuActive=true;

 sf::RenderWindow window(sf::VideoMode(800, 600), "Main Menu");

    // Loading font
    sf::Font font;
    if (!font.loadFromFile("Textures/GAMEDAY.ttf")) {
        return -1; // Ensure you have the font file
    }
   sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("image.png")) {
        return -1;
    }

    // Create  background sprite
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
   


    // Menu options
    sf::Text menu[4];
    std::string options[4] = {"Start level1", "Start level2","Start level3", "Exit"};
    for (int i = 0; i < 4; ++i) {
  
	        menu[i].setFont(font);
        menu[i].setString(options[i]);
        menu[i].setCharacterSize(40);
        menu[i].setFillColor(sf::Color::White);
        menu[i].setPosition(350+30, 200 + i * 60+130); 
    }

    int selectedItemIndex = 0; // Start with the first menu item
    bool isUpPressed = false;
    bool isDownPressed = false;
    sf::Clock inputClock;

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Input handling with a delay
        if (inputClock.getElapsedTime().asMilliseconds() > 200) { // 200 ms delay between key presses
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isUpPressed) {
            cout<<selectedItemIndex<<endl;
                selectedItemIndex = (selectedItemIndex-1 );
                isUpPressed = true;
                inputClock.restart(); // Restart the clock for the delay
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !isDownPressed) {
                selectedItemIndex = (selectedItemIndex +1) ;
                isDownPressed = true;
                inputClock.restart();
            }

            // Handle Enter key
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                if (selectedItemIndex == 0) {
                    start_game(window);
                } else if (selectedItemIndex == 1) {
                  start_level2(window);
                } else if (selectedItemIndex == 2) {
                  start_level3( window);
                }
                else if (selectedItemIndex == 3) {
                    window.close(); // Exit the game
                }
            }
        }

        // Reset the input flags after the key is released
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            isUpPressed = false;
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            isDownPressed = false;
        }

        // Highlight the selected menu item
        for (int i = 0; i < 4; ++i) {
            if (i == selectedItemIndex) {
                menu[i].setFillColor(sf::Color::Red);  // Highlight selected item
                menu[i].setCharacterSize(50);          // Increase font size on selection
            } else {
                menu[i].setFillColor(sf::Color::White); // Normal color
                menu[i].setCharacterSize(40);          // Regular font size
            }
        }

        // Clear and redraw
        window.clear();
         window.draw(backgroundSprite);
        for (int i = 0; i < 3; ++i) {
            window.draw(menu[i]);
            
        }
        window.display();
        
    }

    return 0;
}
void start_game(RenderWindow& window){
    RenderWindow localwindow(VideoMode(resolutionX, resolutionY), "Buzz Bombers", Style::Close | Style::Titlebar);
    window.setPosition(Vector2i(700, 400));
sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("image.png")) {
        return ;
    }

    // Create  background sprite
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
 Music bgMusic;
	if (!bgMusic.openFromFile("music1.ogg")) {
              cout << "Error: Could not load music file!" << endl;
               return ;
	   }
	   
	bgMusic.setVolume(50);
	bgMusic.setLoop(true);
	bgMusic.play();

    // set up bird texture
    Texture birdTexture;
    if (!birdTexture.loadFromFile("Textures/bird.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }

    Sprite birdSprite;
    birdSprite.setTexture(birdTexture);

    float birdMovement_x = rand() % (resolutionX - boxPixelsX);
    float birdMovement_y = rand() % (resolutionY - boxPixelsY);
    
    float bird_moveSpeed = 2.0f;
    int moveDirection = 5;
    
    sf::Vector2f birdDirection(0, 0); // Direction of movement
    sf::Clock birdClock; // For timing pauses
    int bird_hit = 0;
    bool isbirdPaused = false;
    
    IntRect birdTextureRect = birdSprite.getTextureRect();
    float bird_width = static_cast<float>(birdTextureRect.width);
    float bird_height = static_cast<float>(birdTextureRect.height);
    
    // Initializing Player and Player Sprites.
    float player_x = (gameRows / 2) * boxPixelsX;
    float player_y = (gameColumns - 4) * boxPixelsY;
    
    // Set up player texture
    Texture playerTexture;
    Sprite playerSprite;
    if (!playerTexture.loadFromFile("Textures/spray.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    playerSprite.setTexture(playerTexture);
    playerSprite.setScale(1.f, 1.f);    
    float bullet_x = player_x;
    float bullet_y = player_y;
    bool bullet_exists = false;

    Clock bulletClock;
    Texture bulletTexture;
    Sprite bulletSprite;
    bulletTexture.loadFromFile("Textures/bullet.png");
    
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setScale(1.f, 1.f);
    bulletSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    // The ground on which player moves
    RectangleShape groundRectangle(Vector2f(960, 64));
    groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);
    groundRectangle.setFillColor(Color::Green);
    
    Font font;
    if (!font.loadFromFile("Textures/GAMEDAY.ttf")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    Text scoretext;
    scoretext.setFont(font);
    scoretext.setFillColor(Color::Black);
    scoretext.setCharacterSize(35);
    scoretext.setStyle(Text::Bold);
    FloatRect textBounds = scoretext.getLocalBounds();     // Get text bounds
    
    Vector2f rectPos = groundRectangle.getPosition();       // Rectangle position
    Vector2f rectSize = groundRectangle.getSize();          // Rectangle size

    scoretext.setPosition(
        rectPos.x + 20,
        rectPos.y + 10 
    );
    int score = 0;
    scoretext.setString("Score: " + std::to_string(score));
    int max_bullet=56;
    Text bullettext;
      bullettext.setFont(font);
      bullettext.setFillColor(Color::Black);
      bullettext.setCharacterSize(35);
      int can=0;
      bullettext.setStyle(Text::Bold);
      textBounds = bullettext.getLocalBounds();     // Get text bounds
      
      
      bullettext.setPosition(
           rectPos.x +760 ,
           rectPos.y +10 
      );
     int bullet_count=0;
     
     
    float moveSpeed = 5.f;
    
    //<----------------------BEE--------------------------------->
    Texture RegularbeeTexture;
    if (!RegularbeeTexture.loadFromFile("Textures/Regularbee.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    
    const int size = 20;
    Vector2f OriginalDirection[size];
    Sprite RegularbeeSprite[size];
    sf::Vector2f RegularbeeDirection[size]; // Direction of movement
    sf::Clock RegularbeeClock[size]; // For timing pauses
    
    float RegularbeeMovement_x = rand() % (resolutionX - boxPixelsX);
    float RegularbeeMovement_y = rand() % (resolutionY - boxPixelsY);
    
    for (int i = 0; i < size; i++) {
        RegularbeeSprite[i].setTexture(RegularbeeTexture);
        RegularbeeSprite[i].setScale(-1.f, 1.f);
        RegularbeeDirection[i] = Vector2f(0, 0);
    }
    
    int Regularbee_direction[size];
    bool isRegularbeePaused = true;
    bool bee_active[size];
    
    for (int i = 0; i < size; i++) {
        IntRect RegularbeeTextureRect = RegularbeeSprite[i].getTextureRect();
        float Regularbee_width = static_cast<float>(RegularbeeTextureRect.width);
        float Regularbee_height = static_cast<float>(RegularbeeTextureRect.height);
        
        Regularbee_direction[i] = 0;
        bee_active[i] = false;
    }
    
    int Regularbee_moveSpeed = 5.f;
    
    //<--------------------------HONEYCOMB----------------------------->
    Texture honeycombTexture;
    if (!honeycombTexture.loadFromFile("Textures/honeycomb.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    
    Sprite honeycombSprite[size];
    bool ishoneycombvisible[size] = {false};
    
    for (int i = 0; i < size; i++) {
        honeycombSprite[i].setTexture(honeycombTexture);
        ishoneycombvisible[i] = false;
    }
    
    Vector2f honeycombposition[size];
    
    //<-------------------------------REDBEE------------------------------->
    Texture Fast_beeTexture;
    if (!Fast_beeTexture.loadFromFile("Textures/Fast_bee.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    const int Fast_size=20;
    Vector2f OriginalfastDirection[Fast_size];
    Sprite Fast_beeSprite[Fast_size];
    sf::Vector2f Fast_beeDirection[Fast_size]; // Direction of movement
    sf::Clock Fast_beeClock[Fast_size]; // For timing pauses
    
    float Fast_beeMovement_x = rand() % (resolutionX - boxPixelsX);
    float Fast_beeMovement_y = rand() % (resolutionY - boxPixelsY);
    
    for (int i = 0; i < Fast_size; i++) {
        Fast_beeSprite[i].setTexture(Fast_beeTexture);
        Fast_beeSprite[i].setScale(-1.f, 1.f);
        Fast_beeDirection[i] = Vector2f(0, 0);
    }
    
    bool isFast_beePaused = true;
    
    for (int i = 0; i < Fast_size; i++) {
        IntRect Fast_beeTextureRect = Fast_beeSprite[i].getTextureRect();
        float Fast_bee_width = static_cast<float>(Fast_beeTextureRect.width);
        float Fast_bee_height = static_cast<float>(Fast_beeTextureRect.height);
    }
    int Fast_bee_moveSpeed = 2.f;
    
    //<------------------------------------------------------------------->//
    //<--------------------------REDHONEYCOMB----------------------------->
    Texture honeycomb_redTexture;
    if (!honeycomb_redTexture.loadFromFile("Textures/honeycomb_red.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    Sprite honeycomb_redSprite[Fast_size];
    
    for (int i = 0; i < Fast_size; i++) {
        honeycomb_redSprite[i].setTexture(honeycomb_redTexture);
    }
    
    bool ishoneycomb_redvisible[Fast_size] = {false};
    Vector2f honeycomb_redposition[Fast_size];
    
    //<---------------------------OBSTACLES--------------------------------->
    
    Texture obstaclesTexture;
    if (!obstaclesTexture.loadFromFile("Textures/obstacles.png")) {
        cout << "Error: Could not load obstacles texture!" << endl;
        return ;
    }
    
    Sprite obstaclesSprite[size];
    bool isobstaclesvisible[size] = {false};
    
    for (int i = 0; i < size; i++) {
        obstaclesSprite[i].setTexture(obstaclesTexture);
        isobstaclesvisible[i] = {false};
        obstaclesSprite[i].setScale(2.f, 2.f);
    }
    
    int flower = 0;       
    Vector2f obstaclesposition[size];
    //<---------------------------MAINLOOP--------------------------------->
    
    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
            }
        }

        if (sessionOver) {
            Text sessionOverText;
            sessionOverText.setFont(font); // Assuming 'font' is already loaded
            sessionOverText.setFillColor(Color::Red);
            sessionOverText.setCharacterSize(50);
            sessionOverText.setStyle(Text::Bold);
            sessionOverText.setString("Session Over");

            FloatRect textBounds = sessionOverText.getLocalBounds();
            sessionOverText.setPosition(
                (resolutionX - textBounds.width) / 2,
                (resolutionY - textBounds.height) / 2
            );

            window.clear(Color::Black);
            window.draw(sessionOverText);
            window.display();

            // Pause the game or wait for user input to close
            while (true) {
                Event e;
                while (window.pollEvent(e)) {
                    if (e.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
                        window.close();
                        
                    }
                }
            }
        }

        birdmovesprite(birdSprite, birdDirection, bird_moveSpeed, birdClock, isbirdPaused);
        
        if (bullet_exists) {
            if (bird_collision(window, bulletSprite, birdSprite, bird_hit)) {
                bird_hit++;
                bulletSprite.setPosition(-1000, -1000);
                bullet_exists = false;
    
                if (bird_hit == 3) {
                    bird_hit = 0;
                    birdMovement_x -= 100;
         
                    birdDirection = Vector2f(birdMovement_x, birdMovement_y);
        
                    if (birdClock.getElapsedTime().asSeconds() > 3) {
                        birdSprite.setPosition(0, 0);
                    }
                }
            }
        }
        
        //<----------------------BEE--------------------------------->
        if (bullet_exists) {
            for (int i = 0; i < size; i++) {
                if (bee_active[i]) {
                    if (collision(window, bulletSprite, RegularbeeSprite, honeycombSprite, i, ishoneycombvisible)) {
    honeycombposition[i].x = RegularbeeSprite[i].getPosition().x;
    honeycombposition[i].y = RegularbeeSprite[i].getPosition().y;

    RegularbeeSprite[i].setPosition(-1000, -1000);
    ishoneycombvisible[i] = true;
    bee_active[i] = false;
    beesHandled++;

   // score += 2000; // Add points for honeycomb creation
    scoretext.setString("Score: " + std::to_string(score)); // Update score display

    if (beesHandled >= 20) {
        sessionOver = true;
    }
}

                }
            }
        }

        for (int i = 0; i < size; i++) {
            if (bee_active[i]) {
                for (int j = 0; j < size; j++) {
                    if (ishoneycombvisible[j]) {  
                        if (honey_bee_collision(window, RegularbeeSprite, honeycombSprite, size)) {
                            RegularbeeSprite[i].setScale(-1.f, 1.f);
                            float y = RegularbeeSprite[i].getPosition().y;
                            y += 50;
                            RegularbeeSprite[i].setPosition(RegularbeeSprite[i].getPosition().x, y);
                            bee_active[i] = false;
                        }
                    }
                }
            }
        }

        for (int i = 0; i < size; i++) {
            if (ishoneycombvisible[i]) {
                if (honey_bullet_collision(window, bulletSprite, honeycombSprite, i, ishoneycombvisible)) {
    honeycombSprite[i].setPosition(-100, -100);
    ishoneycombvisible[i] = false;

    score += 1000; // Add points for honeycomb destruction by birds
    scoretext.setString("Score: " + std::to_string(score)); // Update score display
}

            }
        }

        beemovesprite(window, RegularbeeSprite, RegularbeeDirection, Regularbee_moveSpeed, RegularbeeClock, Regularbee_direction, isRegularbeePaused, size, bee_active, obstaclesSprite, isobstaclesvisible, flower);
        fastbeemovesprite(Fast_beeSprite, Fast_beeDirection, Fast_bee_moveSpeed, Fast_beeClock, isFast_beePaused, size);
    
        if (bullet_exists) {
            for (int i = 0; i < Fast_size; i++) {
                if (Fastbee_collision(window, bulletSprite, Fast_beeSprite, honeycomb_redSprite, i)) {
                    honeycomb_redposition[i].x = Fast_beeSprite[i].getPosition().x;
                    honeycomb_redposition[i].y = Fast_beeSprite[i].getPosition().y;
    
                    Fast_beeSprite[i].setPosition(-100, -100);
                    ishoneycomb_redvisible[i] = true;
                }
            }
        }
    
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            player_x += moveSpeed;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            player_x -= moveSpeed;
        }

        if (player_x < 0) {
            player_x = 0;  // Prevent left off-screen
        }
        if (player_x > resolutionX - boxPixelsX) {
            player_x = resolutionX - boxPixelsX;  // Prevent right off-screen
        }
        if (!bullet_exists && Keyboard::isKeyPressed(Keyboard::Space)) {
            bullet_x = player_x + (boxPixelsX / 2) - 10;  // Adjust bullet spawn position
            bullet_y = player_y;
            bullet_exists = true;
            if(can<3){
           
            bullet_count++;
            if(bullet_count>=57){
            bullet_count=0;
            can++;}
            }
            bullettext.setString("Bullets Left: " + std::to_string(bullet_count));
        }

        window.clear();
         window.draw(backgroundSprite);
        if (bullet_exists) {
            moveBullet(bullet_y, bullet_exists, bulletClock);
            drawBullet(window, bullet_x, bullet_y, bulletSprite);
        }

        window.draw(groundRectangle);
        window.draw(scoretext);
        
        drawPlayer(window, player_x, player_y, playerSprite);
        drawbird(window, birdSprite);
         window.draw(bullettext);
        drawRegularbee(window, RegularbeeSprite, size, bee_active, RegularbeeClock, Regularbee_direction);
        // drawFastbee(window, Fast_beeSprite, size);

        for (int i = 0; i < size; i++) {
            if (ishoneycombvisible[i]) {
                honeycombSprite[i].setPosition(honeycombposition[i]);
                window.draw(honeycombSprite[i]);  
            }
        }

        for (int i = 0; i < Fast_size; i++) {
            if (ishoneycomb_redvisible[i]) {
                honeycomb_redSprite[i].setPosition(honeycomb_redposition[i]);
                // window.draw(honeycomb_redSprite[i]);      
            }
        }

        for (int i = 0; i < size; i++) {
            if (i == 0) {
                if (isobstaclesvisible[i] && isobstaclesvisible[i + 1]) {
                    window.draw(obstaclesSprite[i]);
                    window.draw(obstaclesSprite[i + 1]);
                }
            } else {
                if (isobstaclesvisible[i + 1]) {
                    window.draw(obstaclesSprite[i + 1]);
                }
            }
        }
        for(int i=0;i<3;i++){
        if(i==0){
        honeycombSprite[i].setPosition(340,134);
        window.draw(honeycombSprite[i]);
        }
        if(i==1){
        honeycombSprite[i].setPosition(284,334);
        window.draw(honeycombSprite[i]);}
        if(i==2){
        honeycombSprite[i].setPosition(390,234);
        window.draw(honeycombSprite[i]);}
        
}
        window.display();
    }}
    void start_level2(RenderWindow& window){
    RenderWindow localwindow(VideoMode(resolutionX, resolutionY), "Buzz Bombers", Style::Close | Style::Titlebar);
    window.setPosition(Vector2i(700, 400));
 sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("image2.png")) {
        return ;
    }
Music bgMusic;
	if (!bgMusic.openFromFile("music2.ogg")) {
              cout << "Error: Could not load music file!" << endl;
               return ;
	   }
	   
	bgMusic.setVolume(50);
	bgMusic.setLoop(true);
	bgMusic.play();

    // Create  background sprite
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
   

    // set up bird texture
    Texture birdTexture;
    if (!birdTexture.loadFromFile("Textures/bird.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }

    Sprite birdSprite;
    birdSprite.setTexture(birdTexture);

    float birdMovement_x = rand() % (resolutionX - boxPixelsX);
    float birdMovement_y = rand() % (resolutionY - boxPixelsY);
    
    float bird_moveSpeed = 2.0f;
    int moveDirection = 5;
    
    sf::Vector2f birdDirection(0, 0); // Direction of movement
    sf::Clock birdClock; // For timing pauses
    int bird_hit = 0;
    bool isbirdPaused = false;
    
    IntRect birdTextureRect = birdSprite.getTextureRect();
    float bird_width = static_cast<float>(birdTextureRect.width);
    float bird_height = static_cast<float>(birdTextureRect.height);
    
    // Initializing Player and Player Sprites.
    float player_x = (gameRows / 2) * boxPixelsX;
    float player_y = (gameColumns - 4) * boxPixelsY;
    
    // Set up player texture
    Texture playerTexture;
    Sprite playerSprite;
    if (!playerTexture.loadFromFile("Textures/spray.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    playerSprite.setTexture(playerTexture);
    playerSprite.setScale(1.f, 1.f);    
    float bullet_x = player_x;
    float bullet_y = player_y;
    bool bullet_exists = false;

    Clock bulletClock;
    Texture bulletTexture;
    Sprite bulletSprite;
    bulletTexture.loadFromFile("Textures/bullet.png");
    
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setScale(1.f, 1.f);
    bulletSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    // The ground on which player moves
    RectangleShape groundRectangle(Vector2f(960, 64));
    groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);
    groundRectangle.setFillColor(Color::Green);
    
    Font font;
    if (!font.loadFromFile("Textures/GAMEDAY.ttf")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    Text scoretext;
    scoretext.setFont(font);
    scoretext.setFillColor(Color::Black);
    scoretext.setCharacterSize(35);
    scoretext.setStyle(Text::Bold);
    FloatRect textBounds = scoretext.getLocalBounds();     // Get text bounds
    
    Vector2f rectPos = groundRectangle.getPosition();       // Rectangle position
    Vector2f rectSize = groundRectangle.getSize();          // Rectangle size

    scoretext.setPosition(
        rectPos.x + 20,
        rectPos.y + 10 
    );
    int score = 0;
    scoretext.setString("Score: " + std::to_string(score));
    int max_bullet=56;
    Text bullettext;
      bullettext.setFont(font);
      bullettext.setFillColor(Color::Black);
      bullettext.setCharacterSize(35);
      int can=0;
      bullettext.setStyle(Text::Bold);
      textBounds = bullettext.getLocalBounds();     // Get text bounds
      
      
      bullettext.setPosition(
           rectPos.x +760 ,
           rectPos.y +10 
      );
     int bullet_count=0;
     
     
    float moveSpeed = 5.f;
    
    //<----------------------BEE--------------------------------->
    Texture RegularbeeTexture;
    if (!RegularbeeTexture.loadFromFile("Textures/Regularbee.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    
    const int size = 15;
    Vector2f OriginalDirection[size];
    Sprite RegularbeeSprite[size];
    sf::Vector2f RegularbeeDirection[size]; // Direction of movement
    sf::Clock RegularbeeClock[size]; // For timing pauses
    
    float RegularbeeMovement_x = rand() % (resolutionX - boxPixelsX);
    float RegularbeeMovement_y = rand() % (resolutionY - boxPixelsY);
    
    for (int i = 0; i < size; i++) {
        RegularbeeSprite[i].setTexture(RegularbeeTexture);
        RegularbeeSprite[i].setScale(-1.f, 1.f);
        RegularbeeDirection[i] = Vector2f(0, 0);
    }
    
    int Regularbee_direction[size];
    bool isRegularbeePaused = true;
    bool bee_active[size];
    
    for (int i = 0; i < size; i++) {
        IntRect RegularbeeTextureRect = RegularbeeSprite[i].getTextureRect();
        float Regularbee_width = static_cast<float>(RegularbeeTextureRect.width);
        float Regularbee_height = static_cast<float>(RegularbeeTextureRect.height);
        
        Regularbee_direction[i] = 0;
        bee_active[i] = false;
    }
    
    int Regularbee_moveSpeed = 5.f;
    
    //<--------------------------HONEYCOMB----------------------------->
    Texture honeycombTexture;
    if (!honeycombTexture.loadFromFile("Textures/honeycomb.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    
    Sprite honeycombSprite[size];
    bool ishoneycombvisible[size] = {false};
    
    for (int i = 0; i < size; i++) {
        honeycombSprite[i].setTexture(honeycombTexture);
        ishoneycombvisible[i] = false;
    }
    
    Vector2f honeycombposition[size];
    
    //<-------------------------------REDBEE------------------------------->
    Texture Fast_beeTexture;
    if (!Fast_beeTexture.loadFromFile("Textures/Fast_bee.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    const int  fastsize=5;
    Vector2f OriginalfastDirection[fastsize];
    Sprite Fast_beeSprite[fastsize];
    sf::Vector2f Fast_beeDirection[fastsize]; // Direction of movement
    sf::Clock Fast_beeClock[fastsize]; // For timing pauses
    
    float Fast_beeMovement_x = rand() % (resolutionX - boxPixelsX);
    float Fast_beeMovement_y = rand() % (resolutionY - boxPixelsY);
    
    for (int i = 0; i < fastsize; i++) {
        Fast_beeSprite[i].setTexture(Fast_beeTexture);
        Fast_beeSprite[i].setScale(-1.f, 1.f);
        Fast_beeDirection[i] = Vector2f(0, 0);
    }
    
    bool isFast_beePaused = true;
    
    for (int i = 0; i < fastsize; i++) {
        IntRect Fast_beeTextureRect = Fast_beeSprite[i].getTextureRect();
        float Fast_bee_width = static_cast<float>(Fast_beeTextureRect.width);
        float Fast_bee_height = static_cast<float>(Fast_beeTextureRect.height);
    }
    int Fast_bee_moveSpeed = 6.f;
    
    //<------------------------------------------------------------------->//
    //<--------------------------REDHONEYCOMB----------------------------->
    Texture honeycomb_redTexture;
    if (!honeycomb_redTexture.loadFromFile("Textures/honeycomb_red.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    Sprite honeycomb_redSprite[size];
    
    for (int i = 0; i < size; i++) {
        honeycomb_redSprite[i].setTexture(honeycomb_redTexture);
    }
    
    bool ishoneycomb_redvisible[size] = {false};
    Vector2f honeycomb_redposition[size];
    
    //<---------------------------OBSTACLES--------------------------------->
    
    Texture obstaclesTexture;
    if (!obstaclesTexture.loadFromFile("Textures/obstacles.png")) {
        cout << "Error: Could not load obstacles texture!" << endl;
        return ;
    }
    
    Sprite obstaclesSprite[size];
    bool isobstaclesvisible[size] = {false};
    
    for (int i = 0; i < size; i++) {
        obstaclesSprite[i].setTexture(obstaclesTexture);
        isobstaclesvisible[i] = {false};
        obstaclesSprite[i].setScale(2.f, 2.f);
    }
    
    int flower = 0;       
    Vector2f obstaclesposition[size];
    //<---------------------------MAINLOOP--------------------------------->
    
    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
            }
        }

        if (sessionOver) {
            Text sessionOverText;
            sessionOverText.setFont(font); // Assuming 'font' is already loaded
            sessionOverText.setFillColor(Color::Red);
            sessionOverText.setCharacterSize(50);
            sessionOverText.setStyle(Text::Bold);
            sessionOverText.setString("Session Over");

            FloatRect textBounds = sessionOverText.getLocalBounds();
            sessionOverText.setPosition(
                (resolutionX - textBounds.width) / 2,
                (resolutionY - textBounds.height) / 2
            );

            window.clear(Color::Black);
            window.draw(sessionOverText);
            window.display();

            // Pause the game or wait for user input to close
            while (true) {
                Event e;
                while (window.pollEvent(e)) {
                    if (e.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
                        window.close();
                        
                    }
                }
            }
        }

        birdmovesprite(birdSprite, birdDirection, bird_moveSpeed, birdClock, isbirdPaused);
        
        if (bullet_exists) {
            if (bird_collision(window, bulletSprite, birdSprite, bird_hit)) {
                bird_hit++;
                bulletSprite.setPosition(-1000, -1000);
                bullet_exists = false;
    
                if (bird_hit == 3) {
                    bird_hit = 0;
                    birdMovement_x -= 100;
         
                    birdDirection = Vector2f(birdMovement_x, birdMovement_y);
        
                    if (birdClock.getElapsedTime().asSeconds() > 3) {
                        birdSprite.setPosition(0, 0);
                    }
                }
            }
        }
        
        //<----------------------BEE--------------------------------->
        if (bullet_exists) {
            for (int i = 0; i < size; i++) {
                if (bee_active[i]) {
                    if (collision(window, bulletSprite, RegularbeeSprite, honeycombSprite, i, ishoneycombvisible)) {
    honeycombposition[i].x = RegularbeeSprite[i].getPosition().x;
    honeycombposition[i].y = RegularbeeSprite[i].getPosition().y;

    RegularbeeSprite[i].setPosition(-1000, -1000);
    ishoneycombvisible[i] = true;
    bee_active[i] = false;
    beesHandled++;

   // score += 2000; // Add points for honeycomb creation
    scoretext.setString("Score: " + std::to_string(score)); // Update score display

    if (beesHandled >= 20) {
        sessionOver = true;
    }
}

                }
            }
        }

        for (int i = 0; i < size; i++) {
            if (bee_active[i]) {
                for (int j = 0; j < size; j++) {
                    if (ishoneycombvisible[j]) {  
                        if (honey_bee_collision(window, RegularbeeSprite, honeycombSprite, size)) {
                            RegularbeeSprite[i].setScale(-1.f, 1.f);
                            float y = RegularbeeSprite[i].getPosition().y;
                            y += 50;
                            RegularbeeSprite[i].setPosition(RegularbeeSprite[i].getPosition().x, y);
                            bee_active[i] = false;
                        }
                    }
                }
            }
        }

        for (int i = 0; i < size; i++) {
            if (ishoneycombvisible[i]) {
                if (honey_bullet_collision(window, bulletSprite, honeycombSprite, i, ishoneycombvisible)) {
    honeycombSprite[i].setPosition(-100, -100);
    ishoneycombvisible[i] = false;

    score += 1000; // Add points for honeycomb destruction by birds
    scoretext.setString("Score: " + std::to_string(score)); // Update score display
}

            }
        }

        beemovesprite(window, RegularbeeSprite, RegularbeeDirection, Regularbee_moveSpeed, RegularbeeClock, Regularbee_direction, isRegularbeePaused, size, bee_active, obstaclesSprite, isobstaclesvisible, flower);
        fastbeemovesprite(Fast_beeSprite, Fast_beeDirection, Fast_bee_moveSpeed, Fast_beeClock, isFast_beePaused, fastsize);
    
        if (bullet_exists) {
            for (int i = 0; i < fastsize; i++) {
                if (Fastbee_collision(window, bulletSprite, Fast_beeSprite, honeycomb_redSprite, i)) {
                    honeycomb_redposition[i].x = Fast_beeSprite[i].getPosition().x;
                    honeycomb_redposition[i].y = Fast_beeSprite[i].getPosition().y;
    
                    Fast_beeSprite[i].setPosition(-100, -100);
                    ishoneycomb_redvisible[i] = true;
                }
            }
        }
    
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            player_x += moveSpeed;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            player_x -= moveSpeed;
        }

        if (player_x < 0) {
            player_x = 0;  // Prevent left off-screen
        }
        if (player_x > resolutionX - boxPixelsX) {
            player_x = resolutionX - boxPixelsX;  // Prevent right off-screen
        }
        if (!bullet_exists && Keyboard::isKeyPressed(Keyboard::Space)) {
            bullet_x = player_x + (boxPixelsX / 2) - 10;  // Adjust bullet spawn position
            bullet_y = player_y;
            bullet_exists = true;
            if(can<3){
           
            bullet_count++;
            if(bullet_count>=57){
            bullet_count=0;
            can++;}
            }
            bullettext.setString("Bullets Left: " + std::to_string(bullet_count));
        }

        window.clear();
        window.draw(backgroundSprite);
        if (bullet_exists) {
            moveBullet(bullet_y, bullet_exists, bulletClock);
            drawBullet(window, bullet_x, bullet_y, bulletSprite);
        }

        window.draw(groundRectangle);
        window.draw(scoretext);
        
        drawPlayer(window, player_x, player_y, playerSprite);
        drawbird(window, birdSprite);
         window.draw(bullettext);
        drawRegularbee(window, RegularbeeSprite, size, bee_active, RegularbeeClock, Regularbee_direction);
        drawFastbee(window, Fast_beeSprite, fastsize);

        for (int i = 0; i < size; i++) {
            if (ishoneycombvisible[i]) {
                honeycombSprite[i].setPosition(honeycombposition[i]);
                window.draw(honeycombSprite[i]);  
            }
        }

        for (int i = 0; i < fastsize; i++) {
            if (ishoneycomb_redvisible[i]) {
                honeycomb_redSprite[i].setPosition(honeycomb_redposition[i]);
                // window.draw(honeycomb_redSprite[i]);      
            }
        }

        for (int i = 0; i < size; i++) {
            if (i == 0) {
                if (isobstaclesvisible[i] && isobstaclesvisible[i + 1]) {
                    window.draw(obstaclesSprite[i]);
                    window.draw(obstaclesSprite[i + 1]);
                }
            } else {
                if (isobstaclesvisible[i + 1]) {
                    window.draw(obstaclesSprite[i + 1]);
                }
            }
        }
        for(int i=0;i<9;i++){
        
        if(i==0){
        honeycombSprite[i].setPosition(340,134);
        window.draw(honeycombSprite[i]);
        }
        if(i==1){
        honeycombSprite[i].setPosition(284,334);
        window.draw(honeycombSprite[i]);}
        if(i==2){
        honeycombSprite[i].setPosition(390,234);
        window.draw(honeycombSprite[i]);}
        if(i==3){
        honeycombSprite[i].setPosition(400,234);
        window.draw(honeycombSprite[i]);}
        if(i==4){
        honeycombSprite[i].setPosition(390,264);
        window.draw(honeycombSprite[i]);}
        if(i==5){
        honeycombSprite[i].setPosition(390,534);
        window.draw(honeycombSprite[i]);}
        if(i==6){
        honeycombSprite[i].setPosition(490,434);
        window.draw(honeycombSprite[i]);}
        if(i==7){
        honeycombSprite[i].setPosition(320,634);
        window.draw(honeycombSprite[i]);}
        if(i==8){
        honeycombSprite[i].setPosition(190,434);
        window.draw(honeycombSprite[i]);}
        
}
        window.display();
    }}
void start_level3(RenderWindow& window){
    RenderWindow localwindow(VideoMode(resolutionX, resolutionY), "Buzz Bombers", Style::Close | Style::Titlebar);
    window.setPosition(Vector2i(700, 400));
 sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("image3.png")) {
        return ;
    }
Music bgMusic;
	if (!bgMusic.openFromFile("Music3.ogg")) {
              cout << "Error: Could not load music file!" << endl;
               return ;
	   }
	   
	bgMusic.setVolume(50);
	bgMusic.setLoop(true);
	bgMusic.play();
        
    // Create  background sprite
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
   

    // set up bird texture
    Texture birdTexture;
    if (!birdTexture.loadFromFile("Textures/bird.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }

    Sprite birdSprite;
    birdSprite.setTexture(birdTexture);

    float birdMovement_x = rand() % (resolutionX - boxPixelsX);
    float birdMovement_y = rand() % (resolutionY - boxPixelsY);
    
    float bird_moveSpeed = 2.0f;
    int moveDirection = 5;
    
    sf::Vector2f birdDirection(0, 0); // Direction of movement
    sf::Clock birdClock; // For timing pauses
    int bird_hit = 0;
    bool isbirdPaused = false;
    
    IntRect birdTextureRect = birdSprite.getTextureRect();
    float bird_width = static_cast<float>(birdTextureRect.width);
    float bird_height = static_cast<float>(birdTextureRect.height);
    
    // Initializing Player and Player Sprites.
    float player_x = (gameRows / 2) * boxPixelsX;
    float player_y = (gameColumns - 4) * boxPixelsY;
    
    // Set up player texture
    Texture playerTexture;
    Sprite playerSprite;
    if (!playerTexture.loadFromFile("Textures/spray.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    playerSprite.setTexture(playerTexture);
    playerSprite.setScale(1.f, 1.f);    
    float bullet_x = player_x;
    float bullet_y = player_y;
    bool bullet_exists = false;

    Clock bulletClock;
    Texture bulletTexture;
    Sprite bulletSprite;
    bulletTexture.loadFromFile("Textures/bullet.png");
    
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setScale(1.f, 1.f);
    bulletSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    // The ground on which player moves
    RectangleShape groundRectangle(Vector2f(960, 64));
    groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);
    groundRectangle.setFillColor(Color::Green);
    
    Font font;
    if (!font.loadFromFile("Textures/GAMEDAY.ttf")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    Text scoretext;
    scoretext.setFont(font);
    scoretext.setFillColor(Color::Black);
    scoretext.setCharacterSize(35);
    scoretext.setStyle(Text::Bold);
    FloatRect textBounds = scoretext.getLocalBounds();     // Get text bounds
    
    Vector2f rectPos = groundRectangle.getPosition();       // Rectangle position
    Vector2f rectSize = groundRectangle.getSize();          // Rectangle size

    scoretext.setPosition(
        rectPos.x + 20,
        rectPos.y + 10 
    );
    int score = 0;
    scoretext.setString("Score: " + std::to_string(score));
    int max_bullet=56;
    Text bullettext;
      bullettext.setFont(font);
      bullettext.setFillColor(Color::Black);
      bullettext.setCharacterSize(35);
      int can=0;
      bullettext.setStyle(Text::Bold);
      textBounds = bullettext.getLocalBounds();     // Get text bounds
      
      
      bullettext.setPosition(
           rectPos.x +760 ,
           rectPos.y +10 
      );
     int bullet_count=0;
     
     
    float moveSpeed = 5.f;
    
    //<----------------------BEE--------------------------------->
    Texture RegularbeeTexture;
    if (!RegularbeeTexture.loadFromFile("Textures/Regularbee.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    
    const int size = 15;
    Vector2f OriginalDirection[size];
    Sprite RegularbeeSprite[size];
    sf::Vector2f RegularbeeDirection[size]; // Direction of movement
    sf::Clock RegularbeeClock[size]; // For timing pauses
    
    float RegularbeeMovement_x = rand() % (resolutionX - boxPixelsX);
    float RegularbeeMovement_y = rand() % (resolutionY - boxPixelsY);
    
    for (int i = 0; i < size; i++) {
        RegularbeeSprite[i].setTexture(RegularbeeTexture);
        RegularbeeSprite[i].setScale(-1.f, 1.f);
        RegularbeeDirection[i] = Vector2f(0, 0);
    }
    
    int Regularbee_direction[size];
    bool isRegularbeePaused = true;
    bool bee_active[size];
    
    for (int i = 0; i < size; i++) {
        IntRect RegularbeeTextureRect = RegularbeeSprite[i].getTextureRect();
        float Regularbee_width = static_cast<float>(RegularbeeTextureRect.width);
        float Regularbee_height = static_cast<float>(RegularbeeTextureRect.height);
        
        Regularbee_direction[i] = 0;
        bee_active[i] = false;
    }
    
    int Regularbee_moveSpeed = 5.f;
    
    //<--------------------------HONEYCOMB----------------------------->
    Texture honeycombTexture;
    if (!honeycombTexture.loadFromFile("Textures/honeycomb.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    
    Sprite honeycombSprite[size];
    bool ishoneycombvisible[size] = {false};
    
    for (int i = 0; i < size; i++) {
        honeycombSprite[i].setTexture(honeycombTexture);
        ishoneycombvisible[i] = false;
    }
    
    Vector2f honeycombposition[size];
    
    //<-------------------------------REDBEE------------------------------->
    Texture Fast_beeTexture;
    if (!Fast_beeTexture.loadFromFile("Textures/Fast_bee.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    const int  fastsize=10;
    Vector2f OriginalfastDirection[fastsize];
    Sprite Fast_beeSprite[fastsize];
    sf::Vector2f Fast_beeDirection[fastsize]; // Direction of movement
    sf::Clock Fast_beeClock[fastsize]; // For timing pauses
    
    float Fast_beeMovement_x = rand() % (resolutionX - boxPixelsX);
    float Fast_beeMovement_y = rand() % (resolutionY - boxPixelsY);
    
    for (int i = 0; i < fastsize; i++) {
        Fast_beeSprite[i].setTexture(Fast_beeTexture);
        Fast_beeSprite[i].setScale(-1.f, 1.f);
        Fast_beeDirection[i] = Vector2f(0, 0);
    }
    
    bool isFast_beePaused = true;
    
    for (int i = 0; i < fastsize; i++) {
        IntRect Fast_beeTextureRect = Fast_beeSprite[i].getTextureRect();
        float Fast_bee_width = static_cast<float>(Fast_beeTextureRect.width);
        float Fast_bee_height = static_cast<float>(Fast_beeTextureRect.height);
    }
    int Fast_bee_moveSpeed = 6.f;
    
    //<------------------------------------------------------------------->//
    //<--------------------------REDHONEYCOMB----------------------------->
    Texture honeycomb_redTexture;
    if (!honeycomb_redTexture.loadFromFile("Textures/honeycomb_red.png")) {
        cout << "Error: Could not load player texture!" << endl;
        return ;
    }
    Sprite honeycomb_redSprite[size];
    
    for (int i = 0; i < size; i++) {
        honeycomb_redSprite[i].setTexture(honeycomb_redTexture);
    }
    
    bool ishoneycomb_redvisible[size] = {false};
    Vector2f honeycomb_redposition[size];
    
    //<---------------------------OBSTACLES--------------------------------->
    
    Texture obstaclesTexture;
    if (!obstaclesTexture.loadFromFile("Textures/obstacles.png")) {
        cout << "Error: Could not load obstacles texture!" << endl;
        return ;
    }
    
    Sprite obstaclesSprite[size];
    bool isobstaclesvisible[size] = {false};
    
    for (int i = 0; i < size; i++) {
        obstaclesSprite[i].setTexture(obstaclesTexture);
        isobstaclesvisible[i] = {false};
        obstaclesSprite[i].setScale(2.f, 2.f);
    }
    
    int flower = 0;       
    Vector2f obstaclesposition[size];
    //<---------------------------MAINLOOP--------------------------------->
    
    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
            }
        }

        if (sessionOver) {
            Text sessionOverText;
            sessionOverText.setFont(font); // Assuming 'font' is already loaded
            sessionOverText.setFillColor(Color::Red);
            sessionOverText.setCharacterSize(50);
            sessionOverText.setStyle(Text::Bold);
            sessionOverText.setString("Session Over");

            FloatRect textBounds = sessionOverText.getLocalBounds();
            sessionOverText.setPosition(
                (resolutionX - textBounds.width) / 2,
                (resolutionY - textBounds.height) / 2
            );

            window.clear(Color::Black);
            window.draw(sessionOverText);
            window.display();

            // Pause the game or wait for user input to close
            while (true) {
                Event e;
                while (window.pollEvent(e)) {
                    if (e.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
                        window.close();
                        
                    }
                }
            }
        }

        birdmovesprite(birdSprite, birdDirection, bird_moveSpeed, birdClock, isbirdPaused);
        
        if (bullet_exists) {
            if (bird_collision(window, bulletSprite, birdSprite, bird_hit)) {
                bird_hit++;
                bulletSprite.setPosition(-1000, -1000);
                bullet_exists = false;
    
                if (bird_hit == 3) {
                    bird_hit = 0;
                    birdMovement_x -= 100;
         
                    birdDirection = Vector2f(birdMovement_x, birdMovement_y);
        
                    if (birdClock.getElapsedTime().asSeconds() > 3) {
                        birdSprite.setPosition(0, 0);
                    }
                }
            }
        }
        
        //<----------------------BEE--------------------------------->
        if (bullet_exists) {
            for (int i = 0; i < size; i++) {
                if (bee_active[i]) {
                    if (collision(window, bulletSprite, RegularbeeSprite, honeycombSprite, i, ishoneycombvisible)) {
    honeycombposition[i].x = RegularbeeSprite[i].getPosition().x;
    honeycombposition[i].y = RegularbeeSprite[i].getPosition().y;

    RegularbeeSprite[i].setPosition(-1000, -1000);
    ishoneycombvisible[i] = true;
    bee_active[i] = false;
    beesHandled++;

   // score += 2000; // Add points for honeycomb creation
    scoretext.setString("Score: " + std::to_string(score)); // Update score display

    if (beesHandled >= 20) {
        sessionOver = true;
    }
}

                }
            }
        }

        for (int i = 0; i < size; i++) {
            if (bee_active[i]) {
                for (int j = 0; j < size; j++) {
                    if (ishoneycombvisible[j]) {  
                        if (honey_bee_collision(window, RegularbeeSprite, honeycombSprite, size)) {
                            RegularbeeSprite[i].setScale(-1.f, 1.f);
                            float y = RegularbeeSprite[i].getPosition().y;
                            y += 50;
                            RegularbeeSprite[i].setPosition(RegularbeeSprite[i].getPosition().x, y);
                            bee_active[i] = false;
                        }
                    }
                }
            }
        }

        for (int i = 0; i < size; i++) {
            if (ishoneycombvisible[i]) {
                if (honey_bullet_collision(window, bulletSprite, honeycombSprite, i, ishoneycombvisible)) {
    honeycombSprite[i].setPosition(-100, -100);
    ishoneycombvisible[i] = false;

    score += 1000; // Add points for honeycomb destruction by birds
    scoretext.setString("Score: " + std::to_string(score)); // Update score display
}

            }
        }

        beemovesprite(window, RegularbeeSprite, RegularbeeDirection, Regularbee_moveSpeed, RegularbeeClock, Regularbee_direction, isRegularbeePaused, size, bee_active, obstaclesSprite, isobstaclesvisible, flower);
        fastbeemovesprite(Fast_beeSprite, Fast_beeDirection, Fast_bee_moveSpeed, Fast_beeClock, isFast_beePaused, fastsize);
    
        if (bullet_exists) {
            for (int i = 0; i < fastsize; i++) {
                if (Fastbee_collision(window, bulletSprite, Fast_beeSprite, honeycomb_redSprite, i)) {
                    honeycomb_redposition[i].x = Fast_beeSprite[i].getPosition().x;
                    honeycomb_redposition[i].y = Fast_beeSprite[i].getPosition().y;
    
                    Fast_beeSprite[i].setPosition(-100, -100);
                    ishoneycomb_redvisible[i] = true;
                }
            }
        }
    
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            player_x += moveSpeed;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            player_x -= moveSpeed;
        }

        if (player_x < 0) {
            player_x = 0;  // Prevent left off-screen
        }
        if (player_x > resolutionX - boxPixelsX) {
            player_x = resolutionX - boxPixelsX;  // Prevent right off-screen
        }
        if (!bullet_exists && Keyboard::isKeyPressed(Keyboard::Space)) {
            bullet_x = player_x + (boxPixelsX / 2) - 10;  // Adjust bullet spawn position
            bullet_y = player_y;
            bullet_exists = true;
            if(can<3){
           
            bullet_count++;
            if(bullet_count>=57){
            bullet_count=0;
            can++;}
            }
            bullettext.setString("Bullets Left: " + std::to_string(bullet_count));
        }

        window.clear();
        window.draw(backgroundSprite);
        if (bullet_exists) {
            moveBullet(bullet_y, bullet_exists, bulletClock);
            drawBullet(window, bullet_x, bullet_y, bulletSprite);
        }

        window.draw(groundRectangle);
        window.draw(scoretext);
        
        drawPlayer(window, player_x, player_y, playerSprite);
        drawbird(window, birdSprite);
         window.draw(bullettext);
        drawRegularbee(window, RegularbeeSprite, size, bee_active, RegularbeeClock, Regularbee_direction);
        drawFastbee(window, Fast_beeSprite, fastsize);

        for (int i = 0; i < size; i++) {
            if (ishoneycombvisible[i]) {
                honeycombSprite[i].setPosition(honeycombposition[i]);
                window.draw(honeycombSprite[i]);  
            }
        }

        for (int i = 0; i < fastsize; i++) {
            if (ishoneycomb_redvisible[i]) {
                honeycomb_redSprite[i].setPosition(honeycomb_redposition[i]);
                // window.draw(honeycomb_redSprite[i]);      
            }
        }

        for (int i = 0; i < size; i++) {
            if (i == 0) {
                if (isobstaclesvisible[i] && isobstaclesvisible[i + 1]) {
                    window.draw(obstaclesSprite[i]);
                    window.draw(obstaclesSprite[i + 1]);
                }
            } else {
                if (isobstaclesvisible[i + 1]) {
                    window.draw(obstaclesSprite[i + 1]);
                }
            }
        }
        for(int i=0;i<9;i++){
        
        if(i==0){
        honeycombSprite[i].setPosition(340,134);
        window.draw(honeycombSprite[i]);
        }
        if(i==1){
        honeycombSprite[i].setPosition(284,334);
        window.draw(honeycombSprite[i]);}
        if(i==2){
        honeycombSprite[i].setPosition(390,234);
        window.draw(honeycombSprite[i]);}
        if(i==3){
        honeycombSprite[i].setPosition(400,234);
        window.draw(honeycombSprite[i]);}
        if(i==4){
        honeycombSprite[i].setPosition(390,264);
        window.draw(honeycombSprite[i]);}
        if(i==5){
        honeycombSprite[i].setPosition(390,534);
        window.draw(honeycombSprite[i]);}
        if(i==6){
        honeycombSprite[i].setPosition(490,434);
        window.draw(honeycombSprite[i]);}
        if(i==7){
        honeycombSprite[i].setPosition(320,634);
        window.draw(honeycombSprite[i]);}
        if(i==8){
        honeycombSprite[i].setPosition(190,434);
        window.draw(honeycombSprite[i]);}
        
}
        window.display();
    }}

void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite) {
    playerSprite.setPosition(player_x, player_y);
    window.draw(playerSprite);
}
void drawbird(RenderWindow& window, Sprite& birdSprite) {
    window.draw(birdSprite);
}
void drawRegularbee(RenderWindow& window, Sprite RegularbeeSprite[], int size, bool bee_active[], Clock RegularbeeClock[], int Regularbee_direction[]) {
    for (int i = 0; i < size; i++) {
        IntRect textureRect = RegularbeeSprite[i].getTextureRect();
        float Regularbee_width = static_cast<float>(textureRect.width);
        float Regularbee_height = static_cast<float>(textureRect.height);
    
        if (!bee_active[i]) {
            if (RegularbeeClock[i].getElapsedTime().asSeconds() >= 3 * i) {
                bee_active[i] = true;
                int choice = rand() % 2;
                
                switch (choice) {
                    case 0: {  
                        RegularbeeSprite[i].setPosition(0, 0); 
                        Regularbee_direction[i] = 1;
                        RegularbeeSprite[i].setScale(-1.f, 1.f);
                        break;
                    }
                    case 1: { 
                        RegularbeeSprite[i].setPosition(window.getSize().x - Regularbee_width, 0);
                        Regularbee_direction[i] = -1;
                        RegularbeeSprite[i].setScale(1.f, 1.f);
                        break;
                    }
                }
            }
        }
        window.draw(RegularbeeSprite[i]);
    }
}
void drawFastbee(RenderWindow& window, Sprite Fast_beeSprite[], int fastsize) {
    for (int i = 0; i < fastsize; i++) {
        window.draw(Fast_beeSprite[i]);
    }
}
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock) {
    if (bulletClock.getElapsedTime().asMilliseconds() < 20)
        return;
    bulletClock.restart();
    bullet_y -= 35;

    if (bullet_y < -32)
        bullet_exists = false;
}

void drawBullet(sf::RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite) {
    bulletSprite.setPosition(bullet_x, bullet_y);
    window.draw(bulletSprite);
}
void beemovesprite(RenderWindow& window, Sprite RegularbeeSprite[], Vector2f RegularbeeDirection[], float Regularbee_moveSpeed, Clock RegularbeeClock[], int Regularbee_direction[], bool& isRegularbeePaused, int size, bool bee_active[], Sprite obstaclesSprite[], bool isobstaclesvisible[], int flower) {
    for (int i = 0; i < size; i++) {
      
        IntRect textureRect = RegularbeeSprite[i].getTextureRect();
        float Regularbee_width = static_cast<float>(textureRect.width);
        float Regularbee_height = static_cast<float>(textureRect.height);
    
        if (!bee_active[i]) {
            continue;
        }
        
        if (bee_active[i]) {
            float x = RegularbeeSprite[i].getPosition().x;
            float y = RegularbeeSprite[i].getPosition().y;

            x += Regularbee_direction[i] * Regularbee_moveSpeed;
        
            if (x >= window.getSize().x - Regularbee_width) {
                x = window.getSize().x - Regularbee_width;
                y += 50;
                Regularbee_direction[i] = -1;
                RegularbeeSprite[i].setScale(1.f, 1.f);
            }
            else if (x <= 0) {
                x = 0;
                y += 50;
                Regularbee_direction[i] = 1;
                RegularbeeSprite[i].setScale(-1.f, 1.f);
            }
            if ((x <= 0 || x >= window.getSize().x - Regularbee_width) && y >= resolutionY - 120) {
                if (i == 0) {
                    obstaclesSprite[i].setPosition(0, resolutionY - 125);
                    obstaclesSprite[i + 1].setPosition(resolutionX - 30, resolutionY - 125);
                    isobstaclesvisible[i] = true;
                    isobstaclesvisible[i + 1] = true;
                }
                else if (i > 0 && i < 20) {
                    if (x <= 0) { 
                        obstaclesSprite[i + 1].setPosition(abs(30 * (i + 1)), resolutionY - 125);
                    }
                    else if (x >= window.getSize().x - Regularbee_width) {
                        obstaclesSprite[i + 1].setPosition(resolutionX - 30 * (i + 1), resolutionY - 125);
                    }
                    isobstaclesvisible[i + 1] = true;
                }
                RegularbeeSprite[i].setPosition(-1000, -1000);
                bee_active[i] = false;
                RegularbeeClock[i].restart();
            }
            else {
                RegularbeeSprite[i].setPosition(x, y);
            }
        }
    }
}
void birdmovesprite(Sprite& bird, Vector2f& birdDirection, float bird_moveSpeed, Clock& birdClock, bool& isbirdPaused) {
    IntRect textureRect = bird.getTextureRect();
    float bird_width = static_cast<float>(textureRect.width);
    float bird_height = static_cast<float>(textureRect.height);
        
    bird.move(birdDirection * bird_moveSpeed);

    // Handle screen boundaries
    if (bird.getPosition().x <= 0) {
        birdDirection.x = 1;
    }
    else if (bird.getPosition().x + bird_width >= resolutionX) {
        birdDirection.x = -1;
    }
    
    if (bird.getPosition().y <= 0) {
        birdDirection.y = 1; // Reverse vertical direction
    } 
    else if (bird.getPosition().y + bird_height >= resolutionY - 100) {
        birdDirection.y = -1;
    }
    
    if (birdClock.getElapsedTime().asSeconds() > 2) {
        if (isbirdPaused) {
            // End pause: Choose a new direction and start moving
            int random_choice = rand() % 7;
            switch (random_choice) {
                case 0: birdDirection = Vector2f(0, -1); break; // Move up
                case 1: birdDirection = Vector2f(0, 1); break;  // Move down
                case 2: birdDirection = Vector2f(1, 0); break;  // Move right
                case 3: birdDirection = Vector2f(1, 1); break;
                case 4: birdDirection = Vector2f(1, -1); break;
                case 5: birdDirection = Vector2f(-1, 1); break;
                case 6: birdDirection = Vector2f(-1, -1); break;
                default: birdDirection = Vector2f(0, 0); break; // Stay still
            }
            isbirdPaused = false; // Bird starts moving
        }
        else {
            // End movement: Pause the bird
            birdDirection = Vector2f(0, 0); // Stop movement
            isbirdPaused = true; // Enter pause state
        }
        birdClock.restart(); // Restart the clock for the next phase
    }
}

bool collision(RenderWindow& window, Sprite& bulletSprite, Sprite RegularbeeSprite[], Sprite honeycombSprite[], int i, bool ishoneycombvisible[]) {
    float x1 = RegularbeeSprite[i].getPosition().x;
    float y1 = RegularbeeSprite[i].getPosition().y;
    float width1 = RegularbeeSprite[i].getTextureRect().width;
    float height1 = RegularbeeSprite[i].getTextureRect().height;
    
    float x2 = bulletSprite.getPosition().x;
    float y2 = bulletSprite.getPosition().y;
    float width2 = bulletSprite.getTextureRect().width;
    float height2 = bulletSprite.getTextureRect().height;
    
    if (x1 < x2 + width2 && x1 + width1 > x2 && y1 < y2 + height2 && y1 + height1 > y2) {
        return true;
    }
    return false;
}
bool bird_collision(RenderWindow& window, Sprite& bulletSprite, Sprite& birdSprite, int bird_hit) {
    float x1 = birdSprite.getPosition().x;
    float y1 = birdSprite.getPosition().y;
    float width1 = birdSprite.getTextureRect().width;
    float height1 = birdSprite.getTextureRect().height;
    
    float x2 = bulletSprite.getPosition().x;
    float y2 = bulletSprite.getPosition().y;
    float width2 = bulletSprite.getTextureRect().width;
    float height2 = bulletSprite.getTextureRect().height;
    
    if (x1 < x2 + width2 && x1 + width1 > x2 && y1 < y2 + height2 && y1 + height1 > y2) {
        return true;
    }
    return false;
}
bool honey_bullet_collision(RenderWindow& window, Sprite& bulletSprite, Sprite honeycombSprite[], int i, bool ishoneycombvisible[]) {
    float x1 = honeycombSprite[i].getPosition().x;
    float y1 = honeycombSprite[i].getPosition().y;
    float width1 = honeycombSprite[i].getTextureRect().width;
    float height1 = honeycombSprite[i].getTextureRect().height;
    
    float x2 = bulletSprite.getPosition().x;
    float y2 = bulletSprite.getPosition().y;
    float width2 = bulletSprite.getTextureRect().width;
    float height2 = bulletSprite.getTextureRect().height;
    
    if (x1 < x2 + width2 && x1 + width1 > x2 && y1 < y2 + height2 && y1 + height1 > y2) {
        return true;
    }
    return false;
}
bool honey_bee_collision(RenderWindow& window, Sprite RegularbeeSprite[], Sprite honeycombSprite[], int size) {
    bool check_collision = false;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            float x1 = RegularbeeSprite[i].getPosition().x;
            float y1 = RegularbeeSprite[i].getPosition().y;
            float width1 = RegularbeeSprite[i].getTextureRect().width;
            float height1 = RegularbeeSprite[i].getTextureRect().height;
            
            float x2 = honeycombSprite[j].getPosition().x;
            float y2 = honeycombSprite[j].getPosition().y;
            float width2 = honeycombSprite[j].getTextureRect().width;
            float height2 = honeycombSprite[j].getTextureRect().height;
            
            if (x1 < x2 + width2 && x1 + width1 > x2 && y1 < y2 + height2 && y1 + height1 > y2) {
                RegularbeeSprite[i].setPosition(RegularbeeSprite[i].getPosition().x, RegularbeeSprite[i].getPosition().y + 50);
                check_collision = true;        
            }
        }
    }
    return check_collision;
}

void fastbeemovesprite(Sprite Fast_beeSprite[], Vector2f Fast_beeDirection[], float Fast_bee_moveSpeed, Clock Fast_beeClock[], bool& isFast_beePaused, int fastsize) {
    for (int i = 0; i < fastsize; i++) {
        IntRect textureRect = Fast_beeSprite[i].getTextureRect();
        float Fast_bee_width = static_cast<float>(textureRect.width);
        float Fast_bee_height = static_cast<float>(textureRect.height);
        
        // Move the bees smoothly
        Fast_beeSprite[i].move(Fast_beeDirection[i] * Fast_bee_moveSpeed);
        
        if (Fast_beeSprite[i].getPosition().x <= 0) {
            Fast_beeDirection[i].x = 1;
        }
        if (Fast_beeSprite[i].getPosition().x + Fast_bee_width >= resolutionX) {
            Fast_beeDirection[i].x = -1;
        }
        
        if (Fast_beeSprite[i].getPosition().y <= 0) {
            Fast_beeDirection[i].y = 1;
        }
        if (Fast_beeSprite[i].getPosition().y + Fast_bee_height >= resolutionY - 64) { 
            Fast_beeDirection[i].y = -1; // Reverse vertical direction
        }

        if (Fast_beeClock[i].getElapsedTime().asMilliseconds() > 400) {
            if (isFast_beePaused) {
                // End pause: Choose a new direction and start moving
                int random_choice = rand() % 8;
                switch (random_choice) {
                    case 0: Fast_beeDirection[i] = Vector2f(1, 1); break;
                    case 1: Fast_beeDirection[i] = Vector2f(1, -1); break;
                    case 2: Fast_beeDirection[i] = Vector2f(0, 1); break;  // Move down
                    case 3: Fast_beeDirection[i] = Vector2f(0, -1); break; // Move up                
                    case 4: Fast_beeDirection[i] = Vector2f(-1, 1); break;
                    case 5: Fast_beeDirection[i] = Vector2f(1, 0); break;  // Move right
                    case 6: Fast_beeDirection[i] = Vector2f(-1, -1); break;
                    default: Fast_beeDirection[i] = Vector2f(0, 0); break; // Stay still
                }
                isFast_beePaused = false; // Bird starts moving
            }
            else {
                // End movement: Pause the bird
                Fast_beeDirection[i] = Vector2f(0, 0); // Stop movement
                isFast_beePaused = true; // Enter pause state
            }
            Fast_beeClock[i].restart(); // Restart the clock for the next phase
        }
    }
}
bool Fastbee_collision(RenderWindow& window, Sprite& bulletSprite, Sprite Fast_beeSprite[], Sprite honeycomb_redSprite[], int i) {
    float x1 = Fast_beeSprite[i].getPosition().x;
    float y1 = Fast_beeSprite[i].getPosition().y;
    float width1 = Fast_beeSprite[i].getTextureRect().width;
    float height1 = Fast_beeSprite[i].getTextureRect().height;
    
    float x2 = bulletSprite.getPosition().x;
    float y2 = bulletSprite.getPosition().y;
    float width2 = bulletSprite.getTextureRect().width;
    float height2 = bulletSprite.getTextureRect().height;
    
    if (x1 < x2 + width2 && x1 + width1 > x2 && y1 < y2 + height2 && y1 + height1 > y2) {
        return true;
    }
    return false;
}
