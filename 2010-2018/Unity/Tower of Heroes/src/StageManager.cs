using UnityEngine;
using System.Collections;

public class StageManager : MonoBehaviour {

    void OnApplicationQuit()
    {
        // 재화 저장
        PlayerPrefs.SetInt("ResGold", GameManager.GetGold(false));
        PlayerPrefs.SetInt("ResRune", GameManager.GetRune(false));

        // 용사 구매여부 저장 및 부활시간 저장
        for (int i = 0; i < GameManager.MAX_HERO_TYPE; ++i)
        {
            PlayerPrefs.SetInt("Hero" + (i + 1).ToString() + "Bought", GameManager.heroesState[i].bBought ? 1 : 0);
            PlayerPrefs.SetFloat("Hero" + (i + 1).ToString() + "ReviveTime", GameManager.heroesState[i].fReviveLeftTime);
        }

        // 업그래이드 정보 저장
        PlayerPrefs.SetInt("HeroLv", GameManager.iHeroLevel);
        PlayerPrefs.SetInt("ItemHealthLv", GameManager.iItemHealthLevel);
        PlayerPrefs.SetInt("ItemJumpCntLv", GameManager.iItemJumpCntLevel); 
        PlayerPrefs.SetInt("ItemInvinTimeLv", GameManager.iItemInvinTimeLevel); 

        // 스테이지 정보 저장
        for (int i = 0; i < GameManager.MAX_STAGE_CNT; ++i)
        {
            PlayerPrefs.SetInt("Stage" + (i + 1).ToString() + "Note", GameManager.stageInfo[i].bGotNote ? 1 : 0);
            PlayerPrefs.SetInt("Stage" + (i + 1).ToString() + "Score", GameManager.stageInfo[i].iHighScore);
        }
    }

	// Use this for initialization
	void Start () {
        // 용사 숨기기 해제
        try
        {
            Transform heroTrans = GameObject.Find("PlayerHero").transform.FindChild("Hero");
            GameObject objHero = null;
            Hero playerHero = null;

            if (GameManager.enSelectedHeroType == Hero.HeroType.HERO_LUKU)
            {
                objHero = heroTrans.FindChild("Hero_Luke").gameObject;
                objHero.SetActive(true);
                playerHero = heroTrans.FindChild("Hero_Luke").GetComponent<Hero>();
            }
            else if (GameManager.enSelectedHeroType == Hero.HeroType.HERO_LULU)
            {
                objHero = heroTrans.FindChild("Hero_Lulu").gameObject;
                objHero.SetActive(true);
                playerHero = heroTrans.FindChild("Hero_Lulu").GetComponent<Hero>();
            }
            else if (GameManager.enSelectedHeroType == Hero.HeroType.HERO_DORAGON)
            {
                objHero = heroTrans.FindChild("Hero_Doragon").gameObject;
                objHero.SetActive(true);
                playerHero = heroTrans.FindChild("Hero_Doragon").GetComponent<Hero>();
            }

            // 카메라 붙이기
            GameObject.Find("PlayerHero").GetComponentInChildren<CameraMove>().CameraTarget = objHero;

            // 용사 레벨에 따른 강화 적용
            playerHero.iHealth += GameManager.GetHealthBonusByLevel();
            playerHero.fMoveSpeed += GameManager.GetSpeedBonusByLevel();
            playerHero.fCooldownReduce += GameManager.GetSpellCooldownReduceByLevel();

            // 아이템에 따른 강화 적용
            playerHero.iHealth += GameManager.GetItemHealthBonusByLevel(false);
            playerHero.iJumpLeft_Default += GameManager.GetItemJumpBonusByLevel();
            playerHero.fInvincibleDuration += GameManager.GetItemInvinTimeBonusByLevel(false);
        }
        catch (System.Exception e) { };
    }
	
	// Update is called once per frame
	void Update () {
        // 사망한 용사 부활시간 감소
        try
        {
            for (int i = 0; i < GameManager.heroesState.Length; ++i)
            {
                GameManager.heroesState[i].fReviveLeftTime -= Time.deltaTime;
                if (GameManager.heroesState[i].fReviveLeftTime < 0.00f) GameManager.heroesState[i].fReviveLeftTime = 0.00f;
            }
        }
        catch (System.Exception e) { };
	}
}
