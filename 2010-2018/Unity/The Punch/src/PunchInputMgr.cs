using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PunchInputMgr : MonoBehaviour {

    private float[] fInputs; // Power input data from device

	// Use this for initialization
	void Start ()
    {
        fInputs = new float[(int)GameDatas.HitTarget.HT_LOWER_LEFT + 1];
        for (int i = 0; i < (int)GameDatas.HitTarget.HT_LOWER_LEFT; ++i) fInputs[i] = 0.00f;
	}
	
	// Update is called once per frame
	void Update ()
    {
        if (GameDatas.BOOL_TESTMODE) TestInputFromKeyboard();
    }

    public void InitInputs()
    {
        for (int i = 0; i < fInputs.Length; ++i)
        {
            fInputs[i] = 0.00f;
        }
    }

    public void SetInputs(float[] input)
    {
        for (int i = 0; i < input.Length; ++i)
        {
            fInputs[i] = input[i];
        }
    }

    public float[] GetInputs()
    {
        float[] fCopyAry = new float[fInputs.Length];

        fCopyAry = fInputs.Clone() as float[];

        InitInputs();

        return fCopyAry;
    }

    public bool CheckInputs()
    {
        for (int i = 0; i < fInputs.Length; ++i)
        {
            if (fInputs[i] > 0.00f)
            {
                InitInputs();
                return true;
            }
        }

        return false;
    }

    public void TestInputFromKeyboard()
    {
        if (Input.GetKeyDown(KeyCode.Q))
        {
            fInputs[0] = 1.00f;
        }

        if (Input.GetKeyDown(KeyCode.A))
        {
            fInputs[1] = 1.00f;
        }

        if (Input.GetKeyDown(KeyCode.S))
        {
            fInputs[2] = 1.00f;
        }

        if (Input.GetKeyDown(KeyCode.Z))
        {
            fInputs[3] = 1.00f;
        }

        if (Input.GetKeyDown(KeyCode.X))
        {
            fInputs[4] = 1.00f;
        }
    }
}
