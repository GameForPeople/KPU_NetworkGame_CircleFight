#include "Pawn.h"

Pawn::Pawn()
{
}

Pawn::Pawn(CharacterName inputCharacterName)
{
	if (inputCharacterName == CharacterName::Archer)
		m_unit = new CArcher;
	else if (inputCharacterName == CharacterName::Zombie)
		m_unit = new CZombie;

	ResetBaseSpeed();
	ResetFallSpeed();
	ResetJumpSpeed();

	m_speed = m_unit->GetBaseSpeed();
	m_state = State::Fall;
	
	m_pos.x = 200;
}

Pawn::Pawn(float x, float y) : BaseObject(x, y)
{

}

Pawn::~Pawn()
{
}


void Pawn::Update(State state) {

	ComputeTotalDistance();
	ProcessCombo();
	ProcessGravity();
	ProcessJump();

	m_unit->Update(state);
}
//void Pawn::Draw(HDC hdc, float x, float y, float sizeX, float sizeY) {
//	m_unit->Draw(hdc, m_pos.x, m_pos.y, m_size.x, m_size.y, );
//}

void Pawn::Draw(HDC hdc, State state) {
	m_unit->Draw(hdc, m_pos.x, m_pos.y, m_size.x, m_size.y, state);
}

void Pawn::ComputeTotalDistance() {
	m_totalDistance += m_speed;
}

void Pawn::ProcessCombo() {
	//if (m_state != State::Stun && m_state != State::Collide && m_state != State::Death) {
	if ((int)m_totalDistance % 100 == 1) {	//이러면 안됨... 1뛰어넘을떄가 많음.
		m_combo++;
	}
}

bool Pawn::InsertKey(WPARAM Key) {
	if (Key == VK_SPACE) {
		if (m_state == State::Run) {
			m_state = State::JumpStart;
			ResetJumpSpeed();
			m_unit->SetImageCount(m_unit->GetJumpImageCount());
			m_state = State::JumpLoop;
			return true;
		}	
		else if (m_state == State::JumpLoop || m_state == State::JumpEnd) {
			ResetJumpSpeed();
			m_state = State::DoubleJumpStart;
			ResetFallSpeed();
			m_unit->SetImageCount(m_unit->GetJumpImageCount());
			m_state = State::DoubleJumpLoop;
			return true;
		}		
	}
	return false;
}

void Pawn::ProcessGravity() {
	if (m_state == State::Fall || m_state == State::JumpEnd || m_state == State::DoubleJumpEnd) {
		m_pos.y += m_fallSpeed;
		m_fallSpeed *= 1.05;
	}
}

void Pawn::ProcessJump() {
	if (m_state == State::JumpLoop || m_state == State::DoubleJumpLoop) {
		m_pos.y -= m_jumpSpeed;
		m_jumpSpeed -= 0.11f;
		std::cout << m_jumpSpeed << "   ";

		if (m_jumpSpeed <= 0 ) {
			if (m_state == State::JumpLoop) {
				m_state = State::JumpEnd;
			}
			else if (m_state == State::DoubleJumpLoop) {
				m_state = State::DoubleJumpEnd;
			}

			ResetFallSpeed();
			ResetJumpSpeed();
		}
	}
}

void Pawn::ResetFallSpeed() {
	m_fallSpeed = 1.5;
}

void Pawn::ResetJumpSpeed() {
	m_jumpSpeed = 6.0;
}

void Pawn::ResetBaseSpeed() {
	m_speed = m_unit->GetBaseSpeed();
}