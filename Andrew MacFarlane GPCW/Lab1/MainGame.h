#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"
#include "Overlay.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);
	void playAudio(unsigned int Source, glm::vec3 pos);
	void blobEffect();
	void setADSLighting();
	void setToonLighting();
	void DrawModels();
	void SetRimToonValues();
	void SetFogValues(float z_pos);
	void SetPhongValues();

	Display _gameDisplay;
	GameState _gameState;
	Mesh guitarMesh;
	Mesh orangeMesh;
	Mesh therockMesh;
	Texture brickTexture;
	Texture rockTexture;
	Texture waterTexture;
	Overlay overlay;
	Shader toonShader;
	Shader blurShader;
	Shader defaultShader;
	Shader rimToonShader;
	Shader fogShader;
	Shader phongShader;
	Shader lightSourceShader;

	glm::mat4 modelView;

	Camera myCamera;
	Audio audioDevice;

	float counter;
	unsigned int whistle;
	unsigned int backGroundMusic;
};

