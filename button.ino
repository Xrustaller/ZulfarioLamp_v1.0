boolean brightDirection, speedDirection, scaleDirection;
//byte numHold;

//#include <effectTicker.ino>

void buttonTick()
{
	touch1.tick();
	touch2.tick();
	touch3.tick();
	touch4.tick();

	if (touch1.isSingle() && touch2.isSingle() && touch3.isSingle() && touch4.isSingle())
	{
		if (ONflag)
		{
			ONflag = false;
			changePower();
		}
		else
		{
			ONflag = true;
			changePower();
		}
	
	}

	if (ONflag)
	{
		// если включено
		if (touch2.isSingle())
		{
			if (++currentMode >= MODE_AMOUNT) currentMode = 0;
			FastLED.setBrightness(modes[currentMode].brightness);
			loadingFlag = true;
			//settChanged = true;
			FastLED.clear();
			delay(1);
		}
		if (touch4.isSingle())
		{
			if (--currentMode < 0) currentMode = MODE_AMOUNT - 1;
			FastLED.setBrightness(modes[currentMode].brightness);
			loadingFlag = true;
			//settChanged = true;
			FastLED.clear();
			delay(1);
		}

		if ((touch1.hasClicks()) && (touch1.getClicks() == 5))
		{
			// если было пятикратное нажатие на кнопку, то производим сохранение параметров // && (touch1.hasClicks())
			if (EEPROM.read(0) != 102) EEPROM.write(0, 102);
			if (EEPROM.read(1) != currentMode) EEPROM.write(1, currentMode); // запоминаем текущий эфект
			for (byte x = 0; x < MODE_AMOUNT; x++)
			{
				// сохраняем настройки всех режимов
				if (EEPROM.read(x * 3 + 11) != modes[x].brightness) EEPROM.write(x * 3 + 11, modes[x].brightness);
				if (EEPROM.read(x * 3 + 12) != modes[x].speed) EEPROM.write(x * 3 + 12, modes[x].speed);
				if (EEPROM.read(x * 3 + 13) != modes[x].scale) EEPROM.write(x * 3 + 13, modes[x].scale);
			}
			// индикация сохранения

			ONflag = false;
			changePower();
			delay(200);
			ONflag = true;
			changePower();
		}

		if (touch1.isHolded3())
		{
			// изменение масштаба "scale" при тройном нажатии и удержании кнопки
			scaleDirection = true;
			numHold = 3;
		}

		if (touch3.isHolded3())
		{
			// изменение масштаба "scale" при тройном нажатии и удержании кнопки
			scaleDirection = false;
			numHold = 3;
		}

		if (touch1.isHolded2())
		{
			// изменение скорости "speed" при двойном нажатии и удержании кнопки
			speedDirection = true;
			numHold = 2;
		}

		if (touch3.isHolded2())
		{
			// изменение скорости "speed" при двойном нажатии и удержании кнопки
			speedDirection = false;
			numHold = 2;
		}

		if (touch1.isHolded())
		{
			// изменение яркости при удержании кнопки
			brightDirection = true;
			numHold = 1;
		}
		if (touch3.isHolded())
		{
			// изменение яркости при удержании кнопки
			brightDirection = false;
			numHold = 1;
		}
		
		if (touch1.isStep() || touch3.isStep())
		{
			if (numHold != 0) numHold_Timer = millis();
			loadingFlag = true;
			switch (numHold)
			{
			case 1:
				modes[currentMode].brightness = constrain(modes[currentMode].brightness + (modes[currentMode].brightness / 25 + 1) * (brightDirection * 2 - 1), 1, 255);
				break;

			case 2:
				modes[currentMode].speed = constrain(modes[currentMode].speed + (modes[currentMode].speed / 25 + 1) * (speedDirection * 2 - 1), 1, 255);
				break;

			case 3:
				modes[currentMode].scale = constrain(modes[currentMode].scale + (modes[currentMode].scale / 25 + 1) * (scaleDirection * 2 - 1), 1, 255);
				break;
			}
		}
		if ((millis() - numHold_Timer) > numHold_Time)
		{
			numHold = 0;
			numHold_Timer = millis();
		}
		FastLED.setBrightness(modes[currentMode].brightness);
	}
}
