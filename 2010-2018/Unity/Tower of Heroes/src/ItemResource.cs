using UnityEngine;
using System.Collections;

public class ItemResource : MonoBehaviour {
    public enum ItemType { ITEM_GOLD, ITEM_RUNE, ITEM_POTION, ITEM_EQUIPMENT, ITEM_NOTE };

    public ItemType enType = ItemType.ITEM_GOLD;
    public int iAmount = 0;         // 획득량(회복량)
    public int iEquipmentID = 0;    // 장비의 경우 장비 번호
    public AudioClip sndGetItem;    // 획득 시 사운드

    private bool bOptained = false; // 획득됨 여부
    private float fScale = 1.00f;   // 사이즈

	// Use this for initialization
	void Start () {
        bOptained = false;
        fScale = 1.00f;
    }
	
	// Update is called once per frame
	void Update () {
        // 모델을 Y축 회전시킴
        this.transform.Rotate(Vector3.up, this.transform.rotation.y + 90.0f * Time.deltaTime);

        // 획득 여부를 확인
        if (bOptained == true)
        {
            // 스케일 및 라이팅 강도 감소
            fScale -= (0.5f * Time.deltaTime);
            
            // 최저치에 도달하면 오브젝트 파괴
            if (fScale < 0.00f)
            {
                fScale = 0.00f;
                Destroy(this.gameObject);
            }
        }

        // 스케일링 동기화
        Vector3 newScale = (this.transform.localScale * fScale);
        this.transform.localScale = newScale;

        // 라이트 크기 동기화
        try
        {
            Transform lightTrans = this.transform.FindChild("Point light");
            lightTrans.GetComponent<Light>().intensity *= fScale;
        }
        catch (System.Exception e) { };
    }

    public bool ObtainItem()
    {
        // 아직 획득되지 않은 경우
        if (bOptained == false)
        {
            bOptained = true;

            // 블록 지역 변수
            GameObject textUI = Instantiate(Resources.Load("UI/DynamicText") as Object, this.gameObject.transform) as GameObject;
            Color color = new Color(1.0f, 1.0f, 1.0f, 1.0f);

            // 자원 증가
            if (enType == ItemType.ITEM_GOLD)
            {
                GameManager.SetGold(GameManager.GetGold(true) + iAmount, true);
                color = new Color(1.0f, 1.0f, 0.0f, 1.0f);
            }
            else if (enType == ItemType.ITEM_RUNE)
            {
                GameManager.SetRune(GameManager.GetRune(true) + iAmount, true);
                color = new Color(0.0f, 1.0f, 1.0f, 1.0f);
            }
            else if (enType == ItemType.ITEM_NOTE)
            {
                int iStageNum = GameObject.Find("Stage").GetComponent<Stage>().iStageNumber;
                GameManager.stageInfo[iStageNum - 1].bGotNote = true;
                PlayerPrefs.SetInt("Stage" + iStageNum.ToString() + "Note", 1);
                color = Color.white;
            }

            // 유동 텍스트 생성 및 위치와 색상 적용
            if (enType != ItemType.ITEM_NOTE)
            {
                DynamicText dynamicText = textUI.GetComponent<DynamicText>();
                dynamicText.SetDynamicText(("+" + iAmount), 30, 2.00f, this.gameObject.transform.position, color, new Vector3(0.0f, 1.0f, 0.0f), new Color(0.0f, 0.0f, 0.0f, -0.5f));
            }
            else
            {
                DynamicText dynamicText = textUI.GetComponent<DynamicText>();
                dynamicText.SetDynamicText("일지 획득!", 30, 2.00f, this.gameObject.transform.position, color, new Vector3(0.0f, 1.0f, 0.0f), new Color(0.0f, 0.0f, 0.0f, -0.5f));
            }

            return true;
        }
        // 이미 획득된 경우
        else
        {
            return false;
        }
    }
    
    // 아이템 속성 설정
    public void SetValueOfItem(ItemType type, int amount, int id = 0)
    {
        enType = type;
        iAmount = amount;
        iEquipmentID = id;
    }
}
