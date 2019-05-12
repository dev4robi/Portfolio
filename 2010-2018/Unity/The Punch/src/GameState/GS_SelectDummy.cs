using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GS_SelectDummy : GameState {

    private TransImage mainTitle;
    private TransImage leftHand;
    private TransImage rightHand;
    private TransImage selectHelp;

    private int iCurrentDummyIndex;     // Selected Dummy's index number
    private float fDemoAnimPlayTime;    // Demo animation loop delay

    // Use this for initialization
    void Start ()
    {
        if (!Initialization()) Debug.Log("GS_SelectDummy Initialization error!");
    }
	
	// Update is called once per frame
	void Update ()
    {
        PlayDemoAnimation();
        UpdateSelection();
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
        mainScene.ShowUI(1);
        iCurrentDummyIndex = 0;
        HideDummyAll();
        ShowDummy(iCurrentDummyIndex);

        Transform uiTrans = mainScene.uiGrp[1].transform;
        mainTitle = uiTrans.FindChild("Main Title Image").GetComponent<TransImage>();
        leftHand = uiTrans.FindChild("Dummy Select Image").FindChild("Left Image").GetComponent<TransImage>();
        rightHand = uiTrans.FindChild("Dummy Select Image").FindChild("Right Image").GetComponent<TransImage>();
        selectHelp = uiTrans.FindChild("Select Help Image").GetComponent<TransImage>();

        mainTitle.SetPosition(new Vector3(0.00f, -128.0f, 0.00f), 10000.0f);
        leftHand.SetPosition(new Vector3(16.00f, 0.00f, 0.00f), 10000.0f);
        rightHand.SetPosition(new Vector3(-16.00f, 0.00f, 0.00f), 10000.0f);
        selectHelp.SetPosition(new Vector3(500.0f, 32.0f, 0.00f), 10000.0f);

        mainTitle.ChangeAlpha(0.50f, 1.00f);
        leftHand.ChangeAlpha(0.50f, 1.00f);
        rightHand.ChangeAlpha(0.50f, 1.00f);
        selectHelp.ChangeAlpha(0.50f, 1.00f);

        selectHelp.SetPosition(new Vector3(-32.00f, 32.00f, 0.00f), 3000.0f);
        return bNoError;
    }

    private void UpdateSelection()
    {
        float[] inputs = inputMgr.GetInputs();

        if (inputs[(int)GameDatas.HitTarget.HT_HEAD] > 0.00)
        {
            FinishState();
        }
        else if (inputs[(int)GameDatas.HitTarget.HT_UPPER_LEFT] > 0.00 || inputs[(int)GameDatas.HitTarget.HT_LOWER_LEFT] > 0.00)
        {
            ChangeDummy(false);
        }
        else if (inputs[(int)GameDatas.HitTarget.HT_UPPER_RIGHT] > 0.00 || inputs[(int)GameDatas.HitTarget.HT_LOWER_RIGHT] > 0.00)
        {
            ChangeDummy(true);
        }
    }

    public void PlayDemoAnimation()
    {
        if (!((fDemoAnimPlayTime -= Time.deltaTime) > 0.00f))
        {
            fDemoAnimPlayTime = Random.Range(GameDatas.FLOAT_DEMO_ANIMPLAY_DELAY_MIN, GameDatas.FLOAT_DEMO_ANIMPLAY_DELAY_MAX);
            mainScene.objDummy.PlayRandomDemoAnim();
        }
    }

    public void ChangeDummy(bool isNextDummy)
    {
        if (!isNextDummy)
        {
            if ((++iCurrentDummyIndex) >= mainScene.objDummyGrp.Length)
            {
                iCurrentDummyIndex = 0;
            }
        }
        else
        {
            if ((--iCurrentDummyIndex) < 0)
            {
                iCurrentDummyIndex = (mainScene.objDummyGrp.Length - 1);
            }
        }

        mainScene.objDummy = mainScene.objDummyGrp[iCurrentDummyIndex];
        HideDummyAll();
        ShowDummy(iCurrentDummyIndex);
    }

    public void HideDummyAll()
    {
        for (int i = 0; i < mainScene.objDummyGrp.Length; ++i)
        {
            mainScene.objDummyGrp[i].gameObject.SetActive(false);
        }
    }

    public void ShowDummy(int index)
    {
        mainScene.objDummyGrp[index].gameObject.SetActive(true);
    }

    public override void FinishState()
    {
        bNextState = true;

        mainTitle.SetPosition(new Vector3(0.00f, 1000.0f, 0.00f), 1000.0f);
        leftHand.SetPosition(new Vector3(-200.00f, 0.00f, 0.00f), 1000.0f);
        rightHand.SetPosition(new Vector3(200.00f, 0.00f, 0.00f), 1000.0f);
        selectHelp.SetPosition(new Vector3(500.0f, 32.0f, 0.00f), 1000.0f);

        mainTitle.ChangeAlpha(0.50f, 0.00f);
        leftHand.ChangeAlpha(0.50f, 0.00f);
        rightHand.ChangeAlpha(0.50f, 0.00f);
        selectHelp.ChangeAlpha(0.50f, 0.00f);

        Invoke("ChangeNextState", 0.50f);
    }

    protected override void ChangeNextState()
    {
        mainScene.ChangeState(gameObject.AddComponent<GS_MainGame>());
        Destroy(this);
    }
}
