using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class GameDatas
{
    public enum HitTarget // The position of sensor insertion
    {
        HT_HEAD, HT_UPPER_RIGHT, HT_UPPER_LEFT, HT_LOWER_RIGHT, HT_LOWER_LEFT
    }

    #region # ANIMATION DATA #
    public const int INT_HEADHIT_ANIM_COUNT = 3;
    public const int INT_UPPERHIT_RIGHT_ANIM_COUNT = 4;
    public const int INT_UPPERHIT_LEFT_ANIM_COUNT = 5;
    public const int INT_LOWERHIT_RIGHT_ANIM_COUNT = 2;
    public const int INT_LOWERHIT_LEFT_ANIM_COUNT = 1;
    public const int INT_FINISHER_ANIM_COUNT = 5;
    public const int INT_DEMO_ANIM_COUNT = 12;
    public const string STR_RANDOMINDEX_NAME = "Anim Index";
    public const string STR_HEADHIT_TRIG_NAME = "Head Hit Trigger"; 
    public const string STR_UPPERHIT_RIGHT_TRIG_NAME = "Upper Hit Right Trigger";
    public const string STR_UPPERHIT_LEFT_TRIG_NAME = "Upper Hit Left Trigger";
    public const string STR_LOWERHIT_RIGHT_TRIG_NAME = "Lower Hit Right Trigger";
    public const string STR_LOWERHIT_LEFT_TRIG_NAME = "Lower Hit Left Trigger";
    public const string STR_FINISHERHIT_TRIG_NAME = "Finisher Hit Trigger";
    public const string STR_DEMO_TRIG_NAME = "Demo Taunt Trigger";
    #endregion

    #region # GAMEPLAY DATA #
    public static MainScene OBJ_MAINSCENE = GameObject.Find("Main Game").GetComponent<MainScene>();
    public const float FLOAT_GAMESTART_DELAY_TIMER = 3.00f; 
    public const float FLOAT_SCOREBOARD_DELAY_TIMER = 0.50f;
    public const float FLOAT_NORMALPUNCH_TIMER = 5.00f;
    public const float FLOAT_FINISHERPUNCH_TIMER = 4.00f;
    public const float FLOAT_DEMO_ANIMPLAY_DELAY_MIN = 3.00f;
    public const float FLOAT_DEMO_ANIMPLAY_DELAY_MAX = 6.00f;
    public static readonly float[] FLOAT_FINISHER_LEVELS = { 0.90f, 0.70f, 0.50f, 0.30f, 0.00f };
    #endregion

    #region # SCORE DATA #
    public const int INT_MAX_SCORE = 999;
    public const int INT_MIN_SCORE = 100;
    public static readonly int[] INT_DEFAULT_SCORES = { 900, 700, 500 };
    public static int[] INT_SAVED_SCORES = new int[] { INT_DEFAULT_SCORES[0], INT_DEFAULT_SCORES[1], INT_DEFAULT_SCORES[2] };
    #endregion

    #region # SETTING #
    public static bool BOOL_TESTMODE = true;
    public static int INT_INSERTED_COIN = (BOOL_TESTMODE == true ? 99 : 0);
    #endregion

    #region # STRING DATA #
    public const string STRING_PUNCH_TO_MAIN = "Punch to main...";
    public const string STRING_SCORE_RANKED = "Ranked!";
    public const string STRING_INSERT_COIN = "Insert Cash";
    public const string STRING_HIT = "Hit";
    #endregion
}
