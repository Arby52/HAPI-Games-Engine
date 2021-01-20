//Ben Ritson t7093422
//Used to identify memory leaks.
#include "pch.h"
#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__, __LINE__)
#endif

#include "WorldModel.h"
#include "Visualisation.h"

//Main
void HAPI_Main()
{
	bool m_restart{ true };
	WorldModel* world{ nullptr };
	Visualisation* m_visualisation = new Visualisation(1200,800);
	while (m_restart) {
		{
			WorldModel* world = new WorldModel(m_visualisation);
			(*world).Run(m_restart);
			delete world;
		}
	}
	delete m_visualisation;
	m_visualisation = nullptr;
}