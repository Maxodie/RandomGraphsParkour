#include "Core.h"
#include "Application.h"

int main(int argc, char** argv)
{
    RGP_Application_t App;
    RGP_AppInit(&App);
    RGP_AppRun(&App);
    RGP_AppShutdown(&App);

    return 0;
}
