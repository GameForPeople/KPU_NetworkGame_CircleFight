#include "BaseCharacter.h"



BaseCharacter::BaseCharacter()
{
}


BaseCharacter::~BaseCharacter()
{
}



void BaseCharacter::Update(State state, double time) {
	
}

void BaseCharacter::Draw(HDC hdc, float x, float y, float sizeX, float sizeY, State state) {

}




float BaseCharacter::GetBaseSpeed() const {
	return m_baseSpeed;
}

int BaseCharacter::GetJumpImageCount() const {
	return 0;
}

void BaseCharacter::SetImageCount(const int input) {
	m_imageCount = input;
}