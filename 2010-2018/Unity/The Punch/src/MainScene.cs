using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MainScene : MonoBehaviour
{
    public enum MapType
    {
        MT_TESTPLANE, MT_BRIDGE, MT_TOWN
    }

    private GameState state;            // Current game state
    private MapType enCurMapType;       // Current map type

    public GameObject objMainCamera;    // Main camera
    public GameObject objMainDirLight;  // Main directional light
    public Transform objSoundGrp;       // Sound group object's transform
    public GameObject prefabSound;      // Sound prefab
    public AudioClip[] punchSFX;        // Puch SFX group
    public DummyControll[] objDummyGrp; // Punch dummy object group
    public DummyControll objDummy;      // Punch dummy object
    public GameObject[] uiGrp;          // UI group
    public Material[] mapSkyboxes;      // Map's skybox material group
    public GameObject[] objMaps;        // Map template group

    // For GS_MainGame.cs
    public Sprite[] sprite_startDelayGrp;
    public GameObject prefab_startDelayGrp;

    // For GS_Score.cs
    public TransText[] uiScoreTextGrp;

    // Use this for initialization
    private void Start()
    {
        if (!InitializeGame()) Debug.Log("MainScene Initialization has error!");
    }

    // Update is called once per frame
    private void Update()
    {

    }

    #region # INITIALIZATION #
    public bool InitializeGame()
    {
        bool bNoError = true;

        SetMapType((MapType)Random.Range(1, objMaps.Length));
        ChangeState(gameObject.AddComponent<GS_InsertCoin>());

        if (uiGrp.Length != 4)
        {
            uiGrp = new GameObject[4];
            uiGrp[0] = GameObject.Find("0. Insert Coin UI");
            uiGrp[1] = GameObject.Find("1. Dummy Select UI");
            uiGrp[2] = GameObject.Find("2. Main Game UI");
            uiGrp[3] = GameObject.Find("3. Score Count UI");
        }
        for (int i = 0; i < uiGrp.Length; ++i)
        {
            if (uiGrp[i] == null)
            {
                Debug.Log("MainScene uiGrp[" + i + "] is null!");
                bNoError &= false;
            }
        }
        return bNoError;
    }
    #endregion

    #region # STATE SETTING #
    public void AddCoin(int coin)
    {
        GameDatas.INT_INSERTED_COIN += coin;
    }

    public void SubCoin()
    {
        --GameDatas.INT_INSERTED_COIN;
    }

    public int GetCoin()
    {
        return GameDatas.INT_INSERTED_COIN;
    }

    public void ChangeState(GameState newState)
    {
        state = newState;
        state.SetMainScene(this);
    }

    public void HideAllUI()
    {
        for (int i = 0; i < uiGrp.Length; ++i)
        {
            uiGrp[i].SetActive(false);
        }
    }

    public void ShowUI(int index)
    {
        uiGrp[index].SetActive(true);
    }
    #endregion

    #region # MAP_SETTING #
    public void SetMapType(MapType type)
    {
        HideMapAll();
        enCurMapType = type;

        switch (enCurMapType)
        {
            default:
            case MapType.MT_BRIDGE:
                objMaps[1].SetActive(true);
                RenderSettings.skybox = mapSkyboxes[1];
                break;
            case MapType.MT_TOWN:
                objMaps[2].SetActive(true);
                RenderSettings.skybox = mapSkyboxes[2];
                break;
            case MapType.MT_TESTPLANE:
                objMaps[0].SetActive(true);
                RenderSettings.skybox = mapSkyboxes[0];
                break;
        }
    }

    private void HideMapAll()
    {
        objMainDirLight.SetActive(false);

        for (int i = 0; i < objMaps.Length; ++i)
        {
            objMaps[i].SetActive(false);
        }
    }
    #endregion

    #region # SOUND SFX #
    public void CreateSFX(AudioClip clip)
    {
        AudioSource source = Instantiate(prefabSound, objSoundGrp).GetComponent<AudioSource>();
        source.PlayOneShot(clip);
        Destroy(source.gameObject, clip.length + 0.05f);
    }
    #endregion
}
