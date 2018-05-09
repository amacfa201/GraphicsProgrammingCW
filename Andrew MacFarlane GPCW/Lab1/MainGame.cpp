#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>



Transform guitarTransform;
Transform orangeTransform;
Transform rockTransform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
    Mesh* guitarMesh();
	Mesh* orangeMesh();
	Mesh* therockMesh();
	Audio* audioDevice();
	Texture* brickTexture(); //load texture
	Texture* waterTexture(); //load texture
	Texture* rockTexture(); //load texture
	Overlay* overlay(); //load texture
	Shader* toonShader();
	Shader* blurShader();
	Shader* defaultShader();
	Shader* rimToonShader();
	Shader* fogShader();
	
}

MainGame::~MainGame() 
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	
	whistle = audioDevice.loadSound("..\\res\\bang.wav");
	backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	
	brickTexture.init("..\\res\\bricks.jpg");
	waterTexture.init("..\\res\\water.jpg"); 
	rockTexture.init("..\\res\\rockskin.jpg");
	
	toonShader.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag");
	rimToonShader.init("..\\res\\shaderRimToon.vert","..\\res\\shaderRimToon.frag");
	blurShader.init("..\\res\\shaderBlur.vert", "..\\res\\shaderBlur.frag");
	defaultShader.init("..\\res\\shader.vert", "..\\res\\shader.frag");
	fogShader.init("..\\res\\fogShader.vert", "..\\res\\fogShader.frag");
	

	overlay.init("..\\res\\bricks.jpg");

	guitarMesh.loadModel("..\\res\\TheRock.obj");
	orangeMesh.loadModel("..\\res\\TheRock.obj");
	therockMesh.loadModel("..\\res\\TheRock.obj");

	
	myCamera.initCamera(glm::vec3(0, 0, -10.0), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
		collision(guitarMesh.getSpherePos(), guitarMesh.getSphereRadius(), orangeMesh.getSpherePos(), orangeMesh.getSphereRadius());
		playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}
	
}


bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		audioDevice.setlistener(myCamera.getPos(), m1Pos); //add bool to mesh
		//playAudio(whistle, m1Pos);
		return true;
	}
	else
	{
		return false;
	}
}

void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
{
	
	ALint state; 
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	/*
	Possible values of state
	AL_INITIAL
	AL_STOPPED
	AL_PLAYING
	AL_PAUSED
	*/
	if (AL_PLAYING != state)
	{
		//audioDevice.playSound(Source, pos);
	}
}

void MainGame::setADSLighting()
{
	modelView = guitarTransform.GetModel() * myCamera.GetView();
	
	toonShader.setMat4("ModelViewMatrix", modelView);
	toonShader.setMat4("ProjectionMatrix", myCamera.GetProjection()); 
	
	glm::mat4 normalMatrix = transpose(inverse(modelView));
	
	toonShader.setMat4("NormalMatrix", normalMatrix);

	toonShader.setVec4("Position", glm::vec4(10.0,10.0,10.0,1.0));
	toonShader.setVec3("Intensity", glm::vec3(0.0, 0.0, 0.0));

	toonShader.setVec3("ka", glm::vec3(0.5, 0.5, 0.5));
	toonShader.setVec3("kd", glm::vec3(0.5, 0.5, 0.5));
	toonShader.setVec3("ks", glm::vec3(0.5, 0.5, 0.5));

	toonShader.setFloat("Shininess", 0.5);
}

void MainGame::setToonLighting()
{
	toonShader.setVec3("lightDir", glm::vec3(0.5, 0.5, 0.5));
	
}

void MainGame::SetRimToonValues()
{
	rimToonShader.setVec3("lightDir", glm::vec3(0.5, 0.5, 0.5));
	rimToonShader.setMat4("u_vm", myCamera.GetView());
	rimToonShader.setMat4("u_pm", myCamera.GetProjection());
	rimToonShader.setMat4("v_pos", orangeTransform.GetModel());
}

void MainGame::SetFogValues(float z_pos)
{
	fogShader.setVec3("lightDir", glm::vec3(0.5, 0.5, 0.5));
	fogShader.setMat4("u_vm", myCamera.GetView());
	fogShader.setMat4("u_pm", myCamera.GetProjection());
	fogShader.setVec3("fogColor", glm::vec3(0.2, 0.8, 0.2));
	fogShader.setFloat("minDist", -5.0f);
	fogShader.setFloat("maxDist", 5.0f);
	fogShader.setFloat("z_pos", z_pos);

}



void MainGame::blobEffect()
{
	GLuint blockIndex = glGetUniformBlockIndex(blurShader.getProgram(), "BlobSettings");

	GLint blockSize;
	glGetActiveUniformBlockiv(blurShader.getProgram(), blockIndex,
		GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize); //get information about blobsettings and save it in blockSize

	GLubyte * blockBuffer = (GLubyte *)malloc(blockSize); //allocates the requested memory and returns a pointer to it.

														  // Query for the offsets of each block variable
	const GLchar *names[] = { "InnerColor", "OuterColor",
		"RadiusInner", "RadiusOuter" };

	GLuint indices[4];
	glGetUniformIndices(blurShader.getProgram(), 4, names, indices); // glGetUniformIndices retrieves the indices of a number of uniforms within program

	GLint offset[4];
	glGetActiveUniformsiv(blurShader.getProgram(), 4, indices, GL_UNIFORM_OFFSET, offset); //Returns information about several active uniform variables for the specified program object

	GLfloat outerColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat innerColor[] = { 1.0f, 1.0f, 0.75f, 1.0f };

	GLfloat innerRadius = 0.0f, outerRadius = 3.0f;

	memcpy(blockBuffer + offset[0], innerColor,
		4 * sizeof(GLfloat)); //destination, source, no of bytes. 
	memcpy(blockBuffer + offset[1], outerColor,
		4 * sizeof(GLfloat));
	memcpy(blockBuffer + offset[2], &innerRadius,
		sizeof(GLfloat));
	memcpy(blockBuffer + offset[3], &outerRadius,
		sizeof(GLfloat));

	GLuint uboHandle;

	glGenBuffers(1, &uboHandle);

	glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer,
		GL_DYNAMIC_DRAW); //creates and initializes a buffer object's data store - targer, size, data, usage

	glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, uboHandle); // bind a buffer object to an indexed buffer target - trager, index, buffer
}

void MainGame::DrawModels()
{
	//Bind Shaders 
	// Position 
	// Bind Texture
	// DrawMesh


}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	//Guitar - fog
	guitarTransform.SetPos(glm::vec3(sinf(counter), 0.5, -sinf(counter) * 5));
	guitarTransform.SetRot(glm::vec3(-0.65, counter * 5, 0));
	//guitarTransform.SetScale(glm::vec3(0.6, 0.6, 0.6));
	guitarTransform.SetScale(glm::vec3(0.01, 0.01, 0.01));
	fogShader.Bind();
	SetFogValues(guitarMesh.getSpherePos().z + 4);
	fogShader.Update(guitarTransform, myCamera);
	rockTexture.Bind(0);
	guitarMesh.draw();
	guitarMesh.updateSphereData(*guitarTransform.GetPos(), 0.62f);
	
	//Orange - Blur
	orangeTransform.SetPos(glm::vec3(-sinf(counter), -1.0, -sinf(counter)*5));
	orangeTransform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	//orangeTransform.SetScale(glm::vec3(0.6, 0.6, 0.6));
	orangeTransform.SetScale(glm::vec3(0.01, 0.01, 0.01));
	rimToonShader.Bind();
	SetRimToonValues();
	rimToonShader.Update(orangeTransform, myCamera);
	orangeMesh.draw();
	orangeMesh.updateSphereData(*orangeTransform.GetPos(), 0.62f);

	//The Rock - Rim Toon
	rockTransform.SetPos(glm::vec3(-sinf(counter), -sinf(counter), -sinf(counter)));
	rockTransform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	rockTransform.SetScale(glm::vec3(0.01, 0.01, 0.01));
	//rockTransform.SetScale(glm::vec3(0.6, 0.6, 0.6));
	//rockTexture.Bind(0);

	blurShader.Bind();
	blobEffect();
	blurShader.Update(rockTransform, myCamera);
	therockMesh.draw();
	therockMesh.updateSphereData(*rockTransform.GetPos(), 0.62f);

	counter = counter + 0.01f;

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 