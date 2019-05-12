using UnityEngine;
using System.Collections;

public class HealthScript : MonoBehaviour
{
    public string name;
    public Sprite Icon;
    public string etc = null;

    public int iBonusDollar;
    public int iBonusCosmic;

    public int maxArmor;
    public int maxShield;

    public float currentArmor;
    public float currentShield;

    public float laserMultiplier;
    public float missileMultiplier;

    public GameObject DestroyEffect;
    public float despawnDelay;

    public float shieldCharge_Interval;
    float current_interval;

    // Use this for initialization
    void Start()
    {
        currentShield = maxShield;
        currentArmor = maxArmor;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        if(current_interval < 0)
        {
            currentShield += 1;
            if (currentShield > maxShield)
                currentShield = maxShield;

            current_interval = shieldCharge_Interval;
        }
        else
        {
            current_interval -= Time.deltaTime;
        }
    }

    void doDamage(object[] damage) // object[0] = shooter tower / object[1] = damage amount
    {
        GameObject obj = damage[0] as GameObject;

        if (currentShield > 0)
        {
            currentShield -= (float)damage[1];
            if (currentShield < 0)
                currentShield = 0;

            obj.GetComponent<LTH.Abs_Tower>().SendMessage("AddTotalDealt", (float)damage[1]);
        }
        else if (currentShield <= 0)
        {
            currentArmor -= (float)damage[1];

            obj.GetComponent<LTH.Abs_Tower>().SendMessage("AddTotalDealt", (float)damage[1]);
        }

        if (currentArmor <= 0)
        {
            DestoryedByPlayer(obj);
        }

        current_interval = shieldCharge_Interval;
    }

    void doLaserDamage(object[] damage) // object[0] = shooter tower / object[1] = damage amount
    {
        GameObject obj = damage[0] as GameObject;

        if (currentShield > 0)
        {
            float dmg = (float)damage[1] * (float)laserMultiplier;
            currentShield -= dmg;

            if (currentShield < 0)
                currentShield = 0;

            obj.GetComponent<LTH.Abs_Tower>().SendMessage("AddTotalDealt", dmg);
        }
        else if (currentShield <= 0)
        {
            currentArmor -= (float)damage[1];
            obj.GetComponent<LTH.Abs_Tower>().SendMessage("AddTotalDealt", (float)damage[1]);
        }

        if (currentArmor <= 0)
        {
            DestoryedByPlayer(obj);
        }

        current_interval = shieldCharge_Interval;
    }

    void doMissileDamage(object[] damage) // object[0] = shooter tower / object[1] = damage amount
    {
        GameObject obj = damage[0] as GameObject;

        if (currentShield > 0)
        {
            float dmg = (float)damage[1] * missileMultiplier;
            currentShield -= dmg;

            if (currentShield < 0)
                currentShield = 0;

            obj.GetComponent<LTH.Abs_Tower>().SendMessage("AddTotalDealt", dmg);
        }
        else if (currentShield <= 0)
        {
            currentArmor -= (float)damage[1];
            obj.GetComponent<LTH.Abs_Tower>().SendMessage("AddTotalDealt", (float)damage[1]);
        }

        if (currentArmor <= 0)
        {
            DestoryedByPlayer(obj);
        }

        current_interval = shieldCharge_Interval;
    }

    void doShieldOnlyDamage(int damage)
    {
        if (currentShield > 0)
        {
            currentShield -= damage;

            if (currentShield < 0)
                currentShield = 0;
        }

        current_interval = shieldCharge_Interval;
    }

    void DestoryedByPlayer(GameObject objKiller)
    {
        // 파괴 효과
        if (DestroyEffect)
        {
            GameObject effect = Instantiate(DestroyEffect, gameObject.transform.position, Quaternion.identity) as GameObject;
            effect.transform.parent = null;
        }

        // 처치한 포탑 킬수 증가
        objKiller.GetComponent<LTH.Abs_Tower>().SendMessage("AddKillCount", (int)1);

        // 은행 잔고 복구
        BankRobbery script = GetComponent<BankRobbery>();
        if (script != null && script.bRobbed == true && script.bRobbed == true)
        {
            // 잔고 복구
            StaticManager.game.objBank.GetComponent<LTH.EnvBank>().AddBalanceLeft(script.iCurRobAmount);

            // 메시지
            Debug.Log(name + "이(가) 강탈한 " + script.iCurRobAmount + "$ 를 되찾았습니다!"); // test

            // 복구 이펙트 생성
            StaticManager.game.CreateResEffect(gameObject.transform.position, "Balance", script.iCurRobAmount);
        }

        // 보상 획득
        StaticManager.game.AddResDollar(iBonusDollar);
        StaticManager.game.AddResCosmic(iBonusCosmic);

        // 보상 이펙트 생성
        if (iBonusDollar > 0) StaticManager.game.CreateResEffect(gameObject.transform.position, "Dollor", iBonusDollar);
        if (iBonusCosmic > 0) StaticManager.game.CreateResEffect(gameObject.transform.position, "Cosmic", iBonusCosmic);

        // 오브젝트 파괴
        Destroy(this.gameObject);
    }

    void doRecoverShield()
    {
        currentShield = maxShield;
    }
}