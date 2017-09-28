#include "Pawn.h"



Pawn::Pawn()
{
}

Pawn::Pawn(CharacterName inputCharacterName)
{
	if (inputCharacterName == CharacterName::Archer)
		m_unit = new CArcher;

	m_baseSpeed = m_unit->GetBaseSpeed(); 
		m_speed = m_baseSpeed;

		m_pos.x = 200;
}

Pawn::Pawn(float x, float y) : BaseObject(x, y)
{

}

Pawn::~Pawn()
{
}

void Pawn::InsertKey(WPARAM Key) {
	if (Key == VK_SPACE) {
		if (m_state == State::Run) {
			m_isJump = true;
			m_jumpSpeed = 9.0f;
			m_state = State::Jump;
			m_unit->SetImageCount(21);
		}
		else if (m_state == State::Jump) {
			m_isJump = true;
			m_jumpSpeed = 9.0f;
			m_state = State::DoupleJump;
			m_fallSpeed = 2.0f;
			m_unit->SetImageCount(21);
		}
	}
}

void Pawn::Update(State state) {
	
	if (m_pos.y >= 400) {
		m_state = State::Run;
		m_isFalling = false;
		m_pos.y = 400;
	}

	if (m_isFalling == false) {
		m_isFalling = true;
		m_fallSpeed = 2.0f;
	}	
	else if (m_isFalling) {
		m_pos.y += m_fallSpeed;
		m_fallSpeed *= 1.05;
	}

	if (m_isJump) {
		m_pos.y -= m_jumpSpeed;
		if (m_jumpSpeed <= m_fallSpeed) {
			m_isJump = false;
			m_fallSpeed = 2.0f;
		}
	}

	m_unit->Update(state);
}

void Pawn::Draw(HDC hdc, float x, float y, float sizeX, float sizeY) {
	m_unit->Draw(hdc, m_pos.x, m_pos.y, m_size.x, m_size.y);
}

void Pawn::Draw(HDC hdc) {
	m_unit->Draw(hdc, m_pos.x, m_pos.y, m_size.x, m_size.y);
}
