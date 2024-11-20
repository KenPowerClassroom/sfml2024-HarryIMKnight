//Harry Knight
#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int MINE_CHANCE = 5;

//Refactor 3: added more comments
int minesweeper()
{
    srand(time(0)); // Random number generator

    RenderWindow app(VideoMode(400, 400), "Minesweeper!"); // Create the game window


    int w = 32; // Width of each tile
    int grid[12][12]; // Mine information: 0 for empty, 9 for mine
    int sgrid[12][12]; // Display: 10 for unrevealed, 11 for flagged

    Texture tilesTexture; // Refactor 1: I changed the varible name from t to tilesTexture to specificaly say what the varible holds
    tilesTexture.loadFromFile("images/minesweeper/tiles.jpg");  // Defines image for texture
    Sprite s(tilesTexture); // Defines sprite texture

    // Refactor 2: Tidied up
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            sgrid[i][j] = 10; // Set cell as unrevealed
            grid[i][j] = (rand() % MINE_CHANCE == 0) ? 9 : 0; // Assign mine with a 1 in MINE_CHANCE
        }
    }
    //

    // Refactor 2: Tidied up
    // Number of adjacent mines for each cell
    for (int i = 1; i <= 10; i++)
     for (int j = 1; j <= 10; j++)
      {
        int n = 0;
        if (grid[i][j] == 9) continue;
        if (grid[i + 1][j] == 9) n++;
        if (grid[i][j + 1] == 9) n++;
        if (grid[i - 1][j] == 9) n++;
        if (grid[i][j - 1] == 9) n++;
        if (grid[i + 1][j + 1] == 9) n++;
        if (grid[i - 1][j - 1] == 9) n++;
        if (grid[i - 1][j + 1] == 9) n++;
        if (grid[i + 1][j - 1] == 9) n++;
        grid[i][j] = n; // Set the number of touching mines in the grid
      }
    //

    while (app.isOpen())
    {
        // Refactor 2: Tidied up
        // Get mouse position to calculate the clicked grid cell
        Vector2i pos = Mouse::getPosition(app);
        int x = pos.x / w;
        int y = pos.y / w;
        //

        Event e;
        while (app.pollEvent(e))
        {
            // Refactor 2: Tidied up
            // Close the window if the close button is clicked
            if (e.type == Event::Closed) {
                app.close();
            }

            // Handle mouse button press events
            if (e.type == Event::MouseButtonPressed) {
                if (e.key.code == Mouse::Left) {
                    sgrid[x][y] = grid[x][y];
                }
                else if (e.key.code == Mouse::Right) {
                    sgrid[x][y] = 11;
                }
            //
            }
        }

        app.clear(Color::White); // Clear the screen before drawing new tiles

        // Refactor 2: Tidied up
        // Draw all tiles based on the state in sgrid
        for (int i = 1;i <= 10; i++)
         for (int j = 1; j <= 10; j++)
          {
           if (sgrid[x][y] == 9) sgrid[i][j] = grid[i][j]; // If a mine is revealed, show the entire grid
           s.setTextureRect(IntRect(sgrid[i][j] * w,0,w,w)); // Set the correct texture for the tile
           s.setPosition(i * w, j * w); // Set position of the tile
           app.draw(s);
          }
        //

        app.display(); // Display current frame
    }

    return 0;
}
