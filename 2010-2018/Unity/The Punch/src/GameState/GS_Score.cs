using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GS_Score : GameState {

    private int iLastScoreIndex;
    private bool bFinisherScoreShown;
    private float fScoreDelay;
    private TransText[] scoreGrp;
    private TransText scoreHead;
    private TransText scoreUpperR;
    private TransText scoreUpperL;
    private TransText scoreLowerR;
    private TransText scoreLowerL;
    private TransImage finisherScoreImg;
    private TransText finisherScoreText;
    private TransText punchToMainText;
    private TransImage blackFadeoutImg;

    void Start()
    {
        if (!Initialization()) Debug.Log("GS_Score Initialization error!");
    }

    // Update is called once per frame
    void Update ()
    {
        if (iLastScoreIndex < mainScene.uiScoreTextGrp.Length)
        {
            if ((fScoreDelay -= Time.deltaTime) <= 0.00f)
            {
                fScoreDelay = GameDatas.FLOAT_SCOREBOARD_DELAY_TIMER;

                // Head -> UpperR -> UpperL -> LowerR -> LowerL
                scoreGrp[iLastScoreIndex].text.text = (mainScene.objDummy.hitCounter[iLastScoreIndex] + GameDatas.STRING_HIT); 
                scoreGrp[iLastScoreIndex].ChangeSize(0.00f, 125);
                scoreGrp[iLastScoreIndex].ChangeScale(0.50f, 1.00f);

                if (mainScene.objDummy.hitCounter[iLastScoreIndex] > 10)
                {
                    scoreGrp[iLastScoreIndex].ChangeSize(0.10f, 130);
                    scoreGrp[iLastScoreIndex].text.text += "!";
                }
                else if (mainScene.objDummy.hitCounter[iLastScoreIndex] > 30)
                {
                    scoreGrp[iLastScoreIndex].ChangeSize(0.10f, 135);
                    scoreGrp[iLastScoreIndex].text.text += "!!";
                }

                ++iLastScoreIndex;
            }
        }
        else if (!bFinisherScoreShown)
        {
            bFinisherScoreShown = true;

            int iFinisherPower = (int)(mainScene.objDummy.fFinisherPower);

            finisherScoreImg.ChangeScale(0.20f, 1.00f);
            finisherScoreImg.ChangeAlpha(0.20f, 1.00f);
            finisherScoreText.text.text = iFinisherPower.ToString();
            finisherScoreText.ChangeScale(0.25f, 1.00f);

            RenewRanking(iFinisherPower);
        }
        else
        {
            // Waiting for punching...
            if (punchToMainText.text.color.a == 1.00f)
            {
                punchToMainText.ChangeAlpha(0.75f, 0.00f);
            }
            else if (punchToMainText.text.color.a == 0.00f)
            {
                punchToMainText.ChangeAlpha(0.50f, 1.00f);
            }

            if (inputMgr.CheckInputs()) FinishState();
        }

        inputMgr.InitInputs();
	}

    public bool Initialization()
    {
        bool bNoError = true;

        if (mainScene == null)
        {
            mainScene = GameObject.Find("Main Game").GetComponent<MainScene>();
        }
        if (mainScene == null) bNoError &= false;

        mainScene.HideAllUI();
        mainScene.ShowUI(3);

        iLastScoreIndex = 0;
        bFinisherScoreShown = false;
        fScoreDelay = GameDatas.FLOAT_SCOREBOARD_DELAY_TIMER;

        scoreGrp = new TransText[mainScene.uiScoreTextGrp.Length];
        for (int i = 0; i < mainScene.uiScoreTextGrp.Length; ++i)
        {
            scoreGrp[i] = mainScene.uiScoreTextGrp[i];
            mainScene.uiScoreTextGrp[i].ChangeScale(0.00f, 0.00f);
        }

        scoreHead = mainScene.uiScoreTextGrp[0];
        scoreUpperR = mainScene.uiScoreTextGrp[1];
        scoreUpperL = mainScene.uiScoreTextGrp[2];
        scoreLowerR = mainScene.uiScoreTextGrp[3];
        scoreLowerL = mainScene.uiScoreTextGrp[4];

        finisherScoreImg = mainScene.uiGrp[3].transform.FindChild("Finisher Score Image").GetComponent<TransImage>();
        finisherScoreImg.ChangeAlpha(0.00f, 0.00f);
        finisherScoreImg.ChangeScale(0.00f, 0.00f);
        finisherScoreText = finisherScoreImg.transform.FindChild("Finisher Score Text").GetComponent<TransText>();
        punchToMainText = mainScene.uiGrp[3].transform.FindChild("Punch to Main Text").GetComponent<TransText>();
        blackFadeoutImg = mainScene.uiGrp[3].transform.FindChild("Black Fadeout Image").GetComponent<TransImage>();

        return bNoError;
    }

    public void RenewRanking(int finisherPower)
    {
        bool bRankChanged = false;

        if (GameDatas.INT_SAVED_SCORES[0] < finisherPower)
        {
            GameDatas.INT_SAVED_SCORES[2] = GameDatas.INT_SAVED_SCORES[1];
            GameDatas.INT_SAVED_SCORES[1] = GameDatas.INT_SAVED_SCORES[0];
            GameDatas.INT_SAVED_SCORES[0] = finisherPower;
            bRankChanged = true;
        }
        else if (GameDatas.INT_SAVED_SCORES[1] < finisherPower)
        {
            GameDatas.INT_SAVED_SCORES[2] = GameDatas.INT_SAVED_SCORES[1];
            GameDatas.INT_SAVED_SCORES[1] = finisherPower;
            bRankChanged = true;
        }
        else if (GameDatas.INT_SAVED_SCORES[2] < finisherPower)
        {
            GameDatas.INT_SAVED_SCORES[2] = finisherPower;
            bRankChanged = true;
        }

        if (bRankChanged)
        {
            TransText rankedText = finisherScoreText.transform.FindChild("Got Rank Text").GetComponent<TransText>();
            rankedText.text.text = GameDatas.STRING_SCORE_RANKED;
            rankedText.ChangeScale(0.25f, 1.00f);
        }
    }

    public override void FinishState()
    {
        bNextState = true;

        blackFadeoutImg.ChangeAlpha(0.33f, 1.00f);

        Invoke("ChangeNextState", 0.50f);
    }

    protected override void ChangeNextState()
    {
        SceneManager.LoadScene(SceneManager.GetActiveScene().name); // when game is done, reload main scene again
    }
}
