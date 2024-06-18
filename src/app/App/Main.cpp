#define SDL_MAIN_HANDLED

#include <exception>

#include <iostream>
#include "Core/Application.hpp"
#include "Core/Debug/Instrumentor.hpp"
#include "Core/Log.hpp"

int main() {
  // try {
  // APP_PROFILE_BEGIN_SESSION_WITH_FILE("App", "profile.json");

  {
    App::Application app{"OpenRPGEditor"};
    app.run();
  }

  // APP_PROFILE_END_SESSION();
  //  } catch (std::exception& e) {
  //    APP_ERROR("Main process terminated with: {}", e.what());
  //  }

  return 0;
}
