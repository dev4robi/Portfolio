using UnityEngine;
using System.Collections;
using UnityEngine.UI;
public class TestUI : MonoBehaviour {

    public GameObject enemy;
    HealthScript hp;
    Text text;
    // Use this for initialization
    void Start () {
        hp = enemy.GetComponent<HealthScript>();
        text = GetComponent<Text>();

    }
	
	// Update is called once per frame
	void Update () {
        text.text = "Armor : " + hp.currentArmor + "\n" + "Shield : " + hp.currentShield;
	}
}
