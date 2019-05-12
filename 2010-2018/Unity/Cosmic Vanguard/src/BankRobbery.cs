using UnityEngine;
using System.Collections;

public class BankRobbery : MonoBehaviour {

    // 맴버 변수 (퍼블릭)
    public bool bRobbed = false;                    // 현재 강탈 여부
    public int iRobAmountMax = 0;                   // 은행에서 강탈가능한 최대 금액
    public int iCurRobAmount = 0;                   // 은행에서 강탈한 금액
    public GameObject objRobberyEffect = null;      // 은행에서 강탈 시 머리위에 표시되는 이펙트 종류

    // 맴버 변수 (프라이빗)
    private GameObject objGoldPile = null;          // 머리위에 표시되는 이펙트 오브젝트

    // Use this for initialization
    void Start () {
        bRobbed = false;    // false로 초기화
        iCurRobAmount = 0;  // 0으로 초기화
    }
	
	// Update is called once per frame
	void Update () {
        // 머리 위 이펙트 생성
	    if (objGoldPile == null && bRobbed == true)
        {
            objGoldPile = Instantiate(objRobberyEffect, (gameObject.transform.position + new Vector3(0.00f, 0.50f, 0.00f)),
                                      Quaternion.identity, gameObject.transform) as GameObject;
        }
	}
}
