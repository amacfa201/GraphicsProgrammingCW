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
	void HandleInput();
	void gameLoop();
	void drawLoop();
	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);
	void playAudio(unsigned int Source, glm::vec3 pos);
	void blobEffect();
	void setADSLighting();
	void setToonLighting();
	void DrawModels();
	void SetRimToonValues();
	void SetFogValues(float z_pos);
	void SetPhongValues();
	void SetLightValues();
	void SetHairValues();
	void SetExplosionValues();
	void SetExplosionHairShader(); 
	void ApplyEffect(float effect);
	void SetMeshPositions();

	Display _gameDisplay;
	GameState theGameState;
	Mesh marineMesh;
	Mesh carMesh;
	Mesh therockMesh;
	Mesh phongMesh;
	Mesh phongLightMesh;
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
	Shader hairShader;
	Shader explosionShader;
	Shader explosionHairShader;

	glm::vec3 lightPos;

	glm::mat4 modelView;

	Camera myCamera;
	Audio audioDevice;

	float counter;
	float effect;

	unsigned int whistle;
	unsigned int backGroundMusic;
};

