using UnityEngine;
using System.Collections;

public static class GameManager {
    public struct StageInfo
    {
        public string strName;     // 스테이지 이름
        public string strDirection;// 진행 방향
        public string strNoteMsg;  // 일지 내용
        public int iHighScore;     // 최고 점수
        public bool bGotNote;      // 일지 획득여부
        public int iReqScore;      // 해금 필요 점수
        public int iLevel;         // 난이도
    }
    public struct HeroState
    {
        public int iRuneCost;          // 구매 가격
        public bool bBought;          // 구매 여부
        public float fReviveLeftTime; // 남은 부활 시간
    }

    public static bool bTesterMode = false;              // 테스트 모드
    public static Hero.HeroType enSelectedHeroType;     // 선택한 용사

    public static int iGold;                    // 소유한 골드
    public static int iRune;                    // 소유한 룬
    public static int iGainGold;                // 획득한 골드
    public static int iGainRune;                // 획득한 룬
    public static int iScore;                   // 획득한 점수
    public static int iHeroLevel = 1;           // 용사 레벨
    public static int iItemHealthLevel = 0;     // 생명력 보너스 아이템 레벨
    public static int iItemJumpCntLevel = 0;    // 점프횟수 보너스 아이템 레벨
    public static int iItemInvinTimeLevel = 0;  // 무적시간 보너스 아이템 레벨
    public static StageInfo[] stageInfo;        // 스테이지 정보
    public static HeroState[] heroesState;      // 용사 상태

    public const int MAX_STAGE_CNT = 3;
    public const int MAX_HERO_TYPE = 3;

    public static void InitHeroData()
    {
        if (heroesState == null)
        {
            heroesState = new HeroState[MAX_HERO_TYPE];

            // 루크
            heroesState[0].iRuneCost = 0;
            heroesState[0].bBought = true;
            heroesState[0].fReviveLeftTime = 0.00f;

            // 루루
            heroesState[1].iRuneCost = 200;
            heroesState[1].bBought = false;
            heroesState[1].fReviveLeftTime = 0.00f;

            // 도라곤
            heroesState[2].iRuneCost = 500;
            heroesState[2].bBought = false;
            heroesState[2].fReviveLeftTime = 0.00f;
        }

        // 로드
        try
        {
            for (int i = 1; i < MAX_HERO_TYPE; ++i)
            {
                heroesState[i].bBought = PlayerPrefs.GetInt("Hero" + (i + 1).ToString() + "Bought") == 1 ? true : false;
                heroesState[i].fReviveLeftTime = PlayerPrefs.GetFloat("Hero" + (i + 1).ToString() + "ReviveTime");
            }
            GameManager.SetGold(PlayerPrefs.GetInt("ResGold"), false);
            GameManager.SetRune(PlayerPrefs.GetInt("ResRune"), false);
        }
        catch (PlayerPrefsException e)
        {
            Debug.Log(e.Message.ToString());
        }

        // 아이템 및 용사 레벨
        GameManager.iHeroLevel = PlayerPrefs.GetInt("HeroLv") == 0 ? 1 : PlayerPrefs.GetInt("HeroLv");
        GameManager.iItemHealthLevel = PlayerPrefs.GetInt("ItemHealthLv");
        GameManager.iItemJumpCntLevel = PlayerPrefs.GetInt("ItemJumpCntLv");
        GameManager.iItemInvinTimeLevel = PlayerPrefs.GetInt("ItemInvinTimeLv");
    }

    public static void InitStageData()
    {
        if (stageInfo == null)
        {
            stageInfo = new StageInfo[MAX_STAGE_CNT];

            // 스테이지 1
            stageInfo[0] = new StageInfo();
            stageInfo[0].strDirection = "↑";
            stageInfo[0].strName = "탑 입성!";
            stageInfo[0].iHighScore = 0;
            stageInfo[0].bGotNote = false;
            stageInfo[0].iReqScore = 0;
            stageInfo[0].iLevel = 2;
            stageInfo[0].strNoteMsg = "7322년 1월 7일 : 탑에 입성하다\n 부와 명성! 과연 탑 정상에서 얻을 수 있을까? 모두들 헛된 꿈이라고 말하지만, 그렇다고 도전하지 않을 내가 아니지!" +
                "\n... 탑이 함정 투성이지만 준비해온 도구를 활용한다면 충분히 올라갈 수 있을 것으로 보여. - L";
            
            // 스테이지 2
            stageInfo[1] = new StageInfo();
            stageInfo[1].strDirection = "→";
            stageInfo[1].strName = "위험천만 구름다리!";
            stageInfo[1].iHighScore = 0;
            stageInfo[1].bGotNote = false;
            stageInfo[1].iReqScore = 10000;
            stageInfo[1].iLevel = 3;
            stageInfo[1].strNoteMsg = "7322년 1월 8일 : 다 부셔져가는 구름 다리\n 위로 올라가는 계단이 끊겨서 구름 다리를 건너 옆의 탑으로 이동했다.\n" +
                "누군가 마법으로 상자를 띄워 놔서 무사히 건너갈 수 있었지만, 마법이 내가 내려 올 때도 이 마법이 유지되어 있을까...? - D";

            // 스테이지 3
            stageInfo[2] = new StageInfo();
            stageInfo[2].strDirection = "~↑~↑~";
            stageInfo[2].strName = "독물이 차오른다!";
            stageInfo[2].iHighScore = 0;
            stageInfo[2].bGotNote = false;
            stageInfo[2].iReqScore = 20000;
            stageInfo[2].iLevel = 5;
            stageInfo[2].strNoteMsg = "7322년 1월 9일 : 하수탑?\n 눈 앞의 래버를 당긴것이 문제였다! 엄청난 양의 독극물이 쏟아져 나오면서 탑의 하층부를 채우기 시작했다. 다행히 나는 상층부에서 이를 지켜봤지만 다음에 올 용사들이 고생좀 하겠는걸...? -  LK";
        }

        // 데이터 로드
        string stageStr;
        for (int i = 0; i < MAX_STAGE_CNT; ++i)
        {
            stageStr = "Stage";
            stageInfo[i].bGotNote = PlayerPrefs.GetInt(stageStr + (i + 1).ToString() + "Note") == 1 ? true : false;
            stageInfo[i].iHighScore = PlayerPrefs.GetInt(stageStr + (i + 1).ToString() + "Score");
        }
    }

    static void StageStartInitPlayer()
    {
        SetGold(0, true);
        SetRune(0, true);
    }

    public static int GetGold(bool isGainGold)
    {
        if (isGainGold == false) return iGold;
        else return iGainGold;
    }

    public static void SetGold(int amount, bool isGainGold)
    {
        if (isGainGold == false) iGold = amount;
        else iGainGold = amount;
    }

    public static int GetRune(bool isGainRune)
    {
        if (isGainRune == false) return iRune;
        else return iGainRune;
    }

    public static void SetRune(int amount, bool isGainRune)
    {
        if (isGainRune == false) iRune = amount;
        else iGainRune = amount;
    }

    public static int GetLevelUpCost()
    {
        return 500 + (iHeroLevel - 1) * 100;
    }

    public static int GetHealthBonusByLevel()
    {
        return (iHeroLevel - 1) * 5;
    }

    public static float GetSpeedBonusByLevel()
    {
        float fBonusSpeed = (iHeroLevel - 1) * 0.1f;
        if (fBonusSpeed > 4.00f) fBonusSpeed = 4.00f;

        return fBonusSpeed;
    }

    public static float GetSpellCooldownReduceByLevel()
    {
        float fBonusReduce = (iHeroLevel - 1) * 0.15f;

        return fBonusReduce;
    }

    public static int GetItemHealthBonusByLevel(bool bNextLevel)
    {
        if (bNextLevel == false)
            return iItemHealthLevel * 50;
        else
            return (iItemHealthLevel + 1) * 50;
    }

    public static int GetItemJumpBonusByLevel()
    {
        return iItemJumpCntLevel;
    }

    public static float GetItemInvinTimeBonusByLevel(bool bNextLevel)
    {
        if (bNextLevel == false)
            return iItemInvinTimeLevel * 0.33f;
        else
            return (iItemInvinTimeLevel + 1) * 0.33f;
    }

    public static int GetItemCostByLevel(int itemNumber)
    {
        if (itemNumber == 1)
        {
            return 5 + (iItemHealthLevel) * 15;
        }
        else if (itemNumber == 2)
        {
            return 15;
        }
        else
        {
            return 25 + (iItemInvinTimeLevel) * 25;
        }
    }
}
