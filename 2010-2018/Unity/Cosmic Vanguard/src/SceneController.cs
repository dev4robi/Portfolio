using UnityEngine;
using System.Collections;

public class SceneController : MonoBehaviour {


    public GameObject player1;
    public GameObject player2;
    public GameObject testEnemy;
    public GameObject enemySpline;
    // Use this for initialization
    void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
        if (Input.GetKeyDown(KeyCode.Alpha1))
        {
            player1.SetActive(true);
            player2.SetActive(false);
            player1.transform.position = Vector3.zero;
        }

        if (Input.GetKeyDown(KeyCode.Alpha2))
        {
            player2.SetActive(true);
            player1.SetActive(false);
            player2.transform.position = Vector3.zero;
        }

        if (Input.GetKeyDown(KeyCode.Alpha3))
        {
            GameObject enemy = Instantiate(testEnemy, transform.position, Quaternion.identity) as GameObject;
            //Instantiate(enemySpline, Vector3.zero, Quaternion.identity);
            player1.SendMessage("setSecondaryTarget", enemy);
            player2.SendMessage("setSecondaryTarget", enemy);
        }
    }
}
