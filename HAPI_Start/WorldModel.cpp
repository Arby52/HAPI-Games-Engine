#include "pch.h"
#include "WorldModel.h"
#include "Visualisation.h"
#include "PlayerEntity.h"
#include "EnemyEntity.h"
#include "ProjectileEntity.h"
#include "BackgroundEntity.h"
#include "Rectangle.h"

using namespace HAPISPACE;

struct EnemyBatch {
	std::vector<Vector2D> m_path;
	EEnemyType m_type;
	std::string m_texture;
	int m_batchSize;
};

WorldModel::WorldModel(Visualisation* _vis)
{
	m_visualisation = _vis;
}

WorldModel::~WorldModel()
{	
	
}

void WorldModel::FreeMemory() {
	for (auto p : m_entityVector) {
		delete p;
		p = nullptr;
	}
	m_entityVector.clear();
	m_visualisation->StopStreamedSound("music");
	m_visualisation->DeleteAllSounds();
	m_visualisation->DeleteAllTextures();
	m_visualisation = nullptr;
}

void WorldModel::Run(bool& _restart)
{
	m_gameOver = false;
	if (!LoadLevel()) {
		std::cerr << "Level Failed to load." << std::endl;
		_restart = false;
		FreeMemory();
		return;
	}

	if (Update(_restart)) {
		std::cout << "Restaring Game." << std::endl;
		_restart = true;
	}		
	else {
		std::cout << "Ending Game." << std::endl;
		_restart = false;
	}
	FreeMemory();
	return;
}

bool WorldModel::LoadLevel()
{
	//Load Sounds. Sound Error Handling done in sounds class.
	if (!m_visualisation->LoadSound("playerfire", "Data\\audio\\playerfire.wav")) //Must be 16bit wav file.
		return false;
	m_visualisation->SetVolume("playerfire", (float)0.05);

	if (!m_visualisation->LoadSound("explode1", "Data\\audio\\explosion.wav")) //Must be 16bit wav file.
		return false;
	m_visualisation->SetVolume("explode1", (float)0.2);

	if (!m_visualisation->LoadSound("explode2", "Data\\audio\\explosion2.wav")) //Must be 16bit wav file.
		return false;
	m_visualisation->SetVolume("explode2", (float) 0.2);

	//Stream Music
	m_visualisation->CreateStreamedSound("music", "Data\\audio\\music.wav");  //Set Options Beforehand. 
	m_visualisation->SetVolume("music", 0.5);
	m_visualisation->SetIsLooped("music", true);
	m_visualisation->PlayStreamedSound("music");

	//Load Textures. Error handling done in Visualisation.
	if (!m_visualisation->CreateTexture("Background", "Data\\textures\\space.png"))
		return false;

	if (!m_visualisation->CreateTexture("Icon", "Data\\textures\\icon.png"))
		return false;

	m_visualisation->SetIcon("Icon");

	if (!m_visualisation->CreateTexture("Healthbar", "Data\\textures\\healthbar.png", 6))
		return false;

	if (!m_visualisation->CreateTexture("Player", "Data\\textures\\playerSprite.png", 8))
		return false;

	if (!m_visualisation->CreateTexture("Explode", "Data\\textures\\explode.png", 10))
		return false;

	if (!m_visualisation->CreateTexture("Boss", "Data\\textures\\bossSprite.png", 8))
		return false;

	if (!m_visualisation->CreateTexture("Enemy", "Data\\textures\\enemySprite.png", 8))
		return false;
	
	if (!m_visualisation->CreateTexture("Projectile", "Data\\textures\\projectileSprite.png", 2))
		return false;

	//Create Objects - Objects created earlier are rendered first and thus are blocked by objects created later.
	//Background
	BackgroundEntity *newBackground = new BackgroundEntity("Background", *m_visualisation); //Scrolling background tanks fps to about 10% atm. I believe the current method is extremely inefficient. Possibly remove it from being an entity all together and have it handled in visualisation.
	newBackground->SetTopLeftPosition(Vector2D(0,0));
	m_entityVector.push_back(newBackground);
	
	//Player
	PlayerEntity *newPlayer = new PlayerEntity("Player", *m_visualisation, 8);
	newPlayer->SetPosition(Vector2D((float)m_visualisation->GetCentreWidth(), 400));
	for (int i = 0; i < 15; i++) {
		//Create 15 bullets for the player. With current firerate, no more than 2 are active at once anyway.
		ProjectileEntity *newProjectile = new ProjectileEntity("Projectile", "Explode", *m_visualisation, 2, 10);
		newProjectile->SetSide(ESide::ePlayer);
		newPlayer->AddToProjVector(newProjectile);
		m_entityVector.push_back(newProjectile);
	}
	m_player = newPlayer; //Make sure to populate m_player;
	m_entityVector.push_back(newPlayer);
		
	//Load Level Data
	//Batches are separated on different lines. Variables in the document must have one comma and no spaces between them. For example "numberOfEnemies,textureUsed,enemyType,enemyPath".
	//Batches higher in the document are spawned earlier.
	//The enemyPath variable is an std::vector of Vector2D. In the document it is represented by "x y x y x y". For example "textureUsed,enemyType,0 0 100 200". Would be Point 1: x=0 y=0. Point 2: x=100, y=200.
	std::vector<EnemyBatch> enemyBatches; //A vector eventually containing every enemy batch and their data. EnemyBatch is a struct defined at the top of WorldModel.cpp.
	std::ifstream levelIn;
	levelIn.open("level.txt");
	if (levelIn.fail()) { //Incase the file doesn't open.
		std::cerr << "Error opening score file." << std::endl;
		return false;
	}
	std::vector<std::string> enemyBatchesDataStream; //Used to hold each line from the text document. Each line represents a new batch of enemies and holds the four variables of information as in the EnemyBatch struct.
	std::string batchStr;
	while (std::getline(levelIn, batchStr)) { //Take each line from the document and store it as a string inside enemyBatchesDataStream.
		enemyBatchesDataStream.push_back(batchStr);
	}

	for (int i = 0; i < enemyBatchesDataStream.size(); i++) { //For each line in the txt file (for each batch of enemies).
		std::string splitStream[4];  //An array to store the 4 Struct variables as strings. 0 is batchSize. 1 is texture used. 2 is enemyType. 3 is enemyPath.
		std::stringstream stream(enemyBatchesDataStream[i]); //Only used in the getline.
		std::string batchVariable;
		int x = 0;
		while (std::getline(stream, batchVariable, ',')) {
			splitStream[x] = batchVariable;
			x++;
		}

		EnemyBatch batch; //Temporary batch instansiated from EnemyBatch. Variables populated using the splitStream array. Pushedback into the enemyBatches vector.
		
		try {
			batch.m_batchSize = std::stoi(splitStream[0]); //String to int.
		}
		catch (const std::invalid_argument& ia) {
			std::cerr << "Cannot read batchSize in level.txt, make sure it is an int: " << ia.what() << '\n';
			return false;
		}
		if(m_visualisation->CheckTexture(splitStream[1]))
			batch.m_texture = splitStream[1]; //String to EEnemyType enum using if statements. If more types are added, this needs to be modified to accomodate. Possibly convert to string to char[] and use switch?
		else {
			std::cerr << "Invalid texture name in level.txt.";
			return false;
		}
		if (splitStream[2] == "eBasic") {
			batch.m_type = EEnemyType::eBasic;
		}
		else if (splitStream[2] == "eBoss") {
			batch.m_type = EEnemyType::eBoss;
		}
		else { //If enemy type was unreadable, exit program
			std::cerr << "Failed to read enemy type from file." << std::endl;
			return false;
		} 
		//split the path string into individual float values and push them into a vector.
		std::stringstream pathStream(splitStream[3]);
		std::string stringValue;
		std::vector<float> pathData;
		while (std::getline(pathStream, stringValue, ' ')) {
			float f;
			try {
				f = std::stof(stringValue); //Convert string to float.
			}
			catch (const std::invalid_argument& ia) {
				std::cerr << "Cannot read enemyPath in level.txt, make sure it is a valid float: " << ia.what() << '\n';
				return false;
			}
			pathData.push_back(f);
		}
		std::vector<Vector2D> tempPath;
		for (int i = 0; i < pathData.size(); i+=2) { //Take the first two values in the vector, push them into a Vector2D vector as x and y. Move to the next pair, repeat.
			Vector2D v (pathData[i], pathData[i+1]); 
			tempPath.push_back(v);
		}
		batch.m_path = tempPath; 
		
		enemyBatches.push_back(batch); //With all values filled in, push the instance onto the vector.
	}

	if (enemyBatches.empty()) {
		std::cerr << "enemyBatches vector is empty." << std::endl;
		return false;
	}

	//Creating Enemy Objects based on the enemyBatches vector.
	for (int x = 0; x < enemyBatches.size(); x++) {		
		//p = EnemyName, Vector2d path, EEnemyType;
		for (int y = 0; y < enemyBatches[x].m_batchSize; y++) {				
			EnemyEntity *newEnemy = new EnemyEntity(enemyBatches[x].m_texture, enemyBatches[x].m_type, *m_visualisation, 8);
			newEnemy->SetAIPath(enemyBatches[x].m_path);
			for (int i = 0; i < 10; i++) {
				ProjectileEntity *newProjectile = new ProjectileEntity("Projectile", "Explode", *m_visualisation, 2, 10);
				newProjectile->SetSide(ESide::eEnemy);
				newEnemy->AddToProjVector(newProjectile);
				m_entityVector.push_back(newProjectile);
			}
			m_entityVector.push_back(newEnemy);
			m_enemySpawnStack.push_back(newEnemy);
			if (enemyBatches[x].m_type == EEnemyType::eBoss) {
				m_boss = newEnemy;
			}
		}
	}	
	return true;
}

constexpr DWORD kTickTime{ 50 };
constexpr DWORD kEnemySpawnTime{ 700 };
bool WorldModel::Update(bool& _restart)
{
	//Game Loop
	DWORD spawnlastTimeTicked{ 700 };
	DWORD lastTimeTicked{0};
	while (HAPI.Update())
	{
		if(!m_gameOver){
			//Input and Physics. Player Input located in player class. System input, to exit or restart the game, is done outside of the if(!m_gameOver) at the bottom of the function.
			//Simulation Tick Time
			DWORD timeSinceLastTick{ HAPI.GetTime() - lastTimeTicked };
			if (timeSinceLastTick >= kTickTime) {

				//Enemy Spawning
				if (HAPI.GetTime() - spawnlastTimeTicked >= kEnemySpawnTime) {

					if (!m_enemySpawnStack.empty()) {
						m_enemySpawnStack.front()->SetIsAlive(true);
						m_enemySpawnStack.front()->SetHasSpawned(true);
						m_enemySpawnStack.pop_front();
					}
					spawnlastTimeTicked = HAPI.GetTime();
				}

				//Update Every Entity
				for (auto p : m_entityVector) {
					p->Update(*m_visualisation);
				}

				lastTimeTicked = HAPI.GetTime();

				//Collisions
				for (size_t i = 0; i < m_entityVector.size(); i++) {
					for (size_t j = i + 1; j < m_entityVector.size(); j++) {
						m_entityVector[i]->DetectCollision(*m_visualisation, *m_entityVector[j]);
					}
				}

				timeSinceLastTick = 0;
			}
			//Interpolation
			float s = timeSinceLastTick / (float)kTickTime;
			assert(s >= 0 && s <= 1.0f && "'s' variable used for interpolation outside of 0-100 bounds."); //If, for any reason, the interpolation isn't between 0% and 100%, terminate the program.
			//Input and Physics End.

			//Rendering
			//m_visualisation->WipeScreenToBlack(); //At the start of every update, make the screen black. Can use rgb values instead if needed. Uncomment if there is no scrolling background.
			//Render Every Entity
			for (auto p : m_entityVector) {
				p->Render(*m_visualisation, s);
			}
			//Display UI after everything has rendered to keep anything from appearing above it.
			m_visualisation->DisplayScore(m_visualisation->h_white, m_visualisation->h_black, 3, 20);
			m_visualisation->DisplayPlayerHP(*m_visualisation, m_player->GetHP(), "Healthbar"); //Current method of showing healthbar uses animation. Could save on filespace by changing it to one resusable image rendered multiple times.
			if (GameOverCheck()) { //Check Game over at the end of the loop.
				m_gameOver = true;
			}
		}		
		//If m_gameOver is true, only loop these last few lines.
		static const HAPI_TKeyboardData &keyData = HAPI.GetKeyboardData(); //For Keyboard Inputs
		if (keyData.scanCode[HK_ESCAPE]) { //Close application
			_restart = false;
			m_visualisation->SaveScore();
			HAPI.Close();
			
		}
		if (keyData.scanCode['R']) { //Restart Game
			_restart = true;	
			m_visualisation->SaveScore();
			return true;
		}
			
	}
	return false;
}

bool WorldModel::GameOverCheck() const{
	if (!m_player->GetIsAlive()) {
		m_visualisation->SaveScore();
		m_visualisation->GameOver(0); //Player Loses
		return true;
	}
	else if (m_boss->GetHasSpawned() && !m_boss->GetIsAlive()) {
		m_visualisation->SaveScore();
		m_visualisation->GameOver(1); //Player Wins
		return true;
	}
	return false;
}





