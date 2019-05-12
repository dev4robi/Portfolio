using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GS_MainGame : GameState {

    private enum SubState
    {
        STATE_READY, STATE_PUNCH, STATE_FINISHER, STATE_END
    }
    
    private float fStartDelayTime;
    private CountdownTimer mainTimer;
    private TransText timerText;
    private Transform startTimerImageTrans;
    private bool bFinisherState;
    private SubState enSubState;
    private bool[] startDelayImageShown;
    private Sprite[] startDelayImageGrp;
    private TransImage finisherImg;
    private float fEndDelayTime;

    // Use this for initialization
    void Start ()
    {
        if (!Initialization()) Debug.Log("GS_MainGame Initialization error!");
    }
	
	// Update is called once per frame
	void Update ()
    {
        if (enSubState == SubState.STATE_READY)
        {
            if ((fStartDelayTime -= Time.deltaTime) <= 0.00f) // When ready state timeover
            {
                fStartDelayTime = 0.00f;
                enSubState = SubState.STATE_PUNCH;
            }

            if (!startDelayImageShown[0] && fStartDelayTime <= 3.00f)
            {
                CreateDelayTimerImg(0);
            }
            else if (!startDelayImageShown[1] && fStartDelayTime <= 2.00f)
            {
                CreateDelayTimerImg(1);
            }
            else if (!startDelayImageShown[2] && fStartDelayTime <= 1.00f)
            {
                CreateDelayTimerImg(2);
            }
            else if (!startDelayImageShown[3] && fStartDelayTime <= 0.00f)
            {
                CreateDelayTimerImg(3);
            }
        }
        else if (enSubState == SubState.STATE_PUNCH)
        {
            if ((mainTimer.fTimeLeft -= Time.deltaTime) <= 0.00f) // When punch state timeover
            {
                mainTimer.fTimeLeft = GameDatas.FLOAT_FINISHERPUNCH_TIMER;
                enSubState = SubState.STATE_FINISHER;

                finisherImg.ChangeAlpha(0.75f, 0.00f);
                finisherImg.ChangeScale(0.75f, 2.00f);
                fStartDelayTime = 1.50f;
            }
            else
            {
                UpdateNormalPunchState();
            }
        }
        else if (enSubState == SubState.STATE_FINISHER)
        {
            if ((mainTimer.fTimeLeft -= Time.deltaTime) <= 0.00f) // When finisher state timeover
            {
                mainTimer.fTimeLeft = 0.00f;
                enSubState = SubState.STATE_END;
            }
            else
            {
                if (!((fStartDelayTime -= Time.deltaTime) > 0.00f))
                {
                    UpdateFinisherPunchState();
                }
            }
        }
        else
        {
            if ((fEndDelayTime -= Time.deltaTime) <= 0.00f) // When finisherdown(end) state timeover
            {
                mainTimer.timerText.ChangeAlpha(0.50f, 0.00f);
                FinishState();
            }
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

        enSubState = SubState.STATE_READY;

        mainScene.HideAllUI();
        mainScene.ShowUI(2);

        bFinisherState = false;
        fStartDelayTime = GameDatas.FLOAT_GAMESTART_DELAY_TIMER;
        fEndDelayTime = 5.00f;
        startDelayImageShown = new bool[mainScene.sprite_startDelayGrp.Length];
        startDelayImageGrp = new Sprite[mainScene.sprite_startDelayGrp.Length];
        for (int i = 0; i < mainScene.sprite_startDelayGrp.Length; ++i)
        {
            startDelayImageShown[i] = false;
            startDelayImageGrp[i] = mainScene.sprite_startDelayGrp[i];
        }

        Transform uiTrans = mainScene.uiGrp[2].transform;
        mainTimer = uiTrans.FindChild("Game Timer Image").GetComponent<CountdownTimer>();
        mainTimer.timerText.ChangeAlpha(0.00f, 0.00f);
        mainTimer.timerText.ChangeAlpha(0.50f, 1.00f);        
        mainTimer.fTimeLeft = GameDatas.FLOAT_NORMALPUNCH_TIMER;
        finisherImg = uiTrans.FindChild("Finisher Image").GetComponent<TransImage>();
        finisherImg.ChangeAlpha(0.00f, 1.00f);
        finisherImg.ChangeScale(0.00f, 0.00f);

        startTimerImageTrans = uiTrans.FindChild("Start Timer Image");

        return bNoError;
    }

    public void CreateDelayTimerImg(int index)
    {
        GameObject uiObject;
        TransImage transImg;

        uiObject = Instantiate(mainScene.prefab_startDelayGrp, startTimerImageTrans);
        transImg = uiObject.GetComponent<TransImage>();
        transImg.rectTransform.anchoredPosition = Vector3.zero;
        transImg.sprite = startDelayImageGrp[index];
        transImg.ChangeAlpha(0.50f, 0.00f);
        transImg.ChangeScale(0.50f, 2.00f);
        Destroy(uiObject, 1.00f);
        startDelayImageShown[index] = true;
    }

    public void UpdateNormalPunchState()
    {
        float[] inputs = inputMgr.GetInputs();

        if (inputs[(int)GameDatas.HitTarget.HT_HEAD] > 0.00f)
        {
            mainScene.objDummy.HitByNormal(GameDatas.HitTarget.HT_HEAD, inputs[(int)GameDatas.HitTarget.HT_HEAD]);
        }
        if (inputs[(int)GameDatas.HitTarget.HT_UPPER_RIGHT] > 0.00f)
        {
            mainScene.objDummy.HitByNormal(GameDatas.HitTarget.HT_UPPER_RIGHT, inputs[(int)GameDatas.HitTarget.HT_UPPER_RIGHT]);
        }
        if (inputs[(int)GameDatas.HitTarget.HT_UPPER_LEFT] > 0.00f)
        {
            mainScene.objDummy.HitByNormal(GameDatas.HitTarget.HT_UPPER_LEFT, inputs[(int)GameDatas.HitTarget.HT_UPPER_LEFT]);
        }
        if (inputs[(int)GameDatas.HitTarget.HT_LOWER_RIGHT] > 0.00f)
        {
            mainScene.objDummy.HitByNormal(GameDatas.HitTarget.HT_LOWER_RIGHT, inputs[(int)GameDatas.HitTarget.HT_LOWER_RIGHT]);
        }
        if (inputs[(int)GameDatas.HitTarget.HT_LOWER_LEFT] > 0.00f)
        {
            mainScene.objDummy.HitByNormal(GameDatas.HitTarget.HT_LOWER_LEFT, inputs[(int)GameDatas.HitTarget.HT_LOWER_LEFT]);
        }
    }

    public void UpdateFinisherPunchState()
    {
        float[] inputs = inputMgr.GetInputs();
        bool bFinisherDone = false;

        if (inputs[(int)GameDatas.HitTarget.HT_HEAD] > 0.00f)
        {
            mainScene.objDummy.HitByFinisher(GameDatas.HitTarget.HT_HEAD, inputs[(int)GameDatas.HitTarget.HT_HEAD]);
            bFinisherDone = true;
        }
        if (inputs[(int)GameDatas.HitTarget.HT_UPPER_RIGHT] > 0.00f)
        {
            mainScene.objDummy.HitByFinisher(GameDatas.HitTarget.HT_UPPER_RIGHT, inputs[(int)GameDatas.HitTarget.HT_UPPER_RIGHT]);
            bFinisherDone = true;
        }
        if (inputs[(int)GameDatas.HitTarget.HT_UPPER_LEFT] > 0.00f)
        {
            mainScene.objDummy.HitByFinisher(GameDatas.HitTarget.HT_UPPER_LEFT, inputs[(int)GameDatas.HitTarget.HT_UPPER_LEFT]);
            bFinisherDone = true;
        }
        if (inputs[(int)GameDatas.HitTarget.HT_LOWER_RIGHT] > 0.00f)
        {
            mainScene.objDummy.HitByFinisher(GameDatas.HitTarget.HT_LOWER_RIGHT, inputs[(int)GameDatas.HitTarget.HT_LOWER_RIGHT]);
            bFinisherDone = true;
        }
        if (inputs[(int)GameDatas.HitTarget.HT_LOWER_LEFT] > 0.00f)
        {
            mainScene.objDummy.HitByFinisher(GameDatas.HitTarget.HT_LOWER_LEFT, inputs[(int)GameDatas.HitTarget.HT_LOWER_LEFT]);
            bFinisherDone = true;
        }

        if (bFinisherDone) mainTimer.fTimeLeft = 0.00f;
    }

    public override void FinishState()
    {
        bNextState = true;
        
        Invoke("ChangeNextState", 0.50f);
    }

    protected override void ChangeNextState()
    {
        mainScene.ChangeState(gameObject.AddComponent<GS_Score>());
        Destroy(this);
    }
}
