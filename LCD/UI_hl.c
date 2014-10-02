void UI_hl (void)
{
	static const double thresholdStep = 0.5;
	static const double attackStep = 0.5;
	static const double ratioStep = 1;
	static const double releaseStep = 0.05;
	static const double mugStep = 0.5;
	
	static const double thresholdMax = 15;
	static const double thresholdMin = -20;
	static const double attackMin = 0.1;
	static const double attackMax = 30;
	static const double ratioMin = 0.1;
	static const double ratioMax = 20;
	static const double releaseMin = 0.1;
	static const double releaseMax = 1.2;
	static const double mugMin = -5;
	static const double mugMax = 12;
	
	int currentLevel = 1;
	int level1CurrentSelection = 0;
	int level2CurrentSelection = 0;
	double threshold = 0;
	double attack = 15;
	double ratio = 10;
	double release = 0.6;
	double mug = 0;
	int autoEN = 0;
	int bypassEN = 0;
	
	int buttonFlag = 0;
	int rotaryPressFlag = 0;
	int rotaryFlag = 0;
	
	char* strLine;
	
	DisplayLine ( 0, "       Manual       " );
	
	while ( 1 )
	{
		switch ( Rotary () )
		{
		case 0:			// No change
			rotaryFlag = 0;
			break;
		case 1:			// Clockwise
			if ( rotaryFlag == 1 )
				break;
			rotaryFlag = 1;
			switch ( currentLevel )
			{
				case 1:
					level1CurrentSelection ++;
					switch (level1CurrentSelection)
					{
						case 0:
							DisplayLine ( 0, "       Manual       " );
							break;
						case 1:
							DisplayLine ( 0, "      Automode      " );
							break;
						case 2:
							DisplayLine ( 0, "       Bypass       " );
							break;
					}
					break;
				case 2:
					switch (level1CurrentSelection)
					{
						case 0:
							if ( level2CurrentSelection == 4 )
								level2CurrentSelection = 0;
							else
								level2CurrentSelection ++;
							switch ( level2CurrentSelection )
							{
								case 0:
									DisplayLine ( 1, "      Threshold     " );
									break;
								case 1:
									DisplayLine ( 1, "     MakeUp Gain    " );
									break;
								case 2:
									DisplayLine ( 1, "       Attack       " );
									break;
								case 3:
									DisplayLine ( 1, "      Release       " );
									break;
								case 4:
									DisplayLine ( 1, "       Ratio        " );
									break;
							}
							break;
						case 1:
							if ( autoEN == 1 )
							{
								autoEN = 0;
								DisplayLine ( 1, "      Disabled      " );
							}
							else
							{
								autoEN = 1;
								DisplayLine ( 1, "      Enabled       " );
							}	
						case 2:
							if ( bypassEN == 1 )
							{
								bypassEN = 0;
								DisplayLine ( 1, "      Disabled      " );
							}
							else
							{
								bypassEN = 1;
								DisplayLine ( 1, "      Enabled       " );
							}	
					}
					break;
				case 3:
					switch ( level2CurrentSelection )
					{
						case 0:
							if ( threshold == thresholdMax )
								break;
							threshold += thresholdStep;
							sprintf ( strLine, "%g", threshold );
							DisplayLine ( 2, strLine);
							break;
						case 1:
							if ( mug == mugMax )
								break;
							mug += mugStep;
							sprintf ( strLine, "%g", mug );
							DisplayLine ( 2, strLine);
							break;
						case 2:
							if ( attack == attackMax)
								break;
							attack += attackStep;
							sprintf ( strLine, "%g", attack );
							DisplayLine ( 2, strLine);
							break;
						case 3:
							if ( release == releaseMax )
								break;
							release += releaseStep;
							sprintf ( strLine, "%g", release );
							DisplayLine ( 2, strLine );
							break;
						case 4:
							if ( ratio == ratioMax )
								break;
							ratio += ratioStep;
							sprintf ( strLine, "%g", ratio );
							DisplayLine ( 2, strLine );
							break;
						default:
							break;
					}
				case 4:
					// No level 4, yet
					break;
				default:
					break;
			}
		case 2:			// Counter-Clockwise
			if ( rotaryFlag == 1 )
				break;
			rotaryFlag = 1;
			switch ( currentLevel )
			{
				case 1:
					level1CurrentSelection --;
					switch ( level1CurrentSelection )
					{
						case 0:
							DisplayLine ( 0, "       Manual       " );
							break;
						case 1:
							DisplayLine (0, "      Automode      " );
							break;
						case 2:
							DisplayLine ( 0, "       Bypass       " );
							break;
					}
					break;
				case 2:
					switch ( level2CurrentSelection )
					{
						case 0:
							if ( level2CurrentSelection == 0 )
								level2CurrentSelection = 4;
							else
								level2CurrentSelection --;
							switch ( level2CurrentSelection )
							{
								case 0:
									DisplayLine ( 1, "      Threshold     " );
									break;
								case 1:
									DisplayLine ( 1, "     MakeUp Gain    " );
									break;
								case 2:
									DisplayLine ( 1, "       Attack       " );
									break;
								case 3:
									DisplayLine ( 1, "      Release       " );
									break;
								case 4:
									DisplayLine ( 1, "       Ratio        " );
									break;
							}
						case 1:
							if ( autoEN == 1 )
							{
								autoEN = 0;
								DisplayLine ( 1, "      Disabled      " );
							}
							else
							{
								autoEN = 1;
								DisplayLine ( 1, "      Enabled       " );
							}	
						case 2:
							if ( bypassEN == 1 )
							{
								bypassEN = 0;
								DisplayLine ( 1, "      Disabled      " );
							}
							else
							{
								bypassEN = 1;
								DisplayLine ( 1, "      Enabled       " );
							}	
					}
					break;
				case 3:
					switch ( level2CurrentSelection )
					{
						case 0:
							if ( threshold == thresholdMin )
								break;
							threshold -= thresholdStep;
							sprintf ( strLine, "%g", threshold );
							DisplayLine ( 2, strLine );
							break;
						case 1:
							if ( mug == mugMin)
								break;
							mug -= mugStep;
							sprintf ( strLine, "%g", mug );
							DisplayLine ( 2, strLine );
							break;
						case 2:
							if ( attack == attackMin )
								break;
							attack -= attackStep;
							sprintf ( strLine, "%g", attack );
							DisplayLine ( 2, strLine );
							break;
						case 3:
							if ( release == releaseMin )
								break;
							release -= releaseStep;
							sprintf ( strLine, "%g", release );
							DisplayLine ( 2, strLine );
							break;
						case 4:
							if ( ratio == ratioMin )
								break;
							ratio -= ratioStep;
							sprintf ( strLine, "%g", ratio );
							break;
						default:
							break;
					}
				case 4:
					// No level 4, yet
					break;
				default:
					break;
			}
		default:
			break;
		}
		if ( GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_10 ) > 0 && buttonFlag == 0 )
		{
			buttonFlag = 1;
			switch ( currentLevel )
			{
				case 1:
					// Do Nothing
					break;
				case 2:
					currentLevel --;
					DisplayLine ( 1, "                    " );
					if ( autoEN == 1 )
						autoEN = 0;
					if ( bypassEN == 1 )
						bypassEN = 0;
					break;
				case 3:
					currentLevel --;
					DisplayLine ( 2, "                    " );
					break;
				case 4:
					// Do Nothing
					break;
			}
		}
		else
			buttonFlag = 0;
		if ( GPIO_ReadInputDataBit( GPIOE, GPIO_Pin_9 ) > 0 && rotaryPressFlag == 0 )
		{
			rotaryPressFlag = 1;
			switch ( currentLevel )
			{
				case 1:
					currentLevel ++;
					switch ( level1CurrentSelection )
					{
						case 0:
							DisplayLine ( 1, "     Threshold      " );
							level2CurrentSelection = 0;
							break;
						case 1:
							DisplayLine ( 1, "      Disabled      " );
							break;
						case 2:
							DisplayLine ( 1, "      Disabled      " );
							break;
					}
					break;
				case 2:
					currentLevel ++;
					break;
				case 3:
					currentLevel ++;
					break;
				case 4:
					// Do Nothing
					break;
			}
		}
		else
			rotaryPressFlag = 0;
	}
}