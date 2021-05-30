#include "Player.h"
#include "Image.h"
#include "CommonFunction.h"
#include "Camera.h"
#include "ItemManager.h"
#include "PlayerEffect.h"

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
	Animation(PlayerState::idle);

	playerEffect = new PlayerEffect[10];

	for (int i = 0; i < 4; i++) 
	{
		playerEffect[i].Init((EffectType)i, this);
	}
	for (int i = 4; i < 7; i++) 
	{
		playerEffect[i].Init(EffectType::dustEffect, this);
	}
	srand((unsigned)time(NULL));
	Worldpos.x = 400;
	Worldpos.y = 100;
	currFrame = 0.0f;
	maxFrame = 4.0f;
	moveSpeed = 200.0f;
	velocity = 80;
	fallForce = 0;
	isMove = false;
	isGround = false;
	isFall = false;
	isPhysics = true;
	Camera::GetSingleton()->Init(this);
	size = 100;
	StartchangeWallIndex = -1;
	attackShape = { 0,0,0,0 };

	
	return S_OK;
}

void Player::Release()
{
	
}

void Player::Update()
{
	Camera::GetSingleton()->Update();
	
	for (int i = 0; i < 7; i++) 
	{
		playerEffect[i].Update();
	}

	if (isPhysics) 
	{
		fallForce -= Gravity * TimerManager::GetSingleton()->GetElapsedTime()* velocity/* * TimerManager::GetSingleton()->GetElapsedTime()*/;
		Worldpos.y -= fallForce * TimerManager::GetSingleton()->GetElapsedTime();
	}

	Clientpos.x = Worldpos.x - Camera::GetSingleton()->GetCameraPos().x;
	Clientpos.y = Worldpos.y - Camera::GetSingleton()->GetCameraPos().y;

	shape.left = Worldpos.x - size/4;
	shape.top = Worldpos.y - size/4;
	shape.right = Worldpos.x + size/5;
	shape.bottom = Worldpos.y + size/4;

	hitShape.left = Worldpos.x - (size-30) /2;
	hitShape.top = Worldpos.y - (size-30) / 2;
	hitShape.right = Worldpos.x + (size-30) / 2;
	hitShape.bottom = Worldpos.y + (size-30) / 2;

	PlayerKeyMove();
	PlayerFSM();
	JumpingEnd();
	GrabEnd();
	PixelCollisionBottom();
	PixelCollisionTop();
	PixelCollisionLeft();
	PixelCollisionRight();
	Getitem();

	currFrame += TimerManager::GetSingleton()->GetElapsedTime() * 15;
	if (currFrame > maxFrame)
	{
		if (isGrab) 
			currFrame = maxFrame-1; 
		else
			currFrame = 0;
		if (frameRun)
			frameRun = false;
		if (isAttack)
		{
			playerstate = PlayerState::fall;
			isAttack = false;
			attackShape = { -100,-100,-100,-100 };
		}
	}

	if (isGround && count == 0)
	{
		playerEffect[3].SetAlive(true);
		playerEffect[3].SetWorldPos(Worldpos.x, Worldpos.y + 30);
		count = 1;
	}
}

void Player::Render(HDC hdc)
{
	if (image)
	{
		if (dir == Direction::LEFT) 
		{
			if (playerstate == PlayerState::grab)
				image->FrameRenderFlip(hdc, Clientpos.x -16, Clientpos.y, currFrame, 0, true);
			else
				image->FrameRenderFlip(hdc, Clientpos.x, Clientpos.y, currFrame, 0, true);
		}
		else if (dir == Direction::RIHGT) 
		{
			if (playerstate == PlayerState::grab)
				image->FrameRender(hdc, Clientpos.x +22, Clientpos.y, currFrame, 0, true);
			else
				image->FrameRender(hdc, Clientpos.x, Clientpos.y, currFrame, 0, true);
			
		}	
	}
	if (playerEffect) 
	{
		for (int i = 0; i < 7; i++)
		{
			playerEffect[i].Render(hdc);
		}
	}
}

Player* Player::Clone()
{
	return new Player();
}

void Player::Run()
{
	if (isMove) 
	{
		int minus = 0;
		int x, y, index;
		if (dir == Direction::LEFT) 
		{
			minus = 1;
			Worldpos.x -= moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
		}
		else if (dir == Direction::RIHGT) 
		{
			minus = -1;
			Worldpos.x += moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
		}

		if (isGround) 
		{
			if (runCount == 0)
			{
				for (int i = 4; i < 7; i++)
				{
					x = (rand() % 30) + 7 + i;
					y = (rand() % 15) - 5;
					playerEffect[i].SetCurrFrame();
					playerEffect[i].SetAlive(true);
					playerEffect[i].SetWorldPos(Worldpos.x + minus * x, Worldpos.y + y);
				}
				playerEffect[4].SetSize(1.5f);
				runCount = 1;
			}
			Animation(PlayerState::run);
		}
		else
			Animation(PlayerState::fall);
	}
}

void Player::RuntoIdle()
{
	frameRun = true;
	if (isFall) return;
	if (!isMove && isGround) 
	{
		Animation(PlayerState::run_to_idle);
		tick = 0;
	}
}

void Player::Attack()
{
	float range = 100;
	Animation(PlayerState::attack);

	attackShape.left = Worldpos.x + (cosf(angle) * 50) - (size - 50) / 2;
	attackShape.top = Worldpos.y - (sinf(angle) * 50) - (size - 50) / 2;
	attackShape.right = Worldpos.x + (cosf(angle) * 50) + (size - 50) / 2;
	attackShape.bottom = Worldpos.y - (sinf(angle) * 50) + (size - 50) / 2;

	if (angle>0 && angle<PI / 2 || angle<0 && angle>(-1 * PI / 2))
	{
		dir = Direction::RIHGT;
	}
	else if (angle > PI / 2 || angle < (-1 * PI / 2)) 
	{
		dir = Direction::LEFT;
	}

	if (isAttack)
	{
		float distance = sqrtf(pow(Worldpos.x - currPos.x, 2) + pow(Worldpos.y - currPos.y, 2));
		if (distance <= range && angle >=0)
		{
			Worldpos.x += cosf(angle) * 500.0f * TimerManager::GetSingleton()->GetElapsedTime();
			Worldpos.y -= sinf(angle) * fallForce * TimerManager::GetSingleton()->GetElapsedTime();
		}
		else if (distance <= range && angle <0) 
		{
			Worldpos.x += cosf(angle) * 500.0f * TimerManager::GetSingleton()->GetElapsedTime();
			Worldpos.y -= sinf(angle) * 500.0f * TimerManager::GetSingleton()->GetElapsedTime();
		}
		frameRun = true;
		isGround = false;
	}
}

void Player::Falling()
{
	if (isFall || !isGround)
	{
		Animation(PlayerState::fall);
		velocity = 80;
		isGrab = false;
	}
}

void Player::Jumping()
{
	if (isJumping) 
	{
		Animation(PlayerState::jump);
		isGround = false;
	}
}

void Player::JumpingEnd()
{
	if (isAttack || isFilp || isFall ) 
		return;
	if (fallForce <= 10 && isJumping)
		isJumping = false;
}
void Player::Roll()
{
	if (isGround) 
	{
		frameRun = true;
		Animation(PlayerState::roll);
		if (dir == Direction::RIHGT )
			Worldpos.x += cosf(DegToRad(0)) * 500 * TimerManager::GetSingleton()->GetElapsedTime();
		else if (dir == Direction::LEFT) 
			Worldpos.x += cosf(DegToRad(180)) * 500 * TimerManager::GetSingleton()->GetElapsedTime();
	}
}
void Player::Flip()
{
	isJumping = false;
	frameRun = true;
	isGrab = false;
	float wallJumpTime = 0;
	wallJumpTime += TimerManager::GetSingleton()->GetElapsedTime();
	if (isFilp) 
	{
		if (leftWall)
			StartchangeWallIndex = 0;
		else if (RightWall)
			StartchangeWallIndex = 1;

		if (StartchangeWallIndex != EndchangeWallIndex)
		{
			playerstate = PlayerState::grab;
			EndchangeWallIndex = StartchangeWallIndex;
			isGrab = true;
			return;
		}

		Animation(PlayerState::flip);

		if (EndchangeWallIndex == 0)
		{
			dir = Direction::RIHGT;
			Worldpos.x += cosf(DegToRad(25)) * 800 * TimerManager::GetSingleton()->GetElapsedTime();
			Worldpos.y -= sinf(DegToRad(25)) * 800 * TimerManager::GetSingleton()->GetElapsedTime();
		}
		else if (EndchangeWallIndex == 1)
		{
			dir = Direction::LEFT;
			Worldpos.x -= cosf(DegToRad(25)) * 800 * TimerManager::GetSingleton()->GetElapsedTime();
			Worldpos.y -= sinf(DegToRad(25)) * 800 * TimerManager::GetSingleton()->GetElapsedTime();
		}
	}
}
void Player::Grab()
{
	if (RightWall == false && leftWall == false)
	{
		isGrab = false;
		Animation(PlayerState::fall);
		return;
	}
	Animation(PlayerState::grab);
	if (fallForce <= -10 && isGrab)
		velocity = 20;
	if (!isGround || isFilp || StartchangeWallIndex >=0) 
		return;
	fallForce = 500;
	isGround = false;
	
}
void Player::GrabEnd()
{
	if (isAttack || playerstate == PlayerState::run_to_idle || playerstate == PlayerState::run) 
	{
		isGrab = false;
		return;
	}	
}
void Player::Getitem()
{
	if (itemManager)
	{
		for (int i = 0; i < itemManager->GetItemList().size(); i++) 
		{
			if (Distance(Worldpos, itemManager->GetItemList()[i]->GetWorldPos()) < 100) 
			{
				if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RBUTTON))
				{
					if (itemCount == 1)
					{
						angle = GetAngle(Worldpos, GetWorldMousePos(Worldpos));
						itemManager->GetItemList()[itemIndex]->SetAngle(angle);
						itemManager->GetItemList()[itemIndex]->SetAlive(true);
						itemManager->GetItemList()[itemIndex]->SetisFire(true);
						itemCount = 0;
						break;
					}
					else
					{
						itemManager->GetItemList()[i]->SetAlive(false);
						itemManager->GetItemList()[i]->SetGetItem(true);
						itemManager->GetItemList()[i]->ChangeImage(true);
						itemIndex = i;
						itemCount = 1;
					}
				}
				itemManager->GetItemList()[i]->SetArrowOn(true);
			}
			else 
			{
				itemManager->GetItemList()[i]->ChangeImage(false);
				itemManager->GetItemList()[i]->SetArrowOn(false);
			}
		}
	}
	
}
void Player::PlayerFSM()
{
	switch (playerstate)
	{
	case PlayerState ::idle:
		Animation(PlayerState::idle);
		break;
	case PlayerState::run:
		Run();
		break;
	case PlayerState::run_to_idle:
		RuntoIdle();
		break;
	case PlayerState::attack:
		Attack();
		break;
	case PlayerState::jump:
		Jumping();
		break;
	case PlayerState::flip:
		Flip();
		break;
	case PlayerState::grab:
		Grab();
		break;
	case PlayerState::roll:
		Roll();
		break;
	case PlayerState::hit_fly:
		break;
	case PlayerState::hit_ground:
		break;
	case PlayerState::fall:
		Falling();
		break;
	}
}

void Player::PlayerKeyMove()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown('W'))
	{
		isJumping = true;
		if (RightWall || leftWall)
		{
			if (isGrab)
			{
				currFrame = 0;
				fallForce = 200;
				isFilp = true;
				playerstate = PlayerState::flip;
				playerEffect[2].SetAlive(true);

				if (dir == Direction::RIHGT)
					playerEffect[2].SetWorldPos(Worldpos.x-10, Worldpos.y);
				else
					playerEffect[2].SetWorldPos(Worldpos.x+10, Worldpos.y);
				return;
			}
			if (KeyManager::GetSingleton()->IsStayKeyDown('W'))
			{
				if (isFilp)return;
				isGrab = true;
				StartchangeWallIndex = -1;
				playerstate = PlayerState::grab;
				return;
			}
		}
		if (isGround==false) return;
		playerEffect[1].SetAlive(true);
		if(dir == Direction::RIHGT)
			playerEffect[1].SetWorldPos(Worldpos.x+10, Worldpos.y+10);
		else
			playerEffect[1].SetWorldPos(Worldpos.x, Worldpos.y+10);
		playerstate = PlayerState::jump;
		currFrame = 0;
		fallForce = 300;
		frameRun = true;
		return;
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
	{
		fallForce = 250 - attCount*80;
		attCount++;
		playerEffect[0].SetAlive(true);
		playerEffect[0].SetCurrFrame();
		angle = GetAngle(Worldpos, GetWorldMousePos(Worldpos));
		currFrame = 0;
		isAttack = true;
		playerstate = PlayerState::attack;
		mousPos = g_ptMouse;
		currPos.x = Worldpos.x;
		currPos.y = Worldpos.y;
		return;
	}
	if (KeyManager::GetSingleton()->IsOnceKeyDown('S'))
	{
		if (black == false) return;
		if (isFall) return;
		currFrame = 0;
		fallForce = 0;
		playerstate = PlayerState::fall;
		frameRun = true;
		isFall = true;
		return;
	}
	if (KeyManager::GetSingleton()->IsOnceKeyUp('D'))
	{
		currFrame = 0;
		isMove = false;
		isGrab = false;
		isFilp = false;
		isJumping = false;
		playerstate = PlayerState::run_to_idle;	
		return;
	}
	else if (KeyManager::GetSingleton()->IsStayKeyDown('D')) // 오
	{
		if (isFilp) return;
		if (isAttack) return;
		dir = Direction::RIHGT;
		if (isGrab && !isGround)
		{
			playerstate = PlayerState::grab;
			return;
		}
		if (KeyManager::GetSingleton()->IsStayKeyDown('S'))
		{
			currPos.x = Worldpos.x;
			playerstate = PlayerState::roll;
			return;
		}
		playerstate = PlayerState::run;
		isMove = true;
		return;
	}
	else if (KeyManager::GetSingleton()->IsOnceKeyUp('A'))
	{
		currFrame = 0;
		isMove = false;
		isGrab = false;
		isFilp = false;
		isJumping = false;
		playerstate = PlayerState::run_to_idle;
		return;
	}
	else if (KeyManager::GetSingleton()->IsStayKeyDown('A')) // 왼
	{
		if (isFilp) return;
		if (isAttack) return;
		dir = Direction::LEFT;
		if (isGrab && !isGround)
		{
			playerstate = PlayerState::grab;
			return;
		}
		if (KeyManager::GetSingleton()->IsStayKeyDown('S'))
		{
			currPos.x = Worldpos.x;
			playerstate = PlayerState::roll;
			return;
		}
		playerstate = PlayerState::run;
		isMove = true;
		return;
	}
	
	if (isGround && isGrab)
	{
		isGrab = false;
		playerstate = PlayerState::idle;
	}
	if (isGround && isJumping == false && isGrab == false && frameRun == false && isFilp == false) 
	{
		playerstate = PlayerState::idle;
	}
	else if (!isGround && isJumping == false&& isAttack == false && isGrab == false && isFilp == false)
		playerstate = PlayerState::fall;
}

void Player::PixelCollisionLeft()
{
	COLORREF color;
	int R, G, B;
	
	float playerwidth = size/2;
	float currPosLeft = shape.left -15;

	for (int i = currPosLeft+10; i < currPosLeft+15; i++)
	{
		color = GetPixel(Camera::GetSingleton()->GetCollisionBG()->GetMemDC(),
			i, Worldpos.y);

		R = GetRValue(color);
		G = GetGValue(color);
		B = GetBValue(color);
		if (!(R == 255 && G == 0 && B == 255))
		{
			if (R == 0 && G == 0 && B == 0)
				break;
			else if (R == 255 && G == 0 && B == 0) 
			{
				leftWall = true;
				break;
			}
			leftWall = false;;
			Worldpos.x = i + playerwidth-18;
			break;
		}
		else if((R == 255 && G == 0 && B == 255))
			leftWall = false;
	}
}

void Player::PixelCollisionRight()
{
	COLORREF color;
	int R, G, B;
	float playerWidth = size/2;
	float currPosRight = shape.right+15;
	for (int i = currPosRight-10; i < currPosRight-5; i++)
	{
		color = GetPixel(Camera::GetSingleton()->GetCollisionBG()->GetMemDC(),
			i, Worldpos.y);

		R = GetRValue(color);
		G = GetGValue(color);
		B = GetBValue(color);
		if (R == 0 && G == 0 && B == 255)
			break;
		if (!(R == 255 && G == 0 && B == 255))
		{
			if (R == 0 && G == 0 && B == 0)
				break;
			else if (R == 255 && G == 0 && B == 0) 
			{
				RightWall = true;
				break;
			}	
			RightWall = false;
			Worldpos.x = i - playerWidth+17;
			break;
		}
		else if ((R == 255 && G == 0 && B == 255)) 
		{
			RightWall = false;
		}
	}
}

void Player::PixelCollisionTop()
{
	COLORREF color;
	int R, G, B;
	float playerHeight = 36;
	float currPosTop = Worldpos.y - playerHeight;
	for (int i = currPosTop - 5; i < currPosTop + 5; i++)
	{
		color = GetPixel(Camera::GetSingleton()->GetCollisionBG()->GetMemDC(),
			Worldpos.x, i);

		R = GetRValue(color);
		G = GetGValue(color);
		B = GetBValue(color);
		if (!(R == 255 && G == 0 && B == 255))
		{
			if (R == 0 && G == 0 && B == 0)
				break;
			velocity = 100;
			Worldpos.y = i + playerHeight+6;
			break;
		}
	}
}

void Player::PixelCollisionBottom()
{
	if (isJumping)return;
	COLORREF color;
	int R, G, B;
	float playerHeight = 36;
	float currPosBottom = Worldpos.y + playerHeight;
	for (int i = currPosBottom-10; i < currPosBottom+10; i++)
	{
		color = GetPixel(Camera::GetSingleton()->GetCollisionBG()->GetMemDC(),
			Worldpos.x, i);

		R = GetRValue(color);
		G = GetGValue(color);
		B = GetBValue(color);
			
		if (!(R == 255 && G == 0 && B == 255))
		{
			if ((R == 0 && G == 0 && B == 0))
				black = true;
			else
				black = false;
			velocity = 80;
			if ((R == 0 && G == 0 && B == 0) && isAttack) 
			{
				attCount = 0; 
				break;
			}
			if (isAttack) 
			{
				attCount = 0;
				//Worldpos.y = i - playerHeight-10;
				break;
			}
			if ((R == 0 && G == 0 && B == 0) && isFall) 
				break;

			fallForce = 0;
			Worldpos.y = i - playerHeight+4;
			isGround = true;
			isFall = false;
			isFilp = false;
			//attCount = 0;
			break;
		}
		else if ((R == 255 && G == 0 && B == 255))
		{
			isGround = false;
		}	
	}
}

void Player::Animation(PlayerState ani) // 더좋은방법이 생기면 수정
{
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
				currFrame = 0;
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
		runCount = 0;
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
		isGround = false;
		if(isMove==false)
			count = 0;
		image = ImageManager::GetSingleton()->FindImage("player_fall");
		break;
	}
}
