/*+===================================================================
  File:      MAIN.CPP

  Summary:   This application demonstrates creating a Direct3D 11 
             device in a object-oriented fashion

  Origin:    https://docs.microsoft.com/en-us/previous-versions//ff729719(v=vs.85)

  Originally created by Microsoft Corporation under MIT License
  © 2022 Kyung Hee University
===================================================================+*/

#include "Common.h"

#include <memory>

#include "Game/Game.h"
#include "Cube/MainCube.h"
#include "Cube/RotateCube1.h"
#include "Cube//atomic.h"

/*F+F+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Function: wWinMain

  Summary:  Entry point to the program. Initializes everything and
            goes into a message processing loop. Idle time is used to
            render the scene.

  Args:     HINSTANCE hInstance
              Handle to an instance.
            HINSTANCE hPrevInstance
              Has no meaning.
            LPWSTR lpCmdLine
              Contains the command-line arguments as a Unicode
              string
            INT nCmdShow
              Flag that says whether the main application window
              will be minimized, maximized, or shown normally

  Returns:  INT
              Status code.
-----------------------------------------------------------------F-F*/
INT WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nCmdShow)
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    std::unique_ptr<library::Game> game = std::make_unique<library::Game>(L"Game Graphics Programming Lab 04: 3D Spaces and Transformations");

    std::shared_ptr<library::VertexShader> vertexShader = std::make_shared<library::VertexShader>(L"Shaders/Shaders.fxh", "VS", "vs_5_0");
    if (FAILED(game->GetRenderer()->AddVertexShader(L"MainShader", vertexShader)))
    {
        return 0;
    }

    std::shared_ptr<library::PixelShader> pixelShader = std::make_shared<library::PixelShader>(L"Shaders/Shaders.fxh", "PS", "ps_5_0");
    if (FAILED(game->GetRenderer()->AddPixelShader(L"MainShader", pixelShader)))
    {
        return 0;
    }

    /*--------------------------------------------------------------------
      TODO: Add your cubes and set their shaders (remove the comment)
    --------------------------------------------------------------------*/
    std::shared_ptr<MainCube> Maincube = std::make_shared<MainCube>();
    game->GetRenderer()->AddRenderable(L"MainCube", Maincube);
    game->GetRenderer()->SetVertexShaderOfRenderable(L"MainCube", L"MainShader");
    game->GetRenderer()->SetPixelShaderOfRenderable(L"MainCube", L"MainShader");

    std::shared_ptr<RotateCube1> rotatecube1 = std::make_shared<RotateCube1>();
    game->GetRenderer()->AddRenderable(L"RotateCube1", rotatecube1);
    game->GetRenderer()->SetVertexShaderOfRenderable(L"RotateCube1", L"MainShader");
    game->GetRenderer()->SetPixelShaderOfRenderable(L"RotateCube1", L"MainShader");

    std::shared_ptr<atomic> atomcube = std::make_shared<atomic>();
    game->GetRenderer()->AddRenderable(L"atomic", atomcube);
    game->GetRenderer()->SetVertexShaderOfRenderable(L"atomic", L"MainShader");
    game->GetRenderer()->SetPixelShaderOfRenderable(L"atomic", L"MainShader");
    if (FAILED(game->Initialize(hInstance, nCmdShow)))
    {
        return 0;
    }

    return game->Run();
}