using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class MainMenu : MonoBehaviour {

    public GameObject uiobjMainMenuGrp;
    public GameObject uiobjGameStartGrp;
    public GameObject uiobjSettingsGrp;

    // Use this for initialization
    void Start()
    {
        // 사용자 데이터 로드
        GameData.LoadDefaultDatas();

        // 버전 텍스트 변경
        uiobjMainMenuGrp.transform.FindChild("Version Text").GetComponent<Text>().text = GameData.GameVersion;
    }

    // Update is called once per frame
    void Update()
    {

    }

    public void GameStart()
    {
        SceneManager.LoadScene("MainGame");
    }

    public void UI_SelectGameStart()
    {
        uiobjMainMenuGrp.SetActive(false);
        uiobjGameStartGrp.SetActive(true);
    }

    public void UI_Settings()
    {
        uiobjMainMenuGrp.SetActive(false);
        uiobjSettingsGrp.SetActive(true);
    }

    public void UI_StartNormalMode()
    {
        PlayerData.SelectedGameMode = PlayerData.GameModes.MODE_NORMAL;
        GameStart();
    }

    public void UI_StartInfiniteMode()
    {
        PlayerData.SelectedGameMode = PlayerData.GameModes.MODE_INFINITE;
        GameStart();
    }

    public void UI_StartMeditationMode()
    {
        PlayerData.SelectedGameMode = PlayerData.GameModes.MODE_MEDITATION;
        GameStart();
    }

    public void UI_MainMenuButton()
    {
        uiobjMainMenuGrp.SetActive(true);
        uiobjGameStartGrp.SetActive(false);
        uiobjSettingsGrp.SetActive(false);
        GameData.SaveDefaultDatas();
    }
}
