using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CountdownTimer : MonoBehaviour {

    public bool bEnable;
    public float fTimeLeft;
    public TransText timerText;

	// Use this for initialization
	void Start ()
    {
        bEnable = false;
	}
	
	// Update is called once per frame
	void Update ()
    {
        if (bEnable == true)
        {
            if (!((fTimeLeft -= Time.deltaTime) > 0.00f))
            {
                fTimeLeft = 0.00f;
            }
        }

        timerText.text.text = ((int)fTimeLeft).ToString();
	}
}
