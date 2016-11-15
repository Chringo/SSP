#ifndef SSPAPPLICATION_GAMESTATES_GAMESTATE_H
#define SSPAPPLICATION_GAMESTATES_GAMESTATE_H
class GameStateHandler;
class GameState
{
private:	//Variables
protected:
	GameStateHandler* m_gsh;
public:
	GameState();
	virtual ~GameState();
	int InitializeBase(GameStateHandler* gsh);
private:	//Helper functions
};

#endif