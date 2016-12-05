#include "main.h"

void PlayAnimation(Player_Animation &Ani, const int character_down_state)
{
	switch (character_down_state) {
	case 0://가만히 서있을 때
		Ani.left_leg_x = 0;
		Ani.left_leg_y = 0;
		Ani.left_leg_z = 0;
		Ani.left_knee_x = 0;
		Ani.right_leg_x = 0;
		Ani.right_leg_y = 0;
		Ani.right_leg_z = 0;
		Ani.right_knee_x = 0;
		break;
	case 1://전진,후진
		Ani.left_leg_z = 0;
		Ani.right_leg_z = 0;
		if (Ani.legtimer < 150)
		{
			Ani.legtimer += 2;
			if (Ani.legtimer < 75)
			{
				Ani.left_leg_x = Ani.legtimer - 45;
				Ani.right_leg_x = 30 - Ani.legtimer;
				Ani.left_knee_x = 0;
				if (Ani.legtimer < 38)
					Ani.right_knee_x = Ani.legtimer;
				else
					Ani.right_knee_x = 38 - (Ani.legtimer - 38);
			}
			else
			{
				Ani.left_leg_x = 105 - Ani.legtimer;
				Ani.right_leg_x = Ani.legtimer - 120;
				Ani.right_knee_x = 0;

				if (Ani.legtimer < 113)
					Ani.left_knee_x = Ani.legtimer - 75;
				else
					Ani.left_knee_x = 38 - (Ani.legtimer - 113);
			}
		}
		else
			Ani.legtimer = 0;
		break;
	case 2://좌우이동
		break;
	case 3://점프
		Ani.left_leg_x = -75;
		Ani.left_leg_y = 0;
		Ani.left_leg_z = 40;
		Ani.left_knee_x = 130;
		Ani.right_leg_x = -75;
		Ani.right_leg_y = 0;
		Ani.right_leg_z = -15;
		Ani.right_knee_x = 130;
		break;
	}
}