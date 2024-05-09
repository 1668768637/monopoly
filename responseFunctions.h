#ifndef RESPONSEFUNCTIONS_H
#define RESPONSEFUNCTIONS_H

#include "qjsonobject.h"
class monopolyGame;

void heartPackage(monopolyGame* game,QJsonObject* msg);
void login(monopolyGame* game,QJsonObject* msg);
void beginGame(monopolyGame* game,QJsonObject* msg);
void dontcare(monopolyGame* game,QJsonObject* msg);
void run(monopolyGame* game,QJsonObject* msg);
#endif // RESPONSEFUNCTIONS_H
