using UnityEngine;
using System.Collections;

public class CameraMove : MonoBehaviour {

    public GameObject CameraTarget; // 카메라가 따라다닐 대상
    public string FollowingAxis;    // 카메라가 따라다닐 축 (X, Y, ALL)

    private Vector3 Offset;          // 카메라 오프셋

    /* [Tip]
    Reset           :   오브젝트 생성 후 인스펙터 뷰에서 리셋을 눌러줄 때 실행. (객체의 속성을 초기값으로 설정해 줄 때 사용)
    Awake           :   스크립트가 실행될 때 한 번만 호출. (모든 오브젝트가 초기화 된 후 호출되어 GameObject.Find 등 사용 안전)
    OnEnable        :   인스펙터 뷰에서 체크를 통해 게임 오브젝트를 활성화 할 때 사용. (활성화 시 마다 호출)
    Start           :   다른 모든 스크립트의 Awake가 실행된 후 Update 수행 전에 한번만 호출.

    FixedUpdate     :   주로 물리 엔진을 사용하는 경우 특정 시간을 간격으로 힘을 가할 때 사용. 발생 주기가 일정. (Default: 0.02sec)
    OnTriggerXXX    :   
    OnCollisionXXX  :

    Update          :   매 프레임마다 호출되는 함수. 게임의 핵심 로직에 사용됨. (스크립트가 활성화 되어 있어야 하고, 주기가 일정적이지 않음)
    LateUpdate      :   모든 Update함수가 실행된 후 호출됨. (주로 카메라 이동 등에 사용)

    OnDisable           :   게임 오브젝트 또는 스크립트가 비활성화 되었을 때 호출.
    OnDestroy           :   오브젝트 생존기간의 마지막 프레임이 업데이트 된 후 실행. (Object.Destroy 또는 씬 종료 시)
    OnApplicationQuit   :   응용프로그램 종료 전에 모든 오브젝트들에서 호출. (에디터는 플레이모드 중지 시 호출)
    */

    void Start () {
        try
        {
            if (CameraTarget == null)
                CameraTarget = GameObject.Find("PlayerHero").transform.FindChild("Hero").GetComponentInChildren<Hero>().gameObject;
        }
        catch (System.Exception e) { };

        Offset = this.transform.position - CameraTarget.transform.position;
        Offset.x = 0.0f;
        Offset.z = 0.0f;
	}
	
	void LateUpdate () {
        if (FollowingAxis == "X")
        {
            this.transform.position = new Vector3(CameraTarget.transform.position.x, this.transform.position.y, this.transform.position.z);
        }
        else if (FollowingAxis == "Y")
        {
            this.transform.position = new Vector3(this.transform.position.x, CameraTarget.transform.position.y, this.transform.position.z);
        }
        else
        {
            this.transform.position = new Vector3(CameraTarget.transform.position.x, CameraTarget.transform.position.y, this.transform.position.z);
        }
        this.transform.position += Offset;
    }
}
