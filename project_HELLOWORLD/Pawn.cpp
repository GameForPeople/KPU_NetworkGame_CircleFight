#include "Pawn.h"

Pawn::Pawn()
{
}

Pawn::Pawn(CharacterName inputCharacterName)
{
	if (inputCharacterName == CharacterName::Archer)
		m_unit = new CArcher;

	m_baseSpeed = m_unit->GetBaseSpeed(); 
		m_speed = m_unit->GetBaseSpeed();

	m_fallSpeed = 2.0;
	m_pos.x = 200;
}

Pawn::Pawn(float x, float y) : BaseObject(x, y)
{

}

Pawn::~Pawn()
{
}

bool Pawn::InsertKey(WPARAM Key) {
	if (Key == VK_SPACE) {
		if (m_state == State::Run) {
			m_state = State::Jump;
			m_jumpSpeed = 9.0f;
			m_unit->SetImageCount(21);
			return true;
		}	
		else if (m_state == State::Jump) {
			m_jumpSpeed = 9.0f;
			m_state = State::DoupleJump;
			m_fallSpeed = 2.0f;
			m_unit->SetImageCount(21);
			return true;
		}		
	}
	return false;
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
		std::cout << m_combo << std::endl;
	}
}

void Pawn::ProcessGravity() {
	if (m_state == State::Fall || m_state == State::Jump) {
		m_pos.y += m_fallSpeed;
		m_fallSpeed *= 1.05;
	}
}

void Pawn::ProcessJump() {
	if (m_state == State::Jump) {
		m_pos.y -= m_jumpSpeed;
		if (m_jumpSpeed <= m_fallSpeed) {
			m_fallSpeed = 3.0f;
			m_jumpSpeed = 0;
		}
	}
}