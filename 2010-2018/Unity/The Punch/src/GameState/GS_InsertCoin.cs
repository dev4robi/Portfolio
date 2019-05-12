using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GS_InsertCoin : GameState {

    private TransImage uiMainTitle;      // Main title UI's TransImage script
    private TransText uiInsertCoin;      // Insert coin UI's TransText script
    private Outline uiInsertCoinOutline; // Insert coin UI's Outline script
    private TransImage uiRanking;        // Ranking UI's TransImage script
    private float fDemoAnimPlayTime;     // Demo animation loop delay

	// Use this for initialization
	private void Start ()
    {
        if (!Initialization()) Debug.Log("GS_InsertCoin Initialization has error!");
    }
	
	// Update is called once per frame
	private void Update ()
    {
        if (!bNextState)
        {
            PlayDemoAnimation();
            UpdateInsertCoins();
            FinishState();
        }
        inputMgr.InitInputs();
    }

    #region # INITIALIZATION #
    public bool Initialization()
    {
        bool bNoError = true;

        if (mainScene == null)
        {
            mainScene = GameObject.Find("Main Game").GetComponent<MainScene>();
        }
        if (mainScene == null) bNoError &= false;

        mainScene.HideAllUI();
        mainScene.ShowUI(0);
        uiInsertCoin = mainScene.uiGrp[0].transform.FindChild("Insert Coin Image").FindChild("Insert Coin Text").GetComponent<TransText>();
        uiInsertCoinOutline = uiInsertCoin.gameObject.GetComponent<Outline>();
        uiRanking = mainScene.uiGrp[0].transform.FindChild("Ranking Image").GetComponent<TransImage>();
        uiRanking.SetPosition(new Vector3(0.00f, 32.00f, 0.00f), 1000.0f);
        uiRanking.GetComponent<Ranking>().RenewRankingScore();
        uiMainTitle = mainScene.uiGrp[0].transform.FindChild("Main Title Image").GetComponent<TransImage>();
        uiMainTitle.ChangeAlpha(1.00f, 1.00f);
        fDemoAnimPlayTime = Random.Range(GameDatas.FLOAT_DEMO_ANIMPLAY_DELAY_MIN, GameDatas.FLOAT_DEMO_ANIMPLAY_DELAY_MAX);
        bNextState = false;

        return bNoError;
    }
    #endregion

    public void UpdateInsertCoins()
    {
        if (uiInsertCoin.text.color.a == 1.00f)
        {
            uiInsertCoin.ChangeAlpha(0.75f, 0.00f);
        }
        else if (uiInsertCoin.text.color.a == 0.00f)
        {
            uiInsertCoin.ChangeAlpha(0.50f, 1.00f);
        }
        uiInsertCoin.text.text = GameDatas.STRING_INSERT_COIN + " (" + mainScene.GetCoin() + ")";
    }

    public void PlayDemoAnimation()
    {
        if (!((fDemoAnimPlayTime -= Time.deltaTime) > 0.00f))
        {
            fDemoAnimPlayTime = Random.Range(GameDatas.FLOAT_DEMO_ANIMPLAY_DELAY_MIN, GameDatas.FLOAT_DEMO_ANIMPLAY_DELAY_MAX);
            mainScene.objDummy.PlayRandomDemoAnim();
        }
    }

    public override void FinishState()
    {
        if (mainScene.GetCoin() > 0 && inputMgr.CheckInputs())
        {
            bNextState = true;

            uiMainTitle.ChangeAlpha(0.50f, 0.00f);
            uiInsertCoin.ChangeAlpha(0.50f, 0.00f);
            uiInsertCoinOutline.effectColor = new Color(uiInsertCoinOutline.effectColor.r, uiInsertCoinOutline.effectColor.g, uiInsertCoinOutline.effectColor.b, 0.00f);
            uiRanking.SetPosition(new Vector3(0.00f, -500.0f, 0.00f), 1000.0f);
            mainScene.SubCoin();

            Invoke("ChangeNextState", 0.50f);
        }
    }

    protected override void ChangeNextState()
    {
        mainScene.ChangeState(gameObject.AddComponent<GS_SelectDummy>());
        Destroy(this);
    }
}
