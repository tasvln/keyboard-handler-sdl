#include "main.h"

bool init()
{
  // Initialization flag
  bool success = true;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    success = false;
  }
  else
  {
    // Create window
    gWindow = SDL_CreateWindow("SDLKEYHANDLER", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (gWindow == NULL)
    {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      success = false;
    }
    else
    {
      // Get window surface
      gScreenSurface = SDL_GetWindowSurface(gWindow);
    }
  }

  return success;
}

SDL_Surface* loadSurface( std::string path ) {
  SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );

  if( loadedSurface == NULL )
  {
    printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
  }

  return loadedSurface;
}

bool loadMedia()
{
  bool loading = true;

  gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface( "lib/press.bmp" );

  if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL )
  {
    printf( "Failed to load default image!\n" );
    loading = false;
  }

  gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadSurface( "lib/up.bmp" );

  if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL )
  {
    printf( "Failed to load up image!\n" );
    loading = false;
  }

  gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadSurface( "lib/down.bmp" );

  if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL )
  {
    printf( "Failed to load down image!\n" );
    loading = false;
  }

  gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadSurface( "lib/left.bmp" );

  if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL )
  {
    printf( "Failed to load left image!\n" );
    loading = false;
  }

  gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadSurface( "lib/right.bmp" );

  if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL )
  {
    printf( "Failed to load right image!\n" );
    loading = false;
  }

  return loading;
}

void close()
{
	//Deallocate surfaces
	for( int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i )
	{
		SDL_FreeSurface( gKeyPressSurfaces[ i ] );
		gKeyPressSurfaces[ i ] = NULL;
	}

  // SDL_FreeSurface(gCurrentSurface);
  // gCurrentSurface = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char *args[])
{
  // Start up SDL and create window
  if (!init())
  {
    printf("Failed to initialize!\n");
  }
  else
  {
    // Load media
    if (!loadMedia())
    {
      printf("Failed to load media!\n");
    }
    else
    {
      // Hack to get window to stay up
      SDL_Event e;

      bool quit = false;

      gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

      while (!quit)
      {
        while (SDL_PollEvent(&e) != 0)
        {
          if (e.type == SDL_QUIT) {
            quit = true;
          } 
          else if (e.type == SDL_KEYDOWN) 
          {
            switch( e.key.keysym.sym )
            {
              case SDLK_UP:
              gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
              break;

              case SDLK_DOWN:
              gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
              break;

              case SDLK_LEFT:
              gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
              break;

              case SDLK_RIGHT:
              gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
              break;

              default:
              gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
              break;
            }
          }
        }
    
        // Apply the image
        SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

        // Update the surface
        SDL_UpdateWindowSurface(gWindow);
      }
    }
  }

  // Free resources and close SDL
  close();

  return 0;
}