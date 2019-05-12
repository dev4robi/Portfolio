using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Ranking : MonoBehaviour
{
    public Text[] uiTextGrp;

	// Use this for initialization
	void Start ()
    {
        if (!Initailze()) Debug.Log("Ranking Initialization has error!");
	}
	
	// Update is called once per frame
	void Update ()
    {
		
	}

    #region # INITAIALIZATION #
    public bool Initailze()
    {
        bool bNoError = true;

        if (uiTextGrp.Length != GameDatas.INT_SAVED_SCORES.Length)
        {
            uiTextGrp = new Text[GameDatas.INT_SAVED_SCORES.Length];
            uiTextGrp[0] = gameObject.transform.FindChild("Rank 1").GetComponent<Text>();
            uiTextGrp[1] = gameObject.transform.FindChild("Rank 2").GetComponent<Text>();
            uiTextGrp[2] = gameObject.transform.FindChild("Rank 3").GetComponent<Text>();
        }
        for (int i = 0; i < uiTextGrp.Length; ++i)
        {
            if (uiTextGrp[i] == null)
            {
                bNoError &= false;
                break;
            }
        }

        return bNoError;
    }
    #endregion

    public void RenewRankingScore()
    {
        for (int i = 0; i < GameDatas.INT_SAVED_SCORES.Length; ++i)
        {
            uiTextGrp[i].text = GameDatas.INT_SAVED_SCORES[i].ToString();
        }
    }
}
