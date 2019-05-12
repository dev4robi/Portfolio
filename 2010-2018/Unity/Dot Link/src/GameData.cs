using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class GameData
{
    public const string GameVersion = "1.0.0";

    public enum GameMode
    {
        MODE_NORMAL, MODE_INFINITE, MODE_MEDITATION
    }

    public const float fNormalModeMaxTime = 12.100f;

    public static Color ColorBase = new Color(120.00f / 255.00f, 49.00f / 255.00f, 49.00f / 255.00f);
    public static Color ColorBaseLight = GetExColorFromBase(ColorBase, true);
    public static Color ColorBaseDark = GetExColorFromBase(ColorBase, false);

    public static Color GetExColorFromBase(Color baseColor, bool isLight)
    {
        float H, S, V;
        Color.RGBToHSV(ColorBase, out H, out S, out V);

        if (isLight)
        {
            S -= (60.00f / 255.00f);
            V += (10.00f / 255.00f);
        }
        else
        {
            S += (45.00f / 255.00f);
            V -= (70.00f / 255.00f);
        }

        return Color.HSVToRGB(H, S, V);
    }

    public static Color ColorWhite     = new Color(235.00f / 255.00f, 235.00f / 255.00f, 235.00f / 255.00f);
    public static Color ColorRed       = new Color(240.00f / 255.00f,  80.00f / 255.00f,  90.00f / 255.00f);
    public static Color ColorGreen     = new Color( 35.00f / 255.00f, 180.00f / 255.00f,  75.00f / 255.00f);
    public static Color ColorBlue      = new Color( 90.00f / 255.00f, 100.00f / 255.00f, 210.00f / 255.00f);
    public static Color ColorYellow    = new Color(242.00f / 255.00f, 187.00f / 255.00f,  40.00f / 255.00f);

    public static float fBoardWidth = 12.0f;                          // 보드 너비
    public static float fBoardHeight = 12.0f;                         // 보드 높이
    public static float fBoardHalfWidth = fBoardWidth / 2.00f;        // 보드 반너비
    public static float fBoardHalfHeight = fBoardHeight / 2.00f;      // 보드 반높이
    public static int iBoardRow = 9;                                  // 보드 열 수
    public static int iBoardCol = 9;                                  // 보드 행 수
    public static float fWidthDist = fBoardWidth / (iBoardCol - 1);    // Dot사이의 간격
    public static float fHeightDist = fBoardHeight / (iBoardRow - 1);

    // 보드 데이터를 갱신하는 메서드
    public static void RenewBoardData() 
    {
        fBoardHalfWidth = fBoardWidth / 2.00f;
        fBoardHalfHeight = fBoardHeight / 2.00f;
        fWidthDist = fBoardWidth / (iBoardCol - 1);
        fHeightDist = fBoardHeight / (iBoardRow - 1);
    }

    // 사용자 지정 기본 데이터들을 로드
    public static void LoadDefaultDatas()
    {
        iBoardRow = iBoardCol = PlayerPrefs.GetInt("BoardSize", 9);
        RenewBoardData();
        PlayerData.iSoundMultiply = PlayerPrefs.GetInt("SoundMultiply", 100);
        if (PlayerPrefs.GetInt("EasyColor") == 1) PlayerData.bUseEasyColor = true;
        else PlayerData.bUseEasyColor = false;
        PlayerData.PlayerDotSkin = (DotObject.DotSkin)PlayerPrefs.GetInt("DotSkin", 0);
    }

    // 사용자 지정 기본 데이터들을 저장
    public static void SaveDefaultDatas()
    {
        PlayerPrefs.SetInt("BoardSize", iBoardRow);
        PlayerPrefs.SetInt("SoundMultiply", PlayerData.iSoundMultiply);
        if (PlayerData.bUseEasyColor) PlayerPrefs.SetInt("EasyColor", 1);
        else PlayerPrefs.SetInt("EasyColor", 0);
        PlayerPrefs.SetInt("DotSkin", (int)PlayerData.PlayerDotSkin);
    }

    public static string[] RankString = { "SSS", "SS", "S", "A", "B", "C", "D", "F" };

    public static Sprite SpriteCircle     = Resources.Load<Sprite>("Prefabs/Dot/Circle");
    public static Sprite SpriteRect       = Resources.Load<Sprite>("Prefabs/Dot/Rect");
    public static Sprite SpriteStar       = Resources.Load<Sprite>("Prefabs/Dot/Star");
    public static Sprite SpriteHexagon    = Resources.Load<Sprite>("Prefabs/Dot/Hexagon");
    public static Sprite SpriteClober     = Resources.Load<Sprite>("Prefabs/Dot/Clober");
    public static Sprite SpriteHeart      = Resources.Load<Sprite>("Prefabs/Dot/Heart");
    public static Sprite SpriteGravity_A  = Resources.Load<Sprite>("UI/GravityDirection_A");
    public static Sprite SpriteGravity_B  = Resources.Load<Sprite>("UI/GravityDirection_B");

    public static GameObject pfabDot      = Resources.Load("Prefabs/Dot/Dot") as GameObject;                 // Dot오브젝트 프리팝
    public static GameObject pfabLine     = Resources.Load("Prefabs/LinkLine/LinkLine") as GameObject;       // LinkLine오브젝트 프리팝
    public static GameObject pfabSoundEft = Resources.Load("Prefabs/SoundEffect/SoundEffect") as GameObject; // SoundEffect 프리팝
    public static GameObject pfabScoreTextUI = Resources.Load("UI/DynamicScoreText") as GameObject;          // DynamicScoreText 프리팝

    public static AudioClip[] OctaveAudio = { Resources.Load<AudioClip>("Sounds/Base_Do"),
                                              Resources.Load<AudioClip>("Sounds/Base_Re"),
                                              Resources.Load<AudioClip>("Sounds/Base_Mi"),
                                              Resources.Load<AudioClip>("Sounds/Base_Fa"),
                                              Resources.Load<AudioClip>("Sounds/Base_Sol"),
                                              Resources.Load<AudioClip>("Sounds/Base_Ra"),
                                              Resources.Load<AudioClip>("Sounds/Base_Ti") };
}