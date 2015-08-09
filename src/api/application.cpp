#include "application.h"

Application::Application(const std::string &root) : _root(root)
{

}

RoutesContainer &Application::route()
{
    return _routes;
}

