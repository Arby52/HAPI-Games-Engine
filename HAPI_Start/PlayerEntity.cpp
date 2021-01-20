#include "pch.h"
#include "PlayerEntity.h"
using namespace HAPISPACE;

PlayerEntity::PlayerEntity(std::string _associatedSprite, Visualisation &v, int _numFrames) : ShipEntity(_associatedSprite, v, _numFrames)
{
	Initialize(v);
}

PlayerEntity::~PlayerEntity()
{
}

void PlayerEntity::Initialize(Visualisation & v)
{
	m_entityRectangle = (v.GetTexRect(m_spriteName));
	m_side = ESide::ePlayer;
	m_health = 50;
}

void PlayerEntity::Update(Visualisation &v)
{
	Vector2D pos{ GetPosition() };

	//PLAYER INPUT
	static const HAPI_TKeyboardData &keyData = HAPI.GetKeyboardData(); //For Keyboard Inputs
	const HAPI_TControllerData controller = HAPI.GetControllerData(0); //Only works with one controller. On an exbox controller, make sure the light is in the top left of the circle representing controller 1.
	if (controller.isAttached) { //If controller is attached
		if (keyData.scanCode['W'] || keyData.scanCode[HK_UP] || controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > HK_GAMEPAD_LEFT_THUMB_DEADZONE || controller.digitalButtons[HK_DIGITAL_DPAD_UP]) {
			pos.y -= m_speed;
		}
		else if (keyData.scanCode['S'] || keyData.scanCode[HK_DOWN] ||  controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE || controller.digitalButtons[HK_DIGITAL_DPAD_DOWN]) {
			pos.y += m_speed;
		}
		if (keyData.scanCode['A'] || keyData.scanCode[HK_LEFT] || controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE || controller.digitalButtons[HK_DIGITAL_DPAD_LEFT]) {
			pos.x -= m_speed;
		}
		else if (keyData.scanCode['D'] || keyData.scanCode[HK_RIGHT] || controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > HK_GAMEPAD_LEFT_THUMB_DEADZONE || controller.digitalButtons[HK_DIGITAL_DPAD_RIGHT]) {
			pos.x += m_speed;
		}
		if (keyData.scanCode[HK_SPACE] || controller.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER] || controller.digitalButtons[HK_DIGITAL_A]) {
			Fire(v);
		}
	}
	else { //If controller isn't attached
		if (keyData.scanCode['W'] || keyData.scanCode[HK_UP]) {
			pos.y -= m_speed;
		}
		else if (keyData.scanCode['S'] || keyData.scanCode[HK_DOWN]) {
			pos.y += m_speed;
		}
		if (keyData.scanCode['A'] || keyData.scanCode[HK_LEFT]) {
			pos.x -= m_speed;
		}
		else if (keyData.scanCode['D'] || keyData.scanCode[HK_RIGHT]) {
			pos.x += m_speed;
		}
		if (keyData.scanCode[HK_SPACE]) {
			Fire(v);
		}
	}
	//Make sure player can't leave the screen.
	if (pos.x - m_entityRectangle.GetCentreWidth() < 0) {
		pos.x = (float)(0 + m_entityRectangle.GetCentreWidth());
	}
	else if (pos.x + m_entityRectangle.GetCentreWidth() > v.GetScreenDimensions().x) {
		pos.x  = v.GetScreenDimensions().x - m_entityRectangle.GetCentreWidth();
	}
	if (pos.y - m_entityRectangle.GetCentreHeight() < 0) {
		pos.y = (float)(0 + m_entityRectangle.GetCentreHeight());
	}
	else if (pos.y + m_entityRectangle.GetCentreHeight() > v.GetScreenDimensions().y) {
		pos.y = v.GetScreenDimensions().y - m_entityRectangle.GetCentreHeight();
	}
	SetPosition(pos);
	//Have projectiles always on player position.
	for (auto p : m_projectileVector) {
		if(!p->GetIsAlive() && !p->GetIsExplosion())
			p->SetPosition(Vector2D(GetPosition().x , GetPosition().y - m_entityRectangle.GetCentreHeight()));
	}
}

void PlayerEntity::DetectCollision(Visualisation & v, Entity & other)
{
	if (!GetIsAlive() || !other.GetIsAlive())
		return;

	if (!CanCollideWith(GetSide(), other.GetSide()))
		return;

	Rectangle thisRect{ m_entityRectangle };

	int w{ thisRect.GetWidth() };
	int h{ thisRect.GetHeight() };

	//Decrease hitbox slightly
	thisRect.left += w / 15;
	thisRect.right -= w / 15;
	thisRect.top += w / 15;
	thisRect.bottom -= w / 15;

	Rectangle otherRect{ v.GetTexRect(other.GetSpriteName()) };

	thisRect.Translate((int)GetTopLeftPosition().x, (int)GetTopLeftPosition().y);
	otherRect.Translate((int)other.GetTopLeftPosition().x, (int)other.GetTopLeftPosition().y);

	if (!thisRect.IsFullyOutsideOf(otherRect)) {
		//Collision
		other.HasCollided(v, *this);
		HasCollided(v, other);
	}

}

void PlayerEntity::HasCollided(Visualisation & v, Entity & other)
{
	TakeDamage(other.GetDamage(), v);
}

void PlayerEntity::Fire(Visualisation &v)
{
	if (GetIsAlive()) {
		if (HAPI.GetTime() - m_lastTimeUpdatedFireRate >= m_fireRate)
		{
			v.PlaySound("playerfire");
			for (auto p : m_projectileVector) {
				if (!p->GetIsAlive() && !p->GetIsExplosion()) {
					p->SetPosition(Vector2D(GetPosition().x, GetPosition().y - m_entityRectangle.GetCentreHeight()));
					p->SetIsAlive(1);
					break;
				}
			}
			m_lastTimeUpdatedFireRate = HAPI.GetTime();
		}
	}
}

void PlayerEntity::Render(Visualisation &v, float s)
{
	if (!m_isAlive)
		return;

	Vector2D interPos{ GetOldPos() + (GetPosition() - Vector2D((float)m_entityRectangle.GetCentreWidth(), (float)m_entityRectangle.GetCentreHeight()) - GetOldPos()) * s };

	v.Render(m_spriteName, (int)interPos.x, (int)interPos.y, m_frameNumber);

	if (HAPI.GetTime() - m_lastTimeUpdatedAnimation >= m_animationTime)
	{
		m_frameNumber++;
		if (m_frameNumber >= m_numFrames)
			m_frameNumber = 0;

		m_lastTimeUpdatedAnimation = HAPI.GetTime();
	}

}

void PlayerEntity::TakeDamage(int amount, Visualisation &v)
{
	m_health -= amount;

	if (m_health <= 0) {
		SetIsAlive(0);
	}
}