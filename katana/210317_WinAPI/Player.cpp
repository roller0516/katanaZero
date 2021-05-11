#include "Player.h"
#include "Image.h"
#include "CommonFunction.h"
#include "Camera.h"

HRESULT Player::Init()
{

	ImageManager::GetSingleton()->AddImage("player_idle",
		"Image/Katana/player/player_idle_11x2.bmp", 770, 140,11,2,true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("player_attack",
		"Image/Katana/player/player_attack_7x2.bmp", 840, 164, 7, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("player_flip",
		"Image/Katana/player/player_flip_11x2.bmp", 1056, 176, 11, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("player_roll",
		"Image/Katana/player/player_roll_7x2.bmp", 658, 128, 7, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("player_run",
		"Image/Katana/player/player_run_10x2.bmp", 880, 128, 10, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("player_jump",
		"Image/Katana/player/player_jump_4x2.bmp", 240, 168, 4, 2, true, RGB(255, 0, 255));	
	ImageManager::GetSingleton()->AddImage("player_hit_ground",
		"Image/Katana/player/player_hurtground_6x2.bmp", 684, 92, 6, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("player_hit_fly",
		"Image/Katana/player/player_hurtfly_4x2.bmp", 400, 132, 4, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("player_grab",
		"Image/Katana/player/player_grab_wall_4x2.bmp", 312, 200, 4, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("player_fall",
		"Image/Katana/player/player_fall_4x2.bmp", 328, 192, 4, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("player_run_to_idle",
		"Image/Katana/player/player_run_to_idle_5x2.bmp", 400, 140, 5, 2, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("player_idle_to_run",
		"Image/Katana/player/player_idle_to_run_4x2.bmp", 336, 128, 4, 2, true, RGB(255, 0, 255));

	image = ImageManager::GetSingleton()->FindImage("idle");
	Animation(PlayerState::idle, true);
	
	pos.x = WINSIZE_X / 2;
	pos.y = WINSIZE_Y / 2;
	currFrame = 0.0f;
	maxFrame = 4.0f;
	moveSpeed = 200.0f;
	isMove = false;
	isAlive = true;
	isDying = false;
	isGround = true;
	jumpHeight = 0.0f;
	velocity = 300.0f;
	camera = new Camera;
	camera->Init(this);
	return S_OK;
}

void Player::Release()
{
}

void Player::Update()
{
	camera->Update();
	if (isGround == false) 
	{
		velocity -= Gravity * 0.03f;
	}

	if (isMove == false && frameRun == false && isAttack ==false)
	{
		Animation(PlayerState::idle,true);
	}

	Move();
	Attack();
	Jumping();

	if (isGround == false && frameRun == false)
	{
		Animation(PlayerState::fall, false);
		frameRun = true;
	}

	currFrame += TimerManager::GetSingleton()->GetElapsedTime()*15;
	if (currFrame > maxFrame)
	{
		currFrame = 0;
		if (frameRun)
		{
			frameRun = false;
		}	
		if (isAttack)
			isAttack = false;
	}	
}

void Player::Render(HDC hdc)
{
	float x = pos.x - camera->GetCameraPos().x;
	float y = pos.y - camera->GetCameraPos().y;
	if (camera)
		camera->Render(hdc);
	if (image)
	{
		if(dir == Direction::LEFT)
			image->FrameRenderFlip(hdc, x, y + jumpHeight, currFrame, 0, true);
		else
			image->FrameRender(hdc, x, y + jumpHeight, currFrame, 0, true);
	}
}

void Player::Jumping()
{
	if (KeyManager::GetSingleton()->IsStayKeyDown('W')) // 위
	{
		pos.y -= 100 * TimerManager::GetSingleton()->GetElapsedTime();
		Animation(PlayerState::jump, false);
		frameRun = true;
		isGround = false;
	}

	if (isGround) return;
	
	if (velocity <= -250) //
	{
		isGround = true;
		velocity = 300;
		jumpHeight = 0.0f;
	}
	jumpHeight -= velocity * 0.0003f;
}

void Player::Move()
{
	if (isDying)
	{
		OnDead();
	}
	else
	{
		if (KeyManager::GetSingleton()->IsStayKeyDown('D')) // 오
		{
			dir = Direction::RIHGT;
			pos.x += moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
			isMove = true;
		}
		else if (KeyManager::GetSingleton()->IsStayKeyDown('A')) // 왼
		{
			dir = Direction::LEFT;
			pos.x -= moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
			isMove = true;
		}
		else if(isMove && isAttack == false)
		{
			Animation(PlayerState::run_to_idle, false);
			frameRun = true;
			isMove = false;
			tick = 0;
		}

		if (KeyManager::GetSingleton()->IsStayKeyDown('S')) // 아래 픽셀 충돌로 
		{
			pos.y += 100 * TimerManager::GetSingleton()->GetElapsedTime();
			isGround = false;
			isMove = true;
		}
		if (isAttack == false) 
		{
			Animation(PlayerState::run, true);
		}	
	}
}

void Player::Attack()
{
	float range = 100;
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON)&& isAttack == false)
	{
		isAttack = true;
		Animation(PlayerState::attack, false);
		mousPos = g_ptMouse;
		currPos.x = pos.x;
		currPos.y = pos.y;
	}
	if (isAttack)
	{
		angle = GetAngle(pos, mousPos);
		
		if (sqrtf(pow(pos.x - currPos.x, 2) + pow(pos.y - currPos.y, 2)) <= 200)
		{
			pos.x += cosf(angle) * 2000 * TimerManager::GetSingleton()->GetElapsedTime();
			pos.y -= sinf(angle) * 2000 * TimerManager::GetSingleton()->GetElapsedTime();
		}
		frameRun = true;
		isGround = false;
	}
}

void Player::OnDead()
{
	if (image)
	{
		BLENDFUNCTION* blendFunc = image->GetBlendFunc();

		if (blendFunc->SourceConstantAlpha > 120)
		{
			blendFunc->SourceConstantAlpha--;
		}
		else
		{
			isDying = false;
			isAlive = false;
		}
	}
}

void Player::Animation(PlayerState ani,bool loop) // 더좋은방법이 생기면 수정
{
	if (loop == false)
		currFrame = 0;
	switch (ani)
	{
	case PlayerState::idle:
		maxFrame = 11;
		image = ImageManager::GetSingleton()->FindImage("player_idle");
		break;
	case PlayerState::run: 
		if (isMove) 
		{
			if (tick == 0)
			{
				maxFrame = 4;
				image = ImageManager::GetSingleton()->FindImage("player_idle_to_run");
				frameRun = true;
				tick++;
			}
			else if (frameRun == false)
			{
				maxFrame = 10;
				image = ImageManager::GetSingleton()->FindImage("player_run");
			}
		}
		break;
	case PlayerState::run_to_idle: 
		maxFrame = 5;
		image = ImageManager::GetSingleton()->FindImage("player_run_to_idle");
		break;
	case PlayerState::attack: 
		maxFrame = 7;
		image = ImageManager::GetSingleton()->FindImage("player_attack");
		break;
	case PlayerState::jump: 
		maxFrame = 4;
		image = ImageManager::GetSingleton()->FindImage("player_jump");
		break;
	case PlayerState::flip: 
		maxFrame = 11;
		image = ImageManager::GetSingleton()->FindImage("player_flip");
		break;
	case PlayerState::grab: 
		maxFrame = 4;
		image = ImageManager::GetSingleton()->FindImage("player_grab");
		break;
	case PlayerState::roll: 
		maxFrame = 7;
		image = ImageManager::GetSingleton()->FindImage("player_roll");
		break;
	case PlayerState::hit_fly: 
		maxFrame = 6;
		image = ImageManager::GetSingleton()->FindImage("player_hit_fly");
		break;
	case PlayerState::hit_ground: 
		maxFrame = 4;
		image = ImageManager::GetSingleton()->FindImage("player_hit_ground");
		break;
	case PlayerState::fall: 
		maxFrame = 4;
		image = ImageManager::GetSingleton()->FindImage("player_fall");
		break;
	}
}
