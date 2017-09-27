#include "Pawn.h"



Pawn::Pawn()
{
}

Pawn::Pawn(float x, float y, Role role) : BaseObject(x, y)
{
	if (role == Role::Attacker) {
		//for (int i = 0; i < FULL_IMG_COUNT; i++)
			//m_imgArr[i].Load();
	}
}

Pawn::~Pawn()
{
}

void Pawn::InsertKey() {

}

void Pawn::Update() {

}

void Pawn::Draw() {

}
