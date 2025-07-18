#include "Utility/PreLibrary.h"
#include "Application.h"
#include "Utility/EntityRuntimeCache.h"
#include "Utility/ResourceCache.h"
#include "Utility/RendererCache.h"
#include "Output/Output.h"

Application::Application() : mGameWindow(Window("Breakout",800,600))
{
	//auto loadThread = std::thread(&Application::Load,this);
	Init();
	Load();
	//loadThread.join();
	Update();
}

Application::~Application()
{
}

void Application::Update()
{
	while (mGameWindow.GetWindowIsOpen())
	{
		mGameWindow.Events();

		GRenderer->Update();

		RuntimeCache->Update();

		mGameWindow.Swap();
	}

	for (auto& sprite : Cache->FindEntireMap<OpenGLSprite>())
	{
		sprite.second.Free();
	}
	GOutput->Free();
	GRenderer->Free();
	RuntimeCache->Free();
	Cache->Free();
}

void Application::Init()
{
	if (GameState == GameState::GameStart)
	{
		Cache->Insert<OpenGLShader>("Text", OpenGLShader("res/Shaders/Text.vert", "res/Shaders/Text.frag"));

		Cache->Insert<OpenGLText>("Breakout", OpenGLText(75, "Breakout", { .X = 160,.Y = 425,.Scale = 1.45f,.Color = glm::vec3(0.75,0.0,0.75) }, *Cache->Find<OpenGLShader>("Text")));

		Cache->Find<OpenGLText>("Breakout")->LoadFont("res/Fonts/Frohburg.ttf");

		static_cast<Entity&>(mTitleText) = mMainScene.CreateTextEntity(*Cache->Find<OpenGLText>("Breakout"), *Cache->Find<OpenGLShader>("Text"));
		RuntimeCache->Add(mTitleText);


		Cache->Insert<OpenGLText>("Play", OpenGLText(35, "Play", { .X = 350,.Y = 275,.Scale = 1.3f,.Color = glm::vec3(0.75,0.0,0.75) }, *Cache->Find<OpenGLShader>("Text")));

		Cache->Find<OpenGLText>("Play")->LoadFont("res/Fonts/Frohburg.ttf");

		static_cast<Entity&>(mPlayText) = mMainScene.CreateTextEntity(*Cache->Find<OpenGLText>("Play"), *Cache->Find<OpenGLShader>("Text"));
		RuntimeCache->Add(mPlayText);

		Cache->Insert<OpenGLText>("Quit", OpenGLText(35, "Quit", { .X = 350,.Y = 175,.Scale = 1.3f,.Color = glm::vec3(1.0) }, *Cache->Find<OpenGLShader>("Text")));

		Cache->Find<OpenGLText>("Quit")->LoadFont("res/Fonts/Frohburg.ttf");

		static_cast<Entity&>(mQuitText) = mMainScene.CreateTextEntity(*Cache->Find<OpenGLText>("Quit"), *Cache->Find<OpenGLShader>("Text"));
		RuntimeCache->Add(mQuitText);

		mPlayer = Player(mBall, mGameWindow,mTitleText,mPlayText,mQuitText,mGameController,mLivesText,mLevelText,mScoreText);
		Cache->Insert<Material>("Player", ::Material({ .Red = 0.25, .Green = 0.35, .Blue = 1.0 }));

		Cache->Insert<OpenGLSprite>("Square", ::OpenGLSprite());


																// 340
		Cache->Insert<Transform>("Player", ::Transform(glm::vec2(-500, 550), glm::vec2(120, 20)));

		static_cast<Entity&>(mPlayer) = mMainScene.CreateSpriteEntity(*Cache->Find<Transform>("Player"), *Cache->Find<OpenGLSprite>("Square"), *Cache->Find<Material>("Player"));

		RuntimeCache->Add(mPlayer);


		mTextRenderer = ::OpenGLTextRenderer(mMainScene);

		GRenderer->Add(mTextRenderer, 1);

	}

		auto color = 0;

		for (auto i = 0; i < 6; i++)
		{
			switch ((BlockColor)color)
			{
				using enum BlockColor;
			case Green:
				Cache->Insert<Material>("Block" + std::to_string(i), ::Material({ .Red = 0.25, .Green = 0.95, .Blue = 0.23 }));
				break;
			case Red:
				Cache->Insert<Material>("Block" + std::to_string(i), ::Material({ .Red = 1.0, .Green = 0.0, .Blue = 0.0 }));
				break;
			case Blue:
				Cache->Insert<Material>("Block" + std::to_string(i), ::Material({ .Red = 0.0, .Green = 0.0, .Blue = 1.0 }));
				break;
			case Yellow:
				Cache->Insert<Material>("Block" + std::to_string(i), ::Material({ .Red = 0.85, .Green = 0.68, .Blue = 0.23 }));
				break;
			case Orange:
				Cache->Insert<Material>("Block" + std::to_string(i), ::Material({ .Red = 1.0, .Green = 0.0, .Blue = 1.0 }));
				break;
			case Purple:
				Cache->Insert<Material>("Block" + std::to_string(i), ::Material({ .Red = 1.0, .Green = 1.0, .Blue = 0.0 }));
				break;
			}
			color++;
		}
		color = 0;

		auto xPos = 10;
		auto yPos = -40;
		for (auto i = 0; i < 30; i++)
		{
			if (i % 10 == 0)
			{
				xPos = 10;
				yPos += 50;
			}
			//xPos,yPos
			Cache->Insert<Transform>("Block" + std::to_string(i), ::Transform(glm::vec2(-500, -500), glm::vec2(60, 30)));
			xPos += 80;
		}

		for (auto i = 0; i < 30; i++)
		{
			if (color >= 6)
			{
				color = 0;
			}
			static_cast<Entity&>(mBlockArray[i]) = mMainScene.CreateSpriteEntity(*Cache->Find<Transform>("Block" + std::to_string(i)), *Cache->Find<OpenGLSprite>("Square"), *Cache->Find<Material>("Block" + std::to_string(color)));

			RuntimeCache->Add(mBlockArray[i]);
			color++;
		}

		

		// Text

		//Cache->Insert<OpenGLShader>("Text", OpenGLShader("res/Shaders/Text.vert", "res/Shaders/Text.frag"));

		Cache->Insert<OpenGLText>("Lives", OpenGLText(35, "LIVES   " + std::to_string(GameController::GameLives), { .X = 70,.Y = 1,.Scale = 1.0f,.Color = glm::vec3(0.0) }, *Cache->Find<OpenGLShader>("Text")));

		Cache->Find<OpenGLText>("Lives")->LoadFont("res/Fonts/Frohburg.ttf");

		static_cast<Entity&>(mLivesText) = mMainScene.CreateTextEntity(*Cache->Find<OpenGLText>("Lives"), *Cache->Find<OpenGLShader>("Text"));

		RuntimeCache->Add(mLivesText);


		Cache->Insert<OpenGLText>("Score", OpenGLText(35, "Score   " + std::to_string(GameController::GameLives), { .X = 620,.Y = 1,.Scale = 1.0f,.Color = glm::vec3(0.0) }, *Cache->Find<OpenGLShader>("Text")));

		Cache->Find<OpenGLText>("Score")->LoadFont("res/Fonts/Frohburg.ttf");

		static_cast<Entity&>(mScoreText) = mMainScene.CreateTextEntity(*Cache->Find<OpenGLText>("Score"), *Cache->Find<OpenGLShader>("Text"));

		RuntimeCache->Add(mScoreText);


		Cache->Insert<OpenGLText>("Level", OpenGLText(35, "Level   " + std::to_string(GameController::GameLevel), { .X = 345,.Y = 1,.Scale = 1.0f,.Color = glm::vec3(0.0) }, *Cache->Find<OpenGLShader>("Text")));

		Cache->Find<OpenGLText>("Level")->LoadFont("res/Fonts/Frohburg.ttf");

		static_cast<Entity&>(mLevelText) = mMainScene.CreateTextEntity(*Cache->Find<OpenGLText>("Level"), *Cache->Find<OpenGLShader>("Text"));

		RuntimeCache->Add(mLevelText);
																			//160
		Cache->Insert<OpenGLText>("GameOver", OpenGLText(75, "GameOver", { .X = -1000,.Y = 425,.Scale = 1.45f,.Color = glm::vec3(0.0) }, * Cache->Find<OpenGLShader>("Text")));

		Cache->Find<OpenGLText>("GameOver")->LoadFont("res/Fonts/Frohburg.ttf");

		mEndScreenTexts[0] = mMainScene.CreateTextEntity(*Cache->Find<OpenGLText>("GameOver"), *Cache->Find<OpenGLShader>("Text"));

		Cache->Insert<OpenGLText>("LvAndSco", OpenGLText(35, "Levels | " + std::to_string(GameController::GameLevel) + "                  Score | " 
			+ std::to_string(GameController::GameScore)
			, {.X = -1000,.Y = 350,.Scale = 1.3f,.Color = glm::vec3(1.0)}, * Cache->Find<OpenGLShader>("Text")));

		Cache->Find<OpenGLText>("LvAndSco")->LoadFont("res/Fonts/Frohburg.ttf");

		mEndScreenTexts[1] = mMainScene.CreateTextEntity(*Cache->Find<OpenGLText>("LvAndSco"), *Cache->Find<OpenGLShader>("Text"));


		mBall = Ball(static_cast<Entity&>(mPlayer), mBlockArray, mGameController);

		Cache->Insert<Transform>("Ball", ::Transform(glm::vec2(385, 500), glm::vec2(30)));
		Cache->Insert<OpenGLTexture>("Ball", ::OpenGLTexture("res/Textures/circle.png"));
		Cache->Insert<OpenGLShader>("Ball", ::OpenGLShader("res/Shaders/SpriteTextured.vert", "res/Shaders/SpriteTextured.frag"));

		static_cast<Entity&>(mBall) = mMainScene.CreateShaderSpriteEntity(*Cache->Find<Transform>("Ball"), *Cache->Find<OpenGLSprite>("Square"), *Cache->Find<OpenGLShader>("Ball"));
		mBall.AddComponent<OpenGLTexture>(*Cache->Find<OpenGLTexture>("Ball"));

		RuntimeCache->Add(mBall);

		
		Cache->Insert<OpenGLShader>("ShakeScreen", OpenGLShader("res/Shaders/ShakeScreen.vert","res/Shaders/ShakeScreen.frag"));

		mSpriteRenderer = ::OpenGLSpriteRenderer(mMainScene,*Cache->Find<OpenGLShader>("ShakeScreen"));

		GRenderer->Add(mSpriteRenderer, 0);
	

		
	
}

void Application::Load()
{
	GOutput->Init();


		GOutput->LoadSound("Hit", "res/Sounds/Pickup.wav");
		GOutput->LoadSound("LostLife", "res/Sounds/LostLife.wav");
		GOutput->LoadSound("Bounce", "res/Sounds/Bounce.wav");
		GOutput->LoadSound("Menu", "res/Sounds/Menu.wav");

		Cache->Insert<Transform>("GC", Transform());

		mGameController = GameController(mBlockArray, mBall, mPlayer,mLivesText,mLevelText,mScoreText,mEndScreenTexts,mPlayText,mQuitText,mGameWindow);

		static_cast<Entity&>(mGameController) = mMainScene.CreateEntity(*Cache->Find<Transform>("GC"));

		RuntimeCache->Add(mGameController);
	
}
