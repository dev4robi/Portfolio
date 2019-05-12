using UnityEngine;
using System.Collections;

public class Stage : MonoBehaviour {

    public int iStageNumber = 1;
    public float fElapsedTime = 0.00f;          // 경과 시간
    public GameObject objStartingPoint = null;  // 시작 지점
    public GameObject objFinishPoint = null;    // 도착 지점
    public GameObject objNote = null;           // 일지 아이템
    public AudioClip sndStageClear;             // 스테이지 클리어 사운드

    private float fDistBetweenStartAndFinish = 0.00f;   // 도착 지점과 시작 지점 사이의 거리

	// Use this for initialization
	void Start () {
        // 일지 아이템 획득한 적이 있을 시 파괴
	    if (GameManager.stageInfo[iStageNumber - 1].bGotNote == true)
        {
            Destroy(objNote);
        }
	}
	
	// Update is called once per frame
	void Update () {
        // 경과 시간 연산
        fElapsedTime += Time.deltaTime;

        // 도착 지점과 시작 지점 사이의 거리 연산
        fDistBetweenStartAndFinish = (objStartingPoint.transform.position - objFinishPoint.transform.position).magnitude;
	}

    // 경과 시간을 반환
    public float GetStageElapsedTime()
    {
        return fElapsedTime;
    }

    // 시작 지점과 도착 지점 사이의 거리 반환
    public float GetDistBetweenStartAndFinish()
    {
        return fDistBetweenStartAndFinish;
    }
}
