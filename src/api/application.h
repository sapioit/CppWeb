#ifndef APPLICATION_H
#define APPLICATION_H

#include "routescontainer.h"
#include <string>
class Application
{
public:
    Application(const std::string&);
    RoutesContainer &route();

private:
    RoutesContainer _routes;
    std::string _root;
};

#endif // APPLICATION_H
