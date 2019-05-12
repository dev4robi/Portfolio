using UnityEngine;
using System.Collections;

public class BossScript : MonoBehaviour {
    

    public enum BossType
    {
        NeverDie, InitialS, Betrayer, Escaper
    }

    public BossType bossType;
    BankRobbery bankrobbery;

    public float initialS_slowMultiplier;

    public float Nameless_recoverInterval;
    float current_interval;
    public int betrayer_initNode;

    public GameObject Escaper;
    public int Escaper_initNode;
    bool mounted = false;

    // Use this for initialization
    void Start () {
        bankrobbery = GetComponent<BankRobbery>();

        if(bossType == BossType.Betrayer)
        {
            if (Escaper)
            {
                GameObject gameManager = GameObject.FindGameObjectWithTag("GameManager");
                GameManager managerScript = gameManager.GetComponent<GameManager>();

                GameObject escaper = Instantiate(Escaper, managerScript.objWaypointNode.transform.GetChild(Escaper_initNode).position, Quaternion.identity) as GameObject;

                escaper.transform.parent = GameObject.Find("Enemy Group").transform;


            }
        }
        else if(bossType == BossType.Escaper)
        {
            EnemyMovement enemyMove = GetComponent<EnemyMovement>();

            GameObject gameManager = GameObject.FindGameObjectWithTag("GameManager");
            GameManager managerScript = gameManager.GetComponent<GameManager>();

            enemyMove.Route = managerScript.objWaypointNode;
        }
    }
	
	// Update is called once per frame
	void Update () {

        if (bossType == BossType.InitialS)
        {

            if (bankrobbery.bRobbed)
            {
                this.gameObject.SendMessage("doSlow", initialS_slowMultiplier);
                this.gameObject.SendMessage("doSlowDuration", 999999);
            }
        }
        else if(bossType == BossType.NeverDie)
        {
            if(current_interval < 0)
            {
                current_interval = Nameless_recoverInterval;
                this.gameObject.SendMessage("doRecoverShield");
            }
            else
            {
                current_interval -= Time.deltaTime;
            }
        }
        else if (bossType == BossType.Betrayer)
        {
            EnemyMovement move = GetComponent<EnemyMovement>();

            if (move.currentNode < betrayer_initNode)
            {
                this.gameObject.SendMessage("setCurrentNode", betrayer_initNode);
            }
           

        }

    }


    void OnCollisionEnter(Collision other)
    {

        if (bossType == BossType.Escaper)
        {
            if (other.gameObject.tag == "Enemy")
            {

                if (!mounted)
                {
                    EnemyMovement moveScript = GetComponent<EnemyMovement>();
                    moveScript.setCurrentNode(Escaper_initNode);
                    moveScript.enabled = true;

                    mounted = true;
                }
            }
        }

        else if (bossType == BossType.Betrayer)
        {
            if (other.gameObject.tag == "Enemy")
            {
                BankRobbery otherRobbery = other.gameObject.GetComponent<BankRobbery>();
                BankRobbery robbery = GetComponent<BankRobbery>();

                otherRobbery.iCurRobAmount = robbery.iCurRobAmount;
                otherRobbery.bRobbed = true;

                Destroy(this.gameObject);

            }
        }
    }


}
