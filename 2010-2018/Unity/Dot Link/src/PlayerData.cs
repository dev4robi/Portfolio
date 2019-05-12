using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class PlayerData
{
    public struct PlayerRank
    {
        public string strName;     // 플레이어명
        public int iScore;         // 달성 점수
        public int iRank;          // 달성 랭크
        public float fPlaytime;    // 레벨 플레이 시간
        public int iLinks;         // 링크 개수
        public int iMaxLink;       // 최대 링크수
        public int iSpS;           // 초당 점수

        
    }

    public enum GameModes
    {
        MODE_NORMAL, MODE_INFINITE, MODE_MEDITATION
    }
    public static int iModeCount = 3;   // 모드 개수

    public static int iSoundMultiply;                                                                                      // 소리 배율
    public static bool bUseEasyColor;                                                                                      // 색맹 모드 사용 여부
    public static GameModes SelectedGameMode = GameModes.MODE_MEDITATION;                                                  // 선택된 게임 모드
    public static DotObject.DotSkin PlayerDotSkin = DotObject.DotSkin.DOT_SKIN_DEFAULT;                                    // 플레이어 Dot 스킨
    public static PlayerRank[][] PlayerSavedRank = RankPaser.GetRankdataFromString(PlayerPrefs.GetString("RankingDatas")); // 플레이어 랭킹
}
