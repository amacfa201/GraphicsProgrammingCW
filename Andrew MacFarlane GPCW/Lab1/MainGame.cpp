#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>




Transform marineTransform;
Transform carTransform;
Transform rockTransform;
Transform phongTransform;
Transform phongLightTransform;



MainGame::MainGame()
{
	theGameState = GameState::PLAY;
	Display* gameDisplay = new Display(); //new display
    Mesh* marineMesh();
	Mesh* carMesh();
	Mesh* therockMesh();
	Mesh* phongLightMesh();
	Mesh* phongMesh();
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
	Shader* phongShader();
	Shader* lightSourceShader();
	Shader* hairShader();
	Shader* explosionShader();
	Shader* explosionHairShader();


	
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
	
	
	
	toonShader.initialise("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag");
	rimToonShader.initialise("..\\res\\shaderRimToon.vert","..\\res\\shaderRimToon.frag");
	blurShader.initialise("..\\res\\shaderBlur.vert", "..\\res\\shaderBlur.frag");
	defaultShader.initialise("..\\res\\shader.vert", "..\\res\\shader.frag");
	fogShader.initialise("..\\res\\fogShader.vert", "..\\res\\fogShader.frag");
	phongShader.initialise("..\\res\\phongShader.vert", "..\\res\\phongShader.frag");
	lightSourceShader.initialise("..\\res\\lightSourceShader.vert", "..\\res\\lightSourceShader.frag");
	hairShader.initialise("..\\res\\hairShader.vert", "..\\res\\hairShader.frag", "..\\res\\hairShader.geo");
	explosionShader.initialise("..\\res\\explosionShader.vert", "..\\res\\explosionShader.frag", "..\\res\\explosionShader.geo");
	explosionHairShader.initialise("..\\res\\explodeHairShader.vert", "..\\res\\explodeHairShader.frag", "..\\res\\explodeHairShader.geo");

	

	marineMesh.loadModel("..\\res\\marine.obj");
	carMesh.loadModel("..\\res\\car.obj");
	therockMesh.loadModel("..\\res\\TheRock.obj");
	phongMesh.loadModel(("..\\res\\dragon.obj"));
	phongLightMesh.loadModel(("..\\res\\monkey3.obj"));
	
	lightPos = glm::vec3(-sinf(counter), -sinf(counter), -sinf(counter));

	myCamera.initCamera(glm::vec3(0, 0, -10.0), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;
}

void MainGame::gameLoop()
{
	while (theGameState != GameState::EXIT)
	{
		HandleInput();
		
		drawLoop();
		
		
	}
}

void MainGame::HandleInput()
{
	//Gets player input and responds accordingly
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym)

			{
			case SDLK_1:
				effect = 1;

				
				break;
			case SDLK_2:
				effect = 2;
				break;



			case SDLK_3:
				
				
				effect = 3;
				break;
			case SDLK_4:
				

				effect = 4;
				break;
			case SDLK_5:
				effect = 5;
				


				break;
			case SDLK_6:
				effect = 6;



				break;
			case SDLK_7:
				effect = 7;



				break;

			case SDLK_8:
				effect = 8;



				break;
			}

			break;
		case SDL_QUIT:
			theGameState = GameState::EXIT;
			break;
		}


	}

	
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
	rimToonShader.setMat4("v_pos", carTransform.GetModel());
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

void MainGame::SetPhongValues()
{
	

	
	
	
	phongShader.setVec3("viewPos", myCamera.getPos());
	phongShader.setVec3("lightPos", glm::vec3(0.5,0.5,0.5));
	
	
	phongShader.setBool("blinn", true);

}

void MainGame::SetHairValues()
{
	hairShader.setMat4("projection", myCamera.GetProjection());
	hairShader.setMat4("view", myCamera.GetView());
	hairShader.setMat4("model", carTransform.GetModel());

	
}

void MainGame::SetLightValues()
{

}

void MainGame::SetExplosionValues()
{

	explosionShader.setMat4("projection", myCamera.GetProjection());
	explosionShader.setMat4("view", myCamera.GetView());
	explosionShader.setMat4("model", marineTransform.GetModel());
	explosionShader.setFloat("time", counter /2);
}

void MainGame::SetExplosionHairShader()
{
	explosionHairShader.setMat4("projection", myCamera.GetProjection());
	explosionHairShader.setMat4("view", myCamera.GetView());
	explosionHairShader.setMat4("model", carTransform.GetModel());
	explosionHairShader.setFloat("time", counter / 2);
}


void MainGame::ApplyEffect(float effect)
{
	if (effect == 1)
	{
		fogShader.Bind();
		SetFogValues(marineMesh.getSpherePos().z + 4);
		fogShader.Update(marineTransform, myCamera);
		marineMesh.draw();
		marineMesh.updateSphereData(*marineTransform.GetPos(), 0.62f);

		fogShader.Bind();
		SetFogValues(carMesh.getSpherePos().z + 4);
		fogShader.Update(carTransform, myCamera);
		carMesh.draw();
		carMesh.updateSphereData(*carTransform.GetPos(), 0.62f);

		fogShader.Bind();
		SetFogValues(therockMesh.getSpherePos().z + 4);
		fogShader.Update(rockTransform, myCamera);
		therockMesh.draw();
		therockMesh.updateSphereData(*rockTransform.GetPos(), 0.62f);
	}
	
	if (effect == 2)
	{
		rimToonShader.Bind();
		SetRimToonValues();
		rimToonShader.Update(marineTransform, myCamera);
		marineMesh.draw();
		marineMesh.updateSphereData(*marineTransform.GetPos(), 0.62f);

		rimToonShader.Bind();
		SetRimToonValues();
		rimToonShader.Update(carTransform, myCamera);
		carMesh.draw();
		carMesh.updateSphereData(*carTransform.GetPos(), 0.62f);

		rimToonShader.Bind();
		SetRimToonValues();
		rimToonShader.Update(rockTransform, myCamera);
		therockMesh.draw();
		therockMesh.updateSphereData(*rockTransform.GetPos(), 0.62f);
	}
	
	if (effect == 3)
	{
		explosionShader.Bind();
		SetExplosionValues();
		explosionShader.Update(marineTransform, myCamera);
		marineMesh.draw();
		marineMesh.updateSphereData(*marineTransform.GetPos(), 0.62f);

		explosionShader.Bind();
		SetExplosionValues();
		explosionShader.Update(carTransform, myCamera);
		//carMesh.draw();
		//carMesh.updateSphereData(*carTransform.GetPos(), 0.62f);

		explosionShader.Bind();
		SetExplosionValues();
		explosionShader.Update(rockTransform, myCamera);
		//therockMesh.draw();
		//therockMesh.updateSphereData(*rockTransform.GetPos(), 0.62f);
	}
	
	if (effect == 4)
	{
		explosionHairShader.Bind();
		SetExplosionHairShader();
		explosionHairShader.Update(marineTransform, myCamera);
		//marineMesh.draw();
		//marineMesh.updateSphereData(*marineTransform.GetPos(), 0.62f);

		explosionHairShader.Bind();
		SetExplosionHairShader();
		explosionShader.Update(carTransform, myCamera);
		carMesh.draw();
		carMesh.updateSphereData(*carTransform.GetPos(), 0.62f);

		explosionHairShader.Bind();
		SetExplosionHairShader();
		explosionHairShader.Update(rockTransform, myCamera);
		//therockMesh.draw();
		//therockMesh.updateSphereData(*rockTransform.GetPos(), 0.62f);
	}

	if (effect == 5)
	{
		hairShader.Bind();
		SetHairValues();
		hairShader.Update(marineTransform, myCamera);
		//marineMesh.draw();
		//marineMesh.updateSphereData(*marineTransform.GetPos(), 0.62f);

		hairShader.Bind();
		SetHairValues();
		explosionShader.Update(carTransform, myCamera);
		//carMesh.draw();
		//carMesh.updateSphereData(*carTransform.GetPos(), 0.62f);

		hairShader.Bind();
		SetHairValues();
		hairShader.Update(rockTransform, myCamera);
		therockMesh.draw();
		therockMesh.updateSphereData(*rockTransform.GetPos(), 0.62f);
	}
	

	if (effect == 6)
	{
		toonShader.Bind();
		setToonLighting();
		toonShader.Update(marineTransform, myCamera);
		marineMesh.draw();
		marineMesh.updateSphereData(*marineTransform.GetPos(), 0.62f);

		toonShader.Bind();
		setToonLighting();
		toonShader.Update(carTransform, myCamera);
		carMesh.draw();
		carMesh.updateSphereData(*carTransform.GetPos(), 0.62f);

		toonShader.Bind();
		setToonLighting();
		toonShader.Update(rockTransform, myCamera);
		therockMesh.draw();
		therockMesh.updateSphereData(*rockTransform.GetPos(), 0.62f);
	}

	if (effect == 7)
	{
		phongShader.Bind();
		SetPhongValues();
		phongShader.Update(phongTransform, myCamera);
		
		phongMesh.draw();
		phongMesh.updateSphereData(*phongTransform.GetPos(), 0.62f);
	}
	if (effect == 8)
	{
		blurShader.Bind();
		blobEffect();
		blurShader.Update(marineTransform, myCamera);
		marineMesh.draw();
		marineMesh.updateSphereData(*marineTransform.GetPos(), 0.62f);

		blurShader.Bind();
		blobEffect();
		toonShader.Update(carTransform, myCamera);
		carMesh.draw();
		carMesh.updateSphereData(*carTransform.GetPos(), 0.62f);

		blurShader.Bind();
		blobEffect();
		blurShader.Update(rockTransform, myCamera);
		therockMesh.draw();
		therockMesh.updateSphereData(*rockTransform.GetPos(), 0.62f);
	
	}


	
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

void MainGame::SetMeshPositions()
{
	marineTransform.SetPos(glm::vec3(-sinf(counter), -1.0, -sinf(counter)));
	marineTransform.SetRot(glm::vec3(-0.65, counter / 2, 0));
	marineTransform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	carTransform.SetPos(glm::vec3(-sinf(counter), -1.0, -sinf(counter)));
	carTransform.SetRot(glm::vec3(0.0, counter / 2, 0.0));
	carTransform.SetScale(glm::vec3(0.01, 0.01, 0.01));

	rockTransform.SetPos(glm::vec3(-sinf(counter), -sinf(counter), -sinf(counter)));
	rockTransform.SetRot(glm::vec3(0.0, counter / 2, 0.0));
	rockTransform.SetScale(glm::vec3(0.01, 0.01, 0.01));

	phongTransform.SetPos(glm::vec3(-sinf(counter), -1.0, -sinf(counter) * 5));
	phongTransform.SetRot(glm::vec3(0.0, counter / 2, 0.0));
	phongTransform.SetScale(glm::vec3(0.8, 0.8, 0.8));

}

void MainGame::drawLoop()
{
	_gameDisplay.clearDisplay(0.25f, 0.0f, 0.0f, 1.0f);

	SetMeshPositions();
	ApplyEffect(effect);
	

	counter = counter + 0.005f;
	
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 